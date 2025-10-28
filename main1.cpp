
#include <iostream>
#include <string>
#include <ctime>
using namespace std;

const int MAX_PRODUCTS = 10;
const string ADMIN_PASSWORD = "admin123";

int nextCategoryId = 1;

struct Product {
    int id;
    string name;
    double price;
    string productionDate;
    string expiryDate;
    int quantity;
    string company;
};

struct Category {
    int id;
    string name;
    Product products[MAX_PRODUCTS];
    int productCount;
    int nextProductId;
    Category* next;
};

Category* start = nullptr;

bool categoryExists(const string& name) {
    Category* temp = start;
    while (temp) {
        if (temp->name == name) return true;
        temp = temp->next;
    }
    return false;
}

bool productExists(Category* category, const string& productName) {
    for (int i = 0; i < category->productCount; ++i) {
        if (category->products[i].name == productName) return true;
    }
    return false;
}

Category* findCategory(const string& name) {
    Category* temp = start;
    while (temp) {
        if (temp->name == name) return temp;
        temp = temp->next;
    }
    return nullptr;
}

bool isExpired(int day, int month, int year) {
    time_t t = time(0);
    tm* now = localtime(&t);
    int currentYear = now->tm_year + 1900;
    int currentMonth = now->tm_mon + 1;
    int currentDay = now->tm_mday;

    if (year < currentYear) return true;
    if (year == currentYear && month < currentMonth) return true;
    if (year == currentYear && month == currentMonth && day < currentDay) return true;
    return false;
}

void sortCategories() {
    if (!start || !start->next) return;
    for (Category* i = start; i && i->next; i = i->next) {
        for (Category* j = i->next; j; j = j->next) {
            if (i->name > j->name) {
                swap(i->name, j->name);
                swap(i->products, j->products);
                swap(i->productCount, j->productCount);
                swap(i->nextProductId, j->nextProductId);
            }
        }
    }
}

void sortProducts(Category* category) {
    for (int i = 1; i < category->productCount; ++i) {
        Product key = category->products[i];
        int j = i - 1;
        while (j >= 0 && category->products[j].id > key.id) {
            category->products[j + 1] = category->products[j];
            j--;
        }
        category->products[j + 1] = key;
    }
}

void addCategory(const string& name) {
    if (categoryExists(name)) {
        cout << "Category already exists.\n";
        return;
    }
    Category* newCategory = new Category{nextCategoryId++, name, {}, 0, 1, nullptr};
    newCategory->next = start;
    start = newCategory;
    sortCategories();
    cout << "Category added successfully.\n";
}

void addProduct(const string& categoryName) {
    Category* category = findCategory(categoryName);
    if (!category) {
        cout << "Incorrect category name.\n";
        return;
    }
    if (category->productCount >= MAX_PRODUCTS) {
        cout << "Product list is full in this category.\n";
        return;
    }

    Product p;
    cout << "Enter product name: ";
    cin >> p.name;
    if (productExists(category, p.name)) {
        cout << "Product already exists.\n";
        return;
    }

    p.id = category->nextProductId++;

    cout << "Enter price: ";
    cin >> p.price;

    int pdDay, pdMonth, pdYear;
    int expDay, expMonth, expYear;

    cout << "Enter production date (day month year): ";
    cin >> pdDay >> pdMonth >> pdYear;
    p.productionDate = to_string(pdDay) + "/" + to_string(pdMonth) + "/" + to_string(pdYear);

    cout << "Enter expiry date (day month year): ";
    cin >> expDay >> expMonth >> expYear;
    if (isExpired(expDay, expMonth, expYear)) {
        cout << "Error: This product is already expired. Cannot add.\n";
        return;
    }
    p.expiryDate = to_string(expDay) + "/" + to_string(expMonth) + "/" + to_string(expYear);

    cout << "Enter quantity: ";
    cin >> p.quantity;

    cout << "Enter company: ";
    cin >> p.company;

    category->products[category->productCount++] = p;
    sortProducts(category);

    cout << "Product added successfully. ID = "<< p.id<<"\n";
}

void updateProduct(const string& categoryName, int productId) {
    Category* category = findCategory(categoryName);
    if (!category) {
        cout << "Incorrect category name.\n";
        return;
    }
    for (int i = 0; i < category->productCount; ++i) {
        if (category->products[i].id == productId) {
            int choice;
            cout << "1. Update price\n2. Update quantity\n3. Update both\nChoice: ";
            cin >> choice;
            if (choice == 1 || choice == 3) {
                cout << "Enter new price: ";
                cin >> category->products[i].price;
            }
            if (choice == 2 || choice == 3) {
                cout << "Enter new quantity: ";
                cin >> category->products[i].quantity;
            }
            cout << "Product updated successfully.\n";
            return;
        }
    }
    cout << "Product ID not found.\n";
}

