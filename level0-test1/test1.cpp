//#include <bits/stdc++.h>  //��֪�����Ի����ܲ�����
#include <iostream>
#include <string>

using namespace std;

int main() {
    string a;
    while (1) {
        cout << "������: ";
        getline(cin, a);//����cin��ֹ�����ո�ʱը��
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