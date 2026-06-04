#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

int main()
{
    ofstream outFile("students.txt");

    if (outFile.is_open())
    {
        outFile << left << setw(10) << "ID"
                << setw(20) << "Name"
                << setw(10) << "CGPA" << endl;
        outFile << string(40, '-') << endl;

        outFile << setw(10) << 101
                << setw(20) << "Arjun"
                << setw(10) << fixed << setprecision(2) << 8.75 << endl;

        outFile << setw(10) << 102
                << setw(20) << "Priya"
                << setw(10) << fixed << setprecision(2) << 9.20 << endl;

        outFile.close();
    }

    ifstream inFile("students.txt");
    string line;

    if (inFile.is_open())
    {
        cout << "=== Student Records ===" << endl;
        while (getline(inFile, line))
        {
            cout << line << endl;
        }
        inFile.close();
    }

    ifstream numFile("numbers.txt");
    int sum = 0, num;

    ofstream createNum("numbers.txt");
    createNum << 10 << " " << 20 << " " << 30 << " " << 40 << " " << 50;
    createNum.close();

    numFile.open("numbers.txt");
    while (numFile >> num)
    {
        sum += num;
    }
    numFile.close();

    cout << "\nSum of numbers: " << sum << endl;

    return 0;
}