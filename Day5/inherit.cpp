#include <iostream>
#include <string>
using namespace std;

class Animal
{
protected:
    string name;
    int age;

public:
    Animal(string n, int a) : name(n), age(a) {}

    void eat()
    {
        cout << name << " is eating." << endl;
    }

    void sleep()
    {
        cout << name << " is sleeping." << endl;
    }
};

class dog : public Animal
{
private:
    string breed;

public:
    dog(string n, int a, string b) : Animal(n, a), breed(b) {}

    void bark()
    {
        cout << name << " (the " << breed << ") is barking!" << endl;
    }

    void display()
    {
        cout << "Dog: " << name << ", Age: " << age << ", breed: " << breed << endl;
    }
};

int main()
{
    dog mydog{"Buddy", 3, "Golden Retriever"};

    mydog.eat();
    mydog.display();
    mydog.bark();
    mydog.display();

    return 0;
}