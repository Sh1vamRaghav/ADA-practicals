#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int value, weight;
} Item;

int knapSackDP(int W, Item arr[], int n) {
    int **K = (int**)malloc((n + 1) * sizeof(int*));
    for (int i = 0; i <= n; i++)
        K[i] = (int*)malloc((W + 1) * sizeof(int));

    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (i == 0 || w == 0)
                K[i][w] = 0;
            else if (arr[i - 1].weight <= w) {
                int include = arr[i - 1].value + K[i - 1][w - arr[i - 1].weight];
                int exclude = K[i - 1][w];
                K[i][w] = (include > exclude) ? include : exclude;
            } else {
                K[i][w] = K[i - 1][w];
            }
        }
    }

    int result = K[n][W];
    for (int i = 0; i <= n; i++) free(K[i]);
    free(K);
    return result;
}

Item* generate_items(int n, int maxValue, int maxWeight) {
    Item* arr = (Item*)malloc(n * sizeof(Item));
    for (int i = 0; i < n; i++) {
        arr[i].value = rand() % maxValue + 1;
        arr[i].weight = rand() % maxWeight + 1;
    }
    return arr;
}

int main() {
    srand(time(NULL));

    // Same sizes as Fractional Knapsack
    int sizes[] = {1000, 2000, 3000, 4000, 5000, 7000, 9000, 12000, 15000, 20000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int capacity = 10000;   // moderate capacity
    int repetitions = 1;    // single run because DP is heavy

    double dp_times[100];

    printf("Size\tDP_Time(s)\n");

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        Item* items = generate_items(n, 1000, 500);

        clock_t start = clock();
        int val = knapSackDP(capacity, items, n);
        clock_t end = clock();

        double time_sec = ((double)(end - start)) / CLOCKS_PER_SEC;
        dp_times[s] = time_sec;

        printf("%d\t%.6f\n", n, time_sec);

        free(items);
    }

    // Print comma-separated times for graphing
    printf("\nSizes: ");
    for (int i = 0; i < num_sizes; i++) {
        printf("%d", sizes[i]);
        if (i < num_sizes - 1) printf(",");
    }

    printf("\nDP Times: ");
    for (int i = 0; i < num_sizes; i++) {
        printf("%.6f", dp_times[i]);
        if (i < num_sizes - 1) printf(",");
    }
    printf("\n");

    return 0;
}
