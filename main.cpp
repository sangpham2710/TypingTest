#include <bits/stdc++.h>
#include <conio.h>
#define _WIN32_WINNT 0x0500
#include <windows.h>

using namespace std;

const int W = 60, H = 15, P = 10, T = 60, SPACE = 32, ESC = 27, BACKSPACE = 8;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

enum colors {
    BLACK,
    BLUE,
    GREEN,
    AQUA,
    RED,
    PURPLE,
    YELLOW,
    WHITE,
    GRAY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_AQUA,
    LIGHT_RED,
    LIGHT_PURPLE,
    LIGHT_YELLOW,
    BRIGHT_WHITE
};

void SetWindowName(string name) {
    string tmp = string("title ") + name;
    system(tmp.c_str());
}

void SetWindowSize(int width, int height) {
    COORD pos{width, height};
    SMALL_RECT rect{0, 0, width - 1, height - 1};
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleScreenBufferSize(handle, pos);
    SetConsoleWindowInfo(handle, TRUE, &rect);
}

void FixWindowSize() {
    HWND consoleWindow = GetConsoleWindow();
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
}

void SetWindowColor() {
    system("color f0");
}

void ShowConsoleCursor(bool flag) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO tmp;
    GetConsoleCursorInfo(handle, &tmp);
    tmp.bVisible = flag;
    SetConsoleCursorInfo(handle, &tmp);
}

void gotoxy(int x, int y) {
    COORD pos{x, y};
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(handle, pos);
}

void textcolor(int text, int background) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, (background << 4) | text);
}

void Main();
void Setup();
void Input();
void Start();
void Leaderboard();
void Exit();

void center(const string&, int);
void displayOptions();
void SlectingEffect();
bool isAllowedChar(const char&);
void importDict();
void displayBoxes();
void generateRandomWords(vector<string>&, vector<int>&);
void displayRandomWords(vector<string>&, vector<int>&);
bool samePrefix(const string&, const string&);
void checkSpelling(string&, string&, int);
void displayClock(int);
void displayWpm(double);
char displayClockWpmAndGetChar(int);
void importLeaderboardData();
void resetLeaderboard();
void importLeaderboardData();
void updateLeaderboardData();
void displayLeaderboardData();
void Leaderboard();
void displayExit();
void Exit();

void Setup() {
    SetWindowName("Typing Test (Written by Sang Pham)");
    SetWindowSize(W, H);
    FixWindowSize();
    ShowConsoleCursor(0);
}

int option = 0, CorrectWord, WrongWord, CorrectKeystroke, WrongKeystroke, wpm;
vector<pair<string, int>> List;
vector<string> options = {
    "Start",
    "Leaderboard",
    "Exit"
};
const int n = (int)options.size();
vector<string> dict;
clock_t startTime;
bool flag;

void center(const string& tmp, int y) {
    gotoxy((W - (int)tmp.size()) / 2, y);
    cout << tmp;
}

void displayOptions() {
    center("TYPING TEST", 0);
    center("Press [W] or [S] to move, [J] to select", 1);
    for (int i = 0; i < n; ++i) {
        gotoxy(24, 2 + i);
        if (i == option) {
            textcolor(LIGHT_GREEN, BRIGHT_WHITE);
            cout << " > " << options[option];
            textcolor(BLACK, BRIGHT_WHITE);
        } else cout << "   " << options[i];
    }
}

void SelectingEffect() {
    bool selected = 0;
    while (!selected) {
        char c = getch();
        if (c == 'w' || c == 'W' || c == 72) {
            gotoxy(24, 2 + option);
            textcolor(BLACK, BRIGHT_WHITE);
            cout << "   " << options[option];
            textcolor(LIGHT_GREEN, BRIGHT_WHITE);
            option = ((option - 1) % n + n) % n;
            gotoxy(24, 2 + option);
            cout << " > " << options[option];
        } else if (c == 's' || c == 'S' || c == 80) {
            gotoxy(24, 2 + option);
            textcolor(BLACK, BRIGHT_WHITE);
            cout << "   " << options[option];
            textcolor(LIGHT_GREEN, BRIGHT_WHITE);
            option = ((option + 1) % n + n) % n;
            gotoxy(24, 2 + option);
            cout << " > " << options[option];
        } else if (c == 'j' || c == 'J' || c == 13) {
            selected = 1;
            break;
        }
    }
}

bool isAllowedChar(const char& c) {
    return isalnum(c) || c == '-' || c == '_' || c == '\'';
}

void Input() {
    SetWindowColor();
    system("cls");
    displayOptions();
    SelectingEffect();
}

void importDict() {
    ifstream inp("dict.txt");
    string tmp;
    while (inp >> tmp) dict.push_back(tmp);
    inp.close();
}

