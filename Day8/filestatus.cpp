#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    ifstream file("noneexistent.txt");

    if (!file)
    {
        cout << "File not found!" << endl;
    }

    if (file.fail())
    {
        cout << "File operation failed!" << endl;
    }

    if (file.is_open())
    {
        cout << "File is open." << endl;
        file.close();
    }

    ifstream file2("example.txt");
    if (file2.good())
    {
        cout << "File is in good state." << endl;
    }

    return 0;
}