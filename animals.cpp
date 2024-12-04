#include <iostream>

using namespace std;

class Animal { // родительский класс животных 
    public: //
    int x, y;
    Animal(int a_x, int a_y, int a_direction, int a_k) : x(a_x), y(a_y), direction(a_direction), k(a_k) {}
    virtual ~Animal() {} // деструктор

    void moving(int M, int N) {
        static int moveCount = 0;
        if (moveCount % k == 0 && moveCount != 0) {
            direction = (direction + 1) % 4;
        }
        switch (direction) {
            case 0:
                y = (y - 1 + M) % M;
                break;
            case 1:
                x = (x + 1) % N;
                break;
            case 2:
                y = (y + 1) % M;
                break;
            case 3:
                x = (x - 1 + N) % N;
                break;
        }
        moveCount++;
        cout << x << ", " << y << endl;
    }

    private:
    //int x, y;
    int direction;
    int k;
};

void printField(int M, int N, Animal& a) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (i == a.y && j == a.x) {
                cout << "A";
            }
            else {
                cout << "*";
            }
        }
        cout << endl;
    }
}

int main() {
    int M, N;//, T; // размеры поля и количество ходов
    cin >> M >> N; // M - длина, N - ширина
    Animal a(0, 0, 1, 2);
    printField(M, N, a);
    cout << endl << endl;
    // int R, W; // начальное количество жертв и хищников
    // int x, y, d, k; // x, y - координаты ({0, 0} - левый верхний угол), d - начальное направление, k - ходы перед сменой направления
    // for (int i = 0; i < R; i++) {
    //     cin >> x >> y >> d >> k;
    // }
    // for (int i = 0; i < W; i++) {
    //     cin >> x >> y >> d >> k;
    // }
    for (int i = 0; i < 7; i++) {
        a.moving(M, N);
        printField(M, N, a);
        cout << endl << endl;
    }
    //delete &a;
    return 0;
}