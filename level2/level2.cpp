#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <vector>
#include <algorithm>

using namespace std;

const string dat = "library_data.txt";
const string dys[] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };

// --- ���ݽṹ���� ---
struct Seat {
    char user = '0'; // '0' ��ʾ����
};

struct Row {
    vector<Seat> seats;
};

struct Floor {
    vector<Row> rows;
};

// --- �������� ---
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

// --- ���ݼ����뱣�棨�����޸ģ�Ĭ�ϳ�ʼ��5��4��4��---
void load(vector<Floor> library[7]) {
    // 1. �ȳ�ʼ��Ĭ�Ͻṹ��7��ͨ��5�㣬ÿ��4��4��
    for (int day = 0; day < 7; day++) {
        library[day].resize(5); // Ĭ��5��
        for (int fl_idx = 0; fl_idx < 5; fl_idx++) { // ����5��
            library[day][fl_idx].rows.resize(4); // ÿ��4��
            for (int row_idx = 0; row_idx < 4; row_idx++) { // ����4��
                library[day][fl_idx].rows[row_idx].seats.resize(4); // ÿ��4��
            }
        }
    }

    // 2. �����������ļ��������ļ����ݸ���Ĭ�ϣ������û���ʷ���ã�
    ifstream fin(dat);
    if (!fin.is_open()) {
        cout << "δ�ҵ���ʷ�����ļ����ѳ�ʼ��Ĭ�Ͻṹ��5�㣬ÿ��4��4��λ" << endl;
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
    cout << "�Ѽ�����ʷ���ݣ���ǰ�����ṹ��5�㣬ÿ��4��4��λ����ͨ������Ա�����޸ģ�" << endl;
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

// --- ����Ա���ܺ��������䣬������̬��ɾ��---
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
    cout << "����ԤԼ��������ա�" << endl;
}

void adminReserve(vector<Floor> library[7]) {
    string user, day_str, fkey, skey;
    int floor_num, row_num, col_num;
    cout << "������ԤԼ��Ϣ����ʽ���û��� ���� Floor ¥�� Seat �� �У�ʾ����A Monday Floor 1 Seat 1 2����";
    cin >> user >> day_str >> fkey >> floor_num >> skey >> row_num >> col_num;

    if (!validuser(user) || user == "Admin") {
        cout << "��Ч���û���������Ϊ Admin ��ǵ�����д��ĸ���û�ԤԼ��" << endl;
        return;
    }
    if (fkey != "Floor" || skey != "Seat") {
        cout << "ԤԼָ���ʽ��������� Floor �� Seat �ؼ��ʣ�ʾ����A Monday Floor 1 Seat 1 2" << endl;
        return;
    }

    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "��Ч�����ڣ�����������Ӣ�����ڣ��� Monday/Tuesday����" << endl;
        return;
    }

    if (floor_num < 1 || floor_num >(int)library[day_idx].size()) {
        cout << "��Ч��¥�㣬��ǰ�����������¥��Ϊ " << library[day_idx].size() << "��" << endl;
        return;
    }
    Floor& fl = library[day_idx][floor_num - 1];
    if (row_num < 1 || row_num >(int)fl.rows.size()) {
        cout << "��Ч���źţ���ǰ��¥��������ź�Ϊ " << fl.rows.size() << "��" << endl;
        return;
    }
    Row& rw = fl.rows[row_num - 1];
    if (col_num < 1 || col_num >(int)rw.seats.size()) {
        cout << "��Ч���кţ���ǰ����������к�Ϊ " << rw.seats.size() << "������λ�����ڣ���" << endl;
        return;
    }

    if (rw.seats[col_num - 1].user == '0') {
        rw.seats[col_num - 1].user = user[0];
        cout << "OK����Ϊ�û� " << user << " ���ԤԼ��" << endl;
    }
    else {
        cout << "����λ�ѱ�ԤԼ����ǰԤԼ�û�Ϊ " << rw.seats[col_num - 1].user << "��" << endl;
    }
}

