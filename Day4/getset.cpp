#include<iostream>
using namespace std;

class student{
    private:
        string name;
        int age;
        double cgpa;

    public: 
        string getname() const { return name;}
        int getage() const { return age;}
        double getcgpa() const { return cgpa;}

        void setname(const string& n) {name = n;}

        void setage(int a) {
            if (a > 0 && a < 150) {
                age = a;
            } else {
                cout<<"Invalid age!"<<endl;
            }
        }

        void setcgpa(double c) {
            if (c>=0.0 && c<=10.0) {
                cgpa = c;
            } else {
                cout<<"Invalid CGPA! Must be between 0.0 to 10.0"<<endl;
            }
        }
};

int main() {
    student s1;
    s1.setname("Bob");
    s1.setage(20);
    s1.setcgpa(8.9);

    cout<<"Name: "<<s1.getname()<<endl;
    cout<<"Age: "<<s1.getage()<<endl;
    cout<<"CGPA: "<<s1.getcgpa()<<endl;

     student s2;
    s2.setname("Oggy");
    s2.setage(200);
    s2.setcgpa(11.9);

    cout<<"Name: "<<s2.getname()<<endl;
    cout<<"Age: "<<s2.getage()<<endl;
    cout<<"CGPA: "<<s2.getcgpa()<<endl;
}