void displayBoxes() {
    cout << setw(W + 1) << setfill('=') << '\n';
    cout << '|' << setw(W - 1) << setfill(' ') << '|' << '\n';
    cout << setw(W + 1) << setfill('=') << '\n';
    center("Press [Esc] to stop the test", 4);
    cout << '\n';
    cout << setw(W + 1) << setfill('=') << '\n';
    cout << '|' << setw(W - 1) << setfill(' ') << '|' << '\n';
    cout << setw(W + 1) << setfill('=') << '\n';
}

void generateRandomWords(vector<string>& words, vector<int>& pos) {
    words.clear();
    pos.clear();
    pos.push_back(1);
    while (1) {
        words.push_back(dict[rng() % ((int)dict.size())]);
        pos.push_back(pos.back() + (int)words.back().size() + 1);
        if (pos.back() > W - 2) {
            words.pop_back();
            break;
        }
    }
}

void displayRandomWords(vector<string>& words, vector<int>& pos) {
    textcolor(BLACK, BRIGHT_WHITE);
    gotoxy(0, 1);
    cout << '|' << setw(W - 1) << setfill(' ') << '|' << '\n';
    gotoxy(pos[0], 1);
    for (string& t : words) cout << t << ' ';
}

bool samePrefix(const string& a, const string& b) {
    if ((int)a.size() > (int)b.size()) return 0;
    for (int i = 0; i < min((int)a.size(), (int)b.size()); ++i)
        if (a[i] != b[i]) return 0;
    return 1;
}

void checkSpelling(string& s, string& word, int p) {
    if (s.empty()) {
        gotoxy(p, 1);
        textcolor(BLACK, LIGHT_YELLOW);
        cout << word;
    } else if (!samePrefix(s, word)) {
        ++WrongKeystroke;
        gotoxy(p, 1);
        textcolor(BLACK, LIGHT_RED);
        cout << word;
    } else {
        ++CorrectKeystroke;
        gotoxy(p, 1);
        textcolor(BLACK, LIGHT_GREEN);
        cout << word;
    }
}

void displayClock(int sec) {
    gotoxy(19, 3);
    textcolor(BLACK, BRIGHT_WHITE);
    sec = T - sec;
    cout << "Time: ";
    cout << setw(2) << setfill('0') << right << sec / 60;
    cout << ":";
    cout << setw(2) << setfill('0') << right << sec % 60;
}

void displayWpm(double sec) {
    gotoxy(19 + 13, 3);
    textcolor(BLACK, BRIGHT_WHITE);
    if (sec == 0) cout << "WPM:   0";
    else {
        cout << "WPM: ";
        wpm = int(((double)CorrectKeystroke / 5.0) / sec * 60.0);
        cout << setw(3) << setfill(' ') << wpm;
    }
}

char displayClockWpmAndGetChar(int p) {
    while (1) {
        clock_t tmp = clock();
        gotoxy(p, 6);
        ShowConsoleCursor(1);
        while (clock() - tmp < 200) {
            if (kbhit()) {
                ShowConsoleCursor(0);
                if (!flag) {
                    flag = 1;
                    startTime = clock();
                }
                return getch();
            }
        }
        ShowConsoleCursor(0);
        double t = (flag ? (((double)clock() - startTime) / (double)CLOCKS_PER_SEC) : 0);
        if (t > T) return ' ';
        displayClock(t);
        displayWpm(t);
    }
}

void updateLeaderboardData() {
    SetWindowColor();
    system("cls");
    textcolor(BLACK, BRIGHT_WHITE);
    importLeaderboardData();
    if (wpm <= List.back().second) {
        center("Your WPM: " + to_string(wpm), 0);
        center("Press [J] to go back", 1);
    } else {
        int rnk;
        for (rnk = 0; rnk < P && List[rnk].second > wpm; ++rnk);
        center("Congrats! With " + to_string(wpm) + " WPM you have achieved Rank " + to_string(rnk + 1), 0);
        string name;
        center("Enter your name", 1);
        center(string(12, '='), 2);
        center('|' + string(10, ' ') + '|', 3);
        center(string(12, '='), 4);
        while (1) {
            gotoxy((int)name.size() + 25, 3);
            ShowConsoleCursor(1);
            char c(getch());
            ShowConsoleCursor(0);
            if (isAllowedChar(c) && (int)name.size() + 1 > 10) continue;
            if (c == 13) {
                if (!name.empty()) break;
            } else if (c == BACKSPACE) {
                if (!name.empty()) {
                    textcolor(BLACK, BRIGHT_WHITE);
                    gotoxy((int)name.size() + 25, 3);
                    cout << "\b \b";
                    name.pop_back();
                }
            } else if (isAllowedChar(c)) {
                name.push_back(c);
                textcolor(BLACK, BRIGHT_WHITE);
                gotoxy((int)name.size() + 24, 3);
                cout << c;
            }
        }
        List.insert(List.begin() + rnk, {name, wpm});
        List.pop_back();
        ofstream out("rank.txt");
        for (auto& t : List) out << t.first << ' ' << t.second << '\n';
        out.close();
        center("Press [J] to go back", 7);
    }
    // Go back
    while (1) {
        char c = getch();
        if (c == 'j' || c == 'J' || c == 13) {
            Main();
            return;
        }
    }
}

