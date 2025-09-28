#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <vector>
#include <algorithm>

using namespace std;

const string dat = "library_data.txt";
const string dys[] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };

// --- 数据结构定义 ---
struct Seat {
    char user = '0'; // '0' 表示空闲
};

struct Row {
    vector<Seat> seats;
};

struct Floor {
    vector<Row> rows;
};

// --- 辅助函数 ---
int getday(string d) {
    for (int i = 0; i < 7; i++) {
        if (d == dys[i]) return i;
    }
    return -1;
}

bool validuser(string u) {
    if (u == "Admin") return true;
    if (u.size() == 1 && u[0] >= 'A' && u[0] <= 'Z') return true;
    return false;
}

// --- 数据加载与保存（核心修改：默认初始化5层4×4）---
void load(vector<Floor> library[7]) {
    // 1. 先初始化默认结构：7天通用5层，每层4排4列
    for (int day = 0; day < 7; day++) {
        library[day].resize(5); // 默认5层
        for (int fl_idx = 0; fl_idx < 5; fl_idx++) { // 遍历5层
            library[day][fl_idx].rows.resize(4); // 每层4排
            for (int row_idx = 0; row_idx < 4; row_idx++) { // 遍历4排
                library[day][fl_idx].rows[row_idx].seats.resize(4); // 每排4列
            }
        }
    }

    // 2. 若存在数据文件，加载文件内容覆盖默认（保留用户历史配置）
    ifstream fin(dat);
    if (!fin.is_open()) {
        cout << "未找到历史数据文件，已初始化默认结构：5层，每层4×4座位" << endl;
        return;
    }

    string type, day_str;
    int day_idx, floor_num, row_num, col_num;
    char user;

    while (fin >> type) {
        if (type == "FLOOR") {
            fin >> day_str >> floor_num;
            day_idx = getday(day_str);
            if (day_idx != -1 && library[day_idx].size() < floor_num) {
                library[day_idx].resize(floor_num);
            }
        }
        else if (type == "ROW") {
            fin >> day_str >> floor_num >> row_num;
            day_idx = getday(day_str);
            if (day_idx != -1 && floor_num > 0 && floor_num <= library[day_idx].size()) {
                Floor& fl = library[day_idx][floor_num - 1];
                if (fl.rows.size() < row_num) {
                    fl.rows.resize(row_num);
                }
            }
        }
        else if (type == "SEAT") {
            fin >> day_str >> floor_num >> row_num >> col_num;
            day_idx = getday(day_str);
            if (day_idx != -1 && floor_num > 0 && floor_num <= library[day_idx].size()) {
                Floor& fl = library[day_idx][floor_num - 1];
                if (row_num > 0 && row_num <= fl.rows.size()) {
                    Row& rw = fl.rows[row_num - 1];
                    if (rw.seats.size() < col_num) {
                        rw.seats.resize(col_num);
                    }
                }
            }
        }
        else if (type == "BOOK") {
            fin >> day_str >> floor_num >> row_num >> col_num >> user;
            day_idx = getday(day_str);
            if (day_idx != -1 && floor_num > 0 && floor_num <= library[day_idx].size()) {
                Floor& fl = library[day_idx][floor_num - 1];
                if (row_num > 0 && row_num <= fl.rows.size()) {
                    Row& rw = fl.rows[row_num - 1];
                    if (col_num > 0 && col_num <= rw.seats.size()) {
                        rw.seats[col_num - 1].user = user;
                    }
                }
            }
        }
    }
    fin.close();
    cout << "已加载历史数据，当前基础结构：5层，每层4×4座位（可通过管理员功能修改）" << endl;
}

void save(vector<Floor> library[7]) {
    ofstream fout(dat);
    for (int d = 0; d < 7; d++) {
        for (size_t f_idx = 0; f_idx < library[d].size(); f_idx++) {
            fout << "FLOOR " << dys[d] << " " << (f_idx + 1) << endl;
            Floor& fl = library[d][f_idx];
            for (size_t r_idx = 0; r_idx < fl.rows.size(); r_idx++) {
                fout << "ROW " << dys[d] << " " << (f_idx + 1) << " " << (r_idx + 1) << endl;
                Row& rw = fl.rows[r_idx];
                for (size_t c_idx = 0; c_idx < rw.seats.size(); c_idx++) {
                    fout << "SEAT " << dys[d] << " " << (f_idx + 1) << " " << (r_idx + 1) << " " << (c_idx + 1) << endl;
                    if (rw.seats[c_idx].user != '0') {
                        fout << "BOOK " << dys[d] << " " << (f_idx + 1) << " " << (r_idx + 1) << " " << (c_idx + 1) << " " << rw.seats[c_idx].user << endl;
                    }
                }
            }
        }
    }
    fout.close();
}

