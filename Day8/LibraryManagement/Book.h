#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <iostream>
using namespace std;

class book
{
private:
    int id;
    string title;
    string author;
    int year;
    bool isIssued;

public:
    book();
    book(int id, string title, string author, int year);

    int getId() const;
    string getTitle() const;
    string getAuthor() const;
    int getYear() const;
    bool getIsIssued() const;

    void setTitle(const string &title);
    void setAuthor(const string &author);
    void setYear(int year);
    void issue();
    void returnBook();

    void display() const;

    string toFileString() const;
    static book fromFileString(const string &line);
};

#endif