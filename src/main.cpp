#include "nonogram.h"
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{
    cout << "Generate (g) board or input (i) existing board? ";
    char com;
    cin >> com;
    if (com == 'i')
    {
        int size;
        cout << "Enter board size: ";
        cin >> size;

        vector<vector<int>> rowConstraints, colConstraints;
        cout << "Enter row contraints (separated by line):\n";
        string line;
        getline(cin, line);
        for (int i = 0; i < 2 * size; i++)
        {
            if (i == size)
            {
                cout << "Enter column contraints (separated by line):\n";
            }

            getline(cin, line);
            stringstream ss(line);

            vector<int> constraint;
            int n;
            ss >> n;
            while (!ss.fail())
            {
                constraint.push_back(n);
                ss >> n;
            }

            if (i >= size)
                colConstraints.push_back(constraint);
            else
                rowConstraints.push_back(constraint);
        }

        Nonogram game = Nonogram(size, rowConstraints, colConstraints);
        game.solve();
        game.print();
    }
    else if (com == 'g')
    {
        int size;
        cout << "Enter board size: ";
        cin >> size;
        if (size < 2)
        {
            cout << "Board size too small." << endl;
            return 0;
        }
        Nonogram game = Nonogram(size);
        game.generate();

        // You can't even play it. Who made this? Dumbass.
        game.solve();
        game.print();
    }
    return 0;
}