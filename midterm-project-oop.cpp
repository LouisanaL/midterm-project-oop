#include <iostream>
#include <string>

using namespace std;

class AbstractItem {
public:
    virtual string getId() = 0;
    virtual string getName() = 0;
    virtual int getQuantity() = 0;
    virtual double getPrice() = 0;
    virtual void setQuantity(int newQuantity) = 0;
    virtual void setPrice(double newPrice) = 0;
    virtual ~AbstractItem() {}
};

class Item : public AbstractItem {
private:
    string id;
    string name;
    int quantity;
    double price;

public:
    Item(string id, string name, int quantity, double price)
        : id(id), name(name), quantity(quantity), price(price) {}

    string getId() override { return id; }
    string getName() override { return name; }
    int getQuantity() override { return quantity; }
    double getPrice() override { return price; }

    void setQuantity(int newQuantity) override { quantity = newQuantity; }
    void setPrice(double newPrice) override { price = newPrice; }
};

string toLowerCase(const string& str) {
    string lowerStr = str;
    for (char& c : lowerStr) {
        c = tolower(c);
    }
    return lowerStr;
}

class Category {
private:
    string categoryName;
    AbstractItem* items[100]; 
    int itemCount;

public:
    Category(string name) : categoryName(name), itemCount(0) {}

    string getName() { return categoryName; }

    void addItem(string id, string name, int quantity, double price) {
        items[itemCount] = new Item(id, name, quantity, price);
        itemCount++;
        cout << "Item added successfully to " << categoryName << " category!" << endl;
    }

    int getItemCount() { return itemCount; }

    void updateItem(string id) {
        for (int i = 0; i < itemCount; i++) {
            if (toLowerCase(items[i]->getId()) == toLowerCase(id)) {
                string choice;
                cout << "Update (quantity/price): ";
                cin >> choice;

                if (toLowerCase(choice) == "quantity") {
                    int newQuantity;
                    cout << "Enter new quantity: ";
                    cin >> newQuantity;

                    if (newQuantity <= 0) {
                        cout << "Quantity must be at least 1. Please try again." << endl;
                        return; 
                    }

                    cout << "Quantity of Item " << items[i]->getName() << " is updated from "
                         << items[i]->getQuantity() << " to " << newQuantity << endl;
                    items[i]->setQuantity(newQuantity);
                } else if (toLowerCase(choice) == "price") {
                    double newPrice;
                    cout << "Enter new price: ";
                    cin >> newPrice;
                    cout << "Price of Item " << items[i]->getName() << " is updated from "
                         << items[i]->getPrice() << " to " << newPrice << endl;
                    items[i]->setPrice(newPrice);
                } else {
                    cout << "Invalid option!" << endl;
                }
                return;
            }
        }
        cout << "Item not found!" << endl;
    }

    bool removeItem(string id) {
        for (int i = 0; i < itemCount; i++) {
            if (toLowerCase(items[i]->getId()) == toLowerCase(id)) {
                cout << "Item " << items[i]->getName() << " has been removed from the inventory." << endl;
                delete items[i];
                for (int j = i; j < itemCount - 1; j++) {
                    items[j] = items[j + 1];
                }
                itemCount--;
                return true; 
            }
        }
        cout << "Item not found!" << endl;
        return false; 
    }

    void displayItems() {
        if (itemCount == 0) {
            cout << "No items in the " << categoryName << " category." << endl;
            return; 
        }

        cout << "ID\tName\tQuantity\tPrice" << endl;
        for (int i = 0; i < itemCount; i++) {
            cout << items[i]->getId() << "\t" << items[i]->getName() << "\t"
                 << items[i]->getQuantity() << "\t" << items[i]->getPrice() << endl;
        }
    }

