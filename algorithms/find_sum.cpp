#include <iostream>
using namespace std;
 
void printResult(int start,int end) {
    cout<<"[";
    for(int i=start;i<=end;i++) {
        cout<<i<<",";
    }
    cout<<"]"<<endl;
}
 
void findSumIsN(int n) {
    cout << std::endl << "SUM is " << n << ": " << std::endl;
    int start=1,end=2;
    int sum=start+end;;
    while(start<(n+1)/2) {
        if(sum==n) {
            printResult(start, end);   
        }
        while(sum<n) {
            end++;
            sum+=end;
            if(sum==n){
                printResult(start,end);
            }
        }
        sum-=start;
        start++;
    }
 } 
 
int main(int argc, char** argv) {
    findSumIsN(3);
    findSumIsN(10);
    findSumIsN(15);
    findSumIsN(20);
    findSumIsN(28);
    findSumIsN(100);
    findSumIsN(1000);
    findSumIsN(1024);
    findSumIsN(2048);
    findSumIsN(4096);
    findSumIsN(8192);
    findSumIsN(16384);
    findSumIsN(20000);
    return 0;
}
