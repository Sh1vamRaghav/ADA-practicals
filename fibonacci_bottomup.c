#include <stdio.h>
#include <time.h>

#define MOD 1000000007LL

// Bottom-Up Fibonacci (iterative), modulo to prevent overflow
long long fib_iter_large(long long n) {
    if (n <= 1) return n;
    long long prev = 0, curr = 1, next;
    for (long long i = 2; i <= n; i++) {
        next = (prev + curr) % MOD;
        prev = curr;
        curr = next;
    }
    return curr;
}

int main() {
    long long size[20] = {
        1000000, 5000000, 10000000, 20000000, 30000000,
        40000000, 50000000, 60000000, 70000000, 80000000,
        90000000, 100000000, 150000000, 200000000, 250000000,
        300000000, 400000000, 500000000, 750000000, 1000000000
    };
    double time_taken[20];

    for (int i = 0; i < 20; i++) {
        long long n = size[i];

        clock_t start = clock();
        long long result = fib_iter_large(n); // compute Fibonacci (mod MOD)
        clock_t end = clock();

        double elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        time_taken[i] = elapsed_time;

        printf("fib(%lld) computed modulo %lld\n", n, MOD);
    }

    printf("\n");
    printf("n\tTime(seconds)\n");
    for (int i = 0; i < 20; i++) {
        printf("%lld\t%.9f\n", size[i], time_taken[i]);
    }

    printf("\nCSV-like output:\n");
    for (int i = 0; i < 20; i++) {
        printf("%.8f, ", time_taken[i]);
    }
    printf("\n");

    return 0;
}
