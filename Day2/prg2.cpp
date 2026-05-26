#include<iostream>
using namespace std;

int value = 100;

class Demo {
    public:
    int value;

    void setvalue(int value)
    {
        this->value = value;
    }

    void printall()
    {
        int value = 50;
        cout<<"Local value: "<<value<<endl;
        cout<<"Member value: "<<this->value<<endl;
        cout<<"Global value: " <<::value<<endl;
    }
};

int main()
{
    Demo d;
    d.setvalue(200);
    d.printall();
    return 0;
}
