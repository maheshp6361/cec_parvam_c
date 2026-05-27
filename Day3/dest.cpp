#include <iostream>
using namespace std;

class resource
{
private:
    int *data;
    int id;

public:
    resource(int i, int val) : id(i)
    {
        data = new int(val);
        cout << "Resource " << id << " acquired (value:" << *data << ")" << endl;
    }

    ~resource()
    {
        delete data;
        cout << "Resource " << id << " released" << endl;
    }

    void show()
    {
        cout << "Resource " << id << ": " << *data << endl;
    }
};

void function()
{
    resource local(2, 200);
    local.show();
}

int main()
{
    resource r1(1, 100);
    r1.show();

    function();

    resource *r3 = new resource(3, 300);
    r3->show();
    delete r3;

    cout << "End of main" << endl;

    return 0;
}