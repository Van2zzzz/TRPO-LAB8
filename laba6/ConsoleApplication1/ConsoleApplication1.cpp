#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <chrono>
#include <thread>
#include <iomanip>
#include <algorithm>

using namespace std;


enum class OrderStatus {
    PENDING,
    CONFIRMED,
    IN_PROGRESS,
    COMPLETED,
    CANCELLED
};


class MenuItem {
private:
    string name;
    double price;
    string description;

public:
    MenuItem(const string& n, double p, const string& desc = "")
        : name(n), price(p), description(desc) {
    }

    string getName() const { return name; }
    double getPrice() const { return price; }
    string getDescription() const { return description; }

    void display() const {
        cout << name << " - " << fixed << setprecision(2) << price
            << " руб. (" << description << ")" << endl;
    }
};


class Client {
private:
    int id;
    string name;
    string phone;

public:
    Client(int i, const string& n, const string& ph)
        : id(i), name(n), phone(ph) {
    }

    int getId() const { return id; }
    string getName() const { return name; }
    string getPhone() const { return phone; }

    void display() const {
        cout << name << " (тел: " << phone << ")" << endl;
    }
};


class Order {
private:
    static int nextId;
    int id;
    vector<MenuItem> items;
    Client* client;
    OrderStatus status;
    time_t createdAt;

public:
    Order(const vector<MenuItem>& it, Client* cl)
        : items(it), client(cl), status(OrderStatus::PENDING) {
        id = nextId++;
        createdAt = time(nullptr);
    }

    int getId() const { return id; }
    OrderStatus getStatus() const { return status; }
    Client* getClient() const { return client; }
    const vector<MenuItem>& getItems() const { return items; }

    void setStatus(OrderStatus s) { status = s; }

    double calculateTotal() const {
        double total = 0.0;
        for (const auto& item : items) {
            total += item.getPrice();
        }
        return total;
    }

    void complete() {
        status = OrderStatus::COMPLETED;
        cout << "Заказ #" << id << " завершен и готов к подаче." << endl;
    }

    void displayOrderInfo() const {
        cout << "\n=== ЗАКАЗ #" << id << " ===" << endl;
        cout << "Клиент: " << client->getName() << endl;
        cout << "Статус: " << statusToString() << endl;

        char buffer[26];
        ctime_s(buffer, sizeof(buffer), &createdAt);
        cout << "Создан: " << buffer;

        cout << "Сумма: " << fixed << setprecision(2) << calculateTotal() << " руб." << endl;
        cout << "Состав заказа:" << endl;

        int itemNumber = 1;
        for (const auto& item : items) {
            cout << "  " << itemNumber << ". " << item.getName()
                << " - " << fixed << setprecision(2) << item.getPrice() << " руб." << endl;
            itemNumber++;
        }
        cout << "=================" << endl;
    }

    string statusToString() const {
        switch (status) {
        case OrderStatus::PENDING: return "Ожидает подтверждения";
        case OrderStatus::CONFIRMED: return "Подтвержден";
        case OrderStatus::IN_PROGRESS: return "Готовится";
        case OrderStatus::COMPLETED: return "Завершен";
        case OrderStatus::CANCELLED: return "Отменен";
        default: return "Неизвестно";
        }
    }
};

int Order::nextId = 1;


class Menu {
private:
    vector<MenuItem> items;

public:
    void addItem(const MenuItem& item) {
        items.push_back(item);
        cout << "Блюдо '" << item.getName() << "' добавлено в меню" << endl;
    }

    MenuItem* findDish(const string& name) {
        for (auto& item : items) {
            if (item.getName() == name) {
                return &item;
            }
        }
        return nullptr;
    }

    void displayMenu() const {
        cout << "\n=== МЕНЮ РЕСТОРАНА ===" << endl;
        if (items.empty()) {
            cout << "Меню пустое" << endl;
            return;
        }

        int index = 1;
        for (const auto& item : items) {
            cout << index << ". ";
            item.display();
            index++;
        }
        cout << "=====================" << endl;
    }

    const vector<MenuItem>& getAllItems() const {
        return items;
    }
};


