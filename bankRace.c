#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int balance = 1000;  // Initial balance

void* withdraw(void* arg) {
    int amount = *((int*)arg);
    printf("Withdrawing $%d...\n", amount);
    
    // Check if enough balance (without synchronization)
    if (balance >= amount) {
        // Simulate some processing delay
        sleep(1);
        
        // Introduce a race condition
        balance = balance - amount + 100;
        printf("Withdrawal successful. New balance: $%d\n", balance);
    } else {
        printf("Insufficient balance for withdrawal.\n");
    }
    
    free(arg);
    return NULL;
}

void* deposit(void* arg) {
    int amount = *((int*)arg);
    printf("Depositing $%d...\n", amount);
    
    // Simulate some processing delay
    sleep(1);
    
    balance += amount;
    printf("Deposit successful. New balance: $%d\n", balance);
    
    free(arg);
    return NULL;
}

int main() {
    pthread_t tid1, tid2;
    int* withdrawAmount = malloc(sizeof(int));
    int* depositAmount = malloc(sizeof(int));
    
    *withdrawAmount = 800;
    *depositAmount = 300;
    
    // Create two threads, one for withdrawal and one for deposit
    pthread_create(&tid1, NULL, withdraw, withdrawAmount);
    pthread_create(&tid2, NULL, deposit, depositAmount);
    
    // Wait for threads to finish
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    
    printf("Final balance: $%d\n", balance);
    
    return 0;
}