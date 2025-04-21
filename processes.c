#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#define WRITE 1
#define READ 0
#define COUNT 20

pid_t pid;

void printArray2D (int row, int col, int array[row][col]);
void matrixMultiply (int row1, int col1, int a[row1][col1], int row2, int col2, int b[row2][col2], int c[row1][col2]);
void generateMatrix (int a[100][100], int b[100][100]);
long difftimespec_ns(const struct timespec after, const struct timespec before);

void childProcess(int n, int a[100][100], int b[100][100]);

int main() {

    struct timespec start1, end1;
    
    int a[100][100];
    int b[100][100];
    int m[100][100];

    // generate our matrices with the given ID value
    generateMatrix(a, b);

    //record the start time
    clock_gettime(CLOCK_MONOTONIC, &start1);

    matrixMultiply(100, 100, a, 100, 100, b, m );

    //record the end time
    clock_gettime(CLOCK_MONOTONIC, &end1);

    //calculate the difference
    long elapsed_ns = difftimespec_ns(end1, start1);
    double elapsed_seconds = (double)elapsed_ns / 1000000000.0;

    // Print the elapsed time for naive approach
    printf("Elapsed time naive :  %f seconds\n", elapsed_seconds);
    
    //record the start time
    clock_gettime(CLOCK_MONOTONIC, &start1);

    childProcess(COUNT, a, b);

    //record the end time
    clock_gettime(CLOCK_MONOTONIC, &end1);

    //calculate the difference
     elapsed_ns = difftimespec_ns(end1, start1);
     elapsed_seconds = (double)elapsed_ns / 1000000000.0;


    // Print the elapsed time for multiprocess approach
    printf("Elapsed time %d processes: %f seconds\n",COUNT, elapsed_seconds);

   
}

void printArray2D (int row, int col, int array[row][col]) {
    for (int i = 0;i<row;i++) {
        for (int j = 0;j<col;j++) {
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

void matrixMultiply (int row1, int col1, int a[row1][col1], int row2, int col2, int b[row2][col2], int c[row1][col2]) {

    int sum = 0;
    for (int i = 0;i<row1;i++) {
        for (int j = 0;j<col2;j++) {
            for (int k = 0; k<col1;k++) {
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
* Function that takes the two matrices and multiplies them with the multiprocessing approach
*/
void childProcess(int n, int a[100][100], int b[100][100]) {
    int fd[n][2];
    for (int i = 0; i < COUNT; i++) {
        if (pipe(fd[i]) == -1) {
            perror("Pipe creation failed");
            return 1;
        }
    }
    

    int result[n][100/n][100];
    pid = 1; // Initialize pid

    for (int i = 0; i < n; i++) {
        if (pid != 0) {
            pid = fork();

            if (pid < 0){ //Checking for errors
            perror("Fork failed");
            return 1;

        }
        }

        if (pid == 0) { // Only children go through the if statement
            close(fd[i][READ]);

            matrixMultiply(100/n, 100, a+100/n * i, 100, 100, b, result[i]);

            write(fd[i][WRITE], result[i], sizeof(int) * 100/n * 100);
            close(fd[i][WRITE]);
            exit(0);
        }
    }

    if (pid != 0) {
        for (int i = 0; i < n; i++) {
            close(fd[i][WRITE]); // Close the write end in the parent process
            wait(NULL);
        }

        for (int i = 0; i < n; i++) {
            read(fd[i][READ], result[i], sizeof(int) * 100/n * 100);
            close(fd[i][READ]);
        }
    }
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
