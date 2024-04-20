#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

// Global bank account balance
int account_balance = 1000;

// Mutex for locking the balance update
pthread_mutex_t lock;

// Withdraw function to be executed by threads
void* withdraw(void* amount) {
    int withdraw_amount = *(int*)amount;
    
    // Get time since application started in milliseconds
    clock_t start_time = clock();
    double elapsed_time = (double)(start_time) / CLOCKS_PER_SEC * 1000;
    
    printf("[%.2f ms] Thread %lu started\n", elapsed_time, pthread_self());
    
    // Locking the critical section
    pthread_mutex_lock(&lock);
    
    printf("[%.2f ms] Thread %lu attempting to withdraw $%d. Current balance: $%d\n", elapsed_time, pthread_self(), withdraw_amount, account_balance);
    
    // Check if the balance is sufficient for the withdrawal
    if (account_balance >= withdraw_amount) {
        // Simulate some delay in processing the withdrawal
        sleep(1);  // Sleep for 1 second
        
        account_balance -= withdraw_amount;  // Update the account balance
        
        printf("[%.2f ms] Thread %lu withdrawal of $%d successful. New balance: $%d\n", elapsed_time, pthread_self(), withdraw_amount, account_balance);
    } else {
        printf("[%.2f ms] Thread %lu failed to withdraw $%d due to insufficient funds. Current balance: $%d\n", elapsed_time, pthread_self(), withdraw_amount, account_balance);
    }
    
    // Unlocking after the critical section
    pthread_mutex_unlock(&lock);
    
    printf("[%.2f ms] Thread %lu finished\n", elapsed_time, pthread_self());
    
    return NULL;
}

int main() {
    // Initialize the mutex
    pthread_mutex_init(&lock, NULL);
    
    pthread_t thread1, thread2;
    int withdraw_amount1 = 700;
    int withdraw_amount2 = 400;
    
    // Get time since application started in milliseconds
    clock_t start_time = clock();
    double elapsed_time = (double)(start_time) / CLOCKS_PER_SEC * 1000;
    
    printf("[%.2f ms] Main thread creating two threads to simulate concurrent withdrawals\n", elapsed_time);
    
    // Create two threads to simulate concurrent withdrawals
    pthread_create(&thread1, NULL, withdraw, &withdraw_amount1);
    pthread_create(&thread2, NULL, withdraw, &withdraw_amount2);
    
    // Wait for both threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    // Get time since application started in milliseconds
    elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC * 1000;
    
    printf("[%.2f ms] Final account balance: $%d\n", elapsed_time, account_balance);
    
    // Destroy the mutex
    pthread_mutex_destroy(&lock);
    
    //this is how to run it in terminal: gcc -o Race_condition_bankLock Race_condition_bankLock.c -lpthread
    
    return 0;
}