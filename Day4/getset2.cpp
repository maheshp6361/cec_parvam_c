#include <iostream>
#include <string>
using namespace std;

class employee
{
private:
    int id;
    string name;
    double salary;
    string department;

public:
    employee(int i, string n, double s, string d)
    {
        id = i;
        setname(n);
        setsalary(s);
        department = d;
    }

    int getid() const { return id; }
    string getname() const { return name; }
    double getsalary() const { return salary; }
    string getdepartment() const { return department; }

    void setname(const string &n)
    {
        if (n.length() >= 2)
        {
            name = n;
        }
        else
        {
            cout << "Name must be at least 2 characters!" << endl;
            name = "Unknown";
        }
    }

    void setsalary(double s)
    {
        if (s >= 0)
        {
            salary = s;
        }
        else
        {
            cout << "Salary cannot be negative!" << endl;
            salary = 0;
        }
    }

    void setdepartment(const string &d)
    {
        department = d;
    }

    void giveraise(double percentage)
    {
        if (percentage > 0)
        {
            salary += salary * (percentage / 100);
            cout << name << " received a " << percentage << "% raise. New salary: $" << salary << endl;
        }
    }

    void display() const
    {
        cout << "Id: " << id << " | " << department << endl;
    }
};

int main()
{
    employee emp(101, "Arjun", 50000, "Engineering");
    emp.display();
    emp.giveraise(10);
    emp.setsalary(-100);

    cout << "\n Final details:" << endl;
    cout << "Name:" << emp.getname() << endl;
    cout << "Salary: $" << emp.getsalary() << endl;

    return 0;
}