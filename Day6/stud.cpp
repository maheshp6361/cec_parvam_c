#include <iostream>
using namespace std;
class student
{
public:
    string add(string a, string b)
    {
        cout << "student: ";
        return a + b;
    }
    int add(int a)
    {
        cout << "average attendance: ";
        return a;
    }
    double add(double a, double b)
    {
        cout << "Fee pending: $";
        return a + b;
    }
    double add(int a, double b)
    {
        cout << "CGPA: ";
        return a + b;
    }
};
int main()
{
    student calc;
    cout << calc.add("John ", "Doe") << endl;
    cout << calc.add(85) << endl;
    cout << calc.add(1500.50, 2000.75) << endl;
    cout << calc.add(5, 3.75) << endl;
    return 0;
}