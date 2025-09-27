#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;
#define ll long long int

int main() {
    map<string, string> data;//logN �ܿ�һ�� 
    string line, key, val;
    ll pos;
    ifstream fin("data.txt");
    while (getline(fin, line)) {//getline���ո� 
        // ��������
        if (line.empty()) continue;
        // ����ð��λ�ò���֤��ʽ
        pos = line.find(':');
        //find()δ�ҵ�����-1,�����ޣ����� 
        if (pos != -1 && pos > 0 && pos < line.size() - 1) {//�����ޣ����У�������ǰ������� 
            key = line.substr(0, pos);
            val = line.substr(pos + 1);
            // ��鳤������
//            if (key.size() <= 10 && val.size() <= 10)
                data[key] = val;
        }
    }

    // ������ѯ
    while (true) {
        cout << "�����룺";
        cin >> key;
        if (key == "Quit") break;
        if (data.count(key)) {
            cout << data[key] << endl;
        } else {
            cout << "Error" << endl;
        }
    }

    return 0;
}