class Administrator {
private:
    int id;
    string name;

public:
    Administrator(int i, const string& n) : id(i), name(n) {
        cout << "Администратор " << name << " начал работу" << endl;
    }

    bool confirmOrder(Order* order) {
        if (!order) {
            cout << "Заказ не найден!" << endl;
            return false;
        }

        if (order->getStatus() == OrderStatus::PENDING) {
            order->setStatus(OrderStatus::CONFIRMED);
            cout << "Администратор " << name << " подтвердил заказ #" << order->getId() << "." << endl;
            return true;
        }

        cout << "Заказ #" << order->getId() << " не может быть подтвержден." << endl;
        return false;
    }

    void sendToKitchen(Order* order, class Kitchen* kitchen);

    class Bill* createBill(Order* order);
};


class Kitchen {
public:
    void receiveOrder(Order* order) {
        if (!order) {
            cout << "Заказ не найден!" << endl;
            return;
        }

        cout << "\nКухня получила заказ #" << order->getId() << ". Начинаем приготовление..." << endl;

      
        int cookingTime = 0;
        for (const auto& item : order->getItems()) {
            cout << "  Готовим: " << item.getName() << endl;
            this_thread::sleep_for(chrono::milliseconds(500)); 
            cookingTime += 1;
        }

        order->complete();
        cout << "Заказ #" << order->getId() << " приготовлен за " << cookingTime << " секунд." << endl;
    }
};


class Bill {
private:
    static int nextId;
    int id;
    Order* order;
    double totalAmount;
    bool isPaid;
    time_t issuedAt;

public:
    Bill(Order* ord) : order(ord), isPaid(false) {
        id = nextId++;
        totalAmount = order->calculateTotal();
        issuedAt = time(nullptr);
    }

    int getId() const { return id; }
    double getTotalAmount() const { return totalAmount; }
    bool getIsPaid() const { return isPaid; }
    Order* getOrder() const { return order; }

    void setIsPaid(bool paid) { isPaid = paid; }

    void printBill() const {
        cout << "\n=== СЧЕТ #" << id << " ===" << endl;

        char buffer[26];
        ctime_s(buffer, sizeof(buffer), &issuedAt);
        cout << "Дата: " << buffer;

        cout << "Заказ: #" << order->getId() << endl;
        cout << "Клиент: " << order->getClient()->getName() << endl;
        cout << "Телефон: " << order->getClient()->getPhone() << endl;
        cout << "\nСостав заказа:" << endl;

        int itemNumber = 1;
        for (const auto& item : order->getItems()) {
            cout << "  " << itemNumber << ". " << item.getName()
                << " - " << fixed << setprecision(2) << item.getPrice() << " руб." << endl;
            itemNumber++;
        }

        cout << "\nИтого к оплате: " << fixed << setprecision(2) << totalAmount << " руб." << endl;
        cout << "Статус оплаты: " << (isPaid ? "ОПЛАЧЕНО" : "НЕ ОПЛАЧЕНО") << endl;
        cout << "=================" << endl;
    }
};

int Bill::nextId = 1;

void Administrator::sendToKitchen(Order* order, Kitchen* kitchen) {
    if (!order) {
        cout << "Заказ не найден!" << endl;
        return;
    }

    if (order->getStatus() == OrderStatus::CONFIRMED) {
        cout << "Администратор " << name << " отправляет заказ #" << order->getId() << " на кухню..." << endl;
        kitchen->receiveOrder(order);
        order->setStatus(OrderStatus::IN_PROGRESS);
    }
    else {
        cout << "Заказ #" << order->getId() << " не может быть отправлен на кухню." << endl;
    }
}

Bill* Administrator::createBill(Order* order) {
    if (!order) {
        cout << "Заказ не найден!" << endl;
        return nullptr;
    }

    if (order->getStatus() == OrderStatus::COMPLETED) {
        Bill* bill = new Bill(order);
        cout << "Счет #" << bill->getId() << " на сумму " << fixed << setprecision(2)
            << bill->getTotalAmount() << " руб. выставлен для заказа #" << order->getId() << "." << endl;
        return bill;
    }

    cout << "Нельзя выставить счет для заказа #" << order->getId() << "." << endl;
    return nullptr;
}


