#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define FILENAME "balance.txt"

// ANSI color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

char* get_timestamp() {
    static char buffer[64];
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    sprintf(buffer, "%ld.%09ld", ts.tv_sec, ts.tv_nsec);
    return buffer;
}

double read_balance(pthread_t thread_id, const char* color) {
    double balance;
    pthread_mutex_lock(&mutex);
    printf("[%s] %sThread %lu:%s ACQUIRED LOCK for reading balance\n", get_timestamp(), color, thread_id, RESET);
    FILE* file = fopen(FILENAME, "r");
    fscanf(file, "%lf", &balance);
    fclose(file);
    printf("[%s] %sThread %lu:%s Read balance from file\n", get_timestamp(), color, thread_id, RESET);
    pthread_mutex_unlock(&mutex);
    printf("[%s] %sThread %lu:%s RELEASED LOCK after reading balance\n", get_timestamp(), color, thread_id, RESET);
    return balance;
}

void write_balance(double balance, pthread_t thread_id, const char* color) {
    pthread_mutex_lock(&mutex);
    printf("[%s] %sThread %lu:%s Acquired lock for writing balance\n", get_timestamp(), color, thread_id, RESET);
    FILE* file = fopen(FILENAME, "w");
    fprintf(file, "%lf", balance);
    fclose(file);
    printf("[%s] %sThread %lu:%s Wrote balance to file\n", get_timestamp(), color, thread_id, RESET);
    pthread_mutex_unlock(&mutex);
    printf("[%s] %sThread %lu:%s Released lock after writing balance\n", get_timestamp(), color, thread_id, RESET);
}

void* withdraw(void* arg) {
    double amount = *(double*)arg;
    pthread_t thread_id = pthread_self();
    const char* color = RED;
    double balance = read_balance(thread_id, color);
    printf("[%s] %sThread %lu:%s Withdrawing %.2lf\n", get_timestamp(), color, thread_id, RESET, amount);
    balance -= amount;
    write_balance(balance, thread_id, color);
    printf("[%s] %sThread %lu:%s New balance after withdrawal: %.2lf\n", get_timestamp(), color, thread_id, RESET, balance);
    return NULL;
}

void* deposit(void* arg) {
    double amount = *(double*)arg;
    pthread_t thread_id = pthread_self();
    const char* color = GREEN;
    double balance = read_balance(thread_id, color);
    printf("[%s] %sThread %lu:%s Current Balance %.2lf\n", get_timestamp(), color, thread_id, RESET, balance);
    printf("[%s] %sThread %lu:%s Depositing %.2lf\n", get_timestamp(), color, thread_id, RESET, amount);
    balance += amount;
    write_balance(balance, thread_id, color);
    printf("[%s] %sThread %lu:%s New balance after deposit: %.2lf\n", get_timestamp(), color, thread_id, RESET, balance);
    return NULL;
}

int main() {
    pthread_t tid1, tid2;
    double start_bal = 1000;
    double withdraw_amount = 500;
    double deposit_amount = 200;
    printf("Initial Balance: %.2lf\n", start_bal);
    printf("Depositing %.2lf and withdrawing %.2lf\n", deposit_amount, withdraw_amount);
    printf("Expected final Balance: %.2lf\n", (start_bal - withdraw_amount + deposit_amount));
    printf("Race condition with timestamps:\n");
    printf("%sMain%s thread is Yellow, %sthread1%s is Red, %sthread2%s is green\n", YELLOW, RESET, RED, RESET, GREEN, RESET);
    printf("-------------------------------------------------------------------\n");
    printf("Timestamp (nanosec) | Thread ID | Current Action \n");
    printf("-------------------------------------------------------------------\n");
    const char* color = YELLOW;
    write_balance(start_bal, pthread_self(), color);
    printf("[%s] %sThread %lu:%s Initial Balance: %.2lf\n", get_timestamp(), color, pthread_self(), RESET, read_balance(pthread_self(), color));

    pthread_create(&tid1, NULL, withdraw, &withdraw_amount);
    pthread_create(&tid2, NULL, deposit, &deposit_amount);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    printf("[%s] %sThread %lu:%s Final balance: %.2lf\n", get_timestamp(), color, pthread_self(), RESET, read_balance(pthread_self(), color));

    return 0;
}