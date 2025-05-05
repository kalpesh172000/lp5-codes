#include <algorithm>
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

int partition(vector<int>& arr, int low, int high){
    int pivot = arr[high];
    int i = low -1 ;
    int temp;
    
    for(int j = low; j < high; j++){
        if(arr[j] <= pivot){
            i++;
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    temp= arr[i+1];
    arr[i+1] = arr[high];
    arr[high] = temp;
    return i+1;
}

void squick(vector<int>& arr,int low, int high){
    if(low>=high)
        return;

    int pi = partition(arr, low, high);

    squick(arr, low, pi-1);
    squick(arr, pi+1, high);
}

void pquick(vector<int>& arr,int low, int high){
    if(low>=high)
        return;

    int pi = partition(arr, low, high);

    #pragma omp parallel sections
    {
        #pragma omp section
        squick(arr, low, pi-1);

        #pragma omp section
        squick(arr, pi+1, high);
    }
}

void show(vector<int> arr) {
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    return;
}

int main() {
    vector<int> arr = generateRandom(100000000, 1, 100000000);
    vector<int> arr1 = generateRandom(100000000, 1, 100000000);

    auto start = high_resolution_clock::now();
    squick(arr, 0, arr.size()-1);
    auto end = high_resolution_clock::now();

    omp_set_num_threads(10);
    auto pstart = high_resolution_clock::now();
    pquick(arr1, 0, arr1.size()-1);

    auto pend = high_resolution_clock::now();
    cout << "sequential sorted array: ";
    show(arr);
    cout << "parallel sorted array: ";
      show(arr1);
    cout << endl;

    auto duration = duration_cast<milliseconds>(end - start);
    auto pduration = duration_cast<milliseconds>(pend-pstart);
    cout << "time required by sequential quick sort " << duration.count()
         << endl;
    cout<<"time required by parallel quick sort "<<pduration.count()<<endl;

    return 0;
}
