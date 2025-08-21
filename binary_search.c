#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int binary_search(int arr[], int target, int size) {
    int low = 0, high = size - 1;
    while (high >= low) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == target) return mid;
        else if (arr[mid] > target) high = mid - 1;
        else low = mid + 1;
    }
    return -1;
}

void generate_random_array(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100000;
    }
}

int main() {
    int elements[10] = {10, 50, 100, 500, 1000, 3000, 5000, 6000, 7000, 8000};
    double time_taken[10];
    srand(time(NULL));

    for (int i = 0; i < 10; i++) {
        int size = elements[i];
        int* arr = (int*)malloc(size * sizeof(int));
        if (arr == NULL) {
            printf("Memory allocation failed!\n");
            return 1;
        }

        generate_random_array(arr, size);
        qsort(arr, size, sizeof(int), compare);  

        clock_t start_time = clock();
        for (int j = 0; j < 100000; j++) {
            binary_search(arr, rand() % 100000, size);
        }
        clock_t end_time = clock();

        double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        time_taken[i] = elapsed_time / 100000.0; 
        free(arr);
    }

    printf("\nSize\tAverage Time per Search (seconds)\n");
    for(int i = 0 ; i < 10 ; i++){
        printf("%d\t%.9f\n", elements[i], time_taken[i]);
    }

    // below is code to copy to python array easily...
    // printf("\n");
    // for(int i = 0 ; i < 10 ; i++){
    //     printf("%.9f," , time_taken[i]);
    // }

    return 0;
}
