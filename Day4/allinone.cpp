#include <iostream>
#include <string>
using namespace std;

class Temperature
{
private:
    double celsius;
    double toFahrenheit() const
    {
        return (celsius * 9.0 / 5.0) + 32;
    }
    double toKelvin() const
    {
        return (celsius + 273.15);
    }

public:
    Temperature() : celsius(0) {}

    void setCelsius(double c)
    {
        if (c >= -273.15)
        {
            celsius = c;
        }
        else
        {
            cout << "Temperature cannot be below absolute zero!" << endl;
            celsius = -273.15;
        }
    }
    void setFahrenheit(double f)
    {
        double c = (f - 32) * 5.0 / 9.0;
        setCelsius(c);
    }

    double getCelsius() const { return celsius; }
    double getFahrenheit() const { return toFahrenheit(); }
    double getKelvin() const { return toKelvin(); }

    void display()
    {
        cout << celsius << "°C = " << toFahrenheit() << "K" << endl;
    }
};

int main(int argc, char const *argv[])
{
    Temperature temp;
    cout << "25°C in different units:" << endl;
    temp.setCelsius(25);
    temp.display();

    temp.setFahrenheit(98.6);
    cout << "\n98.6°F in different units:" << endl;
    temp.display();

    temp.setCelsius(-500);
    cout << "\n After invalid input:" << endl;
    temp.display();
    return 0;
}