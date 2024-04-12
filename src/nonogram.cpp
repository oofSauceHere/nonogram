#include "nonogram.h"
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

Nonogram::Nonogram(int size) : board_(size, vector<int>(size, 0)),
                               solvedBoard_(size, vector<int>(size, 0)),
                               rowConstraints_(size),
                               colConstraints_(size)
{
    rowConstraintMaxLen_ = 0;
    colConstraintMaxLen_ = 0;
    size_ = size;
    fails_ = 5;
    status_ = 0;
}

Nonogram::Nonogram(int size, std::vector<std::vector<int>> &rowConstraints, std::vector<std::vector<int>> &colConstraints) : board_(size, vector<int>(size, 0))
{
    rowConstraints_ = rowConstraints;
    colConstraints_ = colConstraints;
    rowConstraintMaxLen_ = 0;
    for (vector<int> i : rowConstraints_)
        rowConstraintMaxLen_ = max(static_cast<int>(i.size()), rowConstraintMaxLen_);
    colConstraintMaxLen_ = 0;
    for (vector<int> i : colConstraints_)
        colConstraintMaxLen_ = max(static_cast<int>(i.size()), colConstraintMaxLen_);
    // solvedBoard_ =
    size_ = size;
    fails_ = 5;
    status_ = 0;
}

void Nonogram::generate()
{
    if (status_ != 0)
        reset();

    srand(time(0));
    int numFilled = ((size_ * size_) / 2) - rand() % (size_ / 2);
    unordered_set<int> spots;

    while (spots.size() != numFilled)
    {
        int ind = rand() % (size_ * size_);
        if (spots.count(ind) == 0)
        {
            solvedBoard_[ind / size_][ind % size_] = 1;
            spots.insert(ind);
        }
    }

    // Rows
    for (int i = 0; i < size_; i++)
    {
        vector<int> constraint;
        int tally = 0;
        for (int j = 0; j < size_; j++)
        {
            if (solvedBoard_[i][j] == 0)
            {
                if (tally != 0)
                    constraint.push_back(tally);
                tally = 0;
            }
            else
            {
                tally++;
            }
        }
        if (tally != 0)
            constraint.push_back(tally);
        if (constraint.size() == 0)
            constraint.push_back(0);

        rowConstraintMaxLen_ = max(rowConstraintMaxLen_, static_cast<int>(constraint.size()));
        rowConstraints_[i] = constraint;
    }

    // Columns
    for (int i = 0; i < size_; i++)
    {
        vector<int> constraint;
        int tally = 0;
        for (int j = 0; j < size_; j++)
        {
            if (solvedBoard_[j][i] == 0)
            {
                if (tally != 0)
                    constraint.push_back(tally);
                tally = 0;
            }
            else
            {
                tally++;
            }
        }
        if (tally != 0)
            constraint.push_back(tally);
        if (constraint.size() == 0)
            constraint.push_back(0);

        colConstraintMaxLen_ = max(colConstraintMaxLen_, static_cast<int>(constraint.size()));
        colConstraints_[i] = constraint;
    }

    // Need some measure to determine if board has multiple solutions. I guess that's where the solver comes in.
}

void Nonogram::reset()
{
    for (int i = 0; i < size_; i++)
    {
        for (int j = 0; j < size_; j++)
        {
            board_[i][j] = 0;
            solvedBoard_[i][j] = 0;
        }
    }

    fails_ = 5;
    status_ = 0;
    rowConstraints_.clear();
    colConstraints_.clear();
    rowConstraintMaxLen_ = 0;
    colConstraintMaxLen_ = 0;
}

bool Nonogram::checkPlacement(int row, int col, int size)
{
    for (int i = 0; i < size; i++)
    {
        int tally = 0, cnt = 0;
        for (int j = 0; j <= row; j++)
        {
            if (cnt >= colConstraints_[col + i].size())
                return false;
            if (board_[j][col + i] == 1)
                tally++;
            else if (board_[j][col + i] == 0)
            {
                if (tally != 0)
                {
                    if (tally != colConstraints_[col + i][cnt])
                        return false;
                    tally = 0;
                    cnt++;
                }
            }
        }
        if (cnt < colConstraints_[col + i].size() && tally > colConstraints_[col + i][cnt])
            return false;
    }
    return true;
}

void Nonogram::helper2(int index, vector<int> &ends, vector<vector<int>> &layouts, int row, int col)
{
    if (index == rowConstraints_[row].size())
    {
        layouts.push_back(board_[row]);
        return;
    }
    for (int i = col; i <= ends[index] - rowConstraints_[row][index]; i++)
    {
        for (int j = 0; j < rowConstraints_[row][index]; j++)
        {
            board_[row][i + j] = 1;
        }

        if (checkPlacement(row, i, rowConstraints_[row][index]))
        {
            helper2(index + 1, ends, layouts, row, i + rowConstraints_[row][index] + 1);
        }

        for (int j = 0; j < rowConstraints_[row][index]; j++)
        {
            board_[row][i + j] = 0;
        }
    }
}

bool Nonogram::helper(int index)
{
    if (index == size_)
    {
        return true;
    }

    vector<int> ends = vector<int>(rowConstraints_[index].size(), size_);
    int runningTotal = 0;
    for (int i = rowConstraints_[index].size() - 1; i > 0; i--)
    {
        runningTotal += rowConstraints_[index][i] + 1;
        ends[i - 1] -= runningTotal;
    }

    vector<vector<int>> layouts;
    helper2(0, ends, layouts, index, 0);

    for (int i = 0; i < layouts.size(); i++)
    {
        board_[index] = layouts[i];
        // print();
        if (helper(index + 1))
            return true;

        board_[index] = vector<int>(size_, 0);
    }

    return false;
}

void Nonogram::solve()
{
    bool possible = helper(0);
    if (!possible)
        cout << "This board is unsolvable.\n";
}

void Nonogram::print() const
{
    for (int i = 0; i < colConstraintMaxLen_; i++)
    {
        for (int j = 0; j < rowConstraintMaxLen_ + 1; j++)
        {
            cout << "  ";
        }
        for (int j = 0; j < size_; j++)
        {
            const vector<int> &constraint = colConstraints_[j];
            if (i >= colConstraintMaxLen_ - constraint.size())
                cout << constraint[i - (colConstraintMaxLen_ - constraint.size())];
            else
                cout << " ";

            if (j != size_ - 1)
                cout << " ";
        }
        cout << "\n";
    }

    for (int i = 0; i < rowConstraintMaxLen_; i++)
    {
        cout << "  ";
    }
    cout << "+";
    for (int i = 0; i < size_; i++)
    {
        cout << "--";
    }
    cout << "-+\n";

    for (int i = 0; i < size_; i++)
    {
        const vector<int> &constraint = rowConstraints_[i];
        for (int j = 0; j < rowConstraintMaxLen_; j++)
        {
            if (j >= rowConstraintMaxLen_ - constraint.size())
                cout << constraint[j - (rowConstraintMaxLen_ - constraint.size())] << " ";
            else
                cout << "  ";
        }
        cout << "| ";
        for (int j = 0; j < size_; j++)
        {
            if (board_[i][j] == 0)
                cout << ".";
            else
                cout << "X";

            if (j != size_ - 1)
                cout << " ";
        }
        cout << " |\n";
    }

    for (int i = 0; i < rowConstraintMaxLen_; i++)
    {
        cout << "  ";
    }
    cout << "+";
    for (int i = 0; i < size_; i++)
    {
        cout << "--";
    }
    cout << "-+\n";
}