#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
using namespace std;

//difficulties
const int WIDTH1 = 10;
const int HEIGHT1 = 5;
const int MINES1 = 10;

const int WIDTH2 = 20;
const int HEIGHT2 = 10;
const int MINES2 = 30;

const int WIDTH3 = 30;
const int HEIGHT3 = 15;
const int MINES3 = 60;
//
//controls and colors
enum Key { LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80, ENTER = 13, SPACE = 32 };
enum Color {
    BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHTGREY,
    DARKGREY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED,
    LIGHTMAGENTA, YELLOW, WHITE
};
//ingame states
bool mines1[HEIGHT1][WIDTH1] = {};
bool revealed1[HEIGHT1][WIDTH1] = {};
bool flagged1[HEIGHT1][WIDTH1] = {};
int adjacent1[HEIGHT1][WIDTH1] = {};

bool mines2[HEIGHT2][WIDTH2] = {};
bool revealed2[HEIGHT2][WIDTH2] = {};
bool flagged2[HEIGHT2][WIDTH2] = {};
int adjacent2[HEIGHT2][WIDTH2] = {};

bool mines3[HEIGHT3][WIDTH3] = {};
bool revealed3[HEIGHT3][WIDTH3] = {};
bool flagged3[HEIGHT3][WIDTH3] = {};
int adjacent3[HEIGHT3][WIDTH3] = {};
//
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); //SetConsoleCursorPosition; SetConsoleTextAttribute

void setCursor(int x, int y) {
    COORD c = { (short)x, (short)y };
    SetConsoleCursorPosition(h, c);
} //cursor position

void setColor(Color c) {
    SetConsoleTextAttribute(h, c);
} //text color

void placeMines1() {
    int placed = 0;
    while (placed < MINES1) {
        int x = rand() % WIDTH1;
        int y = rand() % HEIGHT1;
        if (!mines1[y][x]) {
            mines1[y][x] = true;
            placed++;
        }
    }
}

void placeMines2() {
    int placed = 0;
    while (placed < MINES2) {
        int x = rand() % WIDTH2;
        int y = rand() % HEIGHT2;
        if (!mines2[y][x]) {
            mines2[y][x] = true;
            placed++;
        }
    }
}

void placeMines3() {
    int placed = 0;
    while (placed < MINES3) {
        int x = rand() % WIDTH3;
        int y = rand() % HEIGHT3;
        if (!mines3[y][x]) {
            mines3[y][x] = true;
            placed++;
        }
    }
}

void calculateAdjacent1() {
    for (int y = 0; y < HEIGHT1; y++) {
        for (int x = 0; x < WIDTH1; x++) {
            if (!mines1[y][x]) {
                int count = 0;
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        int ny = y + dy;
                        int nx = x + dx;
                        if (nx >= 0 && ny >= 0 && nx < WIDTH1 && ny < HEIGHT1 && mines1[ny][nx])
                            count++;
                    }
                }
                adjacent1[y][x] = count;
            }
        }
    }
}

void calculateAdjacent2() {
    for (int y = 0; y < HEIGHT2; y++) {
        for (int x = 0; x < WIDTH2; x++) {
            if (!mines2[y][x]) {
                int count = 0;
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        int ny = y + dy;
                        int nx = x + dx;
                        if (nx >= 0 && ny >= 0 && nx < WIDTH2 && ny < HEIGHT2 && mines2[ny][nx])
                            count++;
                    }
                }
                adjacent2[y][x] = count;
            }
        }
    }
}

void calculateAdjacent3() {
    for (int y = 0; y < HEIGHT3; y++) {
        for (int x = 0; x < WIDTH3; x++) {
            if (!mines3[y][x]) {
                int count = 0;
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        int ny = y + dy;
                        int nx = x + dx;
                        if (nx >= 0 && ny >= 0 && nx < WIDTH3 && ny < HEIGHT3 && mines3[ny][nx])
                            count++;
                    }
                }
                adjacent3[y][x] = count;
            }
        }
    }
}

void reveal1(int x, int y) {
    if (x < 0 || y < 0 || x >= WIDTH1 || y >= HEIGHT1 || revealed1[y][x] || flagged1[y][x])
        return;

    revealed1[y][x] = true;

    if (adjacent1[y][x] == 0 && !mines1[y][x]) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx != 0 || dy != 0)
                    reveal1(x + dx, y + dy);
            }
        }
    }
}

