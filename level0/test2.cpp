#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;
#define ll long long int

int main() {
    map<string, string> data;//logN 能快一点 
    string line, key, val;
    ll pos;
    ifstream fin("data.txt");
    while (getline(fin, line)) {//getline防空格 
        // 跳过空行
        if (line.empty()) continue;
        // 查找冒号位置并验证格式
        pos = line.find(':');
        //find()未找到返回-1,处理无：空行 
        if (pos != -1 && pos > 0 && pos < line.size() - 1) {//处理无：空行，：在最前最后的情况 
            key = line.substr(0, pos);
            val = line.substr(pos + 1);
            // 检查长度限制
//            if (key.size() <= 10 && val.size() <= 10)
                data[key] = val;
        }
    }

    // 交互查询
    while (true) {
        cout << "请输入：";
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

