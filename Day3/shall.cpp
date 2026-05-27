#include <iostream>
#include <string>
using namespace std;
class rectangle
{
private:
    double length;
    double width;

public:
    rectangle()
    {
        length = 0;
        width = 0;
        cout << "Default constructor" << endl;
    }
    rectangle(double side)
    {
        length = side;
        width = side;
        cout << "Square constructor" << endl;
    }

    rectangle(double l, double w)
    {
        length = l;
        width = w;
        cout << "Two-paramter constructor" << endl;
    }

    rectangle(const rectangle &r)
    {
        length = r.length;
        width = r.width;
        cout << "Copy constructor" << endl;
    }

    double area() { return length * width; }
};

int main()
{
    rectangle r1;
    rectangle r2(5);
    rectangle r3(4, 6);
    rectangle r4 = r3;

    cout << "\n Area: " << r1.area() << ", " << r2.area() << ", " << r3.area() << ", " << r4.area() << endl;

    return 0;
}