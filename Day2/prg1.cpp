#include <string>
#include <iostream>
using namespace std;

class Bankaccount
{
    double balance;
    string accountnumber;

protected:
    string accountholdername;

public:
    Bankaccount(string name, string accno)
    {
        accountholdername = name;
        accountnumber = accno;
        balance = 0.0;
    }
    void deposit(double amount)
    {
        if (amount > 0)
        {
            balance += amount;
            cout << "Deposited $ " << amount << endl;
        }
    }
    void showbalance()
    {
        cout << "Balance: $" << balance << endl;
    }
};

class savingaccount : public Bankaccount
{
public:
    savingaccount(string name, string accno)
        : Bankaccount(name, accno) {}
    void showholder()
    {
        cout << "Account holder: " << accountholdername << endl;
    }
};

int main()
{
    Bankaccount acc("Arjun", "a1001");
    acc.deposit(1000);
    acc.showbalance();

    savingaccount sav("Priya", "s2001");
    sav.showholder();

    return 0;
}