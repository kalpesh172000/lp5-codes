#include <climits>
#include<iostream>
#include<omp.h>
#include <vector>
#include<chrono>


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

void minmaxsum(vector<int> &array){
    int minv = INT_MAX;
    int maxv = INT_MIN;
    int sum = 0;
    
    int size = array.size();


    for(int i=0;i<size;i++){
        cout<<array[i]<<" ";
        minv = min(minv, array[i]);
        maxv = max(maxv, array[i]);

        sum += array[i];
    }
    cout<<endl;

    int avg = sum / size;
    cout<<"Normal min max "<<endl;
    cout<<"Min : "<<minv<<endl;
    cout<<"Max : "<<maxv<<endl;
    cout<<"Sum : "<<sum<<endl;
    cout<<"Avg : "<<avg<<endl<<endl;
}

void preduce(vector<int> &array){
    int minv = INT_MAX;
    int maxv = INT_MIN;
    int sumv = 0;
    
    int size = array.size();


    #pragma omp parallel for reduction(min: minv) reduction(max:maxv) reduction(+: sumv)
    for(int i=0;i<size;i++){
        minv = min(minv, array[i]);
        maxv = max(maxv, array[i]);

        sumv += array[i];
    }
    cout<<endl;

    int avg = sumv / size;
    cout<<"Normal min max "<<endl;
    cout<<"Min : "<<minv<<endl;
    cout<<"Max : "<<maxv<<endl;
    cout<<"Sum : "<<sumv<<endl;
    cout<<"Avg : "<<avg<<endl<<endl;
}


int main(){
    vector<int> array = generateRandom(100000000, 1, 10000);
    auto start = high_resolution_clock::now(); 
    minmaxsum(array);
    auto end = high_resolution_clock::now(); 

    auto pstart = high_resolution_clock::now(); 
    preduce(array);
    auto pend = high_resolution_clock::now(); 



    auto duration = duration_cast<milliseconds>(end-start);
    auto pduration = duration_cast<milliseconds>(pend-pstart);

    cout<<"time required for normal minmaxsum "<<duration.count()<<endl;
    cout<<"time required for parallel minmaxsum "<<pduration.count()<<endl;
    return 0;
}

