#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct Transaction {
    string type;
    int qtyChange;
    double priceChange;
    string timestamp;
};

struct StockItem {
    int id;
    string name;
    int quantity;
    double price;
    Transaction history[100];
    int historyCount;
    StockItem* next;

    StockItem(int i, string n, int q, double p) {
        id = i; name = n; quantity = q; price = p;
        historyCount = 0;
        next = nullptr;
    }
};

struct StockManager {
    struct User {
        string username;
        string password;
    };

    StockItem* head = nullptr;
    User currentUser;

    bool login() {
        string username, password;
        cout << "===== LOGIN =====\n";
        cout << "Username: "; cin >> username;
        cout << "Password: "; cin >> password;

        if (username == "admin" && password == "admin123") {
            currentUser.username = username;
            cout << "Login successful. Welcome, " << username << "!\n";
            return true;
        }

        cout << "Invalid credentials.\n";
        return false;
    }

    void addStock() {
        int quantity;
        string name;
        double price;

        cin.ignore();
        cout << "Enter name: "; getline(cin, name);

        // Generate ID based on name letters (case-insensitive)
        int id = 0;
        for (char c : name) {
            id += static_cast<int>(tolower(c));
        }
        id %= 1000; // keep ID within 3 digits

        // Ensure unique ID
        while (findByID(id)) {
            id = (id + 1) % 1000; // increment until unique
        }

        cout << "Assigned ID: " << id << "\n";

        cout << "Enter quantity: "; cin >> quantity;
        cout << "Enter price: "; cin >> price;

        StockItem* newItem = new StockItem(id, name, quantity, price);
        addTransaction(newItem, "Added", quantity, price);
        newItem->next = head;
        head = newItem;

        cout << "Stock added.\n";
    }

    void deleteStock() {
        int id;
        cout << "Enter ID to delete: ";
        cin >> id;

        StockItem *curr = head, *prev = nullptr;
        while (curr && curr->id != id) {
            prev = curr;
            curr = curr->next;
        }

        if (!curr) {
            cout << "Stock not found.\n";
            return;
        }

        if (!prev) head = curr->next;
        else prev->next = curr->next;

        cout << "Deleted stock: " << curr->name << "\n";
        delete curr;
    }

    void updateQuantity() {
        int id, qty;
        cout << "Enter ID: "; cin >> id;
        StockItem* item = findByID(id);
        if (!item) { cout << "Not found.\n"; return; }
        cout << "New quantity: "; cin >> qty;
        addTransaction(item, "Updated Quantity", qty - item->quantity, 0);
        item->quantity = qty;
        cout << "Quantity updated.\n";
    }

    void updatePrice() {
        int id;
        double newPrice;
        cout << "Enter ID: "; cin >> id;
        StockItem* item = findByID(id);
        if (!item) { cout << "Not found.\n"; return; }
        cout << "New price: "; cin >> newPrice;
        addTransaction(item, "Updated Price", 0, newPrice - item->price);
        item->price = newPrice;
        cout << "Price updated.\n";
    }

    // New simple table display header
    void displayHeader() {
        cout << "ID    Name                 Quantity   Price\n";
        cout << "---------------------------------------------\n";
    }

    // New simple table display row
    void displayTableRow(StockItem* item) {
        cout << item->id << "     ";

        string name = item->name;
        if (name.length() > 20) name = name.substr(0, 20);
        cout << name;
        for (int i = 0; i < 20 - (int)name.length(); i++) cout << ' ';

        cout << "   " << item->quantity << "     ";

        cout << item->price << "\n";
    }

    // New displayAll using table
    void displayAll() {
        if (!head) {
            cout << "No stocks available.\n";
            return;
        }
        displayHeader();
        StockItem* curr = head;
        while (curr) {
            displayTableRow(curr);
            curr = curr->next;
        }
    }

    void searchSequentialByID() {
        int id;
        cout << "Enter ID: "; cin >> id;
        StockItem* curr = head;
        while (curr) {
            if (curr->id == id) {
                displayHeader();
                displayTableRow(curr);
                return;
            }
            curr = curr->next;
        }
        cout << "Not found.\n";
    }

    void searchSequentialByName() {
        string name;
        cin.ignore();
        cout << "Enter name: ";
        getline(cin, name);
        StockItem* curr = head;
        while (curr) {
            if (curr->name == name) {
                displayHeader();
                displayTableRow(curr);
                return;
            }
            curr = curr->next;
        }
        cout << "Not found.\n";
    }

    void binarySearchByID() {
        sortBy("id", 3);
        int target;
        cout << "Enter ID: "; cin >> target;

        StockItem* arr[100]; int count = 0;
        for (StockItem* curr = head; curr; curr = curr->next) arr[count++] = curr;

        int l = 0, r = count - 1;
        while (l <= r) {
            int m = (l + r) / 2;
            if (arr[m]->id == target) {
                displayHeader();
                displayTableRow(arr[m]);
                return;
            }
            else if (arr[m]->id < target) l = m + 1;
            else r = m - 1;
        }
        cout << "Not found.\n";
    }

    void binarySearchByName() {
        sortBy("name", 3);
        string target;
        cin.ignore();
        cout << "Enter name: ";
        getline(cin, target);

        StockItem* arr[100]; int count = 0;
        for (StockItem* curr = head; curr; curr = curr->next) arr[count++] = curr;

        int l = 0, r = count - 1;
        while (l <= r) {
            int m = (l + r) / 2;
            if (arr[m]->name == target) {
                displayHeader();
                displayTableRow(arr[m]);
                return;
            }
            else if (arr[m]->name < target) l = m + 1;
            else r = m - 1;
        }
        cout << "Not found.\n";
    }

