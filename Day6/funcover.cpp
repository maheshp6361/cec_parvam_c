#include <iostream>
using namespace std;

class demo
{
public:
    void show(int a) { cout << "One int: " << a << endl; }
    void show(int a, int b) { cout << "Two ints: " << a << ", " << b << endl; }

    void display(int a, int b = 0)
    {
        cout << "Display: " << a << ", " << (b ? to_string(b) : "default") << endl;
    }
};

int main()
{
    demo d;
    d.show(10);
    d.show(10, 20);
    d.display(10);
    d.display(10, 20);
    return 0;
}
