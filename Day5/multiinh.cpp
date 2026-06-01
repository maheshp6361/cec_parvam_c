// FULL MULTI TYPE INHERITANCE

#include <iostream>
#include <string>
using namespace std;

class person
{
protected:
    string name;
    int age;

public:
    person(string n, int a) : name(n), age(a) {}
    void display()
    {
        cout << "Name: " << name << ", Age: " << age;
    }
};

class student : public person
{
protected:
    int rollno;

public:
    student(string n, int a, int r) : person(n, a), rollno(r) {}

    void display()
    {
        person::display();
        cout << ", Roll No: " << rollno << endl;
    }
};

class gstudent : public student
{
private:
    string thesistopic;

public:
    gstudent(string n, int a, int r, string t)
        : student(n, a, r), thesistopic(t) {}

    void display()
    {
        student::display();
        cout << "Thesis: " << thesistopic << endl;
    }
};

class teacher : public person
{
private:
    double salary;

public:
    teacher(string n, int a, double s) : person(n, a), salary(s) {}

    void display()
    {
        person::display();
        cout << ", Salary: $" << salary << endl;
    }
};

class teachingassistant : public student, public teacher
{
public:
    teachingassistant(string n, int a, int r, double s)
        : student(n, a, r), teacher(n, a, s) {}

    void display()
    {
        cout << "Teaching Assistant: " << student::name << endl;
        cout << "Age: " << student::age << "Roll: " << student::rollno << endl;
    }
};

int main()
{
    cout << "=== Single Inheritance ===" << endl;
    student s1("Arjun", 20, 101);
    s1.display();

    cout << "\n=== Multilevel Inheritance ===" << endl;
    gstudent gs("Priya", 24, 201, "Machine Learning");
    gs.display();

    cout << "\n=== Hierarchical Inheritance ===" << endl;
    teacher t1("Dr. Sharma", 45, 80000);
    t1.display();

    return 0;
}
