#include<iostream>
#include<cstring>
using namespace std;

class person {
    private:
        char* name;
        int age;

    public:
        person(){
            name = new char[1];
            name[0] = '\0';
            age = 0;
            cout<<"Default constructor"<<endl;
        }

    person(const char* n, int a){
        name = new char[strlen(n)+1];
        strcpy(name, n);
        age = a;
        cout<<"Parameterized constructor:"<<name<<endl;
    }

    person(const person& p){
        name = new char[strlen(p.name)+1];
        strcpy(name,p.name);
        age = p.age;
        cout<<"Copy constructor: "<<name<<endl;
    }

    ~person() {
        cout<<"Destructor: "<<name<<endl;
        delete[] name;
    }

    void display(){
        cout<<name<<" (" <<age<<" years old)"<<endl;
    }
};

int main()
{
    person p1;
    person p2("Arjun",20);
    person p3 = p2;

    cout<<"\n---Person details---"<<endl;
    p1.display();
    p2.display();
    p3.display();

    cout<<"\n---End of main---"<<endl;
    return 0;
}