void reveal2(int x, int y) {
    if (x < 0 || y < 0 || x >= WIDTH2 || y >= HEIGHT2 || revealed2[y][x] || flagged2[y][x])
        return;

    revealed2[y][x] = true;

    if (adjacent2[y][x] == 0 && !mines2[y][x]) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx != 0 || dy != 0)
                    reveal2(x + dx, y + dy);
            }
        }
    }
}

void reveal3(int x, int y) {
    if (x < 0 || y < 0 || x >= WIDTH3 || y >= HEIGHT3 || revealed3[y][x] || flagged3[y][x])
        return;

    revealed3[y][x] = true;

    if (adjacent3[y][x] == 0 && !mines3[y][x]) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx != 0 || dy != 0)
                    reveal3(x + dx, y + dy);
            }
        }
    }
}

void draw1(int cursorX, int cursorY) {
    setCursor(0, 0);
    for (int y = 0; y < HEIGHT1; y++) {
        for (int x = 0; x < WIDTH1; x++) {
            if (x == cursorX && y == cursorY)
                setColor(WHITE);
            else
                setColor(LIGHTGREY);

            if (flagged1[y][x])
                cout << "F";
            else if (!revealed1[y][x])
                cout << (char)177;
            else if (mines1[y][x])
                cout << "*";
            else if (adjacent1[y][x] > 0)
                cout << adjacent1[y][x];
            else
                cout << " ";
        }
        cout << "\n";
    }
}

void draw2(int cursorX, int cursorY) {
    setCursor(0, 0);
    for (int y = 0; y < HEIGHT2; y++) {
        for (int x = 0; x < WIDTH2; x++) {
            if (x == cursorX && y == cursorY)
                setColor(WHITE);
            else
                setColor(LIGHTGREY);

            if (flagged2[y][x])
                cout << "F";
            else if (!revealed2[y][x])
                cout << (char)177;
            else if (mines2[y][x])
                cout << "*";
            else if (adjacent2[y][x] > 0)
                cout << adjacent2[y][x];
            else
                cout << " ";
        }
        cout << "\n";
    }
}

void draw3(int cursorX, int cursorY) {
    setCursor(0, 0);
    for (int y = 0; y < HEIGHT3; y++) {
        for (int x = 0; x < WIDTH3; x++) {
            if (x == cursorX && y == cursorY)
                setColor(WHITE);
            else
                setColor(LIGHTGREY);

            if (flagged3[y][x])
                cout << "F";
            else if (!revealed3[y][x])
                cout << (char)177;
            else if (mines3[y][x])
                cout << "*";
            else if (adjacent3[y][x] > 0)
                cout << adjacent3[y][x];
            else
                cout << " ";
        }
        cout << "\n";
    }
}

bool checkWin1() {
    for (int y = 0; y < HEIGHT1; y++)
        for (int x = 0; x < WIDTH1; x++)
            if (!mines1[y][x] && !revealed1[y][x])
                return false;
    return true;
}

bool checkWin2() {
    for (int y = 0; y < HEIGHT2; y++)
        for (int x = 0; x < WIDTH2; x++)
            if (!mines2[y][x] && !revealed2[y][x])
                return false;
    return true;
}

bool checkWin3() {
    for (int y = 0; y < HEIGHT3; y++)
        for (int x = 0; x < WIDTH3; x++)
            if (!mines3[y][x] && !revealed3[y][x])
                return false;
    return true;
}

bool dif1() {
    placeMines1();
    calculateAdjacent1();

    int cursorX = 0, cursorY = 0;
    bool gameover = false;

    while (true) {
        draw1(cursorX, cursorY);
        if (checkWin1()) {
            setCursor(0, HEIGHT1 + 1);
            setColor(GREEN);
            cout << "won";
            break;
        }
        if (gameover) {
            for (int y = 0; y < HEIGHT1; y++) {
                for (int x = 0; x < WIDTH1; x++) {
                    if (mines1[y][x]) revealed1[y][x] = true;
                }
            }
            draw1(-1, -1);
            setCursor(0, HEIGHT1 + 1);
            setColor(RED);
            cout << "explode";
            break;
        }
        int ch = _getch();
        if (ch == 224) ch = _getch();
        switch (ch) {
        case Key::LEFT:  if (cursorX > 0) cursorX--; break;
        case Key::RIGHT: if (cursorX < WIDTH1 - 1) cursorX++; break;
        case Key::UP:    if (cursorY > 0) cursorY--; break;
        case Key::DOWN:  if (cursorY < HEIGHT1 - 1) cursorY++; break;
        case Key::SPACE:
            if (!flagged1[cursorY][cursorX] && !revealed1[cursorY][cursorX]) {
                if (mines1[cursorY][cursorX]) {
                    gameover = true;
                }
                else {
                    reveal1(cursorX, cursorY);
                }
            }
            break;
        case Key::ENTER:
            if (!revealed1[cursorY][cursorX] && !gameover)
                flagged1[cursorY][cursorX] = !flagged1[cursorY][cursorX];
            break;
        }
    }

    setCursor(0, HEIGHT1 + 2);
    system("pause");
    return 0;
}