void adminCancel(vector<Floor> library[7]) {
    string day_str, fkey, skey;
    int floor_num, row_num, col_num;
    cout << "������Ҫȡ����ԤԼ��Ϣ����ʽ������ Floor ¥�� Seat �� �У�ʾ����Monday Floor 1 Seat 1 2����";
    cin >> day_str >> fkey >> floor_num >> skey >> row_num >> col_num;

    if (fkey != "Floor" || skey != "Seat") {
        cout << "ȡ��ָ���ʽ��������� Floor �� Seat �ؼ��ʣ�ʾ����Monday Floor 1 Seat 1 2" << endl;
        return;
    }

    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "��Ч�����ڣ�����������Ӣ�����ڣ��� Monday/Tuesday����" << endl;
        return;
    }

    if (floor_num < 1 || floor_num >(int)library[day_idx].size()) { cout << "��Ч��¥�㣬��ǰ�����������¥��Ϊ " << library[day_idx].size() << "��" << endl; return; }
    Floor& fl = library[day_idx][floor_num - 1];
    if (row_num < 1 || row_num >(int)fl.rows.size()) { cout << "��Ч���źţ���ǰ��¥��������ź�Ϊ " << fl.rows.size() << "��" << endl; return; }
    Row& rw = fl.rows[row_num - 1];
    if (col_num < 1 || col_num >(int)rw.seats.size()) { cout << "��Ч���кţ���ǰ����������к�Ϊ " << rw.seats.size() << "������λ�����ڣ���" << endl; return; }

    if (rw.seats[col_num - 1].user != '0') {
        char usr = rw.seats[col_num - 1].user;
        rw.seats[col_num - 1].user = '0';
        cout << "OK����ȡ���û� " << usr << " ��ԤԼ��" << endl;
    }
    else {
        cout << "����λ��ǰδ��ԤԼ������ȡ����" << endl;
    }
}

void clearDayReservations(vector<Floor> library[7]) {
    string day_str;
    cout << "������Ҫ���ԤԼ�����ڣ�����������Ӣ�����ڣ��� Monday/Tuesday����";
    cin >> day_str;
    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "��Ч�����ڣ�����������Ӣ�����ڣ��� Monday/Tuesday����" << endl;
        return;
    }
    for (auto& floor : library[day_idx]) {
        for (auto& row : floor.rows) {
            for (auto& seat : row.seats) {
                seat.user = '0';
            }
        }
    }
    cout << day_str << " ������ԤԼ����ա�" << endl;
}

void clearFloorReservations(vector<Floor> library[7]) {
    string day_str;
    int floor_num;
    cout << "������Ҫ���ԤԼ�����ں�¥�㣨��ʽ������ ¥�㣬ʾ����Monday 1����";
    cin >> day_str >> floor_num;
    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "��Ч�����ڣ�����������Ӣ�����ڣ��� Monday/Tuesday����" << endl;
        return;
    }
    if (floor_num < 1 || floor_num >(int)library[day_idx].size()) {
        cout << "��Ч��¥�㣬��ǰ�����������¥��Ϊ " << library[day_idx].size() << "��" << endl;
        return;
    }
    Floor& fl = library[day_idx][floor_num - 1];
    for (auto& row : fl.rows) {
        for (auto& seat : row.seats) {
            seat.user = '0';
        }
    }
    cout << day_str << " Floor " << floor_num << " ������ԤԼ����ա�" << endl;
}

// --- ��̬��ɾ���ܣ����䣬���޸�Ĭ��5��4��4�ṹ��---
void addFloor(vector<Floor> library[7]) {
    string day_str;
    cout << "������Ҫ����¥������ڣ�����������Ӣ�����ڣ��� Monday/Tuesday����";
    cin >> day_str;
    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "��Ч�����ڣ�����������Ӣ�����ڣ��� Monday/Tuesday����" << endl;
        return;
    }
    library[day_idx].push_back(Floor());
    cout << "OK������ " << day_str << " ������¥�㣬��ǰ����������¥����Ϊ " << library[day_idx].size() << "��" << endl;
}

void deleteFloor(vector<Floor> library[7]) {
    string day_str;
    int floor_num;
    cout << "������Ҫɾ�������ں�¥�㣨��ʽ������ ¥�㣬ʾ����Monday 1����";
    cin >> day_str >> floor_num;
    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "��Ч�����ڣ�����������Ӣ�����ڣ��� Monday/Tuesday����" << endl;
        return;
    }
    if (floor_num < 1 || floor_num >(int)library[day_idx].size()) {
        cout << "��Ч��¥�㣬��ǰ�����������¥��Ϊ " << library[day_idx].size() << "��" << endl;
        return;
    }
    library[day_idx].erase(library[day_idx].begin() + floor_num - 1);
    cout << "OK����ɾ�� " << day_str << " �� Floor " << floor_num << "����ǰ����������¥����Ϊ " << library[day_idx].size() << "��" << endl;
}

