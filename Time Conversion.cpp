#include <iostream>

using namespace std;

void clock(string str) {
    
    int hh = (((int)str[0] - 48) * 10) + ((int)str[1] - 48);
    
    if(str[8] == 'P' && hh < 12) {
        hh += 12;
    } else if(hh == 12 && str[8] == 'A') {
        hh = 0;
    }
    if(hh == 0) cout << '0' << hh;
    else if(hh / 10 < 1) cout << '0' << hh;
    else cout << hh;
    for(int i = 2; i < 8; i++) {
        cout << str[i];
    }
}

int main() {
    string ans;
    cin >> ans;
    clock(ans);
}
