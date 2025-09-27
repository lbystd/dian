#include <iostream>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

const string DATA_FILE = "library_data.txt";
const string DAYS[] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };

int getDayIndex(const string& dayStr) {
    for (int i = 0; i < 7; i++) {
        if (dayStr == DAYS[i]) return i;
    }
    return -1;
}

bool isValidUser(const string& username) {
    if (username == "Admin") return true;
    if (username.size() == 1 && username[0] >= 'A' && username[0] <= 'Z') return true;
    return false;
}

void loadData(char seats[7][5][4][4]) {
    for (int day = 0; day < 7; day++)
        for (int floor = 0; floor < 5; floor++)
            for (int row = 0; row < 4; row++)
                for (int col = 0; col < 4; col++)
                    seats[day][floor][row][col] = '0';

    ifstream fin(DATA_FILE);
    if (!fin.is_open()) return;

    string dayStr;
    int floor, row, col;
    char user;
    while (fin >> dayStr >> floor >> row >> col >> user) {
        int day = getDayIndex(dayStr);
        if (day != -1) seats[day][floor - 1][row - 1][col - 1] = user;
    }
    fin.close();
}

void saveData(const char seats[7][5][4][4]) {
    ofstream fout(DATA_FILE);
    for (int day = 0; day < 7; day++) {
        for (int floor = 0; floor < 5; floor++) {
            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    char user = seats[day][floor][row][col];
                    if (user != '0') {
                        fout << DAYS[day] << " " << (floor + 1) << " " << (row + 1) << " " << (col + 1) << " " << user << endl;
                    }
                }
            }
        }
    }
    fout.close();
}

void clearAllData(char seats[7][5][4][4]) {
    for (int day = 0; day < 7; day++)
        for (int floor = 0; floor < 5; floor++)
            for (int row = 0; row < 4; row++)
                for (int col = 0; col < 4; col++)
                    seats[day][floor][row][col] = '0';
    saveData(seats);
    cout << "所有预约数据已清空。" << endl;
}

void showSeats(const char seats[7][5][4][4], const string& currentUser, const string& dayStr, int floorNum) {
    int day = getDayIndex(dayStr);
    int floorIdx = floorNum - 1;

    if (day == -1 || floorNum < 1 || floorNum > 5) {
        cout << "无效的日期或楼层。" << endl;
        return;
    }

    cout << "\n" << dayStr << " Floor " << floorNum << ":" << endl;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            char seatStatus = seats[day][floorIdx][row][col];
            if (currentUser == "Admin") {
                cout << seatStatus;
            }
            else {
                if (seatStatus == '0') cout << '0';
                else if (seatStatus == currentUser[0]) cout << '2';
                else cout << '1';
            }
        }
        cout << endl;
    }
}

void reserveSeat(char seats[7][5][4][4], const string& currentUser, const string& dayStr, int floorNum, int rowNum, int colNum) {
    int day = getDayIndex(dayStr);
    int floorIdx = floorNum - 1;
    int rowIdx = rowNum - 1;
    int colIdx = colNum - 1;

    if (day == -1 || floorNum < 1 || floorNum > 5 || rowNum < 1 || rowNum > 4 || colNum < 1 || colNum > 4) {
        cout << "无效的预约信息。" << endl;
        return;
    }

    if (seats[day][floorIdx][rowIdx][colIdx] == '0') {
        seats[day][floorIdx][rowIdx][colIdx] = currentUser[0];
        cout << "OK" << endl;
    }
    else {
        cout << "该座位已被预约。" << endl;
    }
}

void showMyReservations(const char seats[7][5][4][4], const string& currentUser) {
    bool hasReservation = false;
    cout << "\n我的预约记录：" << endl;
    for (int day = 0; day < 7; day++) {
        for (int floor = 0; floor < 5; floor++) {
            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    if (seats[day][floor][row][col] == currentUser[0]) {
                        cout << DAYS[day] << " Floor " << (floor + 1) << " Seat " << (row + 1) << " " << (col + 1) << endl;
                        hasReservation = true;
                    }
                }
            }
        }
    }
    if (!hasReservation) {
        cout << "暂无预约记录。" << endl;
    }
}

