#include <chrono>
#include <iostream>
#include <omp.h>
#include <vector>

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

void smerge(vector<int> &arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];


    int i=0, j=0, k=left;

    while(i<n1 && j < n2){
        if(L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];

    }


    while(i < n1) arr[k++] = L[i++];
    while(j < n2) arr[k++] = R[j++];
}

void smergeSort(vector<int> &arr, int left, int right) {
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    smergeSort(arr, left, mid);
    smergeSort(arr, mid + 1, right);
    smerge(arr, left, mid, right);
}

void pmergeSort(vector<int> &arr, int left, int right, int depth = 0) {
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    if(depth < 4)
    {
        #pragma omp task shared(arr)
        pmergeSort(arr, left, mid, depth + 1);

        #pragma omp task shared(arr)
        pmergeSort(arr, mid+1, right, depth + 1);

        #pragma omp taskwait

    }else {
        pmergeSort(arr, left, mid, depth+1);

        pmergeSort(arr, mid + 1, right, depth+1);
    }

    smerge(arr, left, mid, right);
}

void show(vector<int> arr) {
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    return;
}

int main() {
    vector<int> arr = generateRandom(1000000, 1, 1000000);
    vector<int> arr1 = generateRandom(1000000, 1, 1000000);

    auto start = high_resolution_clock::now();
    smergeSort(arr, 0, arr.size()-1);
    auto end = high_resolution_clock::now();

      auto pstart = high_resolution_clock::now();
    #pragma omp parallel
    {
        #pragma omp single
        pmergeSort(arr1, 0, arr1.size()-1);
    }

      auto pend = high_resolution_clock::now();
    cout << "sequential sorted array: ";
    show(arr);
    cout << "parallel sorted array: ";
      show(arr1);
    cout << endl;

    auto duration = duration_cast<milliseconds>(end - start);
    auto pduration = duration_cast<milliseconds>(pend-pstart);
    cout << "time required by sequential merge sort " << duration.count()
         << endl;
    cout<<"time required by parallel merge sort "<<pduration.count()<<endl;
    
    return 0;
}