void Start() {
    textcolor(BLACK, BRIGHT_WHITE);
    system("cls");

    importDict();
    displayBoxes();
    vector<string> words;
    vector<int> pos;
    CorrectWord = WrongWord = CorrectKeystroke = WrongKeystroke = flag = 0;
    while (1) {
        generateRandomWords(words, pos);
        displayRandomWords(words, pos);
        int p = 0;
        while (p < (int)words.size()) {
            string s;
            string& word = words[p];
            checkSpelling(s, word, pos[p]);
            while (1) {
                char c(displayClockWpmAndGetChar((int)s.size() + 1));
                if (isAllowedChar(c) && (int)s.size() + 1 > W - 2) continue;

                if (c == ESC) {
                    Main();
                    return;
                } else if (c == BACKSPACE) {
                    if (!s.empty()) {
                        textcolor(BLACK, BRIGHT_WHITE);
                        gotoxy((int)s.size() + 1, 6);
                        cout << "\b \b";
                        s.pop_back();
                    }
                } else if (c == SPACE) {
                    if (!s.empty()) {
                        if (s == word) {
                            ++CorrectWord;
                            gotoxy(pos[p], 1);
                            textcolor(LIGHT_GREEN, BRIGHT_WHITE);
                            cout << word;
                        } else {
                            ++WrongWord;
                            gotoxy(pos[p], 1);
                            textcolor(LIGHT_RED, BRIGHT_WHITE);
                            cout << word;
                        }

                        gotoxy((int)s.size() + 1, 6);
                        while (!s.empty()) {
                            cout << "\b \b";
                            s.pop_back();
                        }

                        ++p;
                        break;
                    }
                } else if (isAllowedChar(c)) {
                    s.push_back(c);
                    textcolor(BLACK, BRIGHT_WHITE);
                    gotoxy((int)s.size(), 6);
                    cout << c;
                }
                checkSpelling(s, word, pos[p]);
                if ((clock() - startTime) / CLOCKS_PER_SEC > T) break;
            }
            if ((clock() - startTime) / CLOCKS_PER_SEC > T) break;
        }
        if ((clock() - startTime) / CLOCKS_PER_SEC > T) break;
    }
    updateLeaderboardData();
}

void resetLeaderboard() {
    ofstream out("rank.txt");
    for (int i = 0; i < P; ++i) out << "Player" << i << ' ' << (P - i) * 5 << '\n';
    out.close();
}

void importLeaderboardData() {
    ifstream inp("rank.txt");
    if (!inp) {
        resetLeaderboard();
        inp.open("rank.txt");
    }
    List = vector<pair<string, int>>(P);
    for (auto& t : List) inp >> t.first >> t.second;
    inp.close();
}

void displayLeaderboardData() {
    center("![HALL OF FAME]!", 0);
    cout << '\n';
    string tmp = string(13, ' ') + "NAME" + string(13, ' ') + '|' + string(13, ' ') + "WPM" + string(13, ' ');
    cout << tmp << '\n';
    for (int i = 0; i < (int)List.size(); ++i) {
        gotoxy(10, 2 + i);
        cout << setw(10) << setfill(' ') << left << List[i].first;
        cout << string(10, ' ') << '|' << string(13, ' ') << setw(3) << setfill(' ') << right << List[i].second;
    }
    // Go back
    center("[R] Reset", P + 2);
    center("Press [J] to go back", P + 4);
    while (1) {
        char c = getch();
        if (c == 'j' || c == 'J' || c == 13) {
            Main();
            return;
        } else if (c == 'r' || c == 'R') {
            textcolor(BLACK, BRIGHT_WHITE);
            center("Do you want to reset the leaderboard ?", 13);
            center(string(string("Press [J] to go back").size(), ' '), 14);
            center("[Y] Yes / [N] No", 14);
            while (1) {
                char c = getch();
                if (c == 'y' || c == 'Y') {
                    resetLeaderboard();
                    break;
                } else if (c == 'n' || c == 'N' || c == ESC) break;
            }
            Leaderboard();
            return;
        }
    }
}

void Leaderboard() {
    textcolor(BLACK, BRIGHT_WHITE);
    system("cls");
    importLeaderboardData();
    displayLeaderboardData();
}

void displayExit() {
    center("Do you want to close the program ?", 7);
    center("[Y] Yes / [N] No", 8);
    while (1) {
        char c = getch();
        if (c == 'y' || c == 'Y') exit(0);
        else if (c == 'n' || c == 'N' || c == ESC) {
            Main();
            return;
        }
    }
}

void Exit() {
    gotoxy(0, 6);
    textcolor(BLACK, BRIGHT_WHITE);
    displayExit();
}

void Main() {
    Input();
    switch (option) {
    case 0:
        Start();
        break;
    case 1:
        Leaderboard();
        break;
    case 2:
        Exit();
        break;
    }
}

int main() {
    Setup();
    Main();
    return 0;
}
