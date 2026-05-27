#include <iostream>
using namespace std;

class point
{
private:
    const int x;
    const int y;

public:
    point(int xval, int yval) : x(xval), y(yval)
    {
        cout << "Point(" << x << ", " << y << ") created" << endl;
    }

    void display() const
    {
        cout << "(" << x << ", " << y << ")" << endl;
    }
};

int main()
{
    point p1(10, 20);
    point p2(-5, 15);

    p1.display();
    p2.display();

    return 0;
}