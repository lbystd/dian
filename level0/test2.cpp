#include <iostream>
#include <map>
#include <string>
#include <fstream>


using namespace std;
#define ll long long int

int main() {
    map <string, string> data;//logN 能快一点 
    string line, key, val;
    ll pos;
    ifstream fin("data.txt");
    while (getline(fin, line)) {//getline防空格 
        if (line.empty()) continue;// 跳过空行
        pos = line.find(':');// 查找冒号位置并验证格式
        //find()未找到返回-1,处理无：空行 
        if (pos != -1 && pos > 0 && pos < line.size() - 1) {//处理无：空行，：在最前最后的情况 
            key = line.substr(0, pos);
            val = line.substr(pos + 1);
//            if (key.size() <= 10 && val.size() <= 10)
                data[key] = val;
        }
    }
    string a;
    while (1) {
        cout << "请输入：";
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

