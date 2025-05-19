#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
using namespace std;

//
//controls and colors
enum Key { LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80, ENTER = 13, SPACE = 32 };
enum Color {
    BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHTGREY,
    DARKGREY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED,
    LIGHTMAGENTA, YELLOW, WHITE
};
//ingame states
struct stats {
    int width, height, mines;
    bool** mine;
    bool** revealed;
    bool** flagged;
    int** adjacent;
};
//
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); //SetConsoleCursorPosition; SetConsoleTextAttribute

void setCursor(int x, int y) {
    COORD c = { (short)x, (short)y };
    SetConsoleCursorPosition(h, c);
} //cursor position

void setColor(Color c) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
} //text color

void placeMines(stats& dif) {
    int placed = 0; // current mine amount int
    while (placed < dif.mines) {
        int x = rand() % dif.width; // mine gen
        int y = rand() % dif.height; // mine gen
        if (!dif.mine[y][x]) {
            dif.mine[y][x] = true;
            placed++;
        }
    }
}

void calcAdj(stats& dif) {
    for (int y = 0; y < dif.height; y++) {
        for (int x = 0; x < dif.width; x++) {
            if (dif.mine[y][x]) {
                dif.adjacent[y][x] = -1;
                continue;
            }
            int count = 0;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int nx = x + dx, ny = y + dy;
                    if (nx >= 0 && nx < dif.width && ny >= 0 && ny < dif.height && dif.mine[ny][nx])
                        count++;
                }
            }
            dif.adjacent[y][x] = count;
        }
    }
}

void reveal(stats& dif, int x, int y) {
    if (x < 0 || y < 0 || x >= dif.width || y >= dif.height || dif.revealed[y][x] || dif.flagged[y][x])
        return;
    dif.revealed[y][x] = true;
    if (dif.adjacent[y][x] == 0 && !dif.mine[y][x]) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx != 0 || dy != 0)
                    reveal(dif, x + dx, y + dy);
            }
        }
    }
}

void draw(const stats& dif, int cursorX, int cursorY) {
    system("cls");
    for (int y = 0; y < dif.height; y++) {
        for (int x = 0; x < dif.width; x++) {
            if (x == cursorX && y == cursorY) 
                setColor(DARKGREY);
            else 
                setColor(WHITE);
            if (dif.revealed[y][x]) {
                if (dif.mine[y][x]) cout << "* ";
                else cout << dif.adjacent[y][x] << ' ';
            }
            else if (dif.flagged[y][x]) {
                setColor(RED);
                cout << "F ";
                setColor(WHITE);
            }
            else {
                cout << "# ";
            }
        }
        cout << "\n";
        setColor(LIGHTGREY);
    }
}

bool checkWin(const stats& dif) {
    for (int y = 0; y < dif.height; y++) {
        for (int x = 0; x < dif.width; x++) {
            if (!dif.mine[y][x] && !dif.revealed[y][x])
                return false;
            if (dif.mine[y][x] && !dif.flagged[y][x])
                return false;
        }
    }
    return true;
}

void lastwinner() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    string uname;
    cout << "you are the last winner! write your username: ";
    cin >> uname;
    FILE* file;
    int error = fopen_s(&file, "C:/Users/Admin/Desktop/win.txt", "w");
    if (error == 0 && file != nullptr) {
        fprintf(file, "the winner is \"%s\"\n", uname.c_str()); //i looked "%s" up cause i coudlnt figure out how to put a string in there.
        fclose(file);
    }
    else {
        cout << "eror\n";
    }
}

void initDif(stats& dif, int width, int height, int mines) {
    dif.width = width;
    dif.height = height;
    dif.mines = mines;
    dif.mine = new bool* [height];
    dif.revealed = new bool* [height];
    dif.flagged = new bool* [height];
    dif.adjacent = new int* [height];
    for (int i = 0; i < height; i++) {
        dif.mine[i] = new bool[width] {};
        dif.revealed[i] = new bool[width] {};
        dif.flagged[i] = new bool[width] {};
        dif.adjacent[i] = new int[width] {};
    }

    placeMines(dif);
    calcAdj(dif);
}

void cleanupDif(stats& dif) {
    for (int i = 0; i < dif.height; i++) {
        delete[] dif.mine[i];
        delete[] dif.revealed[i];
        delete[] dif.flagged[i];
        delete[] dif.adjacent[i];
    }
    delete[] dif.mine;
    delete[] dif.revealed;
    delete[] dif.flagged;
    delete[] dif.adjacent;
}

void difChoice(int width, int height, int mines) {
    stats dif;
    initDif(dif, width, height, mines);

    int cursorX = 0, cursorY = 0;
    bool gameOver = false;
    bool win = false;

    while (!gameOver) {
        draw(dif, cursorX, cursorY);
        char input = _getch();

        switch (input) {
        case UP: if (cursorY > 0) cursorY--; break;
        case DOWN: if (cursorY < height - 1) cursorY++; break;
        case LEFT: if (cursorX > 0) cursorX--; break;
        case RIGHT: if (cursorX < width - 1) cursorX++; break;
        case SPACE:
            if (!dif.flagged[cursorY][cursorX]) {
                if (dif.mine[cursorY][cursorX]) {
                    dif.revealed[cursorY][cursorX] = true;
                    gameOver = true;
                    win = false;
                }
                else {
                    reveal(dif, cursorX, cursorY); // recursive reveal logic
                    if (checkWin(dif)) {
                        gameOver = true;
                        win = true;
                        lastwinner();
                    }
                }
            }
            break;
        case ENTER:
            if (!dif.revealed[cursorY][cursorX])
                dif.flagged[cursorY][cursorX] = !dif.flagged[cursorY][cursorX];
                if (checkWin(dif)) {
                    gameOver = true;
                    win = true;
                    lastwinner();
                }
            break;
        }
    }
    draw(dif, cursorX, cursorY);
    cout << (win ? "winner" : "explode") << "\n";
    cleanupDif(dif);
}

void minesweeper() {
    int choice;
    do {
        setColor(DARKGREY);
        cout << "Minesweeper\n";
        setColor(WHITE);
        cout << "1 Beginner\n";
        cout << "2 Intermediate\n";
        cout << "3 Expert\n";
        cin >> choice;
        system("cls");
    } while (choice < 1 || choice > 3);

    srand(time(0));
    system("title Minesweeper");

    switch (choice) {
    case 1:
        difChoice(8, 8, 10);
        break;
    case 2:
        difChoice(16, 16, 40);
        break;
    case 3:
        difChoice(30, 16, 99);
        break;
    default:
        cout << "wrong.\n";
        break;
    }
}