#include <algorithm>
#include <iostream>
#include <cstring>
#include <vector>
#include <memory>
#include <unordered_set>

using namespace std;

#define NUM_COLS 7
#define FLAG -1

// -----------------------------------------------------------

struct node
{
    int value = 0;

    node *left  = nullptr;
    node *right = nullptr;
    node *up    = nullptr;
    node *down  = nullptr;
    node *collumn_header = nullptr;
};

// -----------------------------------------------------------

class createToroidalLinkedList
{
    public: 

    node *head = nullptr;
    vector<int> solution;
    vector<vector<int>> solution_found;
    vector<vector<int>> sparse_cover_matrix;

    // -------------------------------------------------------
    // the sparse matrix needs a defined order, always the rows first
    // initializer

    createToroidalLinkedList(vector<vector<int>> (&cover_matrix_original))

    {
        sparse_cover_matrix = cover_matrix_original;
        fillLinkedList();
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

        for (int i = 0; i < NUM_COLS; i++)
        {
            node *new_node = new node();

            new_node->collumn_header = new_node;

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
        // ---------------------------------------------------
        //

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
            new_node->collumn_header = connect_col;
            new_node->collumn_header->value += 1;

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

    void fillLinkedList()
    {   
        addHead();
        addNodeColumnHeader();
        addNodes();
    }

    void deallocateStructure()
    {
        if (head == nullptr)
            return;

        node* current_collumn = head;

        while (current_collumn->right != head)
        {
            current_collumn = current_collumn->right;
            node* current_col = current_collumn;

            while (current_col->down != current_collumn)
            {
                node *temp = current_col;
                current_col = current_col->down;
                delete temp;
            }

            delete current_col;
        }

        delete head;
    }

    void printSolutions()
    {
        for (int i = 0; i < solution_found.size(); i++)
        {
            for (int j = 0; j < solution_found[i].size(); j++)
                cout << solution_found[i][j] << " ";

            cout << "\n";
        }
    }

    // -------------------------------------------------------
    // DLX Helping Methods

    void cover(node *target)
    {
        node *current_row = target->collumn_header;
        node *col_header  = target->collumn_header;

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

                current_col->collumn_header->value -= 1;
            }
        }
    }

    void uncover(node *target)
    {
        node *current_row = target->collumn_header;
        node *col_header  = target->collumn_header;
        
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

                current_col->collumn_header->value += 1;
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
        node *collumn;

        if (head->right == head)
        {
            solution_found.push_back(solution);
            return;
        }
        
        collumn = getMinCollumn();
        row_node = collumn;
        cover(collumn);
        
        while (row_node->down != collumn)
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

        uncover(collumn);
    }

    void DLX()
    {
        dancingLinksAlgorithmX();
        deallocateStructure();
    }
};

// -----------------------------------------------------------

int main() 
{
    vector<vector<int>> cover_matrix;

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
    
    createToroidalLinkedList toroidalLinkedList(cover_matrix);
    toroidalLinkedList.DLX();
    toroidalLinkedList.printSolutions();

    return 0;
}