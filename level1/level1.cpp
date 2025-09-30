#include <iostream>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

const string dat = "library_data.txt";
const string dys[] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };

int getday(string d) {
    for (int i = 0; i < 7; i++) {
        if (d == dys[i]) return i;
    }
    return -1;
}

bool validuser(string u) {
    if (u == "Admin") return 1;
    if (u.size() == 1 && u[0] >= 'A' && u[0] <= 'Z') return 1;
    return 0;
}

void load(char seat[10][10][10][10]) {
    for (int d = 0; d < 7; d++) {
        for (int f = 0; f < 5; f++) {
            for (int r = 0; r < 4; r++) {
                for (int c = 0; c < 4; c++) {
                    seat[d][f][r][c] = '0';
                } 
            }            
        }            
    }
        

    ifstream fin(dat);
    if (!fin.is_open()) return;

    string day;
    int flnum, rnum, cnum;
    char usr;
    while (1) {
        fin >> day >> flnum >> rnum >> cnum >> usr;
        if (!fin) {
            break;
        }
        int d = getday(day);
        if (d != -1) seat[d][flnum - 1][rnum - 1][cnum - 1] = usr;
    }
    fin.close();
}

void save(char seat[10][10][10][10]) {
    ofstream fout(dat);
    for (int d = 0; d < 7; d++) {
        for (int f = 0; f < 5; f++) {
            for (int r = 0; r < 4; r++) {
                for (int c = 0; c < 4; c++) {
                    char usr = seat[d][f][r][c];
                    if (usr != '0') {
                        fout << dys[d] << " " << (f + 1) << " " << (r + 1) << " " << (c + 1) << " " << usr << endl;
                    }
                }
            }
        }
    }
    fout.close();
}

void clear(char seat[10][10][10][10]) {
    for (int d = 0; d < 7; d++) {
        for (int f = 0; f < 5; f++) {
            for (int r = 0; r < 4; r++) {
                for (int c = 0; c < 4; c++) {
                    seat[d][f][r][c] = '0';
                }                                  
            }                      
        }
    }
        
    save(seat);
    cout << "����ԤԼ��������ա�" << endl;
}

void show(char seat[10][10][10][10], string user, string day, int flnum) {
    int d = getday(day);
    int flidx = flnum - 1;

    if (d == -1 || flnum < 1 || flnum > 5) {
        cout << "��Ч�����ڻ�¥�㡣" << endl;
        return;
    }

    cout << "\n" << day << " Floor " << flnum << ":" << endl;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            char sts = seat[d][flidx][r][c];
            if (user == "Admin") {
                cout << sts;
            }
            else {
                if (sts == '0') {
                    cout << '0';
                }
                else if (sts == user[0]) {
                    cout << '2';
                }
                else {
                    cout << '1';
                }
            }
        }
        cout << endl;
    }
}

void reserve(char seat[10][10][10][10],string user, string day, int flnum, int rnum, int cnum) {
    int d = getday(day);
    int flidx = flnum - 1,ridx = rnum - 1,cidx = cnum - 1;

    if (d == -1 || flnum < 1 || flnum > 5 || rnum < 1 || rnum > 4 || cnum < 1 || cnum > 4) {
        cout << "��Ч��ԤԼ��Ϣ��" << endl;
        return;
    }

    if (seat[d][flidx][ridx][cidx] == '0') {
        seat[d][flidx][ridx][cidx] = user[0];
        cout << "OK" << endl;
    }
    else {
        cout << "����λ�ѱ�ԤԼ��" << endl;
    }
}

void myres(char seat[10][10][10][10], string user) {
    bool flag = 0;
    cout << "\n�ҵ�ԤԼ��¼��" << endl;
    for (int d = 0; d < 7; d++) {
        for (int f = 0; f < 5; f++) {
            for (int r = 0; r < 4; r++) {
                for (int c = 0; c < 4; c++) {
                    if (seat[d][f][r][c] == user[0]) {
                        cout << dys[d] << " Floor " << (f + 1) << " Seat " << (r + 1) << " " << (c + 1) << endl;
                        flag = 1;
                    }
                }
            }
        }
    }
    if (!flag) {
        cout << "����ԤԼ��¼��" << endl;
    }
}


int main() {
    char seat[10][10][10][10];
    load(seat);

    string user;
    string cmd;

    cout << "ͼ���ԤԼϵͳ" << endl;
    cout << "֧��ָ��: Login, Exit, Quit, ��ѯ(��: Monday Floor 1), Reserve, Reservation, Adminָ��(Clear)" << endl;

    while (1) {
        cout << "\n������ָ�";
        cin >> cmd;

        if (cmd == "Login") {
            string name;
            cout << "�������û��� (Admin �� A-Z): ";
            cin >> name;
            if (validuser(name)) {
                user = name;
                cout << "��¼�ɹ�����ǰ�û���" << user << endl;
            }
            else {
                cout << "��Ч�û�����" << endl;
            }
        }
        else if (cmd == "Exit") {
            if (user.empty()) {
                cout << "����δ��¼�������˳���" << endl;
            }
            else {
                user.clear();
                cout << "���˳���¼��" << endl;
            }
        }
        else if (cmd == "Quit") {
            save(seat);
            cout << "�����ѱ��棬�������˳���" << endl;
            break;
        }
        else if (user.empty()) {
            cout << "���ȵ�¼��" << endl;
        }
        else if (cmd == "Clear" && user == "Admin") {
            clear(seat);
        }
        else if (getday(cmd) != -1) {
            string fkey;
            int flnum;
            cout << "������¥�� (��ʽ: Floor <����>): ";
            cin >> fkey >> flnum;
            if (fkey == "Floor") {
                show(seat, user, cmd, flnum);
            }
            else {
                cout << "��ѯָ���ʽ����ʾ����Monday Floor 1" << endl;
            }
        }
        else if (cmd == "Reserve") {
            string day, fkey, skey;
            int flnum, rnum, cnum;
            cout << "������ԤԼ��Ϣ (��ʽ: ���� Floor ¥�� Seat �� ��): ";
            cin >> day >> fkey >> flnum >> skey >> rnum >> cnum;
            if (fkey == "Floor" && skey == "Seat") {
                reserve(seat, user, day, flnum, rnum, cnum);
            }
            else {
                cout << "ԤԼָ���ʽ����ʾ����Reserve Monday Floor 1 Seat 1 2" << endl;
            }
        }
        else if (cmd == "Reservation") {
            myres(seat, user);
        }
        else {
            cout << "δָ֪�" << endl;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return 0;
}