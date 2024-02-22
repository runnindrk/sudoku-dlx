#include <iostream>
#include <vector>

// -----------------------------------------------------------

struct node
{
    /*
    Node structure.

    Atributes
    ---------

    value
        Information about the node.
    left
        Pointer to the left node.
    right
        Pointer to the right node.
    up
        Pointer to the node above.
    down
        Pointer to the node below.
    column_header
        Pointer to the column header where the node belongs.
    */
    
    int value = 0;

    node *left  = nullptr;
    node *right = nullptr;
    node *up    = nullptr;
    node *down  = nullptr;
    node *column_header  = nullptr;
};

// -----------------------------------------------------------

class CreateToroidalLinkedList
{
    /*
    This creates the doubly linked 2D list necessary for a DLX algorithm given the sparse matrix of the problem.
    It is of extreme importance that the sparse matrix has row first defined order, 
    otherwise, it will output wrong solutions. 

    Atributes
    ---------

    head
        Pointer of the head of the 2D Linked List.
    sparse_cover_matrix
        Exact cover matrix in sparse matrix format of the problem.
    num_columns
        Number of columns of the exact cover matrix.
    FLAG
        Just a flag variable to ensure the correct logic when creating the 2D Linked List.
    

    Methods
    ---------

    deallocateStructure()
        Deletes from memory the 2D Linked List.
    getNumProblemColumns()
        Find the number of columns of the exact cover matrix provided.
    addHead()
        Creates the head of the 2D LL.
    addNodeColumnHeader();
        Creates the column head of the 2D LL specific to the DLX algorithm.
    addNodes()
        Creates the nodes of the exact cover matrix.
    */
    
    public: 

    node *head = nullptr;
    std::vector<std::vector<int>> sparse_cover_matrix;

    CreateToroidalLinkedList(std::vector<std::vector<int>> (&cover_matrix_original))

    {
        sparse_cover_matrix = cover_matrix_original;
        getNumProblemColumns();
        addHead();
        addNodeColumnHeader();
        addNodes();
    }

    ~CreateToroidalLinkedList()

    {
        deallocateStructure();
    }

    private:

    int num_columns = 0;
    const int FLAG = -1;

    // -------------------------------------------------------

    void deallocateStructure()
    {
        /*
        Removes the structure from the memory.

        Returns
        ---------
        None
        */
        
        if (head == nullptr)
            return;

        node* current_column = head;
        node* temp;
        
        while (head->right != head)
        {
            current_column = head->right;
            node* current_col = current_column;

            while (current_column->down != current_column)
            {
                current_col = current_column->down;
                temp = current_col;

                current_col->up->down = current_col->down;
                current_col->down->up = current_col->up;

                delete temp;
            }

            temp = current_column;
            
            current_column->left->right = current_column->right;
            current_column->right->left = current_column->left;

            delete temp;
        }

        delete head;
    }
        
    // -------------------------------------------------------

    void getNumProblemColumns()
    {
        /*
        Finds the number of columns from the sparse exact cover matrix.

        Returns
        ---------
        None
        */

        for (int i = 0; i < sparse_cover_matrix.size(); i++)
            if (sparse_cover_matrix[i][1] > num_columns)
                num_columns = sparse_cover_matrix[i][1];

        num_columns += 1;
    }

    // -------------------------------------------------------

    void addHead()
    {
        /*
        Adds the head of the 2D LL.

        Returns
        ---------
        None
        */

        node *new_node = new node();

        head     = new_node;
        head->right  = head;
        head->left   = head;
        head->up     = head;
        head->down   = head;
    }

    void addNodeColumnHeader()
    {
        /*
        Adds the columns heads of the 2D LL specific to the exact cover problem.

        Returns
        ---------
        None
        */

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
        /*
        Adds the nodes to the 2D LL from the sparse exact cover matrix.

        Returns
        ---------
        None
        */

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

class ExactCoverDancingLinks
{
    /*
    This solves the exact cover problem using the DLX algorithm. 
    The 2D LL is inherited from the CreateToroidalLinkedList.

    Atributes
    ---------

    solutions_found
        Vector contanting all the solutions for the exact cover problem given.
    solution
        Vector contanting only one, and current, solution found.
    

    Methods
    ---------

    cover()
        Removes an entire column and consecuent rows.
    uncover()
        Undoes an entire column and consecuent rows.
    getMinCollumn()
        Find the column with the least number of set rows.
    dancingLinksAlgorithmX()
        Performs the DLX algorithm to solve the exact cover problem.
    */
    
    public:

    int number_backtrack_calls = 0;
    int number_guesses = 0;
    std::vector<std::vector<int>> solutions_found;

    ExactCoverDancingLinks(std::vector<std::vector<int>> (&cover_matrix_original))
    
    {
        CreateToroidalLinkedList LinkedList(cover_matrix_original);
        head = LinkedList.head;
        dancingLinksAlgorithmX();
    }

    private:

    node *head = nullptr;
    std::vector<int> solution;

    // -------------------------------------------------------

    void cover(node *target)
    {
        /*
        Cover an entire column and subsequent rows.

        Parameters
        ---------
        target
            pointer of the node to be removed.

        Returns
        ---------
        None
        */

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
        /*
        Uncovers an entire column and subsequent rows.

        Parameters
        ---------
        target
            pointer of the node to be reinstated.

        Returns
        ---------
        None
        */

        node *current_row = target->column_header;
        node *col_header  = target->column_header;
        
        while (current_row->up != col_header)
        {
            current_row = current_row->up;
            node *current_col = current_row;

            while (current_col->left != current_row)
            {
                current_col = current_col->left;
                
                current_col->up->down = current_col;
                current_col->down->up = current_col;

                current_col->column_header->value += 1;
            }
        }

        col_header->left->right = col_header;
        col_header->right->left = col_header;
    }

    node *getMinColumn()
    {
        /*
        Finds the column with the least number of node of the problem..

        Returns
        ---------
        node
        */

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

    void dancingLinksAlgorithmX(int depth = 0)
    {
        /*
        Performs the DLX algorithm.

        Returns
        ---------
        None
        */

        node *row_node;
        node *right_node;
        node *left_node;
        node *column;

        if (head->right == head)
        {
            solutions_found.push_back(solution);
            return;
        }
        
        column = getMinColumn();
        row_node = column;
        cover(column);
        
        number_backtrack_calls += 1;
        (column->value > 1) ? number_guesses += 1 : 1;
        
        while (row_node->down != column)
        {
            row_node = row_node->down;
            right_node = row_node;
            left_node = row_node;

            while (right_node->right != row_node)
            {
                right_node = right_node->right;
                cover(right_node);
            }

            solution.push_back(row_node->value);

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
