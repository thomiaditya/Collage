#include <iostream>

using namespace std;
int main() {
    long long int arr[5], min, max;
    
    for(int i = 0; i < 5; i++) {
        cin >> arr[i];
    }
    
    for(int i = 0; i < 5; i++) {
        long long int sum = 0;
        for(int j = 0; j < 5; j++) {
            if(i != j) {
                sum += arr[j];
            }
        }
        if(i == 0) {
            max = sum;
            min = sum;
        }
        
        if(sum > max) max = sum;
        if(sum < min) min = sum;
    }
    
    cout << min << " " << max;
}