void addRow(vector<Floor> library[7]) {
    string day_str;
    int floor_num;
    cout << "������Ҫ�����ŵ����ں�¥�㣨��ʽ������ ¥�㣬ʾ����Monday 1����";
    cin >> day_str >> floor_num;
    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "��Ч�����ڣ�����������Ӣ�����ڣ��� Monday/Tuesday����" << endl;
        return;
    }
    if (floor_num < 1 || floor_num >(int)library[day_idx].size()) {
        cout << "��Ч��¥�㣬��ǰ�����������¥��Ϊ " << library[day_idx].size() << "��" << endl;
        return;
    }
    library[day_idx][floor_num - 1].rows.push_back(Row());
    cout << "OK������ " << day_str << " Floor " << floor_num << " �������ţ���ǰ��¥����������Ϊ " << library[day_idx][floor_num - 1].rows.size() << "��" << endl;
}

void deleteRow(vector<Floor> library[7]) {
    string day_str;
    int floor_num, row_num;
    cout << "������Ҫɾ�������ڡ�¥����ţ���ʽ������ ¥�� �ţ�ʾ����Monday 1 1����";
    cin >> day_str >> floor_num >> row_num;
    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "��Ч�����ڣ�����������Ӣ�����ڣ��� Monday/Tuesday����" << endl;
        return;
    }
    if (floor_num < 1 || floor_num >(int)library[day_idx].size()) {
        cout << "��Ч��¥�㣬��ǰ�����������¥��Ϊ " << library[day_idx].size() << "��" << endl;
        return;
    }
    Floor& fl = library[day_idx][floor_num - 1];
    if (row_num < 1 || row_num >(int)fl.rows.size()) {
        cout << "��Ч���źţ���ǰ��¥��������ź�Ϊ " << fl.rows.size() << "��" << endl;
        return;
    }
    fl.rows.erase(fl.rows.begin() + row_num - 1);
    cout << "OK����ɾ�� " << day_str << " Floor " << floor_num << " �ĵ� " << row_num << " �ţ���ǰ��¥����������Ϊ " << fl.rows.size() << "��" << endl;
}

void addCol(vector<Floor> library[7]) {
    string day_str;
    int floor_num, row_num;
    cout << "������Ҫ������λ(��)�����ڡ�¥����ţ���ʽ������ ¥�� �ţ�ʾ����Monday 1 1����";
    cin >> day_str >> floor_num >> row_num;
    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "��Ч�����ڣ�����������Ӣ�����ڣ��� Monday/Tuesday����" << endl;
        return;
    }
    if (floor_num < 1 || floor_num >(int)library[day_idx].size()) {
        cout << "��Ч��¥�㣬��ǰ�����������¥��Ϊ " << library[day_idx].size() << "��" << endl;
        return;
    }
    Floor& fl = library[day_idx][floor_num - 1];
    if (row_num < 1 || row_num >(int)fl.rows.size()) {
        cout << "��Ч���źţ���ǰ��¥��������ź�Ϊ " << fl.rows.size() << "��" << endl;
        return;
    }
    fl.rows[row_num - 1].seats.push_back(Seat());
    cout << "OK������ " << day_str << " Floor " << floor_num << " �� " << row_num << " ����������λ����ǰ����������λ��Ϊ " << fl.rows[row_num - 1].seats.size() << "��" << endl;
}

void deleteCol(vector<Floor> library[7]) {
    string day_str;
    int floor_num, row_num, col_num;
    cout << "������Ҫɾ�������ڡ�¥�㡢�ź��У���ʽ������ ¥�� �� �У�ʾ����Monday 1 1 1����";
    cin >> day_str >> floor_num >> row_num >> col_num;
    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "��Ч�����ڣ�����������Ӣ�����ڣ��� Monday/Tuesday����" << endl;
        return;
    }
    if (floor_num < 1 || floor_num >(int)library[day_idx].size()) {
        cout << "��Ч��¥�㣬��ǰ�����������¥��Ϊ " << library[day_idx].size() << "��" << endl;
        return;
    }
    Floor& fl = library[day_idx][floor_num - 1];
    if (row_num < 1 || row_num >(int)fl.rows.size()) {
        cout << "��Ч���źţ���ǰ��¥��������ź�Ϊ " << fl.rows.size() << "��" << endl;
        return;
    }
    Row& rw = fl.rows[row_num - 1];
    if (col_num < 1 || col_num >(int)rw.seats.size()) {
        cout << "��Ч���кţ���ǰ����������к�Ϊ " << rw.seats.size() << "��" << endl;
        return;
    }
    rw.seats.erase(rw.seats.begin() + col_num - 1);
    cout << "OK����ɾ�� " << day_str << " Floor " << floor_num << " �� " << row_num << " �ŵ� " << col_num << " ����λ����ǰ����������λ��Ϊ " << rw.seats.size() << "��" << endl;
}

