#include <iostream>
#include <string>
using namespace std;

class calculator
{
public:
    int add(int a, int b)
    {
        cout << "int + int = ";
        return a + b;
    }

    double add(double a, double b)
    {
        cout << "double + double = ";
        return a + b;
    }

    int add(int a, int b, int c)
    {
        cout << "int + int + int = ";
        return a + b + c;
    }

    double add(int a, double b)
    {
        cout << "int + double = ";
        return a + b;
    }

    string add(string a, string b)
    {
        cout << "string + string = ";
        return a + b;
    }
};

int main()
{
    calculator calc;

    cout << calc.add(5, 10) << endl;
    cout << calc.add(3.5, 2.7) << endl;
    cout << calc.add(1, 2, 3) << endl;
    cout << calc.add(5, 3.14) << endl;
    cout << calc.add("Hello ", "World!") << endl;

    return 0;
}