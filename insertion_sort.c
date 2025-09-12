#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void insertion_sort(int arr[], int n){
    for(int i = 1; i < n; i++){
        int key = arr[i];
        int j = i - 1;
        while(j >= 0 && arr[j] > key){
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void generate_random_array(int arr[], int n){
    for(int i = 0; i < n; i++){
        arr[i] = rand() % 100000;
    }
}

int main(){
    int size[10] = {10, 50, 100, 500, 1000, 3000, 5000, 6000, 7000, 8000};
    double time[10];

    for(int i = 0; i < 10; i++){
        int n = size[i];
        int *arr = (int*)malloc(n * sizeof(int));
        if(arr == NULL){
            printf("Memory allocation failed!\n");
            return 1;
        }

        clock_t total_time = 0;

        for(int j = 0; j < 5000; j++){
            generate_random_array(arr, n);
            clock_t start = clock();
            insertion_sort(arr, n);
            clock_t end = clock();
            total_time += (end - start);
        }

        double average_time = ((double)total_time) / CLOCKS_PER_SEC / 5000.0;
        time[i] = average_time;
        free(arr);
    }

    printf("xSize\tAverage Time(seconds)\n");
    for(int i = 0 ; i < 10 ; i++){
        printf("%d\t%.9f\n", size[i], time[i]);
    }
    for(int i = 0 ; i < 10 ; i++){
        printf("%.8f, " ,time[i]);
    }

    return 0;
}
