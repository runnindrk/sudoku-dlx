#include <iostream>
#include <string>
#include <vector>

#define NUM_ROWS 9
#define NUM_COLS 9
#define SIZE_BLOCKS 3

// -----------------------------------------------------------

class SolutionTranslator
{
    public:
    
    int sudoku[NUM_ROWS][NUM_COLS]        = {0};
    int solved_sudoku[NUM_ROWS][NUM_COLS] = {0};

    std::vector<std::vector<int>> sparse_exact_cover_row_meaning;
    std::vector<std::vector<int>> solution;

    SolutionTranslator(std::vector<std::vector<int>> (&row_meaning), std::vector<std::vector<int>> (&solution_found))

    {
        sparse_exact_cover_row_meaning = row_meaning;
        solution = solution_found;
        fillSolvedSudoku();
    }

    // -------------------------------------------------------

    void printArrayOnTerminal()
    {
        std::string divisionBar = " _________________________\n";
        std::string emptySpace = " ";
        std::string stringValue = " ";
        bool flagRow = false;

        for (int i = 0; i < NUM_COLS; i++)
        {
            flagRow = false;

            for (int j = 0; j < NUM_ROWS; j++)
            {
                stringValue = std::to_string(solved_sudoku[i][j]);

                if (!(i%SIZE_BLOCKS) and !flagRow)
                {
                    std::cout << divisionBar;
                    flagRow = true;
                }

                if (!(j%SIZE_BLOCKS))
                    std::cout << " |";

                if (stringValue != "0")
                    std::cout << emptySpace + stringValue; 

                else
                    std::cout << emptySpace + emptySpace;    
            }

            std::cout << " |\n";
        }

        std::cout << divisionBar;
    }

    // -------------------------------------------------------

    private:

    void fillSolvedSudoku()
    {
        for (int i = 0; i < solution[0].size(); i++)
        {
            int solution_row = solution[0][i];
            int row = sparse_exact_cover_row_meaning[solution_row][1];
            int col = sparse_exact_cover_row_meaning[solution_row][2];
            int val = sparse_exact_cover_row_meaning[solution_row][0];

            solved_sudoku[row][col] = val;
        }
    }
};