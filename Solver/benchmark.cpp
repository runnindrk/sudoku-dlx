#include "DLX.cpp"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

// -----------------------------------------------------------

void benchmark()
{
    1;
}

int main()
{
    /*
    string sudoku_string = "6..3.2....4.....8..........7.26............543.........8.15........8.2........7..";
    solveSudoku solve(sudoku_string);
    solve.printArrayOnTerminal(solve.sudoku);
    solve.backTrackingSolver();
    solve.printArrayOnTerminal(solve.sudoku_solution);
    cout << "\n" << solve.num_backtrack_calls << " " << solve.num_guesses << "\n";
    */

    std::vector<std::vector<int>> solutions;
    std::vector<std::vector<int>> cover_matrix;

    cover_matrix.push_back({0, 0}); cover_matrix.push_back({0, 3});
    cover_matrix.push_back({0, 6}); cover_matrix.push_back({1, 0});
    cover_matrix.push_back({1, 3}); cover_matrix.push_back({2, 3});
    cover_matrix.push_back({2, 4}); cover_matrix.push_back({2, 6});
    cover_matrix.push_back({3, 2}); cover_matrix.push_back({3, 4});
    cover_matrix.push_back({3, 5}); cover_matrix.push_back({4, 1});
    cover_matrix.push_back({4, 2}); cover_matrix.push_back({4, 5});
    cover_matrix.push_back({4, 6}); cover_matrix.push_back({5, 1});
    cover_matrix.push_back({5, 6}); cover_matrix.push_back({6, 0});
    cover_matrix.push_back({6, 3});
    
    ExactCoverDancingLinks solve(cover_matrix);
    solutions = solve.solutions_found;

    for (int i = 0; i < solutions.size(); i++)
    {
        for (int j = 0; j < solutions[i].size(); j++)
            std::cout << solutions[i][j] << " ";

        std::cout << "\n";
    }

    return 0;
}
