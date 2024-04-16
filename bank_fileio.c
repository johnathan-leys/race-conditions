#include <pthread.h>
#include <unistd.h>

// Global bank account balance
int account_balance = 1000;

// Mutex for locking the balance update
//pthread_mutex_t lock;

// Withdraw function to be executed by threads
void* withdraw(void* amount) {
    int withdraw_amount = *(int*)amount;

    // Locking the critical section
    //pthread_mutex_lock(&lock);

    printf("Attempting to withdraw $%d. Current balance: $%d\n", withdraw_amount, account_balance);

    // Check if the balance is sufficient for the withdrawal
    if (account_balance >= withdraw_amount) {
        // Simulate some delay in processing the withdrawal
        sleep(1); // Sleep for 1 second

        account_balance -= withdraw_amount; // Update the account balance
        printf("Withdrawal of $%d successful. New balance: $%d\n", withdraw_amount, account_balance);
    } else {
        printf("Failed to withdraw $%d due to insufficient funds. Current balance: $%d\n", withdraw_amount, account_balance);
    }
    // Unlocking after the critical section
    //pthread_mutex_unlock(&lock);
#define FILENAME "balance.txt"

// Function to read the current balance from the file
double read_balance() {
    FILE *file = fopen(FILENAME, "r");
    double balance;
    fscanf(file, "%lf", &balance);
    fclose(file);
    return balance;
}

// Function to write the new balance to the file
void write_balance(double balance) {
    FILE *file = fopen(FILENAME, "w");
    fprintf(file, "%lf", balance);
    fclose(file);
}

// Function for withdrawing money
void *withdraw(void *arg) {
    double amount = *(double *)arg;
    double balance = read_balance();
    printf("Withdrawing %.2lf\n", amount);
    balance -= amount;
    write_balance(balance);
    printf("New balance after withdrawal: %.2lf\n", balance);
    return NULL;
}

int main() {
     // Initialize the mutex
    //pthread_mutex_init(&lock, NULL);
// Function for depositing money
void *deposit(void *arg) {
    double amount = *(double *)arg;
    double balance = read_balance();
    printf("Depositing %.2lf\n", amount);
    balance += amount;
    write_balance(balance);
    printf("New balance after deposit: %.2lf\n", balance);
    return NULL;
}

    pthread_t thread1, thread2;
    int withdraw_amount1 = 700;
    int withdraw_amount2 = 400;
int main() {
    // Initialize the balance file with initial balance
    write_balance(1000);
    printf("Initial Balance: %.2lf\n", read_balance());

    // Create two threads to simulate concurrent withdrawals
    pthread_create(&thread1, NULL, withdraw, &withdraw_amount1);
    pthread_create(&thread2, NULL, withdraw, &withdraw_amount2);
    pthread_t tid1, tid2;
    double withdraw_amount = 500;
    double deposit_amount = 200;

    // Wait for both threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    // Create two threads for withdrawing and depositing
    pthread_create(&tid1, NULL, withdraw, &withdraw_amount);
    pthread_create(&tid2, NULL, deposit, &deposit_amount);

    printf("Final account balance: $%d\n", account_balance);
    // Wait for the threads to finish
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // Destroy the mutex
    //pthread_mutex_destroy(&lock);
    printf("Final balance: %.2lf\n", read_balance());

    return 0;
}