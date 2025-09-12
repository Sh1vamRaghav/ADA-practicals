#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int** create_matrix(int n) {
    int** matrix = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        matrix[i] = (int*)malloc(n * sizeof(int));
    }
    return matrix;
}

void free_matrix(int** matrix, int n) {
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void fill_matrix(int** matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = rand() % 10; 
        }
    }
}

int** matrix_multiply(int** A, int** B, int n) {
    int** C = create_matrix(n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    return C;
}

int main() {
    srand(time(NULL));
    int sizes[] = {4,8,16,32,64,128,256,512}; 
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    double time[8];

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];

        int** A = create_matrix(n);
        int** B = create_matrix(n);

        fill_matrix(A, n);
        fill_matrix(B, n);
        clock_t time_taken = 0;
        for(int i = 0 ; i < 100 ; i++){
            clock_t start = clock();
            int** C = matrix_multiply(A, B, n);
            clock_t end = clock();
            time_taken += end-start;
        }

        double avg_time = ((double)time_taken) / CLOCKS_PER_SEC/100.0;

        printf("Matrix size %d x %d -> Time taken: %.4f seconds\n", n, n, avg_time);
        time[s] = avg_time;
        free_matrix(A, n);
        free_matrix(B, n);
    }
    printf("\n");
    for(int i = 0 ; i < 10 ; i++){
        printf("%.8f, " ,time[i]);
    }

    return 0;
}
