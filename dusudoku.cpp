/**
 * Title: dusudoku.cpp
 * Author: Richard Merry
 * Date: 01/04/2018
 *
 * A sudoku puzzle solving demonstration
 * employing a recursive backtracking algorithm
 *
 * Usage: dusudoku <sudoku_string>
 *   example: ./dusudoku 530070000600195000098000060800060003400803001700020006060000280000419005000080079
 *        or:      "     53--7----6--195----98----6-8---6---34--8-3--17---2---6-6----28----419--5----8--79
 *
 *   Solves for the following sudoku:
 *
 *    +-----------------------+
 *    | 5 3   |   7   |       |
 *    | 6     | 1 9 5 |       |
 *    |   9 8 |       |   6   |
 *    + ----------------------+
 *    | 8     |   6   |     3 |
 *    | 4     | 8   3 |     1 |
 *    | 7     |   2   |     6 |
 *    + ----------------------+
 *    |   6   |       | 2 8   |
 *    |       | 4 1 9 |     5 |
 *    |       |   8   |   7 9 |
 *    +-----------------------+
 *
 *   Where both `0' and `-' represent an empty slot
 */

#include <bitset>
#include <iostream>
#include <math.h>
#include <string.h>

using namespace std;

#define DASH 45
#define NUMERAL_ZERO 48
#define NUMERAL_NINE 57

struct sudoku {
  int grid[81];
  bitset<9> columns[9];
  bitset<9> rows[9];
  bitset<9> subgrid[9];
};

// in order to optimise the subgrid lookup procedure
const int ninths[81] = {
  0, 0, 0, 1, 1, 1, 2, 2, 2,
  0, 0, 0, 1, 1, 1, 2, 2, 2,
  0, 0, 0, 1, 1, 1, 2, 2, 2,
  3, 3, 3, 4, 4, 4, 5, 5, 5,
  3, 3, 3, 4, 4, 4, 5, 5, 5,
  3, 3, 3, 4, 4, 4, 5, 5, 5,
  6, 6, 6, 7, 7, 7, 8, 8, 8,
  6, 6, 6, 7, 7, 7, 8, 8, 8,
  6, 6, 6, 7, 7, 7, 8, 8, 8,
};

// function prototypes
void printGrid(int* grid);
void printUsage();
bool readInput(const char* input, sudoku* s, string* err);
bool solve(int p, sudoku* s);

int main(int argc, char* argv[]) {
  if (argc <= 1) {
    printUsage();
    return 1;
  }

  sudoku s;
  string errMsg;
  if (!readInput(argv[1], &s, &errMsg)) {
    cout << "Error: " << errMsg << endl;
    printUsage();
    return 1;
  }

  cout << "solving...";

  if (solve(0, &s)) {
    cout << "solution found!" << endl;
    printGrid(s.grid);
  } else {
    cout << "no solution" << endl;
  }

  return 0;
}

void printGrid(int* grid) {
  for (int i = 0; i < 81; i++) {
    if (i % 9 == 0) cout << "\n";
    cout << grid[i] << " ";
  }
  cout << endl;
}

void printUsage() {
  cout << "Usage: dusudoku <sudoku_string>\n"
       << "\tWhere <sudoku_string> is an 81 character long string of numerals\n"
       << "\tand/or dashes representing a valid sudoku puzzle.\n"
       << "\texample: dusudoku 53--7----6--195----98----6-8---6---34--8-3--17---2---6-6----28----419--5----8--79"
       << endl;
}

// Reads the sudoku string into the sudoku struct.
// Returns true if the sudoku is valid, false otherwise
// and sets the err string appropriately
bool readInput(const char* input, sudoku * s, string* err) {
  // 9 x 9 grid expected
  if (strlen(input) != 81) {
    *err = "The expected input length is 81 characters";
    return false;
  }

  for (int i = 0; i < 81; i++) {
    // fail if not a numeral or a dash (-)
    if ((input[i] < NUMERAL_ZERO || input[i] > NUMERAL_NINE) && input[i] != DASH) {
      *err = string("Invalid input character (") + input[i] + ")";
      return false;
    }
    int num = 0;
    // convert ascii numeral to int in cute way
    if (input[i] != DASH)
      num = input[i]-NUMERAL_ZERO;

    s->grid[i] = num;

    // register this numbers position on row and column
    if (num > 0) {
      int c_i = i%9;
      int r_i = floor(i/9);
      int s_i = ninths[i];

      // if the number already appears in this column,
      // row or subgrid, the sudoku must be invalid
      if (s->columns[c_i][num-1]) {
        *err = string("The number ") + to_string(num) + " appears more than once in column " + to_string(c_i+1);
        return false;
      } else if (s->rows[r_i][num-1]) {
        *err = string("The number ") + to_string(num) + " appears more than once on row " + to_string(r_i+1);
        return false;
      } else if (s->subgrid[s_i][num-1]) {
        *err = string("The number ") + to_string(num) + " appears more than once in subgrid " + to_string(c_i+1);
        return false;
      }

      s->columns[c_i].set(num-1);
      s->rows[r_i].set(num-1);
      s->subgrid[s_i].set(num-1);
    }
  }

  return true;
}

// Recursive sudoku solution algorithm, p represents the grid
// position to start solving from (where a sudoku grid runs
// from 0-80 [81 squares]). Returns true if a solution was found,
// false otherwise
bool solve(int p, sudoku* s) {
  // the last square has been filled: solution found!
  if (p == 81) {
    return true;
  }
  // current square is already set
  if (s->grid[p] > 0) {
    return solve(p+1, s);
  }

  // calculate the current column, row and subgrid indices
  int c_i = p%9;
  int r_i = floor(p/9);
  int s_i = ninths[p];

  // try numbers 1-9 in current grid position
  for (int i = 0; i < 9; i++) {
    // skip this number if it already appears in the current column/row/subgrid
    if (s->columns[c_i][i] || s->rows[r_i][i] || s->subgrid[s_i][i]) continue;

    // add the number to the current grid position
    s->columns[c_i].set(i);
    s->rows[r_i].set(i);
    s->subgrid[s_i].set(i);
    s->grid[p] = i+1;

    if (solve(p+1, s)) return true;

    // no solution: backtrack!
    s->columns[c_i].reset(i);
    s->rows[r_i].reset(i);
    s->subgrid[s_i].reset(i);
    s->grid[p] = 0;
  }

  return false;
}