// --- 管理员功能函数（不变，保留动态增删）---
void clearAllReservations(vector<Floor> library[7]) {
    for (int d = 0; d < 7; d++) {
        for (auto& floor : library[d]) {
            for (auto& row : floor.rows) {
                for (auto& seat : row.seats) {
                    seat.user = '0';
                }
            }
        }
    }
    cout << "所有预约数据已清空。" << endl;
}

void adminReserve(vector<Floor> library[7]) {
    string user, day_str, fkey, skey;
    int floor_num, row_num, col_num;
    cout << "请输入预约信息（格式：用户名 日期 Floor 楼层 Seat 排 列，示例：A Monday Floor 1 Seat 1 2）：";
    cin >> user >> day_str >> fkey >> floor_num >> skey >> row_num >> col_num;

    if (!validuser(user) || user == "Admin") {
        cout << "无效的用户名，不能为 Admin 或非单个大写字母的用户预约。" << endl;
        return;
    }
    if (fkey != "Floor" || skey != "Seat") {
        cout << "预约指令格式错误，需包含 Floor 和 Seat 关键词，示例：A Monday Floor 1 Seat 1 2" << endl;
        return;
    }

    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "无效的日期，需输入完整英文星期（如 Monday/Tuesday）。" << endl;
        return;
    }

    if (floor_num < 1 || floor_num >(int)library[day_idx].size()) {
        cout << "无效的楼层，当前该日期下最大楼层为 " << library[day_idx].size() << "。" << endl;
        return;
    }
    Floor& fl = library[day_idx][floor_num - 1];
    if (row_num < 1 || row_num >(int)fl.rows.size()) {
        cout << "无效的排号，当前该楼层下最大排号为 " << fl.rows.size() << "。" << endl;
        return;
    }
    Row& rw = fl.rows[row_num - 1];
    if (col_num < 1 || col_num >(int)rw.seats.size()) {
        cout << "无效的列号，当前该排下最大列号为 " << rw.seats.size() << "（该座位不存在）。" << endl;
        return;
    }

    if (rw.seats[col_num - 1].user == '0') {
        rw.seats[col_num - 1].user = user[0];
        cout << "OK，已为用户 " << user << " 完成预约。" << endl;
    }
    else {
        cout << "该座位已被预约，当前预约用户为 " << rw.seats[col_num - 1].user << "。" << endl;
    }
}

void adminCancel(vector<Floor> library[7]) {
    string day_str, fkey, skey;
    int floor_num, row_num, col_num;
    cout << "请输入要取消的预约信息（格式：日期 Floor 楼层 Seat 排 列，示例：Monday Floor 1 Seat 1 2）：";
    cin >> day_str >> fkey >> floor_num >> skey >> row_num >> col_num;

    if (fkey != "Floor" || skey != "Seat") {
        cout << "取消指令格式错误，需包含 Floor 和 Seat 关键词，示例：Monday Floor 1 Seat 1 2" << endl;
        return;
    }

    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "无效的日期，需输入完整英文星期（如 Monday/Tuesday）。" << endl;
        return;
    }

    if (floor_num < 1 || floor_num >(int)library[day_idx].size()) { cout << "无效的楼层，当前该日期下最大楼层为 " << library[day_idx].size() << "。" << endl; return; }
    Floor& fl = library[day_idx][floor_num - 1];
    if (row_num < 1 || row_num >(int)fl.rows.size()) { cout << "无效的排号，当前该楼层下最大排号为 " << fl.rows.size() << "。" << endl; return; }
    Row& rw = fl.rows[row_num - 1];
    if (col_num < 1 || col_num >(int)rw.seats.size()) { cout << "无效的列号，当前该排下最大列号为 " << rw.seats.size() << "（该座位不存在）。" << endl; return; }

    if (rw.seats[col_num - 1].user != '0') {
        char usr = rw.seats[col_num - 1].user;
        rw.seats[col_num - 1].user = '0';
        cout << "OK，已取消用户 " << usr << " 的预约。" << endl;
    }
    else {
        cout << "该座位当前未被预约，无需取消。" << endl;
    }
}

