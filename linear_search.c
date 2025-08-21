#include<stdio.h>
#include<time.h>
#include<stdlib.h>

int linear_search(int arr[] , int target , int size){
    for(int i = 0 ; i < size ; i++){
        if(arr[i] == target) return i;
    }
    return -1;
}

void generate_random_array(int arr[] , int n){
    for(int i = 0 ; i < n ; i++){
        arr[i] = rand()%100000; // 0 to 99999
    }
}

int main(){
    int elements[10] = {10,50,100,500,1000,3000,5000,6000,7000,8000};
    double time_taken[10];

    for(int i = 0 ; i < 10 ; i++){
        double total_time = 0;
        int size = elements[i];
        int *arr = (int*)malloc(size*sizeof(int));
        if(arr == NULL){
            printf("Memory allocation failed!\n");
            return 1;
        }

        generate_random_array(arr,size);
        clock_t start = clock();
        for(int j = 0 ; j < 100000 ; j++){
            linear_search(arr , rand()%100000 , size);
        }
        clock_t end = clock();

        total_time += ((double)(end-start))/CLOCKS_PER_SEC;
        total_time /= 100000.0;
        time_taken[i] = total_time;
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