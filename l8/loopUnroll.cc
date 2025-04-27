#include <chrono>
#include <iostream>
using namespace std;

constexpr int SIZE = 1000000;

void sumRegular(int arr[], int size) {
  int sum = 0;
  for (int i = 0; i < size; i++) {
    sum += arr[i];
  }
  cout << "\nRegular sum: " << sum << endl;
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
  cout << "\nUnrolled sum: " << sum << endl;
}

int main() {
  int arr[SIZE];
  for (int i = 0; i < SIZE; i++) {
    arr[i] = 1;
  }

  auto start = chrono::high_resolution_clock::now();
  sumRegular(arr, SIZE);
  auto end = chrono::high_resolution_clock::now();
  chrono::duration<double, milli> regular_duration = end - start;
  cout << "Time taken by regular loop: " << regular_duration.count() << " ms"
       << endl;

  start = chrono::high_resolution_clock::now();
  sumUnrolled(arr, SIZE);
  end = chrono::high_resolution_clock::now();
  chrono::duration<double, milli> unrolled_duration = end - start;
  cout << "Time taken by unrolled loop: " << unrolled_duration.count() << " ms"
       << endl;

  cout << "Time saved by unrolling: "
       << regular_duration.count() - unrolled_duration.count() << " ms" << endl;

  return 0;
}