bool dif2() {
    placeMines2();
    calculateAdjacent2();

    int cursorX = 0, cursorY = 0;
    bool gameover = false;

    while (true) {
        draw2(cursorX, cursorY);

        if (checkWin2()) {
            setCursor(0, HEIGHT2 + 1);
            setColor(GREEN);
            cout << "won";
            break;
        }

        if (gameover) {
            for (int y = 0; y < HEIGHT2; y++) {
                for (int x = 0; x < WIDTH2; x++) {
                    if (mines2[y][x]) revealed2[y][x] = true;
                }
            }
            draw2(-1, -1);
            setCursor(0, HEIGHT2 + 1);
            setColor(RED);
            cout << "explode";
            break;
        }

        int ch = _getch();
        if (ch == 224) ch = _getch();

        switch (ch) {
        case Key::LEFT:  if (cursorX > 0) cursorX--; break;
        case Key::RIGHT: if (cursorX < WIDTH2 - 1) cursorX++; break;
        case Key::UP:    if (cursorY > 0) cursorY--; break;
        case Key::DOWN:  if (cursorY < HEIGHT2 - 1) cursorY++; break;
        case Key::SPACE:
            if (!flagged2[cursorY][cursorX] && !revealed2[cursorY][cursorX]) {
                reveal2(cursorX, cursorY);
                if (mines2[cursorY][cursorX]) {
                    gameover = true;
                }
            }
            break;
        case Key::ENTER:
            if (!revealed2[cursorY][cursorX] && !gameover)
                flagged2[cursorY][cursorX] = !flagged2[cursorY][cursorX];
            break;
        }
    }

    setCursor(0, HEIGHT2 + 2);
    system("pause");
    return 0;
}

bool dif3() {
    placeMines3();
    calculateAdjacent3();

    int cursorX = 0, cursorY = 0;
    bool gameover = false;

    while (true) {
        draw3(cursorX, cursorY);

        if (checkWin3()) {
            setCursor(0, HEIGHT3 + 1);
            setColor(GREEN);
            cout << "won";
            break;
        }

        if (gameover) {
            for (int y = 0; y < HEIGHT3; y++) {
                for (int x = 0; x < WIDTH3; x++) {
                    if (mines3[y][x]) revealed3[y][x] = true;
                }
            }
            draw3(-1, -1);
            setCursor(0, HEIGHT3 + 1);
            setColor(RED);
            cout << "explode";
            break;
        }

        int ch = _getch();
        if (ch == 224) ch = _getch();

        switch (ch) {
        case Key::LEFT:  if (cursorX > 0) cursorX--; break;
        case Key::RIGHT: if (cursorX < WIDTH3 - 1) cursorX++; break;
        case Key::UP:    if (cursorY > 0) cursorY--; break;
        case Key::DOWN:  if (cursorY < HEIGHT3 - 1) cursorY++; break;
        case Key::SPACE:
            if (!flagged3[cursorY][cursorX] && !revealed3[cursorY][cursorX]) {
                reveal3(cursorX, cursorY);
                if (mines3[cursorY][cursorX]) {
                    gameover = true;
                }
            }
            break;
        case Key::ENTER:
            if (!revealed3[cursorY][cursorX] && !gameover)
                flagged3[cursorY][cursorX] = !flagged3[cursorY][cursorX];
            break;
        }
    }

    setCursor(0, HEIGHT3 + 2);
    system("pause");
    return 0;
}

void minesweeper() {
    int choice;

    do {
        setColor(DARKGREY);
        cout << "Minesweeper\n";
        setColor(WHITE);
        cout << "1 Easy\n";
        cout << "2 Normal\n";
        cout << "3 Hard\n";
        cin >> choice;
        system("cls");
    } while (choice < 1 || choice > 3);

    srand((unsigned int)time(0));
    system("title Minesweeper");

    switch (choice) {
    case 1:
        if (dif1()) return;
        break;
    case 2:
        if (dif2()) return;
        break;
    case 3:
        if (dif3()) return;
        break;
    default:
        cout << "wrong.\n";
        break;
    }
}