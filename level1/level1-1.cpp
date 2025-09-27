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
    cout << "����ԤԼ��������ա�" << endl;
}

void showSeats(const char seats[7][5][4][4], const string& currentUser, const string& dayStr, int floorNum) {
    int day = getDayIndex(dayStr);
    int floorIdx = floorNum - 1;

    if (day == -1 || floorNum < 1 || floorNum > 5) {
        cout << "��Ч�����ڻ�¥�㡣" << endl;
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
        cout << "��Ч��ԤԼ��Ϣ��" << endl;
        return;
    }

    if (seats[day][floorIdx][rowIdx][colIdx] == '0') {
        seats[day][floorIdx][rowIdx][colIdx] = currentUser[0];
        cout << "OK" << endl;
    }
    else {
        cout << "����λ�ѱ�ԤԼ��" << endl;
    }
}

void showMyReservations(const char seats[7][5][4][4], const string& currentUser) {
    bool hasReservation = false;
    cout << "\n�ҵ�ԤԼ��¼��" << endl;
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
        cout << "����ԤԼ��¼��" << endl;
    }
}

void cancelSeat(char seats[7][5][4][4], const string& dayStr, int floorNum, int rowNum, int colNum) {
    int day = getDayIndex(dayStr);
    int floorIdx = floorNum - 1;
    int rowIdx = rowNum - 1;
    int colIdx = colNum - 1;

    if (day == -1 || floorNum < 1 || floorNum > 5 || rowNum < 1 || rowNum > 4 || colNum < 1 || colNum > 4) {
        cout << "��Ч��ȡ����Ϣ��" << endl;
        return;
    }

    if (seats[day][floorIdx][rowIdx][colIdx] != '0') {
        seats[day][floorIdx][rowIdx][colIdx] = '0';
        cout << "ԤԼ��ȡ����" << endl;
    }
    else {
        cout << "����λδ��ԤԼ������ȡ����" << endl;
    }
}

int main() {
    char seats[7][5][4][4];
    loadData(seats);

    string currentUser;
    string command;

    cout << "--- ͼ���ԤԼϵͳ ---" << endl;
    cout << "֧��ָ��: Login, Exit, Quit, ��ѯ(��: Monday Floor 1), Reserve, Reservation, Adminָ��(Clear, Cancel)" << endl;

    while (true) {
        cout << "\n������ָ�";
        cin >> command;

        if (command == "Login") {
            string username;
            cout << "�������û��� (Admin �� A-Z): "; // ����������ʾ
            cin >> username;
            if (isValidUser(username)) {
                currentUser = username;
                cout << "��¼�ɹ�����ǰ�û���" << currentUser << endl;
            }
            else {
                cout << "��Ч�û�����" << endl;
            }
        }
        else if (command == "Exit") {
            if (currentUser.empty()) {
                cout << "����δ��¼�������˳���" << endl;
            }
            else {
                currentUser.clear();
                cout << "���˳���¼��" << endl;
            }
        }
        else if (command == "Quit") {
            saveData(seats);
            cout << "�����ѱ��棬�������˳���" << endl;
            break;
        }
        else if (currentUser.empty()) {
            cout << "���ȵ�¼��" << endl;
        }
        else if (command == "Clear" && currentUser == "Admin") {
            clearAllData(seats);
        }
        else if (getDayIndex(command) != -1) {
            string floorKey;
            int floorNum;
            cout << "������¥�� (��ʽ: Floor <����>): "; // ����������ʾ
            cin >> floorKey >> floorNum;
            if (floorKey == "Floor") {
                showSeats(seats, currentUser, command, floorNum);
            }
            else {
                cout << "��ѯָ���ʽ����ʾ����Monday Floor 1" << endl;
            }
        }
        else if (command == "Reserve") {
            string dayStr, floorKey, seatKey;
            int floorNum, rowNum, colNum;
            cout << "������ԤԼ��Ϣ (��ʽ: ���� Floor ¥�� Seat �� ��): "; // ����������ʾ
            cin >> dayStr >> floorKey >> floorNum >> seatKey >> rowNum >> colNum;
            if (floorKey == "Floor" && seatKey == "Seat") {
                reserveSeat(seats, currentUser, dayStr, floorNum, rowNum, colNum);
            }
            else {
                cout << "ԤԼָ���ʽ����ʾ����Reserve Monday Floor 1 Seat 1 2" << endl;
            }
        }
        else if (command == "Reservation") {
            showMyReservations(seats, currentUser);
        }
        else if (command == "Cancel" && currentUser == "Admin") {
            string dayStr, floorKey, seatKey;
            int floorNum, rowNum, colNum;
            cout << "������Ҫȡ����ԤԼ��Ϣ (��ʽ: ���� Floor ¥�� Seat �� ��): "; // ����������ʾ
            cin >> dayStr >> floorKey >> floorNum >> seatKey >> rowNum >> colNum;
            if (floorKey == "Floor" && seatKey == "Seat") {
                cancelSeat(seats, dayStr, floorNum, rowNum, colNum);
            }
            else {
                cout << "ȡ��ָ���ʽ����ʾ����Cancel Monday Floor 1 Seat 1 2" << endl;
            }
        }
        else {
            cout << "δָ֪�" << endl;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return 0;
}