#include <iostream>
#include <string>
#include <fstream> // For file handling
#include <cstdlib>
#include <iomanip> // For formatting tables

using namespace std;

struct Product {
    string name;
    float price;
    int quantity;
};

struct Order {
    string productName;
    int quantity;
    float productPrice;
};

const int MAX_PRODUCTS = 100;
const int MAX_ORDERS = 100;

// Function Declarations
void addProduct(Product products[], int& productCount, const int MAX_PRODUCTS);
void updateProduct(Product products[], int& productCount);
void deleteProduct(Product products[], int& productCount);
void searchProduct(const Product products[], int productCount);
void listProducts(const Product products[], int productCount);
void addOrder(Order orders[], int& orderCount, const int MAX_ORDERS, Product products[], int productCount);
void viewOrders(const Order orders[], int orderCount);
int findProductIndex(const Product products[], int productCount, const string& productName);
void saveProducts(const Product products[], int productCount);
void loadProducts(Product products[], int& productCount);
void saveOrders(const Order orders[], int orderCount);
void loadOrders(Order orders[], int& orderCount);

int flag = 1;

void addProduct(Product products[], int& productCount, const int MAX_PRODUCTS) {
    if (flag != 0) {
        system("cls"); // Clearing screen if we use the updating function
    }

    if (productCount >= MAX_PRODUCTS) {
        cout << "Inventory is full. Cannot add more products.\n";
    }
    else {
        string name;
        float price;
        int quantity;

        cout << "Enter product name: ";
        cin.ignore();
        getline(cin, name);

        cout << "Enter product price: ";
        cin >> price;

        if (price <= 0) {
            cout << "Wrong Input. Price must be positive.\n";
        }
        else {
            cout << "Enter product quantity: ";
            cin >> quantity;

            if (quantity < 0) {
                cout << "Wrong Input. Quantity must be non-negative.\n";
            }
            else {
                products[productCount++] = { name, price, quantity };
                saveProducts(products, productCount); // Save products immediately
                cout << "Product added successfully.\n";
            }
        }
    }
}

void updateProduct(Product products[], int& productCount) { // Pass productCount by reference
    system("cls");

    string name;
    cout << "Enter product name to update: ";
    cin.ignore();
    getline(cin, name);

    int index = findProductIndex(products, productCount, name);

    if (index == -1) {
        flag = 0;
        system("cls"); // Clearing screen to stabilize the interface
        cout << "Product not found. Adding as new product.\n";
        addProduct(products, productCount, MAX_PRODUCTS);
        index = findProductIndex(products, productCount, name); // Find the index of the newly added product
    }

    if (index != -1) {
        float new_Price;
        int new_Quantity;

        cout << "Enter new product price: ";
        cin >> new_Price;

        if (new_Price <= 0) {
            cout << "Wrong Input. Price must be positive.\n";
        }
        else {
            cout << "Enter new product quantity: ";
            cin >> new_Quantity;

            if (new_Quantity < 0) {
                cout << "Wrong Input. Quantity must be non-negative.\n";
            }
            else {
                products[index].price = new_Price;
                products[index].quantity = new_Quantity;
                saveProducts(products, productCount); // Save products immediately
                cout << "Product updated successfully.\n";
            }
        }
    }
}

void deleteProduct(Product products[], int& productCount) {
    system("cls");

    string name;
    cout << "Enter product name to delete: ";
    cin.ignore();
    getline(cin, name);

    int index = findProductIndex(products, productCount, name);

    if (index == -1) {
        cout << "Product not found.\n";
    }
    else {
        for (int i = index; i < productCount - 1; ++i) {
            products[i] = products[i + 1];
        }
        --productCount;
        saveProducts(products, productCount); // Save products immediately
        cout << "Product deleted successfully.\n";
    }
}

void searchProduct(const Product products[], int productCount) {
    system("cls");

    string name;
    cout << "Enter product name to search: ";
    cin.ignore();
    getline(cin, name);

    int index = findProductIndex(products, productCount, name);

    if (index == -1) {
        cout << "Product not found.\n";
    }
    else {
        cout << "Product found: " << products[index].name << " " << products[index].price << " " << products[index].quantity << "\n";
    }
}

void listProducts(const Product products[], int productCount) {
    system("cls");

    if (productCount == 0) {
        cout << "Inventory is empty.\n";
    }
    else {
        cout << "Available Products: " << endl;
        cout << "+--------+---------------------+----------+----------+" << endl;
        cout << "|   No.  | Product Name        |  Price   | Quantity |" << endl;
        cout << "+--------+---------------------+----------+----------+" << endl;

        for (int i = 0; i < productCount; ++i) {
            cout << "| " << setw(6) << i + 1 << " | ";
            cout << setw(19) << left << products[i].name << " | ";
            cout << setw(8) << fixed << setprecision(2) << products[i].price << " | ";
            cout << setw(8) << products[i].quantity << " |" << endl;
        }

        cout << "+--------+---------------------+----------+----------+" << endl;
    }
}

