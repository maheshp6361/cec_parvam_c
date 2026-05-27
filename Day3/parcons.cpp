#include <iostream>
#include <string>
using namespace std;

class student
{
public:
    string name;
    int age;
    double cgpa;

    student(string n, int a, double c)
    {
        name = n;
        age = a;
        cgpa = c;
        cout << "Parameterized constructor called for " << name << endl;
    }

    void display()
    {
        cout << name << " | Age: " << age << " | CGPA: " << cgpa << endl;
    }
};

int main()
{
    student s1("Arjun", 20, 8.75);
    student s2 = student("Priya", 21, 9.2);

    s1.display();
    s2.display();

    return 0;
}