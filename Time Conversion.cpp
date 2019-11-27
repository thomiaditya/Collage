#include <iostream>

using namespace std;

void clock(string str) {
	
	int hh = (((int)str[0] - 48) * 10) + ((int)str[1] - 48);
	
	if(str[8] == 'P') {
		hh = hh + 12 == 24 ? 0: hh + 12;
	}
	if(hh == 0) cout << '0' << hh;
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
