#include <iostream>
#include <string>
using namespace std;

class product
{
private:
    int id;
    string name;
    double price;

public:
    product() : id(0), name("N/A"), price(0.0) {}

    product(int i, string n, double p) : id(i), name(n), price(p) {}

    void display() const
    {
        cout << "ID: " << id << " | " << name << " | $" << price << endl;
    }
};

int main()
{
    int count;
    cout << "How many products? ";
    cin >> count;

    product *inventory = new product[count];

    for (int i = 0; i < count; i++)
    {
        inventory[i] = product(100 + i, "Product-" + to_string(i + 1), (i + 1) * 10.0);
    }

    cout << "\nInventory:" << endl;
    for (int i = 0; i < count; i++)
    {
        inventory[i].display();
    }

    delete[] inventory;
    return 0;
}