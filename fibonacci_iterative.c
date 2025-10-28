#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long fib_iter(int n) {
    if (n <= 1) return n;

    long long prev = 0, curr = 1, next;
    for (int i = 2; i <= n; i++) {
        next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}

int main() {
    int size[20] = {
        5, 7, 9, 11, 13,
        15, 17, 19, 21, 23,
        25, 28, 31, 34, 37,
        40, 43, 46, 48, 50
    };
    double time_taken[20];

    for (int i = 0; i < 20; i++) {
        int n = size[i];

        clock_t start = clock();
        long long result = fib_iter(n);  
        clock_t end = clock();

        double elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        time_taken[i] = elapsed_time;
    }

    printf("\n");
    printf("n\tTime(seconds)\n");
    for (int i = 0; i < 20; i++) {
        printf("%d\t%.9f\n", size[i], time_taken[i]);
    }

    printf("\nCSV-like output:\n");
    for (int i = 0; i < 20; i++) {
        printf("%.8f, ", time_taken[i]);
    }
    printf("\n");

    return 0;
}
