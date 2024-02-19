#include "dlx.cpp"
#include "sudoku_exact_cover_sparse_matrix.cpp"
#include "sudoku_translator.cpp"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

// -----------------------------------------------------------

void benchmark()
{
    int num_puzzles_solved = 0;
    std::string sudoku;
    std::ifstream file("../Data_Sets/puzzles5_forum_hardest_1905_11+");

    if (!file.is_open()) 
        std::cerr << "Failed to open the file.\n";

    getline(file, sudoku);
    getline(file, sudoku);

    auto start = std::chrono::high_resolution_clock::now();

    while (getline(file, sudoku))
    {
        std::vector<std::vector<int>> solutions;
        std::vector<std::vector<int>> cover_matrix;
        std::vector<std::vector<int>> row_meaning;

        CreateSparseExactCoverMatrix create_matrix(sudoku);
        cover_matrix = create_matrix.sparse_exact_cover_matrix;
        row_meaning = create_matrix.sparse_exact_cover_row_meaning;

        ExactCoverDancingLinks solve(cover_matrix);
        solutions = solve.solutions_found;

        SolutionTranslator translator(row_meaning, solutions);

        num_puzzles_solved += 1;
        //std::cout << num_puzzles_solved << " " << sudoku << "\n";
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start); 

    file.close();

    std::cout << "\n";
    std::cout << num_puzzles_solved << " Solved Puzzles\n";
    std::cout << static_cast<float>(num_puzzles_solved)/(duration.count()*1e-6) << " Puzzles/s\n";
}

int main()
{
    // std::string sudoku_string = "6..3.2....4.....8..........7.26............543.........8.15........8.2........7..";
    
    // std::vector<std::vector<int>> solutions;
    // std::vector<std::vector<int>> cover_matrix;
    // std::vector<std::vector<int>> row_meaning;

    // CreateSparseExactCoverMatrix create_matrix(sudoku_string);
    // cover_matrix = create_matrix.sparse_exact_cover_matrix;
    // row_meaning = create_matrix.sparse_exact_cover_row_meaning;
    
    // ExactCoverDancingLinks solve(cover_matrix);
    // solutions = solve.solutions_found;

    // SolutionTranslator translator(row_meaning, solutions);
    // translator.printArrayOnTerminal();

    benchmark();

    return 0;
}
