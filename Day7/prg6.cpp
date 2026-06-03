#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

template <typename T>
class scoremanager
{
private:
    vector<T> scores;

public:
    void addscore(T score)
    {
        scores.push_back(score);
    }

    T getAverage()
    {
        if (scores.empty())
            return T();
        T sum = 0;
        for (T s : scores)
            sum += s;
        return sum / scores.size();
    }

    T getHighest()
    {
        return *max_element(scores.begin(), scores.end());
    }

    T getLowest()
    {
        return *min_element(scores.begin(), scores.end());
    }

    void displayAll()
    {
        cout << "Scores: ";
        for (T s : scores)
            cout << s << " ";
        cout << endl;
    }

    int getcount() { return scores.size(); }
};

int main()
{
    scoremanager<int> mathscores;
    mathscores.addscore(85);
    mathscores.addscore(92);
    mathscores.addscore(78);
    mathscores.addscore(95);
    mathscores.addscore(88);

    cout << "=== Math Scores (int) ===" << endl;
    mathscores.displayAll();
    cout << "Count: " << mathscores.getcount() << endl;
    cout << "Average: " << mathscores.getAverage() << endl;
    cout << "Highest: " << mathscores.getHighest() << endl;
    cout << "lowest: " << mathscores.getLowest() << endl;

    scoremanager<double> sciencescores;
    sciencescores.addscore(88.5);
    sciencescores.addscore(91.2);
    sciencescores.addscore(76.8);

    cout << "\n=== Science Scores (double) ===" << endl;
    sciencescores.displayAll();
    cout << "Average: " << sciencescores.getAverage() << endl;

    return 0;
}