    void sortItems(bool sortByQuantity, bool ascending) {
        for (int i = 0; i < itemCount - 1; i++) {
            for (int j = 0; j < itemCount - i - 1; j++) {
                bool condition;
                if (sortByQuantity) {
                    condition = ascending ? items[j]->getQuantity() > items[j + 1]->getQuantity()
                                          : items[j]->getQuantity() < items[j + 1]->getQuantity();
                } else {
                    condition = ascending ? items[j]->getPrice() > items[j + 1]->getPrice()
                                          : items[j]->getPrice() < items[j + 1]->getPrice();
                }
                if (condition) {
                    AbstractItem* temp = items[j];
                    items[j] = items[j + 1];
                    items[j + 1] = temp;
                }
            }
        }
    }

    AbstractItem** getAllItems(int& count) {
        count = itemCount;
        return items;
    }

    bool searchItem(string id) {
        for (int i = 0; i < itemCount; i++) {
            if (toLowerCase(items[i]->getId()) == toLowerCase(id)) {
                cout << "ID: " << items[i]->getId() << ", Name: " << items[i]->getName()
                     << ", Quantity: " << items[i]->getQuantity() << ", Price: "
                     << items[i]->getPrice() << endl;
                return true;
            }
        }
        return false;
    }

    void displayLowStockItems() {
        bool hasLowStockItems = false; 
        for (int i = 0; i < itemCount; i++) {
            if (items[i]->getQuantity() <= 5) {
                if (!hasLowStockItems) {
                    cout << "Low Stock Items in " << categoryName << " category:" << endl;
                    hasLowStockItems = true;
                }
                cout << "ID: " << items[i]->getId() << ", Name: " << items[i]->getName()
                     << ", Quantity: " << items[i]->getQuantity() << ", Price: "
                     << items[i]->getPrice() << endl;
            }
        }
    }
};

class Inventory {
private:
    Category* categories[3]; 
    bool hasItems; 

public:
    Inventory() : hasItems(false) {
        categories[0] = new Category("Clothing");
        categories[1] = new Category("Electronics");
        categories[2] = new Category("Entertainment");
    }

    int findCategoryIndex(string categoryName) {
        for (int i = 0; i < 3; i++) {
            if (toLowerCase(categories[i]->getName()) == toLowerCase(categoryName)) {
                return i;
            }
        }
        return -1;
    }

    void addItem() {
        string category;
        cout << "Enter category (Clothing, Electronics, Entertainment): ";
        cin >> category;

        int index = findCategoryIndex(category);
        if (index == -1) {
            cout << "Category " << category << " does not exist!" << endl;
            return;
        }

        string id, name;
        int quantity;
        double price;

        cout << "Enter ID: ";
        cin >> id;
        cin.ignore(); 
        cout << "Enter Name: ";
        getline(cin, name);  
        cout << "Enter Quantity: ";
        cin >> quantity;

        if (quantity <= 0) {
            cout << "Quantity must be at least 1. Please try again." << endl;
            return;
        }

        cout << "Enter Price: ";
        cin >> price;

        categories[index]->addItem(id, name, quantity, price);
        hasItems = true; 
    }

    void updateItem() {
        if (!hasItems) {
            cout << "No items in the inventory. Please add items first." << endl;
            return; 
        }

        string id;
        cout << "Enter item ID to update: ";
        cin >> id;

        bool itemFound = false;
        for (int i = 0; i < 3; i++) {
            if (categories[i]->searchItem(id)) {
                categories[i]->updateItem(id);
                itemFound = true;
                break; 
            }
        }

        if (!itemFound) {
            cout << "Item not found!" << endl; 
        }
    }

    void removeItem() {
        if (!hasItems) {
            cout << "No items in the inventory. Please add items first." << endl;
            return;
        }

        string id;
        cout << "Enter item ID to remove: ";
        cin >> id;

        bool itemRemoved = false; 
        for (int i = 0; i < 3; i++) {
            if (categories[i]->removeItem(id)) {
                itemRemoved = true;
                if (categories[i]->getItemCount() == 0) {
                    hasItems = false; 
                }
                break;
            }
        }

        if (!itemRemoved) {
            cout << "Item not found!" << endl;
        }
    }

