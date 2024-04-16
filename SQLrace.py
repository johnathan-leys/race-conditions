import sqlite3
import threading

# Demo of SQL race condition- sometimes successfully deducts 100 and 100, 
# sometime race condition triggers and only 100 is deducted.

# Function to update a record in the database
def update_record():
    conn = sqlite3.connect("bank.db")
    cursor = conn.cursor()
    cursor.execute("SELECT balance FROM accounts WHERE id=1")
    balance = cursor.fetchone()[0]
    new_balance = balance - 100  # Deduct $100
    cursor.execute("UPDATE accounts SET balance=? WHERE id=1", (new_balance,))
    conn.commit()
    print("Record updated successfully")
    # Print SQL information
    print("SQL info after update:")
    cursor.execute("SELECT * FROM accounts")
    print(cursor.fetchall())
    conn.close()

# Function to simulate concurrent updates to the database
def simulate_race_condition():
    # Run update_record function in two threads simultaneously
    thread1 = threading.Thread(target=update_record)
    thread2 = threading.Thread(target=update_record)
    thread1.start()
    thread2.start()
    thread1.join()
    thread2.join()

# Create initial database schema and records
def initialize_database():
    conn = sqlite3.connect("bank.db")
    cursor = conn.cursor()
    cursor.execute("CREATE TABLE IF NOT EXISTS accounts (id INTEGER PRIMARY KEY, balance INTEGER)")
    
    # Check if record with id=1 already exists
    cursor.execute("SELECT COUNT(*) FROM accounts WHERE id=1")
    count = cursor.fetchone()[0]
    
    if count == 0:
        cursor.execute("INSERT INTO accounts (id, balance) VALUES (1, 1000)")
        conn.commit()
        print("Initial record inserted successfully")
    else:
        print("Record with id=1 already exists")
    
    conn.close()


if __name__ == "__main__":
    initialize_database()
    simulate_race_condition()
