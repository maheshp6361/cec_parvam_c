#include "Book.h"
#include <sstream>
#include <iomanip>
using namespace std;

book::book() : id(0), title("N/A"), author("N/A"), year(0), isIssued(false) {}

book::book(int id, string t, string a, int y)
    : id(id), title(t), author(a), year(y), isIssued(false) {}

int book::getId() const { return id; }
string book::getTitle() const { return title; }
string book::getAuthor() const { return author; }
int book::getYear() const { return year; }
bool book::getIsIssued() const { return isIssued; }

void book::setTitle(const string &t) { title = t; }
void book::setAuthor(const string &a) { author = a; }

void book::setYear(int y)
{
    if (y < 0 || y > 2025)
    {
        throw invalid_argument("Invalid year!");
    }
    year = y;
}

void book::issue()
{
    if (isIssued)
    {
        throw runtime_error("Book is already issued!");
    }
    isIssued = true;
}

void book::returnBook()
{
    if (!isIssued)
    {
        throw runtime_error("Book was not issued!");
    }
    isIssued = false;
}

void book::display() const
{
    cout << left << setw(5) << id
         << setw(30) << title
         << setw(20) << author
         << setw(10) << year
         << setw(10) << (isIssued ? "Issued" : "Available") << endl;
}

string book::toFileString() const
{
    return to_string(id) + "|" + title + "|" + author + "|" +
           to_string(year) + "|" + (isIssued ? "1" : "0");
}

book book::fromFileString(const string &line)
{
    stringstream ss(line);
    string token;

    getline(ss, token, '|');
    int id = stoi(token);
    getline(ss, token, '|');
    string title = token;
    getline(ss, token, '|');
    string author = token;
    getline(ss, token, '|');
    int year = stoi(token);
    getline(ss, token, '|');
    bool issued = (token == "1");

    book book(id, title, author, year);
    if (issued)
        book.issue();
    return book;
}