void addOrder(Order orders[], int& orderCount, const int MAX_ORDERS, Product products[], int productCount) {
    system("cls");

    if (orderCount >= MAX_ORDERS) {
        cout << "Cannot place more orders.\n";
    }
    else {
        string productName;
        int quantity;

        cout << "Enter the name of the product you want: ";
        cin.ignore();
        getline(cin, productName);

        int productIndex = findProductIndex(products, productCount, productName);

        if (productIndex == -1) {
            cout << "The product is not found.\n";
        }
        else {
            cout << "Enter the quantity: ";
            cin >> quantity;

            if (quantity <= 0 || quantity > products[productIndex].quantity) {
                cout << "The quantity is invalid.\n";
            }
            else {
                orders[orderCount++] = { productName, quantity, products[productIndex].price }; // Store product price in order
                products[productIndex].quantity -= quantity;
                saveOrders(orders, orderCount);   // Save orders immediately
                saveProducts(products, productCount); // Save updated products immediately
                cout << "Order added successfully.\n";
            }
        }
    }
}

void viewOrders(const Order orders[], int orderCount) {
    system("cls");

    if (orderCount == 0) {
        cout << "There is not any Order" << endl;
    }
    else {
        double totalCost = 0.0;

        cout << "The list of orders: " << endl;
        cout << "+--------+---------------------+----------+----------+" << endl;
        cout << "| Order  | Product Name        | Quantity |   Cost   |" << endl;
        cout << "+--------+---------------------+----------+----------+" << endl;

        for (int i = 0; i < orderCount; i++) {
            double productCost = orders[i].quantity * orders[i].productPrice; // Calculate cost for each product
            totalCost += productCost; // Add to total cost

            cout << "| " << setw(6) << i + 1 << " | ";
            cout << setw(19) << left << orders[i].productName << " | ";
            cout << setw(8) << orders[i].quantity << " | ";
            cout << setw(8) << fixed << setprecision(2) << productCost << " |" << endl;
        }
        cout << "+--------+---------------------+----------+----------+" << endl;
        cout << "| Total Cost:                                        | ";
        cout << setw(8) << fixed << setprecision(2) << totalCost << " |" << endl;
        cout << "+---------------------------------------------------+" << endl;
    }
}

int findProductIndex(const Product products[], int productCount, const string& productName) {
    for (int i = 0; i < productCount; ++i) {
        if (products[i].name == productName) {
            return i;
        }
    }
    return -1; // Not found
}

void saveProducts(const Product products[], int productCount) {
    ofstream outFile("products.txt");

    if (outFile.is_open()) {
        for (int i = 0; i < productCount; i++) {
            outFile << products[i].name << "\n" << products[i].price << "\n" << products[i].quantity << "\n";
        }
        outFile.close();
        cout << "Products saved successfully.\n";
    }
    else {
        cout << "Error saving products.\n";
    }
}

void loadProducts(Product products[], int& productCount) {
    ifstream inFile("products.txt");

    if (inFile.is_open()) {
        productCount = 0;
        while (inFile >> ws && getline(inFile, products[productCount].name)
            && inFile >> products[productCount].price >> products[productCount].quantity) {
            productCount++;
        }
        inFile.close();
    }
    else {
        cout << "Error loading products.\n";
    }
}

void saveOrders(const Order orders[], int orderCount) {
    ofstream outFile("orders.txt");

    if (outFile.is_open()) {
        for (int i = 0; i < orderCount; i++) {
            outFile << orders[i].productName << "\n" << orders[i].quantity << "\n" << orders[i].productPrice << "\n";
        }
        outFile.close();
        cout << "Orders saved successfully.\n";
    }
    else {
        cout << "Error saving orders.\n";
    }
}

void loadOrders(Order orders[], int& orderCount) {
    ifstream inFile("orders.txt");

    if (inFile.is_open()) {
        orderCount = 0;
        while (inFile >> ws && getline(inFile, orders[orderCount].productName)
            && inFile >> orders[orderCount].quantity >> orders[orderCount].productPrice) {
            orderCount++;
        }
        inFile.close();
    }
    else {
        cout << "Error loading orders.\n";
    }
}

int main() {
    Product products[MAX_PRODUCTS];
    Order orders[MAX_ORDERS];
    int productCount = 0, orderCount = 0;

    loadProducts(products, productCount);
    loadOrders(orders, orderCount);

    int choice;

    do {
        cout << "\n* Inventory Management System *\n";
        cout << "1. Add Product\n";
        cout << "2. Update Product\n";
        cout << "3. Delete Product\n";
        cout << "4. Search Product\n";
        cout << "5. List Products\n";
        cout << "6. Add Order\n";
        cout << "7. View Orders\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addProduct(products, productCount, MAX_PRODUCTS);
            break;
        case 2:
            updateProduct(products, productCount);
            break;
        case 3:
            deleteProduct(products, productCount);
            break;
        case 4:
            searchProduct(products, productCount);
            break;
        case 5:
            listProducts(products, productCount);
            break;
        case 6:
            addOrder(orders, orderCount, MAX_ORDERS, products, productCount);
            break;
        case 7:
            viewOrders(orders, orderCount);
            break;
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}
