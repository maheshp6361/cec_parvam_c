#include <iostream>
#include <limits>
#include "Library.h"
using namespace std;

void displayMenu()
{
    cout << "\n=================================" << endl;
    cout << "  LIBRARY MANAGEMENT SYSTEM" << endl;
    cout << "=================================" << endl;
    cout << "  1. Add Book" << endl;
    cout << "  2. Remove Book" << endl;
    cout << "  3. Issue Book" << endl;
    cout << "  4. Return Book" << endl;
    cout << "  5. Search by Title" << endl;
    cout << "  6. Search by Author" << endl;
    cout << "  7. Display All Books" << endl;
    cout << "  8. Display Available Books" << endl;
    cout << "  9. Display Issued Books" << endl;
    cout << "  10. Exit" << endl;
    cout << "=================================" << endl;
    cout << "Enter choice: ";
}

int getIntInput(const string &prompt)
{
    int value;
    while (true)
    {
        cout << prompt;
        cin >> value;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number." << endl;
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

string getStringInput(const string &prompt)
{
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

int main()
{
    Library library("books.txt");
    int choice;

    try
    {
        do
        {
            displayMenu();
            choice = getIntInput("");

            switch (choice)
            {
            case 1:
            {
                int id = getIntInput("Enter book ID: ");
                string title = getStringInput("Enter title: ");
                string author = getStringInput("Enter author: ");
                int year = getIntInput("Enter year: ");

                try
                {
                    book newBook(id, title, author, year);
                    library.addBook(newBook);
                }
                catch (const exception &e)
                {
                    cout << "Error: " << e.what() << endl;
                }
                break;
            }

            case 2:
            {
                int id = getIntInput("Enter book ID to remove: ");
                try
                {
                    library.removeBook(id);
                }
                catch (const exception &e)
                {
                    cout << "Error: " << e.what() << endl;
                }
                break;
            }

            case 3:
            {
                int id = getIntInput("Enter book ID to issue: ");
                try
                {
                    library.issueBook(id);
                }
                catch (const exception &e)
                {
                    cout << "Error: " << e.what() << endl;
                }
                break;
            }

            case 4:
            {
                int id = getIntInput("Enter book ID to return: ");
                try
                {
                    library.returnBook(id);
                }
                catch (const exception &e)
                {
                    cout << "Error: " << e.what() << endl;
                }
                break;
            }

            case 5:
            {
                string title = getStringInput("Enter title to search: ");
                library.searchByTitle(title);
                break;
            }

            case 6:
            {
                string author = getStringInput("Enter author to search: ");
                library.searchByAuthor(author);
                break;
            }

            case 7:
                library.displayAllBooks();
                break;

            case 8:
                library.displayAvailableBooks();
                break;

            case 9:
                library.displayIssuedBooks();
                break;

            case 10:
                cout << "Thank you for using Library Management System!" << endl;
                break;

            default:
                cout << "Invalid choice! Please enter 1-10." << endl;
            }

        } while (choice != 10);
    }
    catch (const exception &e)
    {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    }

    return 0;
}