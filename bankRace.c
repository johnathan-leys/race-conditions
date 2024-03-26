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
    pthread_t tid1, tid2, tid3, tid4;
    int* withdrawAmount1 = malloc(sizeof(int));
    int* withdrawAmount2 = malloc(sizeof(int));
    int* depositAmount1 = malloc(sizeof(int));
    int* depositAmount2 = malloc(sizeof(int));
    
    *withdrawAmount1 = 300;
    *withdrawAmount2 = 400;
    *depositAmount1 = 200;
    *depositAmount2 = 500;
    
    // Create four threads, two for withdrawals and two for deposits
    pthread_create(&tid1, NULL, withdraw, withdrawAmount1);
    pthread_create(&tid2, NULL, withdraw, withdrawAmount2);
    pthread_create(&tid3, NULL, deposit, depositAmount1);
    pthread_create(&tid4, NULL, deposit, depositAmount2);
    
    // Wait for threads to finish
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);
    
    printf("Final balance: $%d\n", balance);
    
    return 0;
}