void clearDayReservations(vector<Floor> library[7]) {
    string day_str;
    cout << "请输入要清空预约的日期（需输入完整英文星期，如 Monday/Tuesday）：";
    cin >> day_str;
    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "无效的日期，需输入完整英文星期（如 Monday/Tuesday）。" << endl;
        return;
    }
    for (auto& floor : library[day_idx]) {
        for (auto& row : floor.rows) {
            for (auto& seat : row.seats) {
                seat.user = '0';
            }
        }
    }
    cout << day_str << " 的所有预约已清空。" << endl;
}

void clearFloorReservations(vector<Floor> library[7]) {
    string day_str;
    int floor_num;
    cout << "请输入要清空预约的日期和楼层（格式：日期 楼层，示例：Monday 1）：";
    cin >> day_str >> floor_num;
    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "无效的日期，需输入完整英文星期（如 Monday/Tuesday）。" << endl;
        return;
    }
    if (floor_num < 1 || floor_num >(int)library[day_idx].size()) {
        cout << "无效的楼层，当前该日期下最大楼层为 " << library[day_idx].size() << "。" << endl;
        return;
    }
    Floor& fl = library[day_idx][floor_num - 1];
    for (auto& row : fl.rows) {
        for (auto& seat : row.seats) {
            seat.user = '0';
        }
    }
    cout << day_str << " Floor " << floor_num << " 的所有预约已清空。" << endl;
}

// --- 动态增删功能（不变，可修改默认5层4×4结构）---
void addFloor(vector<Floor> library[7]) {
    string day_str;
    cout << "请输入要增加楼层的日期（需输入完整英文星期，如 Monday/Tuesday）：";
    cin >> day_str;
    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "无效的日期，需输入完整英文星期（如 Monday/Tuesday）。" << endl;
        return;
    }
    library[day_idx].push_back(Floor());
    cout << "OK，已在 " << day_str << " 增加新楼层，当前该日期下总楼层数为 " << library[day_idx].size() << "。" << endl;
}

void deleteFloor(vector<Floor> library[7]) {
    string day_str;
    int floor_num;
    cout << "请输入要删除的日期和楼层（格式：日期 楼层，示例：Monday 1）：";
    cin >> day_str >> floor_num;
    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "无效的日期，需输入完整英文星期（如 Monday/Tuesday）。" << endl;
        return;
    }
    if (floor_num < 1 || floor_num >(int)library[day_idx].size()) {
        cout << "无效的楼层，当前该日期下最大楼层为 " << library[day_idx].size() << "。" << endl;
        return;
    }
    library[day_idx].erase(library[day_idx].begin() + floor_num - 1);
    cout << "OK，已删除 " << day_str << " 的 Floor " << floor_num << "，当前该日期下总楼层数为 " << library[day_idx].size() << "。" << endl;
}

void addRow(vector<Floor> library[7]) {
    string day_str;
    int floor_num;
    cout << "请输入要增加排的日期和楼层（格式：日期 楼层，示例：Monday 1）：";
    cin >> day_str >> floor_num;
    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "无效的日期，需输入完整英文星期（如 Monday/Tuesday）。" << endl;
        return;
    }
    if (floor_num < 1 || floor_num >(int)library[day_idx].size()) {
        cout << "无效的楼层，当前该日期下最大楼层为 " << library[day_idx].size() << "。" << endl;
        return;
    }
    library[day_idx][floor_num - 1].rows.push_back(Row());
    cout << "OK，已在 " << day_str << " Floor " << floor_num << " 增加新排，当前该楼层下总排数为 " << library[day_idx][floor_num - 1].rows.size() << "。" << endl;
}

void deleteRow(vector<Floor> library[7]) {
    string day_str;
    int floor_num, row_num;
    cout << "请输入要删除的日期、楼层和排（格式：日期 楼层 排，示例：Monday 1 1）：";
    cin >> day_str >> floor_num >> row_num;
    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "无效的日期，需输入完整英文星期（如 Monday/Tuesday）。" << endl;
        return;
    }
    if (floor_num < 1 || floor_num >(int)library[day_idx].size()) {
        cout << "无效的楼层，当前该日期下最大楼层为 " << library[day_idx].size() << "。" << endl;
        return;
    }
    Floor& fl = library[day_idx][floor_num - 1];
    if (row_num < 1 || row_num >(int)fl.rows.size()) {
        cout << "无效的排号，当前该楼层下最大排号为 " << fl.rows.size() << "。" << endl;
        return;
    }
    fl.rows.erase(fl.rows.begin() + row_num - 1);
    cout << "OK，已删除 " << day_str << " Floor " << floor_num << " 的第 " << row_num << " 排，当前该楼层下总排数为 " << fl.rows.size() << "。" << endl;
}

