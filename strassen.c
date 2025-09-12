#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Create a square matrix dynamically
int** create_matrix(int n) {
    int** matrix = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
        matrix[i] = (int*)malloc(n * sizeof(int));
    return matrix;
}

// Free a dynamically allocated matrix
void free_matrix(int** matrix, int n) {
    for (int i = 0; i < n; i++)
        free(matrix[i]);
    free(matrix);
}

// Fill matrix with random numbers 0-9
void fill_matrix(int** matrix, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matrix[i][j] = rand() % 10;
}

// Add two matrices
int** add_matrix(int** A, int** B, int n) {
    int** C = create_matrix(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}

// Subtract two matrices
int** sub_matrix(int** A, int** B, int n) {
    int** C = create_matrix(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
    return C;
}

// Strassen multiplication
int** multiply_strassen(int** A, int** B, int n) {
    int** C = create_matrix(n);

    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return C;
    }

    int newSize = n / 2;

    // Allocate submatrices
    int** A11 = create_matrix(newSize); int** A12 = create_matrix(newSize);
    int** A21 = create_matrix(newSize); int** A22 = create_matrix(newSize);
    int** B11 = create_matrix(newSize); int** B12 = create_matrix(newSize);
    int** B21 = create_matrix(newSize); int** B22 = create_matrix(newSize);

    for (int i = 0; i < newSize; i++)
        for (int j = 0; j < newSize; j++) {
            A11[i][j] = A[i][j]; A12[i][j] = A[i][j + newSize];
            A21[i][j] = A[i + newSize][j]; A22[i][j] = A[i + newSize][j + newSize];
            B11[i][j] = B[i][j]; B12[i][j] = B[i][j + newSize];
            B21[i][j] = B[i + newSize][j]; B22[i][j] = B[i + newSize][j + newSize];
        }

    // Compute Strassen's 7 products
    int** P1 = multiply_strassen(A11, sub_matrix(B12, B22, newSize), newSize);
    int** P2 = multiply_strassen(add_matrix(A11, A12, newSize), B22, newSize);
    int** P3 = multiply_strassen(add_matrix(A21, A22, newSize), B11, newSize);
    int** P4 = multiply_strassen(A22, sub_matrix(B21, B11, newSize), newSize);
    int** P5 = multiply_strassen(add_matrix(A11, A22, newSize), add_matrix(B11, B22, newSize), newSize);
    int** P6 = multiply_strassen(sub_matrix(A12, A22, newSize), add_matrix(B21, B22, newSize), newSize);
    int** P7 = multiply_strassen(sub_matrix(A11, A21, newSize), add_matrix(B11, B12, newSize), newSize);

    // Compute submatrices of C
    int** C11 = add_matrix(sub_matrix(add_matrix(P5, P4, newSize), P2, newSize), P6, newSize);
    int** C12 = add_matrix(P1, P2, newSize);
    int** C21 = add_matrix(P3, P4, newSize);
    int** C22 = sub_matrix(sub_matrix(add_matrix(P1, P5, newSize), P3, newSize), P7, newSize);

    // Combine into C
    for (int i = 0; i < newSize; i++)
        for (int j = 0; j < newSize; j++) {
            C[i][j] = C11[i][j];
            C[i][j + newSize] = C12[i][j];
            C[i + newSize][j] = C21[i][j];
            C[i + newSize][j + newSize] = C22[i][j];
        }

    // Free all submatrices
    free_matrix(A11,newSize); free_matrix(A12,newSize); free_matrix(A21,newSize); free_matrix(A22,newSize);
    free_matrix(B11,newSize); free_matrix(B12,newSize); free_matrix(B21,newSize); free_matrix(B22,newSize);
    free_matrix(P1,newSize); free_matrix(P2,newSize); free_matrix(P3,newSize); free_matrix(P4,newSize);
    free_matrix(P5,newSize); free_matrix(P6,newSize); free_matrix(P7,newSize);
    free_matrix(C11,newSize); free_matrix(C12,newSize); free_matrix(C21,newSize); free_matrix(C22,newSize);

    return C;
}

int main() {
    srand(time(NULL));

    int sizes[] = {4,8,16,32,64,128,256,512};  // Matrix sizes (power of 2)
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    double time[8];
    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        int** A = create_matrix(n);
        int** B = create_matrix(n);
        fill_matrix(A, n);
        fill_matrix(B, n);

        clock_t time_taken = 0;
        for (int i = 0; i < 1; i++) {
            clock_t start = clock();
            int** C = multiply_strassen(A, B, n);
            clock_t end = clock();
            time_taken += end - start;
            free_matrix(C, n);
        }

        double avg_time = ((double)time_taken) / CLOCKS_PER_SEC/1.0;
        printf("[Strassen] Matrix %d x %d -> Avg time: %.6f sec\n",n, n,avg_time);
        time[s] = avg_time;
        free_matrix(A, n);
        free_matrix(B, n);
    }
    printf("\n");
    for(int i = 0 ; i < 8 ; i++){
        printf("%.8f, ",time[i]);
    }

    return 0;
}
