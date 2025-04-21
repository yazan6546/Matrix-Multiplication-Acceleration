#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>


#define COUNT 20
double elapsed_seconds;
pid_t pid;

void printArray2D(int row, int col, int array[row][col]);
void matrixMultiply(int row1, int col1, int a[row1][col1], int row2, int col2, int b[row2][col2], int c[row1][col2]);
void generateMatrix(int a[100][100], int b[100][100]);
long difftimespec_ns(const struct timespec after, const struct timespec before);
void *thread_create(void* arg) ;
void *thread_multiply(void *arg);

int a[100][100];
int b[100][100];
int result[COUNT][100 / COUNT][100];

int main() {

    struct timespec start, end;

    // generate our matrices with the given ID value
    generateMatrix(a, b);

    pthread_t thread_monitor;

    //record the start time
    clock_gettime(CLOCK_MONOTONIC, &start);

    //create the monitoring thread to calculate the time of the program
    pthread_create(&thread_monitor, NULL, thread_create, NULL);

    //wait for the monitoring thread to join the main thread
    pthread_join(thread_monitor, NULL);

    //record the end time
     clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate elapsed time in seconds
    long elapsed_ns = difftimespec_ns(end, start);
    double elapsed_seconds = (double)elapsed_ns / 1000000000.0;

    // Print the elapsed time
    printf("Elapsed time %d detached_threads: %f seconds\n",COUNT, elapsed_seconds);

        
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
    printf("\n");
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
* Function that specifies the task of each created detached thread, which is to multiply specific rows of a matrix
* arguments : arg -> the index of the given thread
* return : void
*/
void *thread_multiply(void *arg) {
    int i = *((int *)arg);
    struct timespec start, end;

    // Perform matrix multiplication for each thread

    matrixMultiply(100 / COUNT, 100, a + 100 / COUNT * i, 100, 100, b, result[i]);

     // Free memory allocated for thread argument
    free(arg);
}

/*
* Function for the monitoring thread whose purpose is to create and initialize a set of detached threads
* and each thread will multiply the rows of the matrix
*/
void* thread_create(void *arg) {

    pthread_t thread[COUNT];
    pthread_attr_t detached;
    pthread_attr_init(&detached);
    pthread_attr_setdetachstate(&detached, PTHREAD_CREATE_DETACHED);

    for (int i = 0; i < COUNT; i++) {
        int *p = malloc(sizeof(int));
        if (p == NULL) {
            perror("malloc failed.\n");
            exit(0);
        }
        *p = i;

        // Create threads for matrix multiplication
        if (pthread_create(thread + i, &detached, thread_multiply, p) != 0) {
            perror("Thread creation failed.\n");
            return (int*) 1;
        }
    }
    pthread_exit(0);
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
