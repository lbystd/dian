//#include <bits/stdc++.h>  //不知到测试环境能不能用
#include <iostream>
#include <string>

using namespace std;

int main() {
    string a;
    while (1) {
        cout << "请输入: ";
        getline(cin, a);//不用cin防止读到空格时炸缸
        if (a == "Dian") {
            cout << "2002" << endl;
        }
        else if (a == "Quit") {
            break;
        }
        else {
            cout << "Error" << endl;
        }
    }
    return 0;
}