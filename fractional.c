#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Item structure
typedef struct {
    int value, weight;
} Item;

// Comparator for greedy (value/weight ratio)
int cmp(const void* a, const void* b) {
    Item* i1 = (Item*)a;
    Item* i2 = (Item*)b;
    double r1 = (double)i1->value / i1->weight;
    double r2 = (double)i2->value / i2->weight;
    return (r2 > r1) - (r2 < r1); // sort descending
}

// Greedy: Fractional Knapsack
double fractionalKnapsack(Item arr[], int n, int W) {
    qsort(arr, n, sizeof(Item), cmp);
    int curWeight = 0;
    double finalValue = 0.0;

    for (int i = 0; i < n; i++) {
        if (curWeight + arr[i].weight <= W) {
            curWeight += arr[i].weight;
            finalValue += arr[i].value;
        } else {
            int remain = W - curWeight;
            finalValue += arr[i].value * ((double)remain / arr[i].weight);
            break;
        }
    }
    return finalValue;
}

// Generate random items
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

    // Denser sizes for smoother graph
    int sizes[] = {1000, 2000, 3000, 4000, 5000, 7000, 9000, 12000, 15000, 20000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int capacity = 10000;
    int repetitions = 5;

    double greedy_times[100];

    printf("Size\tGreedy_Avg_Time(s)\n");

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        double total_time = 0.0;

        for (int rep = 0; rep < repetitions; rep++) {
            Item* items = generate_items(n, 1000, 500);

            clock_t start = clock();
            double val = fractionalKnapsack(items, n, capacity);
            clock_t end = clock();

            total_time += ((double)(end - start)) / CLOCKS_PER_SEC;
            free(items);
        }

        double avg_time = total_time / repetitions;
        greedy_times[s] = avg_time;
        printf("%d\t%.6f\n", n, avg_time);
    }

    // Print comma-separated for plotting
    printf("\nSizes: ");
    for (int i = 0; i < num_sizes; i++) {
        printf("%d", sizes[i]);
        if (i < num_sizes - 1) printf(",");
    }

    printf("\nGreedy Times: ");
    for (int i = 0; i < num_sizes; i++) {
        printf("%.6f", greedy_times[i]);
        if (i < num_sizes - 1) printf(",");
    }
    printf("\n");

    return 0;
}
