#include <iostream>
#include <vector>

// -----------------------------------------------------------

struct node
{
    int value = 0;

    node *left  = nullptr;
    node *right = nullptr;
    node *up    = nullptr;
    node *down  = nullptr;
    node *column_header = nullptr;
};

// -----------------------------------------------------------

class createToroidalLinkedList
{
    public: 

    node *head = nullptr;
    std::vector<std::vector<int>> sparse_cover_matrix;

    // -------------------------------------------------------
    // The sparse matrix needs a defined order, always the rows first
    // Initializer

    createToroidalLinkedList(std::vector<std::vector<int>> (&cover_matrix_original))

    {
        sparse_cover_matrix = cover_matrix_original;
        getNumProblemColumns();
        addHead();
        addNodeColumnHeader();
        addNodes();
    }

    ~createToroidalLinkedList()

    {
        deallocateStructure();
    }

    private:

    int num_columns = 0;
    const int FLAG = -1;

    // -------------------------------------------------------
    // Remove the structure from the memory

    void deallocateStructure()
    {
        if (head == nullptr)
            return;

        node* current_column = head;

        while (current_column->right != head)
        {
            current_column = current_column->right;
            node* current_col = current_column;

            while (current_col->down != current_column)
            {
                node *temp = current_col;
                current_col = current_col->down;
                delete temp;
            }

            delete current_col;
        }

        delete head;
    }
        
    // -------------------------------------------------------
    // Find the number of columns of the problem

    void getNumProblemColumns()
    {
        for (int i = 0; i < sparse_cover_matrix.size(); i++)
            if (sparse_cover_matrix[i][1] > num_columns)
                num_columns = sparse_cover_matrix[i][1];

        num_columns += 1;
    }

    // -------------------------------------------------------
    // Toroidal Linked List Creation

    void addHead()
    {
        node *new_node = new node();

        head     = new_node;
        head->right  = head;
        head->left   = head;
        head->up     = head;
        head->down   = head;
    }

    void addNodeColumnHeader()
    {
        node *current = head;

        for (int i = 0; i < num_columns; i++)
        {
            node *new_node = new node();

            new_node->column_header = new_node;

            new_node->left        = current;
            new_node->right          = head;
            current->right->left = new_node;
            current->right       = new_node;

            new_node->up         = new_node;
            new_node->down       = new_node;

            current = current->right;
        }
    }

    void addNodes()
    {   
        node *connect_col    = head;
        node *connect_row    = head;
        node *loop_row       = head;
        int change_row       = FLAG;

        for (int i = 0; i < sparse_cover_matrix.size(); i++)
        {
            node *new_node     = new node();
            node *current            = head;

            int row_index = sparse_cover_matrix[i][0];
            int col_index = sparse_cover_matrix[i][1];
            new_node->value = row_index;

            for (int j = 0; j <= col_index; j++)
                current = current->right;
            
            connect_col = current;
            new_node->column_header = connect_col;
            new_node->column_header->value += 1;

            while (current->down != connect_col)
                current = current->down;

            new_node->up            = current;
            new_node->down      = connect_col;
            current->down->up      = new_node;
            current->down          = new_node;

            if (change_row != row_index)
            {
                connect_row = new_node;
                loop_row = new_node;
                change_row = row_index;
            }

            new_node->left        = connect_row;
            new_node->right          = loop_row;
            connect_row->right->left = new_node;
            connect_row->right       = new_node;

            connect_row = connect_row->right;
        }
    }
};

// -----------------------------------------------------------

class exactCoverDancingLinks : createToroidalLinkedList
{
    public:

    std::vector<std::vector<int>> solution_found;

    exactCoverDancingLinks(std::vector<std::vector<int>> (&cover_matrix_original)) : createToroidalLinkedList(cover_matrix_original)
    
    {
        dancingLinksAlgorithmX();
    }

    private:

    std::vector<int> solution;

    // -------------------------------------------------------
    // DLX methods

    void cover(node *target)
    {
        node *current_row = target->column_header;
        node *col_header  = target->column_header;

        col_header->left->right = col_header->right;
        col_header->right->left = col_header->left;
        
        while (current_row->down != col_header)
        {
            current_row = current_row->down;
            node *current_col = current_row;

            while (current_col->right != current_row)
            {
                current_col = current_col->right;
                
                current_col->up->down = current_col->down;
                current_col->down->up = current_col->up;

                current_col->column_header->value -= 1;
            }
        }
    }

    void uncover(node *target)
    {
        node *current_row = target->column_header;
        node *col_header  = target->column_header;
        
        col_header->left->right = col_header;
        col_header->right->left = col_header;
        
        while (current_row->down != col_header)
        {
            current_row = current_row->down;
            node *current_col = current_row;

            while (current_col->right != current_row)
            {
                current_col = current_col->right;
                
                current_col->up->down = current_col;
                current_col->down->up = current_col;

                current_col->column_header->value += 1;
            }
        }
    }

    node *getMinCollumn()
    {
        node *start   = head;
        node *min_col = head->right;

        while (start->right != head)
        {
            start =  start->right;

            if (start->value < min_col->value)
                min_col = start;
        }

        return min_col;
    }

    // -------------------------------------------------------
    // DLX

    void dancingLinksAlgorithmX(int depth = 0)
    {
        node *row_node;
        node *right_node;
        node *left_node;
        node *column;

        if (head->right == head)
        {
            solution_found.push_back(solution);
            return;
        }
        
        column = getMinCollumn();
        row_node = column;
        cover(column);
        
        while (row_node->down != column)
        {
            row_node = row_node->down;
            right_node = row_node;
            left_node = row_node;

            solution.push_back(row_node->value);

            while (right_node->right != row_node)
            {
                right_node = right_node->right;
                cover(right_node);
            }

            dancingLinksAlgorithmX(depth + 1);
            solution.pop_back();

            while (left_node->left != row_node)
            {
                left_node = left_node->left;
                uncover(left_node);
            }
        }

        uncover(column);
    }
};

// -----------------------------------------------------------

int main() 
{
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
    
    exactCoverDancingLinks solve(cover_matrix);
    solutions = solve.solution_found;

    for (int i = 0; i < solutions.size(); i++)
    {
        for (int j = 0; j < solutions[i].size(); j++)
            std::cout << solutions[i][j] << " ";

        std::cout << "\n";
    }

    return 0;
}