// --- �û����ܺ��������䣩---
void show(vector<Floor> library[7], string user, string day_str, int floor_num) {
    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "��Ч�����ڣ�����������Ӣ�����ڣ��� Monday/Tuesday����" << endl;
        return;
    }

    if (floor_num < 1 || floor_num >(int)library[day_idx].size()) {
        cout << "��Ч��¥�㣬��ǰ�����������¥��Ϊ " << library[day_idx].size() << "��" << endl;
        return;
    }

    Floor& fl = library[day_idx][floor_num - 1];
    cout << "\n" << day_str << " Floor " << floor_num << " ��λ״̬��0=���У�1=����ԤԼ��2=�Լ�ԤԼ������Ա�ɼ������û�����" << endl;
    if (fl.rows.empty()) {
        cout << "��¥�������ź���λ������ϵ����Ա��ӡ�" << endl;
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
        cout << "���� " << (r_idx + 1) << " �ţ�" << endl;
    }
}

void reserve(vector<Floor> library[7], string user, string day_str, int floor_num, int row_num, int col_num) {
    int day_idx = getday(day_str);
    if (day_idx == -1) {
        cout << "��Ч�����ڣ�����������Ӣ�����ڣ��� Monday/Tuesday����" << endl;
        return;
    }

    if (floor_num < 1 || floor_num >(int)library[day_idx].size()) {
        cout << "��Ч��¥�㣬��ǰ�����������¥��Ϊ " << library[day_idx].size() << "��" << endl;
        return;
    }
    Floor& fl = library[day_idx][floor_num - 1];
    if (row_num < 1 || row_num >(int)fl.rows.size()) {
        cout << "��Ч���źţ���ǰ��¥��������ź�Ϊ " << fl.rows.size() << "��" << endl;
        return;
    }
    Row& rw = fl.rows[row_num - 1];
    if (col_num < 1 || col_num >(int)rw.seats.size()) {
        cout << "��Ч���кţ���ǰ����������к�Ϊ " << rw.seats.size() << "������λ�����ڣ���" << endl;
        return;
    }

    if (rw.seats[col_num - 1].user == '0') {
        rw.seats[col_num - 1].user = user[0];
        cout << "OK��ԤԼ�ɹ���ԤԼ��Ϣ��" << day_str << " Floor " << floor_num << " Seat " << row_num << " " << col_num << endl;
    }
    else {
        cout << "����λ�ѱ�ԤԼ����ǰԤԼ�û�Ϊ " << rw.seats[col_num - 1].user << "����ѡ��������λ��" << endl;
    }
}

void myres(vector<Floor> library[7], string user) {
    bool has = false;
    cout << "\n�ҵ�ԤԼ��¼����ʽ������ ¥�� ��λ(�� ��)����" << endl;
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
        cout << "����ԤԼ��¼����ʹ�� Reserve ָ��ԤԼ��λ��" << endl;
    }
}

