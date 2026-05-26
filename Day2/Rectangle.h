#ifndef RECTANGLE_H
#define RECTANGLE_H

class Rectangle
{
private:
    double length;
    double width;

public:
    Rectangle();
    Rectangle(double l, double w);
    void setdimensions(double l, double w);
    double area();
    double perimeter();
    void display();
};
#endif