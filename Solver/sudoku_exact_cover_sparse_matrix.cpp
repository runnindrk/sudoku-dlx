#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

#define NUM_ROWS 9
#define NUM_COLS 9
#define SIZE_BLOCKS 3
#define NUM_CELLS (NUM_ROWS*NUM_COLS)
#define NUMBER_OF_PEERS 20
#define ALL_BITS 511

// -----------------------------------------------------------

class CreateSparseExactCoverMatrix
{
    public:

    std::vector<std::vector<int>> sparse_exact_cover_row_meaning;
    std::vector<std::vector<int>> sparse_exact_cover_matrix;

    CreateSparseExactCoverMatrix(std::string &sudoku_string)
    
    {
        fillInternalRepresentation(sudoku_string, sudoku);
        fillCellPeers();
        setAllPossileValues();
        fillSparseExactCoverMatrix();
    }

    private:

    int sudoku[NUM_ROWS][NUM_COLS]          = {0};
    int possible_values[NUM_ROWS][NUM_COLS] = {0};
    std::map<std::array<int, 2>, std::array<std::array<int, 2>, NUMBER_OF_PEERS>> cell_peers; 

    // -------------------------------------------------------

    void fillInternalRepresentation(std::string &sudoku_string, int (&sudoku)[NUM_ROWS][NUM_COLS])
    {
        std::string stringValue;

        for (int i = 0; i < NUM_COLS; i++)
            for (int j = 0; j < NUM_ROWS; j++)
            {
                stringValue = sudoku_string[i*NUM_COLS + j];

                if (stringValue != ".")
                    sudoku[i][j] = stoi(stringValue);

                else 
                    sudoku[i][j] = 0;
            }
    }

    // -------------------------------------------------------

    std::set<std::array<int, 2>> findCellPeers(int col, int row)
    {
        int row_block = row/SIZE_BLOCKS;
        int col_block = col/SIZE_BLOCKS;
        std::set<std::array<int, 2>> peers;

        for (int i = 0; i < NUM_ROWS; i++)
            peers.insert({col, i});

        for (int i = 0; i < NUM_COLS; i++)
            peers.insert({i, row});

        for (int i = 0; i < SIZE_BLOCKS; i++)
            for (int j = 0; j < SIZE_BLOCKS; j++)
                peers.insert({col_block*SIZE_BLOCKS + i, row_block*SIZE_BLOCKS + j});

        peers.erase({col, row});

        return peers;         
    }

    void fillCellPeers()
    {
        std::array<std::array<int, 2>, NUMBER_OF_PEERS> peers_array;

        for (int i = 0; i < NUM_ROWS; i++)
            for (int j = 0; j < NUM_COLS; j++)
            {
                std::set<std::array<int, 2>> peers = findCellPeers(i, j);
                auto it = peers.begin();

                for (int i = 0; i < peers.size(); ++i)
                {
                    peers_array[i] = *it;
                    ++it;
                }

                cell_peers[{i, j}] = peers_array;
            }
    }

    // -------------------------------------------------------

    void setCellPossibleValues(int col, int row)
    {
        std::array<std::array<int, 2>, NUMBER_OF_PEERS> peers = cell_peers.find({col, row})->second;
        int base_value = 1;
        int all_values = 0;
        int peer_value = ALL_BITS;

        for (int i = 0; i < NUMBER_OF_PEERS; i++)
        {
            int cell_value = sudoku[peers[i][0]][peers[i][1]];

            if (cell_value)
            {
                all_values = all_values | (base_value <<= (cell_value - 1));
                base_value = 1;
            }
        }

        possible_values[col][row] = peer_value ^ all_values;;
    }

    void setAllPossileValues()
    {
        for (int i = 0; i < NUM_ROWS; i++)
            for (int j = 0; j < NUM_COLS; j++)
                if (!sudoku[i][j])
                    setCellPossibleValues(i, j);
    }

    // -------------------------------------------------------

    void addSparseExactCoverMatrix(int sparse_exact_cover_matrix_row, int row, int col, int value)
    {
        int row_block = row/SIZE_BLOCKS;
        int col_block = col/SIZE_BLOCKS;

        std::vector<int> row_meaning = {value + 1, row, col};
        std::vector<int> cel_element = {sparse_exact_cover_matrix_row, 0*NUM_CELLS};
        std::vector<int> row_element = {sparse_exact_cover_matrix_row, 1*NUM_CELLS};
        std::vector<int> col_element = {sparse_exact_cover_matrix_row, 2*NUM_CELLS};
        std::vector<int> blk_element = {sparse_exact_cover_matrix_row, 3*NUM_CELLS};

        cel_element[1] += row*NUM_ROWS + col;
        row_element[1] += row*NUM_ROWS + value;
        col_element[1] += col*NUM_COLS + value;
        blk_element[1] += (row_block*SIZE_BLOCKS + col_block)*SIZE_BLOCKS*SIZE_BLOCKS + value;

        sparse_exact_cover_matrix.push_back(cel_element);
        sparse_exact_cover_matrix.push_back(row_element);
        sparse_exact_cover_matrix.push_back(col_element);
        sparse_exact_cover_matrix.push_back(blk_element);
        sparse_exact_cover_row_meaning.push_back(row_meaning);
    }

    void fillSparseExactCoverMatrix()
    {
        int sparse_exact_cover_matrix_row = 0;

        for (int i = 0; i < NUM_ROWS; i++)
            for (int j = 0; j < NUM_COLS; j++)
                if (sudoku[i][j])
                {
                    addSparseExactCoverMatrix(sparse_exact_cover_matrix_row, i, j, sudoku[i][j] - 1);
                    sparse_exact_cover_matrix_row += 1;
                }

                else
                {
                    for (int k = 0; k < NUM_COLS; k++)
                    {
                        int base_value = 1;

                        if (possible_values[i][j] & (base_value <<= k))
                            {
                                addSparseExactCoverMatrix(sparse_exact_cover_matrix_row, i, j, k);
                                sparse_exact_cover_matrix_row += 1;
                            }
                    }
                }
    }
};

