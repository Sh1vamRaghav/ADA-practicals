// matbench.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef int elem_t;

elem_t* alloc_mat(int n) {
    size_t bytes = (size_t)n * n * sizeof(elem_t);
    elem_t *m = (elem_t*)malloc(bytes);
    if (!m) { fprintf(stderr, "alloc failed\n"); exit(1); }
    memset(m, 0, bytes);
    return m;
}
void free_mat(elem_t* m) { free(m); }

void gen_mat(elem_t *A, int n) {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            A[i*n + j] = rand() % 10;
}

void iterative_mul(const elem_t *A, const elem_t *B, elem_t *C, int n) {
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            elem_t a = A[i*n + k];
            for (int j = 0; j < n; ++j) {
                C[i*n + j] += a * B[k*n + j];
            }
        }
    }
}

void rec_mul_add(const elem_t *A, int a_stride,
                 const elem_t *B, int b_stride,
                 elem_t *C, int c_stride,
                 int n)
{
    if (n == 1) {
        C[0] += A[0] * B[0];
        return;
    }

    int k = n >> 1;

    const elem_t *A11 = A;
    const elem_t *A12 = A + k;
    const elem_t *A21 = A + k * a_stride;
    const elem_t *A22 = A + k * a_stride + k;

    const elem_t *B11 = B;
    const elem_t *B12 = B + k;
    const elem_t *B21 = B + k * b_stride;
    const elem_t *B22 = B + k * b_stride + k;

    elem_t *C11 = C;
    elem_t *C12 = C + k;
    elem_t *C21 = C + k * c_stride;
    elem_t *C22 = C + k * c_stride + k;

    // C11 += A11*B11 + A12*B21
    rec_mul_add(A11, a_stride, B11, b_stride, C11, c_stride, k);
    rec_mul_add(A12, a_stride, B21, b_stride, C11, c_stride, k);

    // C12 += A11*B12 + A12*B22
    rec_mul_add(A11, a_stride, B12, b_stride, C12, c_stride, k);
    rec_mul_add(A12, a_stride, B22, b_stride, C12, c_stride, k);

    // C21 += A21*B11 + A22*B21
    rec_mul_add(A21, a_stride, B11, b_stride, C21, c_stride, k);
    rec_mul_add(A22, a_stride, B21, b_stride, C21, c_stride, k);

    // C22 += A21*B12 + A22*B22
    rec_mul_add(A21, a_stride, B12, b_stride, C22, c_stride, k);
    rec_mul_add(A22, a_stride, B22, b_stride, C22, c_stride, k);
}

/* -------------------------
   Helpers for Strassen
   ------------------------- */
void add_block(const elem_t *X, int x_stride,
               const elem_t *Y, int y_stride,
               elem_t *T, int k)
{
    for (int i = 0; i < k; ++i) {
        const elem_t *xr = X + i * x_stride;
        const elem_t *yr = Y + i * y_stride;
        elem_t *tr = T + i * k;
        for (int j = 0; j < k; ++j) tr[j] = xr[j] + yr[j];
    }
}

void sub_block(const elem_t *X, int x_stride,
               const elem_t *Y, int y_stride,
               elem_t *T, int k)
{
    for (int i = 0; i < k; ++i) {
        const elem_t *xr = X + i * x_stride;
        const elem_t *yr = Y + i * y_stride;
        elem_t *tr = T + i * k;
        for (int j = 0; j < k; ++j) tr[j] = xr[j] - yr[j];
    }
}

/* -------------------------
   Strassen recursion
   ------------------------- */
