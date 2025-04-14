#include <chrono>
#include <iostream>
using namespace std;

constexpr int SIZE = 1000000;

void sumRegular(int arr[], int size) {
  int sum = 0;
  for (int i = 0; i < size; i++) {
    sum += arr[i];
  }
}

void sumUnrolled(int arr[], int size) {
  int sum = 0;
  int i = 0;

  for (; i <= size - 4; i += 4) {
    sum += arr[i];
    sum += arr[i + 1];
    sum += arr[i + 2];
    sum += arr[i + 3];
  }

  for (; i < size; i++) {
    sum += arr[i];
  }
}

int main() {
  int arr[SIZE];
  for (int i = 0; i < SIZE; i++) {
    arr[i] = 1;
  }

  auto start = chrono::high_resolution_clock::now();
  sumRegular(arr, SIZE);
  auto end = chrono::high_resolution_clock::now();
  chrono::duration<double> regular_duration = end - start;
  cout << "Time taken by regular loop: " << regular_duration.count()
       << " seconds" << endl;

  start = chrono::high_resolution_clock::now();
  sumUnrolled(arr, SIZE);
  end = chrono::high_resolution_clock::now();
  chrono::duration<double> unrolled_duration = end - start;
  cout << "Time taken by unrolled loop: " << unrolled_duration.count()
       << " seconds" << endl;

  return 0;
}