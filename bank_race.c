#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

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

int main() {
    // Initialize the balance file with initial balance
    write_balance(1000);
    printf("Initial Balance: %.2lf\n", read_balance());

    pthread_t tid1, tid2;
    double withdraw_amount = 500;
    double deposit_amount = 200;

    // Create two threads for withdrawing and depositing
    pthread_create(&tid1, NULL, withdraw, &withdraw_amount);
    pthread_create(&tid2, NULL, deposit, &deposit_amount);

    // Wait for the threads to finish
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    printf("Final balance: %.2lf\n", read_balance());

    return 0;
}