void addCol(vector<Floor> library[7]) {
    string day_str;
    int floor_num, row_num;
    cout << "请输入要增加座位(列)的日期、楼层和排（格式：日期 楼层 排，示例：Monday 1 1）：";
    cin >> day_str >> floor_num >> row_num;
    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "无效的日期，需输入完整英文星期（如 Monday/Tuesday）。" << endl;
        return;
    }
    if (floor_num < 1 || floor_num >(int)library[day_idx].size()) {
        cout << "无效的楼层，当前该日期下最大楼层为 " << library[day_idx].size() << "。" << endl;
        return;
    }
    Floor& fl = library[day_idx][floor_num - 1];
    if (row_num < 1 || row_num >(int)fl.rows.size()) {
        cout << "无效的排号，当前该楼层下最大排号为 " << fl.rows.size() << "。" << endl;
        return;
    }
    fl.rows[row_num - 1].seats.push_back(Seat());
    cout << "OK，已在 " << day_str << " Floor " << floor_num << " 第 " << row_num << " 排增加新座位，当前该排下总座位数为 " << fl.rows[row_num - 1].seats.size() << "。" << endl;
}

void deleteCol(vector<Floor> library[7]) {
    string day_str;
    int floor_num, row_num, col_num;
    cout << "请输入要删除的日期、楼层、排和列（格式：日期 楼层 排 列，示例：Monday 1 1 1）：";
    cin >> day_str >> floor_num >> row_num >> col_num;
    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "无效的日期，需输入完整英文星期（如 Monday/Tuesday）。" << endl;
        return;
    }
    if (floor_num < 1 || floor_num >(int)library[day_idx].size()) {
        cout << "无效的楼层，当前该日期下最大楼层为 " << library[day_idx].size() << "。" << endl;
        return;
    }
    Floor& fl = library[day_idx][floor_num - 1];
    if (row_num < 1 || row_num >(int)fl.rows.size()) {
        cout << "无效的排号，当前该楼层下最大排号为 " << fl.rows.size() << "。" << endl;
        return;
    }
    Row& rw = fl.rows[row_num - 1];
    if (col_num < 1 || col_num >(int)rw.seats.size()) {
        cout << "无效的列号，当前该排下最大列号为 " << rw.seats.size() << "。" << endl;
        return;
    }
    rw.seats.erase(rw.seats.begin() + col_num - 1);
    cout << "OK，已删除 " << day_str << " Floor " << floor_num << " 第 " << row_num << " 排第 " << col_num << " 列座位，当前该排下总座位数为 " << rw.seats.size() << "。" << endl;
}

// --- 用户功能函数（不变）---
void show(vector<Floor> library[7], string user, string day_str, int floor_num) {
    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "无效的日期，需输入完整英文星期（如 Monday/Tuesday）。" << endl;
        return;
    }

    if (floor_num < 1 || floor_num >(int)library[day_idx].size()) {
        cout << "无效的楼层，当前该日期下最大楼层为 " << library[day_idx].size() << "。" << endl;
        return;
    }

    Floor& fl = library[day_idx][floor_num - 1];
    cout << "\n" << day_str << " Floor " << floor_num << " 座位状态（0=空闲，1=他人预约，2=自己预约；管理员可见具体用户）：" << endl;
    if (fl.rows.empty()) {
        cout << "该楼层暂无排和座位，可联系管理员添加。" << endl;
        return;
    }
    for (size_t r_idx = 0; r_idx < fl.rows.size(); r_idx++) {
        Row& rw = fl.rows[r_idx];
        for (size_t c_idx = 0; c_idx < rw.seats.size(); c_idx++) {
            char sts = rw.seats[c_idx].user;
            if (user == "Admin") {
                cout << sts << " ";
            }
            else {
                if (sts == '0') cout << "0 ";
                else if (sts == user[0]) cout << "2 ";
                else cout << "1 ";
            }
        }
        cout << "（第 " << (r_idx + 1) << " 排）" << endl;
    }
}

