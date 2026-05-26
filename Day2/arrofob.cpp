#include <iostream>
#include <string>
using namespace std;

class student
{
public:
    string name;
    double marks;
    string email;

    student()
    {
        name = "Unknown";
        marks = 0;
        email = "unknown";
    }
    student(string n, double m,string e)
    {
        name = n;
        marks = m;
        email = e;
    }

    void display()
    {
        cout << name << ": " << marks <<" ; "<<"Email: "<<email<<endl;
    }
};

int main()
{
    student batch1[3] = {
        student("Arjun", 85.9, "arjun85@gmail.com"),
        student("Priya", 92.5, "Priya@gmail.com"),
        student("Rahul", 78, "Rahul@gmail.com")};

    cout << "Batch 1 (Stack array):" << endl;
    for (int i = 0; i < 3; i++)
    {
        batch1[i].display();
    }

    student *batch2 = new student[3]{
        student("Sneha", 88.0, "Sneha@gmail.com"),
        student("Vikram", 76.8, "Vikram@gmail.com"),
        student("Ananya", 95.6, "Ananya@gmail.com")};

    cout << "\nBatch 2(Heap array):" << endl;
    for (int i = 0; i < 3; i++)
    {
        batch2[i].display();
    }

    delete[] batch2;
    return 0;
}