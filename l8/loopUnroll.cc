#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000000

void sumRegular(int arr[], int size) {
  int sum = 0;
  for (int i = 0; i < size; i++) {
    sum += arr[i];
  }
  printf("\nRegular sum: %d\n", sum);
}

void sumUnrolled(int arr[], int size) {
  int sum = 0;
  int i = 0;

  for (; i <= size - 8; i += 8) {
    sum += arr[i];
    sum += arr[i + 1];
    sum += arr[i + 2];
    sum += arr[i + 3];
    sum += arr[i + 4];
    sum += arr[i + 5];
    sum += arr[i + 6];
    sum += arr[i + 7];
  }

  for (; i < size; i++) {
    sum += arr[i];
  }
  printf("\nUnrolled sum: %d\n", sum);
}

int main() {
  int arr[SIZE];
  for (int i = 0; i < SIZE; i++) {
    arr[i] = 1;
  }

  clock_t start, end;

  start = clock();
  sumRegular(arr, SIZE);
  end = clock();
  double regular_duration = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
  printf("Time taken by regular loop: %.6f ms\n", regular_duration);

  start = clock();
  sumUnrolled(arr, SIZE);
  end = clock();
  double unrolled_duration = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
  printf("Time taken by unrolled loop: %.6f ms\n", unrolled_duration);

  printf("Time saved by unrolling: %.6f ms\n",
         regular_duration - unrolled_duration);

  return 0;
}