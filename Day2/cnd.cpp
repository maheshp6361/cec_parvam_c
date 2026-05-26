#include <iostream>
using namespace std;

class demo
{
public:
    int id;
    demo(int i)
    {
        id = i;
        cout << "Constructor called for object " << id << endl;
    }
    ~demo()
    {
        cout << "Destructor called for object " << id << endl;
    }
};

int main()
{
    cout << "---Stack object---" << endl;
    demo stackobj(1);

    cout << "\n---Heap object---" << endl;
    demo *heapobj = new demo(2);

    cout << "\n Stack object goes out of scope automatically... " << endl;

    cout << "Heap object must be explicitly deleted..." << endl;
    delete heapobj;

    cout << "\n End of main()" << endl;
    return 0;
}