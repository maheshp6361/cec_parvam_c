#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
    int x = 42;
    int *ptr;
    ptr = &x;

    cout << "Value of x: " << x << endl;
    cout << "Address of x: " << &x << endl;
    cout << "Value of ptr (address it holds):" << ptr << endl;
    cout << "Value at address ptr: " << *ptr << endl;

    *ptr = 100;
    cout << "x after *ptr = 100 : " << x << endl;

    int *nullPtr = nullptr;
    return 0;
}
