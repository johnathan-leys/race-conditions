#include <stdio.h>
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
    pthread_mutex_lock(&lock);

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

    return NULL;
}

int main() {
     // Initialize the mutex
    //pthread_mutex_init(&lock, NULL);

    pthread_t thread1, thread2;
    int withdraw_amount1 = 700;
    int withdraw_amount2 = 400;

    // Create two threads to simulate concurrent withdrawals
    pthread_create(&thread1, NULL, withdraw, &withdraw_amount1);
    pthread_create(&thread2, NULL, withdraw, &withdraw_amount2);

    // Wait for both threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Final account balance: $%d\n", account_balance);

    // Destroy the mutex
    //pthread_mutex_destroy(&lock);


     //this is how to run it in terminal:  gcc -o race_condition_bank race_condition_bank.c -lpthread
    return 0;
}