void reserve(vector<Floor> library[7], string user, string day_str, int floor_num, int row_num, int col_num) {
    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "无效的日期，需输入完整英文星期（如 Monday/Tuesday）。" << endl;
        return;
    }

    if (floor_num < 1 || floor_num >(int)library[day_idx].size()) {
        cout << "无效的楼层，当前该日期下最大楼层为 " << library[day_idx].size() << "。" << endl;
        return;
    }
    Floor& fl = library[day_idx][floor_num - 1];
    if (row_num < 1 || row_num >(int)fl.rows.size()) {
        cout << "无效的排号，当前该楼层下最大排号为 " << fl.rows.size() << "。" << endl;
        return;
    }
    Row& rw = fl.rows[row_num - 1];
    if (col_num < 1 || col_num >(int)rw.seats.size()) {
        cout << "无效的列号，当前该排下最大列号为 " << rw.seats.size() << "（该座位不存在）。" << endl;
        return;
    }

    if (rw.seats[col_num - 1].user == '0') {
        rw.seats[col_num - 1].user = user[0];
        cout << "OK，预约成功！预约信息：" << day_str << " Floor " << floor_num << " Seat " << row_num << " " << col_num << endl;
    }
    else {
        cout << "该座位已被预约，当前预约用户为 " << rw.seats[col_num - 1].user << "，请选择其他座位。" << endl;
    }
}

void myres(vector<Floor> library[7], string user) {
    bool has = false;
    cout << "\n我的预约记录（格式：日期 楼层 座位(排 列)）：" << endl;
    for (int d = 0; d < 7; d++) {
        for (size_t f_idx = 0; f_idx < library[d].size(); f_idx++) {
            Floor& fl = library[d][f_idx];
            for (size_t r_idx = 0; r_idx < fl.rows.size(); r_idx++) {
                Row& rw = fl.rows[r_idx];
                for (size_t c_idx = 0; c_idx < rw.seats.size(); c_idx++) {
                    if (rw.seats[c_idx].user == user[0]) {
                        cout << dys[d] << " Floor " << (f_idx + 1) << " Seat " << (r_idx + 1) << " " << (c_idx + 1) << endl;
                        has = true;
                    }
                }
            }
        }
    }
    if (!has) {
        cout << "暂无预约记录，可使用 Reserve 指令预约座位。" << endl;
    }
}

