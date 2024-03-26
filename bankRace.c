#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4

int account_balance = 1000; // initial balance
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // mutex for thread synchronization

void* withdraw(void* arg) {
    int amount = *(int*)arg;
    int bal;

    pthread_mutex_lock(&mutex); // acquire the mutex
    bal = account_balance;
    bal -= amount;
    printf("Withdrawing %d, remaining balance: %d\n", amount, bal);
    account_balance = bal;
    pthread_mutex_unlock(&mutex); // release the mutex

    pthread_exit(NULL);
}

void* deposit(void* arg) {
    int amount = *(int*)arg;
    int bal;

    pthread_mutex_lock(&mutex); // acquire the mutex
    bal = account_balance;
    bal += amount;
    printf("Depositing %d, new balance: %d\n", amount, bal);
    account_balance = bal;
    pthread_mutex_unlock(&mutex); // release the mutex

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int amounts[NUM_THREADS] = {200, 300, 400, 500};

    // create threads for withdrawals
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, withdraw, &amounts[i]);
    }

    // create threads for deposits
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, deposit, &amounts[i]);
    }

    // wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final account balance: %d\n", account_balance);

    return 0;
}