#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

// Класс Блюдо
class Dish {
public:
    string title;
    double price;
    string category;

    Dish() : title(""), price(0.0), category("") {}

    Dish(string t, double p, string c) : title(t), price(p), category(c) {}

    void display() const {
        cout << title << " - " << price << " руб. (" << category << ")" << endl;
    }
};

// Класс Меню
class Menu {
public:
    string menuName;
    vector<Dish> dishes;

    Menu() : menuName("") {}

    Menu(string name) : menuName(name) {}

    void addDish(const Dish& dish) {
        dishes.push_back(dish);
    }

    void removeDish(int index) {
        if (index >= 0 && index < dishes.size()) {
            dishes.erase(dishes.begin() + index);
        }
    }

    void displayMenu() const {
        cout << "=== Меню: " << menuName << " ===" << endl;
        for (size_t i = 0; i < dishes.size(); i++) {
            cout << i + 1 << ". ";
            dishes[i].display();
        }
        cout << "======================" << endl;
    }
};

// Класс Заказ
class Order {
public:
    int orderId;
    string status;
    vector<Dish> dishes;

    Order() : orderId(0), status("Новый") {}

    Order(int id) : orderId(id), status("Новый") {}

    void addDish(const Dish& dish) {
        dishes.push_back(dish);
    }

    void removeDish(int index) {
        if (index >= 0 && index < dishes.size()) {
            dishes.erase(dishes.begin() + index);
        }
    }

    double calculateTotal() const {
        double total = 0.0;
        for (const auto& dish : dishes) {
            total += dish.price;
        }
        return total;
    }

    void displayOrder() const {
        cout << "=== Заказ №" << orderId << " ===" << endl;
        cout << "Статус: " << status << endl;
        cout << "Блюда:" << endl;

        for (size_t i = 0; i < dishes.size(); i++) {
            cout << i + 1 << ". ";
            dishes[i].display();
        }

        cout << "Итого: " << calculateTotal() << " руб." << endl;
        cout << "======================" << endl;
    }
};

// Класс Счет
class Bill {
public:
    int billId;
    double amount;
    bool isPaid;

    Bill() : billId(0), amount(0.0), isPaid(false) {}

    Bill(int id) : billId(id), amount(0.0), isPaid(false) {}

    void generateBill(const Order& order) {
        amount = order.calculateTotal();
        cout << "Счет №" << billId << " на сумму "
            << fixed << setprecision(2) << amount
            << " руб. сформирован." << endl;
    }

    void markAsPaid() {
        isPaid = true;
        cout << "Счет №" << billId << " оплачен." << endl;
    }

    void displayBill() const {
        cout << "=== Счет №" << billId << " ===" << endl;
        cout << "Сумма: " << amount << " руб." << endl;
        cout << "Статус оплаты: " << (isPaid ? "Оплачен" : "Не оплачен") << endl;
        cout << "======================" << endl;
    }
};

// Класс Клиент
class Client {
public:
    string name;
    string phone;

    Client() : name(""), phone("") {}

    Client(string n, string p) : name(n), phone(p) {}

    void createOrder() const {
        cout << "Клиент " << name << " создал заказ." << endl;
    }

    void payBill() const {
        cout << "Клиент " << name << " оплатил счет." << endl;
    }

    void displayInfo() const {
        cout << "Клиент: " << name << endl;
        cout << "Телефон: " << phone << endl;
    }
};

// Класс Администратор
class Administrator {
public:
    string name;

    Administrator() : name("") {}

    Administrator(string n) : name(n) {}

    void confirmOrder(Order& order) {
        order.status = "Подтвержден";
        cout << "Заказ №" << order.orderId
            << " подтвержден администратором " << name << "." << endl;
    }

    void issueBill(Order& order, Bill& bill) {
        bill.generateBill(order);
        cout << "Счет №" << bill.billId
            << " выставлен клиенту администратором " << name << "." << endl;
    }

    void manageMenu(Menu& menu, const Dish& dish, const string& action) {
        if (action == "add") {
            menu.addDish(dish);
            cout << "Блюдо добавлено в меню администратором " << name << "." << endl;
        }
        else if (action == "remove") {
            // Поиск блюда по названию
            for (size_t i = 0; i < menu.dishes.size(); i++) {
                if (menu.dishes[i].title == dish.title) {
                    menu.removeDish(i);
                    cout << "Блюдо удалено из меню администратором " << name << "." << endl;
                    break;
                }
            }
        }
    }
};

// Главная функция для демонстрации работы
int main() {
    // Установка локали для русских символов
    setlocale(LC_ALL, "Russian");

    cout << "=== Система Ресторан ===" << endl;

    // Создание объектов
    Client client("Иван Иванов", "+7-999-123-45-67");
    Administrator admin("Анна Петрова");

    // Создание меню
    Menu mainMenu("Основное меню");

    Dish pizza("Пицца Маргарита", 450.0, "Основное блюдо");
    Dish pasta("Паста Карбонара", 350.0, "Основное блюдо");
    Dish salad("Греческий салат", 250.0, "Закуска");
    Dish juice("Апельсиновый сок", 120.0, "Напиток");

    admin.manageMenu(mainMenu, pizza, "add");
    admin.manageMenu(mainMenu, pasta, "add");
    admin.manageMenu(mainMenu, salad, "add");
    admin.manageMenu(mainMenu, juice, "add");

    cout << endl;
    mainMenu.displayMenu();

    // Создание заказа
    client.createOrder();
    Order order(1);

    order.addDish(pizza);
    order.addDish(salad);
    order.addDish(juice);

    cout << endl;
    order.displayOrder();

    // Подтверждение заказа администратором
    admin.confirmOrder(order);

    // Создание счета
    Bill bill(1001);
    admin.issueBill(order, bill);

    // Оплата счета клиентом
    client.payBill();
    bill.markAsPaid();

    cout << endl;
    cout << "=== Итоговая информация ===" << endl;
    client.displayInfo();
    order.displayOrder();
    bill.displayBill();

    return 0;
}