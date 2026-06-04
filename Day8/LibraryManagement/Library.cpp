#include "Library.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <iomanip>
using namespace std;

Library::Library(const string &file) : filename(file)
{
    loadFromFile();
}

void Library::loadFromFile()
{
    ifstream inFile(filename);
    if (!inFile)
        return; // File doesn't exist yet

    string line;
    while (getline(inFile, line))
    {
        if (!line.empty())
        {
            try
            {
                books.push_back(book::fromFileString(line));
            }
            catch (const exception &e)
            {
                cerr << "Error loading record: " << e.what() << endl;
            }
        }
    }
    inFile.close();
}

void Library::saveToFile()
{
    ofstream outFile(filename);
    if (!outFile)
    {
        throw runtime_error("Cannot open file for writing!");
    }

    for (const auto &book : books)
    {
        outFile << book.toFileString() << endl;
    }
    outFile.close();
}

void Library::addBook(const book &book)
{
    // Check for duplicate ID
    for (const auto &b : books)
    {
        if (b.getId() == book.getId())
        {
            throw invalid_argument("Book with this ID already exists!");
        }
    }
    books.push_back(book);
    saveToFile();
    cout << "Book added successfully!" << endl;
}

void Library::removeBook(int id)
{
    auto it = remove_if(books.begin(), books.end(),
                        [id](const book &b)
                        { return b.getId() == id; });

    if (it == books.end())
    {
        throw invalid_argument("Book not found!");
    }

    books.erase(it, books.end());
    saveToFile();
    cout << "Book removed successfully!" << endl;
}

void Library::issueBook(int id)
{
    for (auto &book : books)
    {
        if (book.getId() == id)
        {
            book.issue();
            saveToFile();
            cout << "Book issued successfully!" << endl;
            return;
        }
    }
    throw invalid_argument("Book not found!");
}

void Library::returnBook(int id)
{
    for (auto &book : books)
    {
        if (book.getId() == id)
        {
            book.returnBook();
            saveToFile();
            cout << "Book returned successfully!" << endl;
            return;
        }
    }
    throw invalid_argument("Book not found!");
}

void Library::searchByTitle(const string &title) const
{
    cout << left << setw(5) << "ID" << setw(30) << "Title"
         << setw(20) << "Author" << setw(10) << "Year"
         << setw(10) << "Status" << endl;
    cout << string(75, '-') << endl;

    bool found = false;
    for (const auto &book : books)
    {
        // Case-insensitive search
        string bookTitle = book.getTitle();
        string searchTitle = title;
        transform(bookTitle.begin(), bookTitle.end(), bookTitle.begin(), ::tolower);
        transform(searchTitle.begin(), searchTitle.end(), searchTitle.begin(), ::tolower);

        if (bookTitle.find(searchTitle) != string::npos)
        {
            book.display();
            found = true;
        }
    }

    if (!found)
    {
        cout << "No books found with title containing \"" << title << "\"" << endl;
    }
}

void Library::searchByAuthor(const string &author) const
{
    cout << left << setw(5) << "ID" << setw(30) << "Title"
         << setw(20) << "Author" << setw(10) << "Year"
         << setw(10) << "Status" << endl;
    cout << string(75, '-') << endl;

    bool found = false;
    for (const auto &book : books)
    {
        string bookAuthor = book.getAuthor();
        string searchAuthor = author;
        transform(bookAuthor.begin(), bookAuthor.end(), bookAuthor.begin(), ::tolower);
        transform(searchAuthor.begin(), searchAuthor.end(), searchAuthor.begin(), ::tolower);

        if (bookAuthor.find(searchAuthor) != string::npos)
        {
            book.display();
            found = true;
        }
    }

    if (!found)
    {
        cout << "No books found by author \"" << author << "\"" << endl;
    }
}

void Library::displayAllBooks() const
{
    if (books.empty())
    {
        cout << "No books in the library." << endl;
        return;
    }

    cout << "\n"
         << left << setw(5) << "ID" << setw(30) << "Title"
         << setw(20) << "Author" << setw(10) << "Year"
         << setw(10) << "Status" << endl;
    cout << string(75, '-') << endl;

    for (const auto &book : books)
    {
        book.display();
    }
    cout << "Total books: " << books.size() << endl;
}

void Library::displayAvailableBooks() const
{
    bool found = false;

    cout << "\nAvailable Books:" << endl;
    cout << left << setw(5) << "ID" << setw(30) << "Title"
         << setw(20) << "Author" << setw(10) << "Year" << endl;
    cout << string(65, '-') << endl;

    for (const auto &book : books)
    {
        if (!book.getIsIssued())
        {
            book.display();
            found = true;
        }
    }

    if (!found)
    {
        cout << "No books currently available." << endl;
    }
}

void Library::displayIssuedBooks() const
{
    bool found = false;

    cout << "\nIssued Books:" << endl;
    cout << left << setw(5) << "ID" << setw(30) << "Title"
         << setw(20) << "Author" << setw(10) << "Year" << endl;
    cout << string(65, '-') << endl;

    for (const auto &book : books)
    {
        if (book.getIsIssued())
        {
            book.display();
            found = true;
        }
    }

    if (!found)
    {
        cout << "No books currently issued." << endl;
    }
}