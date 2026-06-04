#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main()
{
    ofstream outFile("example.txt");

    if (outFile.is_open())
    {
        outFile << "Hello, File!" << endl;
        outFile << "This is line 2." << endl;
        outFile << "C++ file handling is easy!" << endl;
        outFile.close();
        cout << "File written successfully!" << endl;
    }
    else
    {
        cout << "Error: Could not open file for writing!" << endl;
    }

    ifstream inFile("example.txt");

    if (inFile.is_open())
    {
        string line;
        cout << "\nFile contents:" << endl;
        while (getline(inFile, line))
        {
            cout << line << endl;
        }
        inFile.close();
    }
    else
    {
        cout << "Error: could not open file for reading!" << endl;
    }

    return 0;
}