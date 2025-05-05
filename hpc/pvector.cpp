#include <iostream>
#include <omp.h>
#include <vector>
#include <chrono>



using namespace std;
using namespace std::chrono;

vector<int> generateRandom(int size,int min, int max){
    vector<int> v(size);
    srand(44);

    for(int i=0;i<size;i++) {
        v[i] = rand() % (max - min + 1) + min ;
    }

    return v;
}

vector<vector<int>> generateMatrix(int row, int col, int min, int max,
                                   int seed) {
    vector<vector<int>> arr(row, vector<int>(col));
    srand(seed);

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            arr[i][j] = rand() % (max - min + 1) + min;
        }
    }
    return arr;
}

void smultiply(vector<vector<int>> &arr1, vector<int> &arr2,
              vector<vector<int>> &res, int a, int b, int c) {
    for (int i = 0; i < a; i++) {
        for (int j = 0; j < c; j++) {
            for (int k = 0; k < b; k++) {
                res[i][j] += arr1[i][k] * arr2[k];
            }
        }
    }
}

void pmultiply(vector<vector<int>> &arr1, vector<int> &arr2,
              vector<vector<int>> &res, int a, int b, int c) {

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < a; i++) {
        for (int j = 0; j < c; j++) {
            for (int k = 0; k < b; k++) {
                res[i][j] += arr1[i][k] * arr2[k];
            }
        }
    }
}

void show(vector<vector<int>> arr,int a , int b){
    for(int i=0;i<a;i++) {
        for(int j=0;j<b;j++) {
            cout<<arr[i][j]<<" ";
        }
        cout<<endl;
    }
}

int main() {
    int a = 10000, b = 10000, c = 1;

    vector<vector<int>> arr1 = generateMatrix(a, b, 1, 10, 23);
    vector<int> arr2 = generateRandom(b, 1, 1000);
    vector<vector<int>> res(a, vector<int>(c));
    vector<vector<int>> res1(a, vector<int>(c));


    auto start = high_resolution_clock::now();
    smultiply(arr1,arr2,res,a,b,c);
    auto end = high_resolution_clock::now();
    show(res,a,c);
    cout<<endl;


    auto pstart = high_resolution_clock::now();
    pmultiply(arr1,arr2,res1,a,b,c);
    auto pend = high_resolution_clock::now();
    show(res,a,c);
    cout<<endl;


    auto duration = duration_cast<milliseconds>(end-start);
    auto pduration = duration_cast<milliseconds>(pend-pstart);


    cout<<"sequetial matrix-vector multiplication: "<<duration.count()<<endl;
    cout<<"parallel matrix-vector multiplication: "<<pduration.count()<<endl;

    return 0;
}