    void sortBy(string attr, int method) {
        if (method == 1) selectionSort(attr);
        else if (method == 2) bubbleSort(attr);
        else if (method == 3) insertionSort(attr);
        else cout << "Invalid sort.\n";
    }

    void bubbleSort(string attr) {
        if (!head || !head->next) return;

        bool swapped;
        do {
            swapped = false;
            StockItem *curr = head, *prev = nullptr;

            while (curr && curr->next) {
                StockItem* next = curr->next;
                bool condition = (attr == "id" && curr->id > next->id) ||
                                 (attr == "name" && curr->name > next->name);

                if (condition) {
                    swapped = true;
                    curr->next = next->next;
                    next->next = curr;

                    if (prev) prev->next = next;
                    else head = next;

                    prev = next;
                } else {
                    prev = curr;
                    curr = curr->next;
                }
            }
        } while (swapped);
    }

    void selectionSort(string attr) {
        for (StockItem* i = head; i && i->next; i = i->next) {
            StockItem* min = i;
            for (StockItem* j = i->next; j; j = j->next) {
                if ((attr == "id" && j->id < min->id) || (attr == "name" && j->name < min->name))
                    min = j;
            }
            if (min != i) {
                swap(i->id, min->id); swap(i->name, min->name);
                swap(i->quantity, min->quantity); swap(i->price, min->price);
                swap(i->historyCount, min->historyCount);
                for (int h = 0; h < 100; h++) swap(i->history[h], min->history[h]);
            }
        }
    }

    void insertionSort(string attr) {
        StockItem* sorted = nullptr;
        StockItem* curr = head;
        while (curr) {
            StockItem* next = curr->next;
            if (!sorted || (attr == "id" && curr->id < sorted->id) || (attr == "name" && curr->name < sorted->name)) {
                curr->next = sorted;
                sorted = curr;
            } else {
                StockItem* temp = sorted;
                while (temp->next && !((attr == "id" && curr->id < temp->next->id) || (attr == "name" && curr->name < temp->next->name)))
                    temp = temp->next;
                curr->next = temp->next;
                temp->next = curr;
            }
            curr = next;
        }
        head = sorted;
    }

    void saveToFile() {
        ofstream out("stocks.txt");
        for (StockItem* curr = head; curr; curr = curr->next) {
            out << curr->id << '\n' << curr->name << '\n' << curr->quantity << '\n' << curr->price << '\n' << curr->historyCount << '\n';
            for (int i = 0; i < curr->historyCount; i++) {
                Transaction& t = curr->history[i];
                out << t.type << '\n' << t.qtyChange << '\n' << t.priceChange << '\n' << t.timestamp << '\n';
            }
        }
    }

    void loadFromFile() {
        ifstream in("stocks.txt");
        while (true) {
            int id, qty, histCount;
            double price;
            string name;
            if (!(in >> id)) break;
            in.ignore();
            getline(in, name);
            in >> qty >> price >> histCount;
            in.ignore();

            StockItem* item = new StockItem(id, name, qty, price);
            item->historyCount = histCount;
            for (int i = 0; i < histCount; i++) {
                Transaction& t = item->history[i];
                getline(in, t.type);
                in >> t.qtyChange >> t.priceChange;
                in.ignore();
                getline(in, t.timestamp);
            }
            item->next = head;
            head = item;
        }
    }

    void menu() {
        int choice;
        do {
            cout << "\n=== Menu ===\n"
                 << "1. Add Stock\n2. Display\n3. Update Qty\n4. Update Price\n"
                 << "5. Search \n6. Sort \n7. Delete Stock\n8. Exit\nChoice: ";
            cin >> choice;

            if (choice == 1) addStock();
            else if (choice == 2) displayAll();
            else if (choice == 3) updateQuantity();
            else if (choice == 4) updatePrice();
            else if (choice == 5) {
                int m, t;
                cout << "Method (1.SEQUENTIAL 2.BINARY): ";
                cin >> m;
                cout << "Type (1.ID 2.Name): ";
                cin >> t;
                if (m == 1 && t == 1) searchSequentialByID();
                else if (m == 1 && t == 2) searchSequentialByName();
                else if (m == 2 && t == 1) binarySearchByID();
                else if (m == 2 && t == 2) binarySearchByName();
                else cout << "Invalid search.\n";
            }
            else if (choice == 6) {
                string attr;
                int m;
                cout << "BY (id/name): "; cin >> attr;
                cout << "Method (1.SELECTION 2.BUBBLE 3.INSERTION): "; cin >> m;
                sortBy(attr, m);
            }
            else if (choice == 7) deleteStock();
            else if (choice == 8) {
                saveToFile();
                cout << "Goodbye!\n";
            }
            else cout << "Invalid.\n";
        } while (choice != 8);
    }

private:
    StockItem* findByID(int id) {
        for (StockItem* curr = head; curr; curr = curr->next)
            if (curr->id == id) return curr;
        return nullptr;
    }

    void addTransaction(StockItem* item, string type, int qty, double price) {
        if (item->historyCount >= 100) return;
        Transaction& t = item->history[item->historyCount++];
        t.type = type;
        t.qtyChange = qty;
        t.priceChange = price;
        t.timestamp = "manual";
    }
};

int main() {
    StockManager sm;
    sm.loadFromFile();

    while (!sm.login()) {
        cout << "Try again.\n";
    }

    sm.menu();
    return 0;
}
