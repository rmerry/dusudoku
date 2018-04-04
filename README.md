dusudoku
========
_dusudoku_ is a command line tool written in C++ for solving standard 9x9 sudoku puzzles.

Compilation
-----------
Use your favourite C++ compiler. With _g++_ for example:

```
$ g++ -Wall -o dusudoku dusudoku.cpp
```

Usage
-----
```
$ ./dusudoku <sudoku_string>
```
Where `<sudoku_string>` is an 81 character long string of numerals and or dashes. Both `-` and `0` characters can be used to represent empty grid positions. As an example, to solve the following sudoku puzzle:

```
 +-----------------------+
 | 5 3   |   7   |       |
 | 6     | 1 9 5 |       |
 |   9 8 |       |   6   |
 + ----------------------+
 | 8     |   6   |     3 |
 | 4     | 8   3 |     1 |
 | 7     |   2   |     6 |
 + ----------------------+
 |   6   |       | 2 8   |
 |       | 4 1 9 |     5 |
 |       |   8   |   7 9 |
 +-----------------------+
```

You would type (and receive output):

```
$ ./dusudoku 53--7----6--195----98----6-8---6---34--8-3--17---2---6-6----28----419--5----8--79
solving...solution found!

5 3 4 6 7 8 9 1 2
6 7 2 1 9 5 3 4 8
1 9 8 3 4 2 5 6 7
8 5 9 7 6 1 4 2 3
4 2 6 8 5 3 7 9 1
7 1 3 9 2 4 8 5 6
9 6 1 5 3 7 2 8 4
2 8 7 4 1 9 6 3 5
3 4 5 2 8 6 1 7 9
```
