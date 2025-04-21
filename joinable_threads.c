#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>


#define COUNT 20

pid_t pid;

void printArray2D(int row, int col, int array[row][col]);
void matrixMultiply(int row1, int col1, int a[row1][col1], int row2, int col2, int b[row2][col2], int c[row1][col2]);
void generateMatrix(int a[100][100], int b[100][100]);
long difftimespec_ns(const struct timespec after, const struct timespec before);
int thread_create();
void *thread_multiply(void *);

int a[100][100];
int b[100][100];
int result[COUNT][100 / COUNT][100];

int main() {
    struct timespec start, end;

    generateMatrix(a, b);

    // Record the start time
    clock_gettime(CLOCK_MONOTONIC, &start);

   thread_create();
    // Record the end time
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate elapsed time in seconds
    long elapsed_ns = difftimespec_ns(end, start);
    double elapsed_seconds = (double)elapsed_ns / 1000000000.0;

    printf("Elapsed time %d joinable_threads: %f seconds\n",COUNT, elapsed_seconds);


    return 0;
}

/*
* Function that takes a 2d array and prints it
*/

void printArray2D(int row, int col, int array[row][col]) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }
}

/*
* Function that returns the multiplication of 2 matrices
* Arguments : dimensions of matrix1, matrix1, dimensions of matrix2, matrix2, result matrix
* return : void
*/

void matrixMultiply(int row1, int col1, int a[row1][col1], int row2, int col2, int b[row2][col2], int c[row1][col2]) {
    int sum = 0;
    for (int i = 0; i < row1; i++) {
        for (int j = 0; j < col2; j++) {
            for (int k = 0; k < col1; k++) {
                sum += a[i][k] * b[k][j];
            }
            c[i][j] = sum;
            sum = 0;
        }
    }
}


/*
* Function that takes 2 empty 2D matrices and initializes them with unique values
* for a given ID
*/
void generateMatrix(int a[100][100], int b[100][100]) {
    int c[] = {1, 2, 1, 0, 1, 4, 5};
    int d[] = {2, 4, 2, 3, 9, 2, 0, 4, 3, 5};
    int j = 0;

    // Fill matrices with sample values
    for (int i = 0; i < 100 * 100; i++) {
        if (i % 100 == 0 && i != 0) {
            j++;
        }
        a[j][i % 100] = c[i % 7];
        b[j][i % 100] = d[i % 10];
    }
}


/*
* Function that specifies the task of each thread, which is to multiply given rows of 2 matrices
* Arguments : void*, the index of the thread
*/

void *thread_multiply(void *arg) {
    int i = *((int *)arg);

    // Perform matrix multiplication for each thread
    matrixMultiply(100 / COUNT, 100, a + 100 / COUNT * i, 100, 100, b, result[i]);

    // Free memory allocated for thread argument
    free(arg);
}


/*
* Function that calculates the difference of time given the time before and after
* arguments : 1) struct timespec that represents the after time
* 2) struct timespec that represents the before time
* return : elapsed time in nanoseconds
*/

long difftimespec_ns(const struct timespec after, const struct timespec before) {
    return (after.tv_sec - before.tv_sec) * 1000000000 + (after.tv_nsec - before.tv_nsec);
}

/*
* Function that creates the joinable threads for the multiplication operation
*/
int thread_create() {
    pthread_t thread[COUNT];
    int j;
    int *p = &j;
    for (int i = 0; i < COUNT; i++) {
        int *p = malloc(sizeof(int));
        if (p == NULL) {
            perror("malloc failed.\n");
        }
        *p = i;

        // Create threads for matrix multiplication
        if (pthread_create(thread + i, NULL, thread_multiply, p) != 0) {
            perror("Thread creation failed.\n");
            return 1;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < COUNT; i++) {
        if (pthread_join(thread[i], NULL) != 0) {
            perror("Thread join failed.\n");
            return 1;
        }
    }
}
