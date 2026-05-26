#include "Rectangle.h"

int main()
{
    Rectangle r1;
    Rectangle r2(5.0, 3.0);

    r1.display();
    r2.display();

    r1.setdimensions(4.5, 2.5);
    r1.display();

    return 0;
}