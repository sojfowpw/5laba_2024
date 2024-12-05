#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Animal { // родительский класс животных 
public:
    Animal(int x, int y, int direction, int k, int maxAge) : x(x), y(y), direction(direction), k(k), age(0), maxAge(maxAge), moveCount(0) {}
    virtual ~Animal() {} // деструктор

    virtual void moving(int M, int N) = 0; // движение
    virtual void eating(vector<Animal*>& animals) = 0; // питание
    virtual void reproduce(vector<Animal*>& animals) = 0;
    
    void aging() { // старение
        age++;
    }
    bool isDead() { // вымирание
        return age >= maxAge;
    }

    int getX() const {
        return x;
    }
    int getY() const {
        return y;
    }

protected:
    int x, y;
    int direction;
    int k;
    int age, maxAge;
    int moveCount;
};

class Victim : public Animal { // класс жертва
public:
    Victim(int x, int y, int direction, int k, int maxAge, int moveCount) : Animal(x, y, direction, k, maxAge), moveCount(moveCount) {}

    void moving(int M, int N) override { // движение
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

    void eating(vector<Animal*>& animals) override {}

    void reproduce(vector<Animal*>& animals) override {
        if (age == 5 || age == 10) {
            animals.push_back(new Victim(x, y, direction, k, maxAge, moveCount));
        }
    }

    private:
    int moveCount;
};

class Predator : public Animal { // класс хищник
public:
    Predator(int x, int y, int direction, int k, int maxAge, int hungerThreshold, int moveCount) : Animal(x, y, direction, k, maxAge), hunger(0), hungerThreshold(hungerThreshold), moveCount(moveCount) {}

    void moving(int M, int N) override { // движение
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

    void eating(vector<Animal*>& animals) override { // поедание
        auto it = remove_if(animals.begin(), animals.end(), [this](Animal* a){
            if (a != this && a->getX() == x && a->getY() == y && dynamic_cast<Victim*>(a)) {
                hunger++; // если координаты совпадают и объект из класса жертва
                cout << "Жертву съел хищник, насыщение = " << hunger << endl;
                return true;
            }
            return false;
        });
        animals.erase(it, animals.end());
    }

    void reproduce(vector<Animal*>& animals) override { // размножение
        if (hunger >= hungerThreshold) { // если насыщение >= 2
            animals.push_back(new Predator(x, y, direction, k, maxAge, hungerThreshold, moveCount));
            hunger = 0;
        }
    }

private:
    int hunger; // текущее насыщение
    int hungerThreshold; // максимальное насыщение
    int moveCount; // добавляем поле moveCount
};

void printField(int M, int N, const vector<Animal*>& animals) { // вывод поля
    vector<vector<int>> field(M, vector<int>(N, 0)); // матрица нулей
    for (const Animal* animal : animals) {
        if (dynamic_cast<const Predator*>(animal)) { // считаем количество хищников
            field[animal->getY()][animal->getX()]--;
        } else if (dynamic_cast<const Victim*>(animal)) { // считаем количество жертв
            field[animal->getY()][animal->getX()]++;
        }
    }
    for (int y = 0; y < M; ++y) {
        for (int x = 0; x < N; ++x) {
            if (field[y][x] == 0) {
                cout << '*';
            } else if (field[y][x] > 0) {
                cout << '+' << field[y][x];
            } else {
                cout << field[y][x];
            }
        }
        cout << endl;
    }
    cout << "\n\n";
}

void life(int M, int N, int T, vector<Animal*>& animals) {
    for (int i = 0; i < T; i++) {
        for (Animal* animal : animals) {
            animal->moving(M, N);
        }
        for (Animal* animal : animals) {
            animal->eating(animals);
        }
        for (Animal* animal : animals) {
            animal->aging();
        }
        for (Animal* animal : animals) {
            animal->reproduce(animals);
        }
        animals.erase(remove_if(animals.begin(), animals.end(), [](Animal* a) {
            return a->isDead();
        }), animals.end());
        printField(M, N, animals);
    }
}

int main() {
    int M, N, T; // размеры поля и количество ходов
    cin >> M >> N >> T; // M - длина, N - ширина
    vector<Animal*> animals;
    int R, W; // начальное количество жертв и хищников
    cin >> R >> W;
    int x, y, d, k; // x, y - координаты ({0, 0} - левый верхний угол), d - начальное направление, k - ходы перед сменой направления
    for (int i = 0; i < R; i++) {
        cin >> x >> y >> d >> k;
        animals.push_back(new Victim(x, y, d, k, 25, 0));
    }
    for (int i = 0; i < W; i++) {
        cin >> x >> y >> d >> k;
        animals.push_back(new Predator(x, y, d, k, 30, 2, 0));
    }
    cout << "Начальное поле:\n";
    printField(M, N, animals);
    life(M, N, T, animals);
    for (Animal* animal : animals) {
        delete animal;
    }
    return 0;
}