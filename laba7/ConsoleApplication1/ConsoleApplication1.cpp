#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// Класс Блюдо
class Dish {
public:
    string name;
    double price;
    int cookingTime; // в минутах

    Dish(string n, double p, int t) : name(n), price(p), cookingTime(t) {}

    void display() const {
        cout << name << " - " << price << " руб. (время: " << cookingTime << " мин)" << endl;
    }
};

// Класс Заказ
class Order {
public:
    int id;
    vector<Dish> dishes;
    string status; // "ожидает", "готовится", "готов"
    double totalPrice;

    Order(int orderId) : id(orderId), status("ожидает"), totalPrice(0) {}

    void addDish(const Dish& dish) {
        dishes.push_back(dish);
        totalPrice += dish.price;
    }

    void showOrder() const {
        cout << "Заказ #" << id << " Статус: " << status << endl;
        cout << "Блюда:" << endl;
        for (const auto& dish : dishes) {
            cout << " - ";
            dish.display();
        }
        cout << "Итого: " << totalPrice << " руб." << endl;
    }

    void setStatus(const string& newStatus) {
        status = newStatus;
    }
};

// Класс Ресторан
class Restaurant {
private:
    vector<Dish> menu;
    vector<Order> orders;
    int nextOrderId;

public:
    Restaurant() : nextOrderId(1) {
        // Наполняем меню по умолчанию
        menu.push_back(Dish("Салат Цезарь", 350, 10));
        menu.push_back(Dish("Стейк Рибай", 1200, 25));
        menu.push_back(Dish("Паста Карбонара", 450, 15));
        menu.push_back(Dish("Торт Наполеон", 250, 5));
        menu.push_back(Dish("Кофе Латте", 180, 3));
    }

    void showMenu() const {
        cout << "\n=== МЕНЮ РЕСТОРАНА ===" << endl;
        for (size_t i = 0; i < menu.size(); ++i) {
            cout << i + 1 << ". ";
            menu[i].display();
        }
    }

    void createOrder() {
        Order newOrder(nextOrderId++);
        int choice;
        do {
            showMenu();
            cout << "Выберите блюдо (0 - завершить заказ): ";
            cin >> choice;
            if (choice > 0 && choice <= static_cast<int>(menu.size())) {
                newOrder.addDish(menu[choice - 1]);
                cout << "Блюдо добавлено." << endl;
            }
        } while (choice != 0);

        if (newOrder.dishes.empty()) {
            cout << "Заказ отменен (пуст)." << endl;
            return;
        }

        orders.push_back(newOrder);
        cout << "Заказ #" << newOrder.id << " создан. Статус: " << newOrder.status << endl;
    }

    void showOrders() const {
        if (orders.empty()) {
            cout << "Нет активных заказов." << endl;
            return;
        }
        cout << "\n=== АКТИВНЫЕ ЗАКАЗЫ ===" << endl;
        for (const auto& order : orders) {
            order.showOrder();
            cout << "-------------------" << endl;
        }
    }

    void updateOrderStatus() {
        int id;
        cout << "Введите номер заказа для обновления статуса: ";
        cin >> id;

        for (auto& order : orders) {
            if (order.id == id) {
                cout << "Текущий статус: " << order.status << endl;
                cout << "Новый статус (1-ожидает, 2-готовится, 3-готов): ";
                int stat;
                cin >> stat;
                if (stat == 1) order.setStatus("ожидает");
                else if (stat == 2) order.setStatus("готовится");
                else if (stat == 3) order.setStatus("готов");
                else cout << "Неверный статус." << endl;
                return;
            }
        }
        cout << "Заказ не найден." << endl;
    }

    void generateBill() {
        int id;
        cout << "Введите номер заказа для выставления счета: ";
        cin >> id;

        for (const auto& order : orders) {
            if (order.id == id) {
                cout << "\n=== СЧЕТ ЗАКАЗ #" << order.id << " ===" << endl;
                order.showOrder();
                cout << "Оплатите: " << order.totalPrice << " руб." << endl;
                return;
            }
        }
        cout << "Заказ не найден." << endl;
    }
};

// Главное меню
void showMainMenu() {
    cout << "\n=== СИСТЕМА РЕСТОРАН ===" << endl;
    cout << "1. Показать меню" << endl;
    cout << "2. Создать заказ" << endl;
    cout << "3. Показать все заказы" << endl;
    cout << "4. Обновить статус заказа" << endl;
    cout << "5. Выставить счет" << endl;
    cout << "6. Выход" << endl;
    cout << "Выберите действие: ";
}

int main() {
    Restaurant restaurant;
    int choice;
    setlocale(LC_ALL, "rus");
    do {
        showMainMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            restaurant.showMenu();
            break;
        case 2:
            restaurant.createOrder();
            break;
        case 3:
            restaurant.showOrders();
            break;
        case 4:
            restaurant.updateOrderStatus();
            break;
        case 5:
            restaurant.generateBill();
            break;
        case 6:
            cout << "Выход из программы." << endl;
            break;
        default:
            cout << "Неверный выбор. Попробуйте снова." << endl;
        }
    } while (choice != 6);

    return 0;
}