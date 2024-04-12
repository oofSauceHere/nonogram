#ifndef NONOGRAM_H
#define NONOGRAM_H

#include <vector>

class Nonogram
{
private:
    std::vector<std::vector<int>> board_;
    std::vector<std::vector<int>> solvedBoard_;
    std::vector<std::vector<int>> rowConstraints_;
    std::vector<std::vector<int>> colConstraints_;
    int rowConstraintMaxLen_;
    int colConstraintMaxLen_;
    int size_;
    int status_;
    int fails_;

    // There's actually little bounds-checking in any of these functions. This is bad.
    bool checkPlacement(int row, int col, int size);
    bool helper(int index);
    void helper2(int index, std::vector<int> &ends, std::vector<std::vector<int>> &layouts, int row, int col);

public:
    Nonogram(int size);
    // Unsure as to whether passing by reference to a constructor is bad practice, but it doesn't matter for this
    // program because "new" is never used.
    Nonogram(int size, std::vector<std::vector<int>> &rowConstraints, std::vector<std::vector<int>> &colConstraints);
    void generate();
    void reset();
    void solve();
    void print() const;
    void place(int r, int c);
    int status() const;
};

#endif