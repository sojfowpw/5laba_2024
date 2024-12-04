#include <iostream>

using namespace std;

class Animal { // родительский класс животных 
    public:
    int x, y;
    Animal(int x, int y, int direction, int k) : x(x), y(y), direction(direction), k(k) {}
    virtual ~Animal() {} // деструктор

    virtual void moving(int M, int N) = 0; // движение
    virtual void eating(Animal& rabbit) = 0; // питание

    protected:
    int direction;
    int k;
};

class Predator : public Animal { // класс хищник
    public:
    Predator(int x, int y, int direction, int k, int hungerThreshold) : Animal(x, y, direction, k), hunger(0),
    hungerThreshold(hungerThreshold) {}

    void moving(int M, int N) override { // движение
        static int moveCount = 0; // счётчик ходов
        if (moveCount % k == 0 && moveCount != 0) {
            direction = (direction + 1) % 4; // смена направления
        }
        switch (direction) {
            case 0: // вверх
                y = (y - 2 + M) % M;
                break;
            case 1: // вправо
                x = (x + 2) % N;
                break;
            case 2: // вниз
                y = (y + 2) % M;
                break;
            case 3: // влево
                x = (x - 2 + N) % N;
                break;
        }
        moveCount++;
    }

    void eating(Animal& rabbit) override {
        if (x == rabbit.x && y == rabbit.y) {
            hunger++;
        }
        cout << hunger << endl;
    }

    private:
    int hunger; // текущее насыщение
    int hungerThreshold; // максимальное насыщение
};

class Victim : public Animal { // класс жертва
    public:
    Victim(int x, int y, int direction, int k) : Animal(x, y, direction, k) {}

    void moving(int M, int N) override { // движение
        static int moveCount = 0; // счётчик ходов
        if (moveCount % k == 0 && moveCount != 0) {
            direction = (direction + 1) % 4; // смена направления
        }
        switch (direction) {
            case 0: // вверх
                y = (y - 1 + M) % M;
                break;
            case 1: // вправо
                x = (x + 1) % N;
                break;
            case 2: // вниз
                y = (y + 1) % M;
                break;
            case 3: // влево
                x = (x - 1 + N) % N;
                break;
        }
        moveCount++;
    }

    void eating(Animal& rabbit) override {}
};

void printField(int M, int N, Animal& wolf, Animal& rabbit) { // вывод поля
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (i == wolf.y && j == wolf.x) {
                cout << "w";
            }
            else if (i == rabbit.y && j == rabbit.x) {
                cout << "r";
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
    Predator wolf(0, 0, 1, 100, 2);
    Victim rabbit(0, 0, 1, 100);
    printField(M, N, wolf, rabbit);
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
        wolf.moving(M, N);
        rabbit.moving(M, N);
        wolf.eating(rabbit);
        printField(M, N, wolf, rabbit);
        cout << endl << endl;
    }
    //delete &a;
    return 0;
}