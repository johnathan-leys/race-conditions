import sqlite3
import threading
import time

# Demo of SQL race condition: value written by one thread is overwritten by other.

# Function to update a record in the database
def update_record(amount=100, thread_num=1):
    conn = sqlite3.connect("bank.db")
    cursor = conn.cursor()
    cursor.execute("SELECT balance FROM accounts WHERE id=1")
    balance = cursor.fetchone()[0]

    # Print SQL information before update
    timestamp = time.time_ns()
    print(f"{timestamp} - Thread {thread_num}: SQL info before update:")
    cursor.execute("SELECT * FROM accounts")
    print(cursor.fetchall())

    new_balance = balance - amount  # Deduct $100
    cursor.execute("UPDATE accounts SET balance=? WHERE id=1", (new_balance,))
    conn.commit()
    print(f"{timestamp} - Thread {thread_num}: Record updated successfully")

    # Print SQL information after update
    print(f"{timestamp} - Thread {thread_num}: SQL info after update:")
    cursor.execute("SELECT * FROM accounts")
    print(cursor.fetchall())
    conn.close()

# Function to simulate concurrent updates to the database
def simulate_race_condition():
    # Run update_record function in two threads simultaneously
    thread1 = threading.Thread(target=update_record, args=(100, 1))
    thread2 = threading.Thread(target=update_record, args=(200, 2))
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
        cursor.execute("INSERT INTO accounts (id, balance) VALUES (1, 10000)")
        conn.commit()
        print("Initial record inserted successfully")
    else:
        print("Record with id=1 already exists")
    conn.close()

def check_database():
    conn = sqlite3.connect("bank.db")
    cursor = conn.cursor()
    cursor.execute("SELECT balance FROM accounts WHERE id=1")
    balance = cursor.fetchone()[0]
    timestamp = time.time_ns()
    print(f"{timestamp} - Current balance in the database: ${balance}")
    conn.close()
    return balance

if __name__ == "__main__":
    
    initialize_database()
    print(f"Starting account balance: ")
    check_database()
    print("Withdrawing $100 and $200")
    print("Expected value after transactions: ")
    simulate_race_condition()
    check_database()