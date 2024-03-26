#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 2
#define ITERATIONS 1000000

int counter = 0;

void *increment_counter(void *thread_id) {
    int tid = *(int*)thread_id;
    for (int i = 0; i < ITERATIONS; i++) {
        counter++; // Critical section
    }
    printf("Thread %d finished.\n", tid);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, increment_counter, (void *)&thread_ids[i]);
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Expected result: %d\n", NUM_THREADS * ITERATIONS);
    printf("Actual result: %d\n", counter);

    return 0;
}