void strassen_rec(const elem_t *A, int a_stride,
                  const elem_t *B, int b_stride,
                  elem_t *C, int c_stride,
                  int n, int threshold)
{
    if (n <= threshold) {
        // base case: iterative multiply
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                long long sum = 0;
                for (int t = 0; t < n; ++t)
                    sum += (long long)A[i*a_stride + t] * B[t*b_stride + j];
                C[i*c_stride + j] = (elem_t)sum;
            }
        }
        return;
    }

    int k = n >> 1;

    const elem_t *A11 = A;
    const elem_t *A12 = A + k;
    const elem_t *A21 = A + k * a_stride;
    const elem_t *A22 = A + k * a_stride + k;

    const elem_t *B11 = B;
    const elem_t *B12 = B + k;
    const elem_t *B21 = B + k * b_stride;
    const elem_t *B22 = B + k * b_stride + k;

    // submatrices of C
    elem_t *C11 = C;
    elem_t *C12 = C + k;
    elem_t *C21 = C + k * c_stride;
    elem_t *C22 = C + k * c_stride + k;

    // temporaries
    elem_t *S1 = alloc_mat(k);
    elem_t *S2 = alloc_mat(k);
    elem_t *M1 = alloc_mat(k);
    elem_t *M2 = alloc_mat(k);
    elem_t *M3 = alloc_mat(k);
    elem_t *M4 = alloc_mat(k);
    elem_t *M5 = alloc_mat(k);
    elem_t *M6 = alloc_mat(k);
    elem_t *M7 = alloc_mat(k);

    // M1 = (A11 + A22)*(B11 + B22)
    add_block(A11, a_stride, A22, a_stride, S1, k);
    add_block(B11, b_stride, B22, b_stride, S2, k);
    strassen_rec(S1, k, S2, k, M1, k, k, threshold);

    // M2 = (A21 + A22)*B11
    add_block(A21, a_stride, A22, a_stride, S1, k);
    strassen_rec(S1, k, B11, b_stride, M2, k, k, threshold);

    // M3 = A11*(B12 - B22)
    sub_block(B12, b_stride, B22, b_stride, S1, k);
    strassen_rec(A11, a_stride, S1, k, M3, k, k, threshold);

    // M4 = A22*(B21 - B11)
    sub_block(B21, b_stride, B11, b_stride, S1, k);
    strassen_rec(A22, a_stride, S1, k, M4, k, k, threshold);

    // M5 = (A11 + A12)*B22
    add_block(A11, a_stride, A12, a_stride, S1, k);
    strassen_rec(S1, k, B22, b_stride, M5, k, k, threshold);

    // M6 = (A21 - A11)*(B11 + B12)
    sub_block(A21, a_stride, A11, a_stride, S1, k);
    add_block(B11, b_stride, B12, b_stride, S2, k);
    strassen_rec(S1, k, S2, k, M6, k, k, threshold);

    // M7 = (A12 - A22)*(B21 + B22)
    sub_block(A12, a_stride, A22, a_stride, S1, k);
    add_block(B21, b_stride, B22, b_stride, S2, k);
    strassen_rec(S1, k, S2, k, M7, k, k, threshold);

    // combine results
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            int idx = i*k + j;
            C11[i*c_stride + j] = M1[idx] + M4[idx] - M5[idx] + M7[idx];
            C12[i*c_stride + j] = M3[idx] + M5[idx];
            C21[i*c_stride + j] = M2[idx] + M4[idx];
            C22[i*c_stride + j] = M1[idx] - M2[idx] + M3[idx] + M6[idx];
        }
    }

    free_mat(S1); free_mat(S2);
    free_mat(M1); free_mat(M2); free_mat(M3); free_mat(M4);
    free_mat(M5); free_mat(M6); free_mat(M7);
}

void recursive_mul(const elem_t *A, const elem_t *B, elem_t *C, int n) {
    rec_mul_add(A, n, B, n, C, n, n);
}

void strassen_mul(const elem_t *A, const elem_t *B, elem_t *C, int n, int threshold) {
    strassen_rec(A, n, B, n, C, n, n, threshold);
}

int main(void) {
    srand((unsigned)time(NULL));
    int sizes[] = {8, 16, 32, 64, 128,256,512,1024,2048};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    const int THRESHOLD = 64;

    printf("Benchmark (iterative, recursive, Strassen). Threshold=%d\n", THRESHOLD);
    for (int si = 0; si < num_sizes; ++si) {
        int n = sizes[si];
        printf("n = %d\n", n);

        elem_t *A = alloc_mat(n);
        elem_t *B = alloc_mat(n);
        gen_mat(A, n);
        gen_mat(B, n);

        elem_t *C_iter = alloc_mat(n);
        clock_t t0 = clock();
        iterative_mul(A, B, C_iter, n);
        clock_t t1 = clock();
        printf("  Iterative: %.4f s\n", (double)(t1 - t0) / CLOCKS_PER_SEC);

        elem_t *C_rec = alloc_mat(n);
        t0 = clock();
        recursive_mul(A, B, C_rec, n);
        t1 = clock();
        printf("  Recursive: %.4f s\n", (double)(t1 - t0) / CLOCKS_PER_SEC);

        elem_t *C_str = alloc_mat(n);
        t0 = clock();
        strassen_mul(A, B, C_str, n, THRESHOLD);
        t1 = clock();
        printf("  Strassen : %.4f s\n\n", (double)(t1 - t0) / CLOCKS_PER_SEC);

        free_mat(A); free_mat(B);
        free_mat(C_iter); free_mat(C_rec); free_mat(C_str);
    }
    return 0;
}
