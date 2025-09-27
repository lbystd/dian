#include <iostream>
#include <map>
#include <string>
#include <fstream>


using namespace std;
#define ll long long int

int main() {
    map <string, string> data;//logN �ܿ�һ�� 
    string line, key, val;
    ll pos;
    ifstream fin("data.txt");
    while (getline(fin, line)) {//getline���ո� 
        if (line.empty()) continue;// ��������
        pos = line.find(':');// ����ð��λ�ò���֤��ʽ
        //find()δ�ҵ�����-1,�����ޣ����� 
        if (pos != -1 && pos > 0 && pos < line.size() - 1) {//�����ޣ����У�������ǰ������� 
            key = line.substr(0, pos);
            val = line.substr(pos + 1);
//            if (key.size() <= 10 && val.size() <= 10)
                data[key] = val;
        }
    }
    string a;
    while (1) {
        cout << "�����룺";
        getline(cin, a);
        if (a == "Quit") break;
        if (data.count(a)) {
            cout << data[a] << endl;
        } else {
            cout << "Error" << endl;
        }
    }

    return 0;
}

