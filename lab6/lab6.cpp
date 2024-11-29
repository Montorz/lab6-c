#include <iostream>
#include <string>
#include <stdexcept>

class InvalidBalanceException : public std::runtime_error {
public:
    InvalidBalanceException() : std::runtime_error("Недопустимый баланс!") {}
};

class User {
protected:
    int id;
    std::string name;
    std::string login;
    std::string password;
    float balance;

public:
    User(int id, const std::string& name, const std::string& login, const std::string& password, float balance)
        : id(id), name(name), login(login), password(password) {
        if (balance < 0) {
            throw InvalidBalanceException();
        }
        this->balance = balance;
    }

    virtual void displayInfo() const {
        std::cout << "ID: " << id << ", Имя: " << name << ", Баланс: " << balance << std::endl;
    }

    int getId() const { return id; }
    const std::string& getName() const { return name; }
    float getBalance() const { return balance; }

    virtual ~User() = default;
};

// Производный класс для VIP-пользователей
class VIPUser : public User {
private:
    float cashbackRate;

public:
    VIPUser(int id, const std::string& name, const std::string& login, const std::string& password, float balance, float cashbackRate)
        : User(id, name, login, password, balance), cashbackRate(cashbackRate) {}

    void displayInfo() const override {
        User::displayInfo();
        std::cout << "Кэшбэк: " << cashbackRate * 100 << "%" << std::endl;
    }

    void addCashback() {
        balance += balance * cashbackRate;
    }

    // Перегрузка оператора присваивания
    VIPUser& operator=(const User& user) {
        if (this != &user) {
            id = user.getId();
            name = user.getName(); 
            login = "";                   // Логин и пароль оставил пустыми
            password = "";
            balance = user.getBalance();
        }
        return *this;
    }
};

// Абстрактный класс для игр
class Game {
protected:
    std::string name;

public:
    Game(const std::string& name) : name(name) {}
    virtual void play() = 0; // Вызов виртуальной функции
    virtual ~Game() = default;
};

// Производный класс SlotGame
class SlotGame : public Game {
private:
    int maxBet;
    int minBet;

public:
    SlotGame(const std::string& name, int maxBet, int minBet)
        : Game(name), maxBet(maxBet), minBet(minBet) {}

    void play() override {
        std::cout << "Играем в слот: " << name << " с мин. ставкой: " << minBet << " и макс. ставкой: " << maxBet << std::endl;
    }
};

// Производный класс BlackjackGame
class BlackjackGame : public Game {
private:
    int numberOfDecks;

public:
    BlackjackGame(const std::string& name, int numberOfDecks)
        : Game(name), numberOfDecks(numberOfDecks) {}

    void play() override {
        std::cout << "Играем в блэкджек: " << name << " с количеством колод: " << numberOfDecks << std::endl;
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    try {
        VIPUser vip(1, "Анна", "anna_vip", "vip123", 5000.0f, 0.05f);
        std::cout << "VIP пользователь:\n";
        vip.displayInfo();
        vip.addCashback();
        std::cout << "После добавления кэшбэка:\n";
        vip.displayInfo();

        VIPUser vipCopy = vip;
        std::cout << "Копия VIP пользователя:\n";
        vipCopy.displayInfo();

        VIPUser anotherVip(2, "Петр", "peter_vip", "vip456", 3000.0f, 0.1f);
        vipCopy = anotherVip;
        std::cout << "После присваивания:\n";
        vipCopy.displayInfo();

        SlotGame slot("Lucky 7", 100, 10);
        std::cout << "\nЗапуск игры в слот:\n";
        slot.play();

        BlackjackGame blackjack("21", 4);
        std::cout << "\nЗапуск игры в блэкджек:\n";
        blackjack.play();

        Game* game1 = new SlotGame("Jackpot", 200, 50);
        Game* game2 = new BlackjackGame("Blackjack Deluxe", 6);

        std::cout << "\nДинамическое создание игры:\n";
        game1->play();
        game2->play();

        delete game1;
        delete game2;

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}