// --- ����Ա�˵����������䣩---
void adminMenu(vector<Floor> library[7]) {
    string subcmd;
    while (true) {
        cout << "\n--- ����Ա�˵� ---" << endl;
        cout << "����ָ�˵����" << endl;
        cout << "1. AdminReserve��Ϊָ���û�ԤԼ��λ���������û�������λ��Ϣ��" << endl;
        cout << "2. AdminCancel��ȡ��ָ����λ��ԤԼ����������λ��Ϣ��" << endl;
        cout << "3. ClearDay�����ĳһ�������ԤԼ�����������ڣ�" << endl;
        cout << "4. ClearFloor�����ĳһ��ĳһ¥�������ԤԼ�����������ں�¥�㣩" << endl;
        cout << "5. ClearAll�����ϵͳ������ԤԼ" << endl;
        cout << "6. AddFloor��Ϊĳһ��������¥�㣨���������ڣ�" << endl;
        cout << "7. DeleteFloor��ɾ��ĳһ���ָ��¥�㣨���������ں�¥�㣩" << endl;
        cout << "8. AddRow��Ϊĳһ��ĳһ¥���������ţ����������ں�¥�㣩" << endl;
        cout << "9. DeleteRow��ɾ��ĳһ��ĳһ¥���ָ���ţ����������ڡ�¥����ţ�" << endl;
        cout << "10. AddCol��Ϊĳһ��ĳһ¥��ĳһ����������λ�����������ڡ�¥����ţ�" << endl;
        cout << "11. DeleteCol��ɾ��ĳһ��ĳһ¥��ĳһ�ŵ�ָ����λ�����������ڡ�¥�㡢�ź��У�" << endl;
        cout << "12. Back���˳�����Ա�˵�������������" << endl;
        cout << "��������Ҫִ�еĹ���ָ�";
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
            cout << "���˳�����Ա�˵������������档" << endl;
            break;
        }
        else {
            cout << "δ֪�Ĺ���ָ������������ָ����ѡ�����루�� AdminReserve/Back����" << endl;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// --- �����������䣩---
int main() {
    vector<Floor> library[7];
    load(library); // ����ʱ�Զ���ʼ��5��4��4�ṹ

    string user;
    string cmd;

    cout << "\n--- ͼ���ԤԼϵͳ����̬�棩---" << endl;
    cout << "��ϵͳ˵������ͨ�û����ȵ�¼���û�����������д��ĸ A-Z��������Ա��¼�û�����Admin" << endl;
    cout << "������ָ�" << endl;
    cout << "- ��¼��Login���˳���¼��Exit���˳�ϵͳ��Quit" << endl;
    cout << "- ��ѯ��λ������Ӣ�����ڣ��� Monday�����ٰ���ʾ����¥��" << endl;
    cout << "- ԤԼ��λ��Reserve���鿴�ҵ�ԤԼ��Reservation" << endl;
    cout << "- ����Աר�������� Manage �������˵�" << endl;

    while (true) {
        cout << "\n������ָ����� Login/Exit/Quit/��ѯ/Reserve/Reservation������Ա���� Manage����";
        cin >> cmd;

        if (cmd == "Login") {
            string name;
            cout << "�������û���������Ա���� Admin����ͨ�û����뵥����д��ĸ A-Z���� A/B����";
            cin >> name;
            if (validuser(name)) {
                user = name;
                cout << "��¼�ɹ�����ǰ��¼�û���" << user << "����ͨ�û��ɲ�ѯ/ԤԼ��λ������Ա���� Manage �������˵���" << endl;
            }
            else {
                cout << "��Ч�û���������Ա������ Admin����ͨ�û������뵥����д��ĸ���� A-Z����" << endl;
            }
        }
        else if (cmd == "Exit") {
            if (user.empty()) {
                cout << "����δ��¼������ִ���˳���¼������" << endl;
            }
            else {
                user.clear();
                cout << "���˳���¼���ٴβ������������� Login ��¼��" << endl;
            }
        }
        else if (cmd == "Quit") {
            save(library);
            cout << "�����ѱ��棬ϵͳ�����˳�����лʹ�ã�" << endl;
            break;
        }
        else if (user.empty()) {
            cout << "���ȵ�¼������ Login ָ��ٰ���ʾ�����û���������Ա��Admin����ͨ�û���������д��ĸ����" << endl;
        }
        else if (cmd == "Manage" && user == "Admin") {
            adminMenu(library);
        }
        else if (getday(cmd) != -1) {
            string fkey;
            int floor_num;
            cout << "������¥�㣨��ʽ��Floor + ���֣��� Floor 1����ǰ�����������¥��Ϊ " << library[getday(cmd)].size() << "����";
            cin >> fkey >> floor_num;
            if (fkey == "Floor") {
                show(library, user, cmd, floor_num);
            }
            else {
                cout << "��ѯ��ʽ���������롸Ӣ������ + �ո� + Floor + �ո� + ¥�����֡���ʾ����Monday Floor 1��" << endl;
            }
        }
        else if (cmd == "Reserve") {
            string day_str, fkey, skey;
            int floor_num, row_num, col_num;
            cout << "������ԤԼ��Ϣ����ʽ������ Floor ¥�� Seat �� �У�ʾ����Monday Floor 1 Seat 1 2����";
            cin >> day_str >> fkey >> floor_num >> skey >> row_num >> col_num;
            if (fkey == "Floor" && skey == "Seat") {
                reserve(library, user, day_str, floor_num, row_num, col_num);
            }
            else {
                cout << "ԤԼ��ʽ��������� Floor �� Seat �ؼ��ʣ���ȷʾ����Monday Floor 1 Seat 1 2��" << endl;
            }
        }
        else if (cmd == "Reservation") {
            myres(library, user);
        }
        else {
            cout << "δָ֪�����ָ��ο���Login/Exit/Quit/��ѯ���� Monday��/Reserve/Reservation������Ա���� Manage��" << endl;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return 0;
}