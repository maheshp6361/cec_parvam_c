#include "Rectangle.h"
#include <iostream>
using namespace std;

Rectangle::Rectangle()
{
    length = 0;
    width = 0;
}

Rectangle::Rectangle(double l, double w)
{
    length = l;
    width = w;
}

void Rectangle::setdimensions(double l, double w)
{
    length = l;
    width = w;
}

double Rectangle::area()
{
    return length * width;
}

double Rectangle::perimeter()
{
    return 2 * (length * width);
}

void Rectangle::display()
{
    cout << "Rectangle: " << length << " x " << width << endl;
    cout << "Area: " << area() << ", Perimeter: " << perimeter() << endl;
}