void cancelSeat(char seats[7][5][4][4], const string& dayStr, int floorNum, int rowNum, int colNum) {
    int day = getDayIndex(dayStr);
    int floorIdx = floorNum - 1;
    int rowIdx = rowNum - 1;
    int colIdx = colNum - 1;

    if (day == -1 || floorNum < 1 || floorNum > 5 || rowNum < 1 || rowNum > 4 || colNum < 1 || colNum > 4) {
        cout << "无效的取消信息。" << endl;
        return;
    }

    if (seats[day][floorIdx][rowIdx][colIdx] != '0') {
        seats[day][floorIdx][rowIdx][colIdx] = '0';
        cout << "预约已取消。" << endl;
    }
    else {
        cout << "该座位未被预约，无需取消。" << endl;
    }
}

int main() {
    char seats[7][5][4][4];
    loadData(seats);

    string currentUser;
    string command;

    cout << "--- 图书馆预约系统 ---" << endl;
    cout << "支持指令: Login, Exit, Quit, 查询(如: Monday Floor 1), Reserve, Reservation, Admin指令(Clear, Cancel)" << endl;

    while (true) {
        cout << "\n请输入指令：";
        cin >> command;

        if (command == "Login") {
            string username;
            cout << "请输入用户名 (Admin 或 A-Z): "; // 增加输入提示
            cin >> username;
            if (isValidUser(username)) {
                currentUser = username;
                cout << "登录成功。当前用户：" << currentUser << endl;
            }
            else {
                cout << "无效用户名。" << endl;
            }
        }
        else if (command == "Exit") {
            if (currentUser.empty()) {
                cout << "您尚未登录，无需退出。" << endl;
            }
            else {
                currentUser.clear();
                cout << "已退出登录。" << endl;
            }
        }
        else if (command == "Quit") {
            saveData(seats);
            cout << "数据已保存，程序已退出。" << endl;
            break;
        }
        else if (currentUser.empty()) {
            cout << "请先登录。" << endl;
        }
        else if (command == "Clear" && currentUser == "Admin") {
            clearAllData(seats);
        }
        else if (getDayIndex(command) != -1) {
            string floorKey;
            int floorNum;
            cout << "请输入楼层 (格式: Floor <数字>): "; // 增加输入提示
            cin >> floorKey >> floorNum;
            if (floorKey == "Floor") {
                showSeats(seats, currentUser, command, floorNum);
            }
            else {
                cout << "查询指令格式错误。示例：Monday Floor 1" << endl;
            }
        }
        else if (command == "Reserve") {
            string dayStr, floorKey, seatKey;
            int floorNum, rowNum, colNum;
            cout << "请输入预约信息 (格式: 日期 Floor 楼层 Seat 排 列): "; // 增加输入提示
            cin >> dayStr >> floorKey >> floorNum >> seatKey >> rowNum >> colNum;
            if (floorKey == "Floor" && seatKey == "Seat") {
                reserveSeat(seats, currentUser, dayStr, floorNum, rowNum, colNum);
            }
            else {
                cout << "预约指令格式错误。示例：Reserve Monday Floor 1 Seat 1 2" << endl;
            }
        }
        else if (command == "Reservation") {
            showMyReservations(seats, currentUser);
        }
        else if (command == "Cancel" && currentUser == "Admin") {
            string dayStr, floorKey, seatKey;
            int floorNum, rowNum, colNum;
            cout << "请输入要取消的预约信息 (格式: 日期 Floor 楼层 Seat 排 列): "; // 增加输入提示
            cin >> dayStr >> floorKey >> floorNum >> seatKey >> rowNum >> colNum;
            if (floorKey == "Floor" && seatKey == "Seat") {
                cancelSeat(seats, dayStr, floorNum, rowNum, colNum);
            }
            else {
                cout << "取消指令格式错误。示例：Cancel Monday Floor 1 Seat 1 2" << endl;
            }
        }
        else {
            cout << "未知指令。" << endl;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return 0;
}