This is a Sudoku Solver that solves it with the Dancing Links Technique, DLX.
This was primarly a learning experience about Exact Cover Problem, Linked Lists and the Algorithm X.
There is a file, dlx.cpp, that solves any Exact Cover problem using the DLX provided the sparse matrix, in the form of {{row_1, col_1}, {row_2, col_2}, ...} using the MVR heuristic.

Then I applied to the Sudoku problem. It is a relative fast solver, even without any pruning before hand. On the hardest_11+ data set I was able to achieve 1080 Puzzles/s on my i5-8600k.
I can further optimize it, instead of computing the sparse matrix and then transform it to a Linked List I could directly compute the Linked List.
Another optimization is to include pruning of the Sudoku Problem, for example and implementation of Naked and Hidden Singles.

But I am happy with the result of this personal project, I learned a lot.
