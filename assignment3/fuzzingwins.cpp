#include <deepstate/DeepState.hpp>

using namespace deepstate;

void swap(int *xp, int *yp) {  
    int temp = *xp;  
    *xp = *yp;  
    *yp = temp;  
}  

void bubbleSort(int arr[], int n) {  
    int i, j;  
    for (i = 0; i < n-1; i++) {    
        // Last i elements are already in place  
        for (j = 0; j < n-i-1; j++) { 
            if (arr[j] > arr[j+1]) {
                swap(&arr[j], &arr[j+1]);  
            }
        }
    }
}  


TEST(Fuzzingwins, StrTest) {
  int array[10];
  for(int i = 0; i < 10; i++) {
    array[i] = DeepState_UInt();
  }
  // LOG(TRACE) << "String: " << inputString;
  bubbleSort(array, 10);
  ASSERT_NE(array[2], 2);
}