void removeProduct(const string& categoryName, int productId, int quantity) {
    Category* category = findCategory(categoryName);
    if (!category) {
        cout << "Incorrect category name.\n";
        return;
    }
    for (int i = 0; i < category->productCount; ++i) {
        if (category->products[i].id == productId) {
            if (category->products[i].quantity < quantity) {
                cout << "Insufficient quantity.\n";
                return;
            }
            category->products[i].quantity -= quantity;
            if (category->products[i].quantity == 0) {
                for (int j = i; j < category->productCount - 1; ++j)
                    category->products[j] = category->products[j + 1];
                category->productCount--;
                cout << "Product removed completely.\n";
            } else {
                cout << "Product quantity reduced.\n";
            }
            return;
        }
    }
    cout << "Product ID not found.\n";
}

int linearSearchProduct(Category* category, int productId) {
    for (int i = 0; i < category->productCount; ++i) {
        if (category->products[i].id == productId) return i;
    }
    return -1;
}

int binarySearchProduct(Category* category, int productId) {
    int left = 0, right = category->productCount - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (category->products[mid].id == productId) return mid;
        else if (category->products[mid].id < productId) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

void searchProduct() {
    int option;
    cout << "Search Options:\n1. Search by category name (Linear Search)\n2. Search product by ID (Linear/Binary)\nEnter choice: ";
    cin >> option;
    if (option == 1) {
        string categoryName;
        cout << "Enter category name: ";
        cin >> categoryName;
        Category* category = findCategory(categoryName);
        if (!category) {
            cout << "Category not found.\n";
            return;
        }
        cout << "Products in category: " << category->name << endl;
        for (int i = 0; i < category->productCount; ++i) {
            Product& p = category->products[i];
            cout << "ID: " << p.id << ", Name: " << p.name << ", Price: " << p.price
                 << ", Quantity: " << p.quantity << endl;
        }
    } else if (option == 2) {
            string categoryName;
        cout << "Enter category name: ";
        cin >> categoryName;
        Category* category1 = findCategory(categoryName);
        if (!category1) {
            cout << "Category not found.\n";
            return;
        }
        int id, method;
        cout << "Enter product ID to search: ";
        cin >> id;
        cout << "Choose method:\n 1 Linear\n 2 Binary\n ";
        cin >> method;
        Category* category = start;
        while (category) {
            int index = (method == 1) ? linearSearchProduct(category, id)
                                      : binarySearchProduct(category, id);
            if (index != -1) {
                Product& p = category->products[index];
                cout << "Found in category: " << category->name << endl;
                cout << "Name: " << p.name << ", Price: " << p.price << ", Quantity: " << p.quantity << endl;
                return;
            }
            category = category->next;
        }
        cout << "Product ID not found.\n";
    }
}

void displayAll() {
    int totalCategories = 0;
    int totalProducts = 0;
    Category* category = start;
    while (category) {
        totalCategories++;
        cout << "\nCategory: " << category->name << endl;
        for (int i = 0; i < category->productCount; ++i) {
            Product& p = category->products[i];
            cout << "ID: " << p.id << ", Name: " << p.name << ", Price: " << p.price
                 << ", Quantity: " << p.quantity << ", Company: " << p.company << endl;
            totalProducts++;
        }
        category = category->next;
    }
    cout << "\nTotal Categories: " << totalCategories << ", Total Products: " << totalProducts << endl;
}

void sortMenu() {
    int choice;
    cout << "1. Sort categories (by name - bubble sort)\n";
    cout << "2. Sort products (by ID - insertion sort)\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        sortCategories();
        cout << "Categories sorted successfully.\n";
        displayAll();
    } else if (choice == 2) {
        string categoryName;
        cout << "Enter category name to sort its products: ";
        cin >> categoryName;
        Category* category = findCategory(categoryName);
        if (!category) {
            cout << "Category not found.\n";
            return;
        }
        sortProducts(category);
        cout << "Products sorted successfully.\n";
        displayAll();
    } else {
        cout << "Invalid choice.\n";
    }
}

void adminMenu() {
    int choice;
    string categoryName;
    do {
        cout << "\nAdmin Menu:\n";
        cout << "1. Add Category\n2. Add Product\n3. Update Product\n4. Remove Product\n";
        cout << "5. Sort\n6. Search\n7. Display All\n8. Exit\nChoice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Enter category name: "; cin >> categoryName;
                addCategory(categoryName);
                break;
            case 2:
                cout << "Enter category name: "; cin >> categoryName;
                addProduct(categoryName);
                break;
            case 3: {
                cout << "Enter category name: "; cin >> categoryName;
                int updateId;
                cout << "Enter product ID to update: "; cin >> updateId;
                updateProduct(categoryName, updateId);
                break;
            }
            case 4: {
                cout << "Enter category name: "; cin >> categoryName;
                int removeId, qty;
                cout << "Enter product ID to remove: "; cin >> removeId;
                cout << "Enter quantity to remove: "; cin >> qty;
                removeProduct(categoryName, removeId, qty);
                break;
            }
            case 5:
                sortMenu();
                break;
            case 6:
                searchProduct();
                break;
            case 7:
                displayAll();
                break;
            case 8:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 8);
}

int main() {
    string password;
    cout << "Enter admin password: ";
    cin >> password;
    if (password == ADMIN_PASSWORD) {
        adminMenu();
    } else {
        cout << "Incorrect password. Access denied.\n";
    }
    return 0;
}
