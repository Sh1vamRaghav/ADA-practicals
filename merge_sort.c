#include<stdio.h>
#include<stdlib.h>
#include<time.h> // clock ticks not per second

void merge(int arr[],int left,int mid,int right){
    int i,j,k;
    int n1 = mid-left+1;
    int n2 = right - mid;
    int *l = (int*)malloc(n1*sizeof(int));
    int *r = (int*)malloc(n2*sizeof(int));
    for(i = 0 ; i < n1 ; i++){
        l[i] = arr[left+i];
    }
    for(j = 0 ; j < n2; j++){
        r[j] = arr[mid+1+j];
    }
    i  = 0 , j = 0;
    k = left;
    while( i < n1 && j < n2){
        if(l[i] <= r[j]){
            arr[k] = l[i];
            i++;
        }
        else{
            arr[k] = r[j];
            j++;
        }
        k++;
    }
    while(i<n1){
        arr[k] = l[i];
        i++;
        k++;
    }
    while(j<n2){
        arr[k] = r[j];
        j++;
        k++;
    }
    free(l);
    free(r);
}

void merge_sort(int arr[] , int left , int right){
    
    if(left < right){
        int mid = left + (right - left) / 2;
        merge_sort(arr , left , mid);
        merge_sort(arr , mid+1 , right);
        merge(arr , left , mid , right);
    }
}

void generate_random_array(int arr[], int n){
    for(int i = 0 ; i < n ; i++){
        arr[i] = rand()%100000; // 0 to 99999
    }
}

int main(){
    int size[10] = {10,50,100,500,1000,3000,5000,6000,7000,8000};
    double time[10];
    for(int i = 0 ; i < 10 ; i++){
        int n = size[i];
        int *arr = (int*)malloc(n*sizeof(int));
        if(arr == NULL){
            printf("Memory allocation failed!\n");
            return 1;
        }

        clock_t total_time = 0;
        for(int i = 0 ; i < 5000 ; i++){
            generate_random_array(arr,n);
            clock_t start = clock();
            merge_sort(arr , 0 , n-1);
            clock_t end = clock();
            total_time += end-start;
        }
        double average_time = ((double)total_time) / CLOCKS_PER_SEC / 5000.0;
        time[i] = average_time;
        free(arr);
    }
    printf("xSize\tAverage Time per Search (seconds)\n");
    for(int i = 0 ; i < 10 ; i++){
        printf("%d\t%.9f\n", size[i], time[i]);
    }
    for(int i = 0 ; i < 10 ; i++){
        printf("%.8f, " ,time[i]);
    }
    return 0;
}

