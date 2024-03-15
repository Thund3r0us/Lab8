/* 
Lab 8 Nicole Walsh
3/15/24
*/

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz) {
  extraMemoryAllocated += sz;
  size_t* ret = malloc(sizeof(size_t) + sz);
  *ret = sz;
  //printf("Extra memory allocated, size: %ld\n", sz);
  return &ret[1];
}


void DeAlloc(void* ptr) {
  size_t* pSz = (size_t*)ptr - 1;
  extraMemoryAllocated -= *pSz;
  //printf("Extra memory deallocated, size: %ld\n", *pSz);
  free((size_t*)ptr - 1);
}


size_t Size(void* ptr) {
  return ((size_t*)ptr)[-1];
}

// Merge the two subarrays into one
void merge(int arr[], int temp[], int left, int mid, int right) {
  int i = left;
  int j = mid + 1;
  int k = 0;

  // Merge the subarrays into temp array
  while (i <= mid && j <= right) {
    if (arr[i] <= arr[j]) {
      temp[k] = arr[i];
      i++;
    } 
    else {
      temp[k] = arr[j];
      j++;
    }
    k++;
  }

  // Copy the remaining elements of left subarray
  while (i <= mid) {
    temp[k] = arr[i];
    i++;
    k++;
  }

  // Copy the remaining elements of right subarray
  while (j <= right) {
    temp[k] = arr[j];
    j++;
    k++;
  }

  // Copy the merged elements back to original array
  for (i = 0; i < k; i++)
    arr[left + i] = temp[i];
}


// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r) {
  if (l < r) {
    int mid = l + (r - l) / 2;

    // Allocate memory for temporary array
    int *temp = (int *)Alloc((r - l + 1) * sizeof(int));

    // Sort first and second halves
    mergeSort(pData, l, mid);
    mergeSort(pData, mid + 1, r);

    // Merge the sorted halves
    merge(pData, temp, l, mid, r);

    // Deallocate memory for temporary array
    DeAlloc(temp);
  }
}


// parses input file to an integer array
int parseData(char *inputFileName, int **ppData) {
  FILE* inFile = fopen(inputFileName,"r");
  int dataSz = 0;
  int i, n, *data;
  *ppData = NULL;

  if (inFile) {
    fscanf(inFile,"%d\n",&dataSz);
    *ppData = (int *)malloc(sizeof(int) * dataSz);
    // Implement parse data block
    if (*ppData == NULL) {
      printf("Cannot allocate memory\n");
      exit(-1);
    }
    for (i=0;i<dataSz;++i) {
      fscanf(inFile, "%d ",&n);
      data = *ppData + i;
      *data = n;
    }

    fclose(inFile);
  }

  return dataSz;
}


// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz) {
  int i, sz = dataSz - 100;
  printf("\tData:\n\t");

  // If array is less than 100 elements
  if (dataSz < 100) {
    for (i = 0; i < dataSz; i++) {
      printf("%d ",pData[i]);
    }
    printf("\n");
  }
    
  else {
    for (i=0;i<100;++i) {
      printf("%d ",pData[i]);
    }
    printf("\n\t");
  
    for (i=sz;i<dataSz;++i) {
      printf("%d ",pData[i]);
    }
    printf("\n\n");
  }
}


int main(void) {
  clock_t start, end;
  int i;
    double cpu_time_used;
  char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };

  for (i=0;i<4;++i) {
    int *pDataSrc, *pDataCopy;
    int dataSz = parseData(fileNames[i], &pDataSrc);

    if (dataSz <= 0)
      continue;

    pDataCopy = (int *)malloc(sizeof(int)*dataSz);

    printf("---------------------------\n");
    printf("Dataset Size : %d\n",dataSz);
    printf("---------------------------\n");

    printf("Merge Sort:\n");
    memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
    extraMemoryAllocated = 0;
    start = clock();
    mergeSort(pDataCopy, 0, dataSz - 1);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
    printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

    free(pDataCopy);
    free(pDataSrc);
  }

}
