#include<iostream>
using namespace std;

class BankAccount{
    private:
      double balance;
      string accountNumber;
    
    public:
      BankAccount(string accNo,double bal) : accountNumber(accNo), balance(bal) {}
      
      friend void displayAccountInfo(const BankAccount& acc);

      friend class Auditor;
};

void displayAccountInfo(const BankAccount& acc){
    cout << "Account: " << acc.accountNumber << endl;
    cout << "Balance: $" << acc.balance << endl;
}

class Auditor{
    public:
      void audit(const BankAccount& acc){
        cout << "AUDIT REPORT" << endl;
        cout << "Account: " << acc.accountNumber << endl;
        cout << "Verified Balance: $" << acc.balance << endl;
      }
};

int main(){
    BankAccount acc("ACC-1001", 50000);
    displayAccountInfo(acc);

    cout << endl;
    Auditor auditor;
    auditor.audit(acc);
    return 0;
};