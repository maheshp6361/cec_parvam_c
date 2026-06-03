#include<iostream>
#include<string>
using namespace std;

template <typename T>
class Pair{
    private:
       T first;
       T second;

    public:
       Pair(T f,T s) : first(f), second(s) {}
       
       T getFirst() const { return first; }
       T getSecond() const { return second; }
         
       T getMax() const { return (first > second) ? first : second;}

       void display() const {
        cout << "(" << first << ", " << second << ")" << endl;
       }
};

template <typename T,typename U>
class KeyValue {
    private:
       T key;
       U value;

    public:
       KeyValue(T k, U v) : key(k), value(v){}
       
       void display() const{
        cout << "[" << key << "] => " << value << endl;
       }
};

int main(int argc, char const *argv[])
{
    Pair<int> p1(10, 20);
    Pair<double> p2(3.14, 2.71);
    Pair<string> p3("Apple", "Orange");

    cout << "Int pair: "; p1.display();
    cout << "Max: " << p1.getMax() << endl;

    cout << "Double pair: "; p2.display();
    cout << "Max: " << p2.getMax() << endl;

    cout << "String pair: "; p3.display();
    cout << "Max: " << p3.getMax() << endl;

    KeyValue<int, string> kv(101, "Arjun");
    KeyValue<string, double> kv2("Pi", 3.14159);

    cout << "\nKey-Value pairs:" << endl;
    return 0;
}
