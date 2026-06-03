#include <iostream>
#include <string>
using namespace std;

template <typename T>
T getMax(T a, T b)
{
    return (a > b) ? a : b;
}

template <typename T, typename U>
auto add(T a, U b) -> decltype(a + b)
{
    return a + b;
}

int main()
{
    cout << "Max of 10 and 20: " << getMax(10, 20) << endl;
    cout << "Max of 3.14 and 2.71: " << getMax(3.14, 2.71) << endl;
    cout << "Max of 'a' and 'z': " << getMax('a', 'z') << endl;

    cout << "Max (explicit double): " << getMax<double>(5, 7.5) << endl;

    cout << "Add int + double: " << add(10, 3.5) << endl;
    cout << "Add string + string: " << add(to_string(42), to_string(100)) << endl;

    return 0;
}