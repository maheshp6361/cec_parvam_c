#include <iostream>
#include <string>
using namespace std;

// Base Class
class HR
{
protected:
    int attendance;
    double salary;

public:
    HR(int att, double sal) : attendance(att), salary(sal) {}

    void displayHR()
    {
        cout << "Attendance: " << attendance
             << ", Salary: $" << salary;
    }
};

// Single Inheritance
class Employee : public HR
{
protected:
    string empName;

public:
    Employee(string n, int att, double sal)
        : HR(att, sal), empName(n) {}

    void display()
    {
        cout << "Employee Name: " << empName << ", ";
        displayHR();
        cout << endl;
    }
};

// Multilevel Inheritance
class Lead : public Employee
{
protected:
    string project;

public:
    Lead(string n, int att, double sal, string p)
        : Employee(n, att, sal), project(p) {}

    void display()
    {
        Employee::display();
        cout << "Project: " << project << endl;
    }
};

// Further Multilevel Inheritance
class Manager : public Lead
{
private:
    string performance;

public:
    Manager(string n, int att, double sal,
            string p, string perf)
        : Lead(n, att, sal, p), performance(perf) {}

    void display()
    {
        cout << "Manager Details:" << endl;
        Employee::display();
        cout << "Project: " << project << endl;
        cout << "Performance: " << performance << endl;
    }
};

int main()
{
    cout << "=== Employee (Single Inheritance) ===" << endl;
    Employee e1("Rahul", 26, 35000);
    e1.display();

    cout << "\n=== Lead (Multilevel Inheritance) ===" << endl;
    Lead l1("Sneha", 27, 50000, "Banking App");
    l1.display();

    cout << "\n=== Manager (Multilevel Inheritance) ===" << endl;
    Manager m1("Amit", 28, 80000,
               "ERP System", "Excellent");
    m1.display();

    return 0;
}