// --- 管理员菜单函数（不变）---
void adminMenu(vector<Floor> library[7]) {
    string subcmd;
    while (true) {
        cout << "\n--- 管理员菜单 ---" << endl;
        cout << "可用指令及说明：" << endl;
        cout << "1. AdminReserve：为指定用户预约座位（需输入用户名和座位信息）" << endl;
        cout << "2. AdminCancel：取消指定座位的预约（需输入座位信息）" << endl;
        cout << "3. ClearDay：清空某一天的所有预约（需输入日期）" << endl;
        cout << "4. ClearFloor：清空某一天某一楼层的所有预约（需输入日期和楼层）" << endl;
        cout << "5. ClearAll：清空系统中所有预约" << endl;
        cout << "6. AddFloor：为某一天增加新楼层（需输入日期）" << endl;
        cout << "7. DeleteFloor：删除某一天的指定楼层（需输入日期和楼层）" << endl;
        cout << "8. AddRow：为某一天某一楼层增加新排（需输入日期和楼层）" << endl;
        cout << "9. DeleteRow：删除某一天某一楼层的指定排（需输入日期、楼层和排）" << endl;
        cout << "10. AddCol：为某一天某一楼层某一排增加新座位（需输入日期、楼层和排）" << endl;
        cout << "11. DeleteCol：删除某一天某一楼层某一排的指定座位（需输入日期、楼层、排和列）" << endl;
        cout << "12. Back：退出管理员菜单，返回主界面" << endl;
        cout << "请输入需要执行的管理指令：";
        cin >> subcmd;

        if (subcmd == "AdminReserve") { adminReserve(library); }
        else if (subcmd == "AdminCancel") { adminCancel(library); }
        else if (subcmd == "ClearDay") { clearDayReservations(library); }
        else if (subcmd == "ClearFloor") { clearFloorReservations(library); }
        else if (subcmd == "ClearAll") { clearAllReservations(library); }
        else if (subcmd == "AddFloor") { addFloor(library); }
        else if (subcmd == "DeleteFloor") { deleteFloor(library); }
        else if (subcmd == "AddRow") { addRow(library); }
        else if (subcmd == "DeleteRow") { deleteRow(library); }
        else if (subcmd == "AddCol") { addCol(library); }
        else if (subcmd == "DeleteCol") { deleteCol(library); }
        else if (subcmd == "Back") {
            cout << "已退出管理员菜单，返回主界面。" << endl;
            break;
        }
        else {
            cout << "未知的管理指令，请从上述可用指令中选择输入（如 AdminReserve/Back）。" << endl;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// --- 主函数（不变）---
int main() {
    vector<Floor> library[7];
    load(library); // 启动时自动初始化5层4×4结构

    string user;
    string cmd;

    cout << "\n--- 图书馆预约系统（动态版）---" << endl;
    cout << "【系统说明】普通用户需先登录（用户名：单个大写字母 A-Z），管理员登录用户名：Admin" << endl;
    cout << "【常用指令】" << endl;
    cout << "- 登录：Login；退出登录：Exit；退出系统：Quit" << endl;
    cout << "- 查询座位：输入英文星期（如 Monday），再按提示输入楼层" << endl;
    cout << "- 预约座位：Reserve；查看我的预约：Reservation" << endl;
    cout << "- 管理员专属：输入 Manage 进入管理菜单" << endl;

    while (true) {
        cout << "\n请输入指令（可输 Login/Exit/Quit/查询/Reserve/Reservation，管理员可输 Manage）：";
        cin >> cmd;

        if (cmd == "Login") {
            string name;
            cout << "请输入用户名（管理员输入 Admin，普通用户输入单个大写字母 A-Z，如 A/B）：";
            cin >> name;
            if (validuser(name)) {
                user = name;
                cout << "登录成功！当前登录用户：" << user << "（普通用户可查询/预约座位，管理员可输 Manage 进入管理菜单）" << endl;
            }
            else {
                cout << "无效用户名！管理员需输入 Admin，普通用户需输入单个大写字母（如 A-Z）。" << endl;
            }
        }
        else if (cmd == "Exit") {
            if (user.empty()) {
                cout << "您尚未登录，无需执行退出登录操作。" << endl;
            }
            else {
                user.clear();
                cout << "已退出登录，再次操作需重新输入 Login 登录。" << endl;
            }
        }
        else if (cmd == "Quit") {
            save(library);
            cout << "数据已保存，系统即将退出，感谢使用！" << endl;
            break;
        }
        else if (user.empty()) {
            cout << "请先登录！输入 Login 指令，再按提示输入用户名（管理员：Admin，普通用户：单个大写字母）。" << endl;
        }
        else if (cmd == "Manage" && user == "Admin") {
            adminMenu(library);
        }
        else if (getday(cmd) != -1) {
            string fkey;
            int floor_num;
            cout << "请输入楼层（格式：Floor + 数字，如 Floor 1，当前该日期下最大楼层为 " << library[getday(cmd)].size() << "）：";
            cin >> fkey >> floor_num;
            if (fkey == "Floor") {
                show(library, user, cmd, floor_num);
            }
            else {
                cout << "查询格式错误！需输入「英文星期 + 空格 + Floor + 空格 + 楼层数字」，示例：Monday Floor 1。" << endl;
            }
        }
        else if (cmd == "Reserve") {
            string day_str, fkey, skey;
            int floor_num, row_num, col_num;
            cout << "请输入预约信息（格式：日期 Floor 楼层 Seat 排 列，示例：Monday Floor 1 Seat 1 2）：";
            cin >> day_str >> fkey >> floor_num >> skey >> row_num >> col_num;
            if (fkey == "Floor" && skey == "Seat") {
                reserve(library, user, day_str, floor_num, row_num, col_num);
            }
            else {
                cout << "预约格式错误！需包含 Floor 和 Seat 关键词，正确示例：Monday Floor 1 Seat 1 2。" << endl;
            }
        }
        else if (cmd == "Reservation") {
            myres(library, user);
        }
        else {
            cout << "未知指令！常用指令参考：Login/Exit/Quit/查询（如 Monday）/Reserve/Reservation，管理员可输 Manage。" << endl;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return 0;
}