int main() {
    cout << "=== СИСТЕМА УПРАВЛЕНИЯ РЕСТОРАНОМ ===" << endl << endl;
    setlocale(LC_ALL, "rus");

   
    Menu menu;

   
    menu.addItem(MenuItem("Пицца Маргарита", 12.99, "Классическая пицца с томатами и моцареллой"));
    menu.addItem(MenuItem("Пицца Пепперони", 14.99, "Острая пицца с пепперони и сыром"));
    menu.addItem(MenuItem("Салат Цезарь", 8.49, "Салат с курицей, сыром и соусом Цезарь"));
    menu.addItem(MenuItem("Греческий салат", 7.99, "Салат с оливками, фетой и овощами"));
    menu.addItem(MenuItem("Томатный суп", 6.99, "Ароматный суп с томатами и травами"));
    menu.addItem(MenuItem("Лазанья", 11.49, "Итальянская лазанья с мясным соусом"));
    menu.addItem(MenuItem("Тирамису", 5.99, "Итальянский десерт"));
    menu.addItem(MenuItem("Кока-Кола", 2.49, "Напиток 0.5л"));

 
    menu.displayMenu();

    
    Client client(1, "Иван Иванов", "+375291234567");
    cout << "Клиент создан: ";
    client.display();

   
    cout << "\nКлиент " << client.getName() << " делает заказ..." << endl;

    vector<MenuItem> selectedItems;
    vector<string> dishNames = { "Пицца Пепперони", "Греческий салат", "Кока-Кола", "Тирамису" };

    for (const auto& dishName : dishNames) {
        MenuItem* item = menu.findDish(dishName);
        if (item) {
            selectedItems.push_back(*item);
            cout << "  Выбрано: " << item->getName() << " - " << fixed << setprecision(2)
                << item->getPrice() << " руб." << endl;
        }
        else {
            cout << "  Блюдо '" << dishName << "' не найдено в меню" << endl;
        }
    }

    if (selectedItems.empty()) {
        cout << "Не выбрано ни одного блюда! Программа завершена." << endl;
        return 0;
    }

    Order order(selectedItems, &client);
    cout << "Заказ #" << order.getId() << " создан. Общая сумма: "
        << fixed << setprecision(2) << order.calculateTotal() << " руб." << endl;

    
    Administrator admin(1, "Анна Петрова");

    
    admin.confirmOrder(&order);
    order.displayOrderInfo();

    
    Kitchen kitchen;
    admin.sendToKitchen(&order, &kitchen);

  
    Bill* bill = admin.createBill(&order);
    if (bill) {
        bill->printBill();
    }

    
    if (bill) {
        cout << "\nКлиент " << client.getName() << " оплачивает счет #" << bill->getId() << "..." << endl;
        this_thread::sleep_for(chrono::milliseconds(1000)); // Имитация процесса оплаты
        bill->setIsPaid(true);
        cout << "Счет #" << bill->getId() << " на сумму " << fixed << setprecision(2)
            << bill->getTotalAmount() << " руб. успешно оплачен." << endl;
    }

   
    cout << "\n=== ИТОГИ РАБОТЫ СИСТЕМЫ ===" << endl;
    cout << "1. Создан заказ #" << order.getId() << endl;
    cout << "2. Заказ подтвержден администратором" << endl;
    cout << "3. Заказ приготовлен на кухне" << endl;
    if (bill) {
        cout << "4. Выставлен счет #" << bill->getId() << endl;
        cout << "5. Счет " << (bill->getIsPaid() ? "оплачен" : "не оплачен") << endl;
    }
    cout << "\nОбщая выручка: " << fixed << setprecision(2) << order.calculateTotal() << " руб." << endl;
    cout << "\nРабота системы успешно завершена!" << endl;

    if (bill) {
        delete bill;
    }

    cout << "\nНажмите Enter для выхода...";
    cin.get();

    return 0;
}