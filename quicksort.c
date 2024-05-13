void swap(int* left, int* right){
    if(*left == *right) return; 
    
    int temp = *right; 
    *right = *left; 
    *left = temp; 
}

int partition(int* array, int start, int end){
    int pivot = array[end];
    int i = start;
    for(int j = start; j < end; j++){
        if(pivot <= array[j]){
            swap(&array[j], &array[i]);
            i++;
        }
    }
    swap(&array[i], &array[end]);
    return i; 
}

void quicksort(int* array, int start, int end){
    if(start >= end) return;

    int pivot = array[end]; 
    int i = partition(array, start, end);
    quicksort(array, start, i-1);
    quicksort(array, i+1, end-1);
}