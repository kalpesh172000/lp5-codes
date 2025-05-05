#include <iostream>
#include <omp.h>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

vector<int> generateRandom(int size, int min, int max) {

  vector<int> arr(size);
  srand(43);

  for (int i = 0; i < size; i++) {
    arr[i] = rand() % (max - min + 1) + min;
  }
  return arr;
}

void bubble(vector<int> &arr) {
  int n = arr.size();
  int temp;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n - i - 1; j++) {
      if (arr[j] > arr[j + 1]) {
        temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
}

void pbubble(vector<int> &arr) {
  int n = arr.size();
  int temp;

  for (int i = 0; i < n; i++) {
    bool swapped = false;
    #pragma omp parallel for shared(arr, swapped)
    for (int j = 0; j < n - i - 1; j++) {
      if (arr[j] > arr[j + 1]) {
        temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
        swapped = true;
      }
    }
  }
}

void show(vector<int> arr) {
  for (int i = 0; i < arr.size(); i++) {
    cout << arr[i] << " ";
  }
  return;
}

int main() {
  vector<int> arr = generateRandom(10000, 1, 10000);
  vector<int> arr1 = generateRandom(10000, 1, 10000);

    auto start = high_resolution_clock::now();
  bubble(arr);
    auto end = high_resolution_clock::now();


    auto pstart = high_resolution_clock::now();
  pbubble(arr1);
    auto pend = high_resolution_clock::now();
  cout << "sequential sorted array: ";
    show(arr);
  cout << "parallel sorted array: ";
    show(arr1);
  cout << endl;


    auto duration = duration_cast<milliseconds>(end-start);
    auto pduration = duration_cast<milliseconds>(pend-pstart);
    cout<<"time required by sequential bubble sort "<<duration.count()<<endl;
    cout<<"time required by parallel bubble sort "<<pduration.count()<<endl;
  return 0;
}