    void displayItemsByCategory() {
        if (!hasItems) {
            cout << "No items in the inventory. Please add items first." << endl;
            return; 
        }

        string category;
        cout << "Enter category (Clothing, Electronics, Entertainment): ";
        cin >> category;

        int index = findCategoryIndex(category);
        if (index == -1) {
            cout << "Category " << category << " does not exist!" << endl;
            return;
        }

        categories[index]->displayItems();
    }

    void displayAllItems() {
        if (!hasItems) {
            cout << "No items in the inventory. Please add items first." << endl;
            return; 
        }

        for (int i = 0; i < 3; i++) {
            cout << "Items in " << categories[i]->getName() << " category:" << endl;
            categories[i]->displayItems();
        }
    }

    void sortItems() {
        if (!hasItems) {
            cout << "No items in the inventory. Please add items first." << endl;
            return; 
        }

        string sortType, order;
        cout << "Sort by (quantity/price): ";
        cin >> sortType;
        cout << "Order (asc/desc): ";
        cin >> order;

        bool sortByQuantity = toLowerCase(sortType) == "quantity";
        bool ascending = toLowerCase(order) == "asc";

        AbstractItem* allItems[300]; 
        int totalCount = 0;

        for (int i = 0; i < 3; i++) {
            int count;
            AbstractItem** itemsInCategory = categories[i]->getAllItems(count);
            for (int j = 0; j < count; j++) {
                allItems[totalCount++] = itemsInCategory[j];
            }
        }

        for (int i = 0; i < totalCount - 1; i++) {
            for (int j = 0; j < totalCount - i - 1; j++) {
                bool condition;
                if (sortByQuantity) {
                    condition = ascending ? allItems[j]->getQuantity() > allItems[j + 1]->getQuantity()
                                          : allItems[j]->getQuantity() < allItems[j + 1]->getQuantity();
                } else {
                    condition = ascending ? allItems[j]->getPrice() > allItems[j + 1]->getPrice()
                                          : allItems[j]->getPrice() < allItems[j + 1]->getPrice();
                }
                if (condition) {
                    AbstractItem* temp = allItems[j];
                    allItems[j] = allItems[j + 1];
                    allItems[j + 1] = temp;
                }
            }
        }

        cout << "Sorted Items:" << endl;
        cout << "ID\tName\tQuantity\tPrice" << endl;
        for (int i = 0; i < totalCount; i++) {
            cout << allItems[i]->getId() << "\t" << allItems[i]->getName() << "\t"
                 << allItems[i]->getQuantity() << "\t" << allItems[i]->getPrice() << endl;
        }
    }

    void searchItem() {
        if (!hasItems) {
            cout << "No items in the inventory. Please add items first." << endl;
            return; 
        }

        string id;
        cout << "Enter item ID to search: ";
        cin >> id;

        bool itemFound = false; 
        for (int i = 0; i < 3; i++) {
            if (categories[i]->searchItem(id)) {
                itemFound = true;
                break; 
            }
        }

        if (!itemFound) {
            cout << "Item not found!" << endl; 
        }
    }

    void displayLowStockItems() {
        if (!hasItems) {
            cout << "No items in the inventory. Please add items first." << endl;
            return; 
        }

        for (int i = 0; i < 3; i++) {
            categories[i]->displayLowStockItems();
        }
    }

    ~Inventory() {
        for (int i = 0; i < 3; i++) {
            delete categories[i]; 
        }
    }
};

int main() {
    Inventory inventory;
    bool exit = false;
    int choice;

    while (!exit) {
        cout << "1. Add Item\n2. Update Item\n3. Remove Item\n4. Display Items by Category\n"
                "5. Display All Items\n6. Search Item\n7. Sort Items\n8. Display Low Stock Items\n9. Exit\n"
                "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                inventory.addItem();
                break;
            case 2:
                inventory.updateItem(); 
                break;
            case 3:
                inventory.removeItem(); 
                break;
            case 4:
                inventory.displayItemsByCategory();
                break;
            case 5:
                inventory.displayAllItems(); 
                break;
            case 6:
                inventory.searchItem(); 
                break;
            case 7:
                inventory.sortItems(); 
                break;
            case 8:
                inventory.displayLowStockItems(); 
                break;
            case 9:
                exit = true;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
}
