#include "../Headers/tree_functions.h"

enum TreeErrors TreeDtor( struct Tree* tree )
{
    assert( tree );

    FreeTree( tree, tree->root );

    return GOOD_DTOR;
}


//=========================== CREATE NODE =====================================
struct Node_t* CreateNode( struct Tree* tree, Data_t data, enum Node_types type )
{
    assert( tree );

    struct Node_t* new_node = nullptr;

    switch( (int)type )
    {
        case NUM:
        {
            CreateNumNode( tree, data.num, &new_node );
            break;
        }
        case VAR:
        {
            CreateVarNode( tree, data.var, &new_node );
            break;  
        }
        case OP:
        {
            CreateOpNode( tree, data.op, &new_node );
            break;
        }
        default:
        {
            printf(RED "Error in node type\n" DELETE_COLOR);
            break;
        }
    }

    return new_node;
}   


enum TreeErrors CreateNumNode( struct Tree* tree, double number, struct Node_t** new_node )
{
    (*new_node) = (Node_t*)calloc( 1, sizeof( Node_t ) );
    (*new_node)->data.num = number;
    (*new_node)->parent = nullptr;
    (*new_node)->left = nullptr;
    (*new_node)->parent = nullptr;
    (*new_node)->type = NUM;

    if( tree->root == nullptr )
    {
        tree->root = (*new_node);
        //-------------------
        for(int i = 0; i < STRING_ARRAY_SIZE; i++)
        {
            strcpy( tree->variables[i].string, "empty" );
            tree->variables[i].links_amount = 0;
        }
        //-------------------
    }

    return GOOD_CREATE;
}   


enum TreeErrors CreateOpNode( struct Tree* tree, enum Operations operation, struct Node_t** new_node )
{
    (*new_node) = (Node_t*)calloc( 1, sizeof( Node_t ) );
    (*new_node)->data.op = operation;
    (*new_node)->parent = nullptr;
    (*new_node)->left = nullptr;
    (*new_node)->parent = nullptr;
    (*new_node)->type = OP;

    if( tree->root == nullptr )
    {
        tree->root = (*new_node);
        //-------------------
        for(int i = 0; i < STRING_ARRAY_SIZE; i++)
        {
            strcpy( tree->variables[i].string, "empty" );
            tree->variables[i].links_amount = 0;
        }
        //-------------------
    }

    return GOOD_CREATE;
}


enum TreeErrors CreateVarNode( struct Tree* tree, char* variable, struct Node_t** new_node )
{
    enum TreeErrors find_status = SAME_STRING_EXISTS; // just initialization
    int string_position = 0;
    find_status = FindString( tree, variable, &string_position );

    if( tree->root == nullptr ) //аналог TreeCtor
    {
        //-------------------
        for(int i = 0; i < STRING_ARRAY_SIZE; i++)
        {
            strcpy( tree->variables[i].string, "empty" );
            tree->variables[i].links_amount = 0;
        }
        //-------------------

        tree->root = (Node_t*)calloc( 1, sizeof( Node_t ) );
        (*new_node) = tree->root;
        (*new_node)->type = VAR;

        tree->variables[string_position].links_amount++; 
        strcpy( tree->variables[string_position].string, variable );
        (*new_node)->data.var = ( (tree->variables + string_position)->string );

        (*new_node)->left = nullptr;
        (*new_node)->right = nullptr;

            ON_DEBUG( printf(RED "created root \n" DELETE_COLOR); )
        return GOOD_CREATE;
    }   
    else 
    {
        if( find_status == SAME_STRING_EXISTS )
        {
            *new_node = (Node_t*)calloc( 1, sizeof( Node_t ) );
            (*new_node)->type = VAR;
            tree->variables[string_position].links_amount++;

            (*new_node)->data.var = ( (tree->variables + string_position)->string );
            (*new_node)->left = nullptr;
            (*new_node)->right = nullptr;
                ON_DEBUG( printf(RED "created with same string\n" DELETE_COLOR); )
            return GOOD_CREATE;
        }
        else 
        {
            if( find_status == FOUND_EMPTY_STRING )
            {
                *new_node = (Node_t*)calloc( 1, sizeof( Node_t ) );
                (*new_node)->type = VAR;
                strcpy( tree->variables[string_position].string, variable );
                tree->variables[string_position].links_amount++;

                (*new_node)->data.var = (tree->variables + string_position)->string;
                (*new_node)->left = nullptr;
                (*new_node)->right = nullptr;
                    ON_DEBUG( printf(RED "created with a new string\n" DELETE_COLOR); )

                return GOOD_CREATE;
            }
            else
            {
                    ON_DEBUG( printf(RED "don't have free space in memory for a new string\n" DELETE_COLOR); )
                return BAD_CREATE;
            }
        }
    }
}   
//=============================================================================

//---------- Tree* tree --- is where to add copied node ------------
struct Node_t* CopyNode( struct Tree* tree, struct Node_t* node_to_copy )
{
    assert( tree );
    assert( node_to_copy );

    struct Node_t* answer = nullptr;

    union Data_t data = {};
    switch( (int)node_to_copy->type )
    {
        case NUM:
        {
            data.num = node_to_copy->data.num;
            answer = CreateNode( tree, data, NUM );
            break;
        }
        case VAR:
        {
            data.var = node_to_copy->data.var;
            answer = CreateNode( tree, data,  VAR );
            break;
        }
        case OP:
        {
            data.op = node_to_copy->data.op;
            answer = CreateNode( tree, data, OP );
            break;
        }
    }  

    return answer;
}


//---------------------------TREE STRINGS FUNCTIONS----------------------------
enum TreeErrors FindString( struct Tree* tree, char* to_find, int* string_position )
{
    *string_position = 0;

    ON_DEBUG( StringDump( tree ); )

    if( FindSameString( tree, to_find, string_position ) == SAME_STRING_EXISTS )
    {
        ON_DEBUG( printf(YELLOW "found same string\n" DELETE_COLOR); )
        return SAME_STRING_EXISTS;
    }
    else if( FindEmptyString( tree, string_position ) == FOUND_EMPTY_STRING ) 
    {   
        ON_DEBUG( printf(YELLOW "found empty string\n" DELETE_COLOR); )
        return FOUND_EMPTY_STRING;
    } 
    else 
    {
        ON_DEBUG( printf(YELLOW "not found empty string\n" DELETE_COLOR); )
        return NOT_FOUND_EMPTY_STRING;
    }
}


enum TreeErrors FindSameString( struct Tree* tree, char* to_find, int* string_position )
{             
    *string_position = 0;
    for(int i = 0; i < STRING_ARRAY_SIZE; i++)
    {
        if( strcmp( tree->variables[i].string, to_find) == 0 )
        {
            *string_position = i;
            return SAME_STRING_EXISTS;
        }
        else 
        {
            continue;
        }
    }

    return SAME_STRING_NOT_EXISTS;
}


enum TreeErrors FindEmptyString( struct Tree* tree, int* string_position )
{
    *string_position = 0;

    for( int i = 0; i < STRING_ARRAY_SIZE; i++)
    {
        if( strcmp( tree->variables[i].string, "empty" ) == 0 )
        {   
            *string_position = i;
            return FOUND_EMPTY_STRING;
        }
        else 
        {
            continue;
        }
    }

    return NOT_FOUND_EMPTY_STRING;
}   


enum TreeErrors DeleteString( struct Tree* tree, char* string )
{   
    int string_position = 0;

    if( (FindSameString( tree, string, &string_position) ) == SAME_STRING_EXISTS )
    {
        tree->variables[string_position].links_amount--;
        if( tree->variables[string_position].links_amount <= 0 )
        {
            strcpy( tree->variables[string_position].string, "empty" );
        }
        return GOOD_STRING_DELETE;
    }   
    else 
    {
        return BAD_STRING_DELETE;
    }
}


void StringDump( struct Tree* tree )
{
    ON_DEBUG( printf(YELLOW "======Start of StringDump======\n" DELETE_COLOR); )
    for(int i = 0; i < 10; i++)
    {
        printf("%s  links: %d\n", tree->variables[i].string, tree->variables[i].links_amount);
    }
    ON_DEBUG( printf(YELLOW "======End of StringDump======\n" DELETE_COLOR); )
}
//-----------------------------------------------------------------------------

enum TreeErrors InsertLeave( struct Tree* tree, struct Node_t* parent, enum Direction branch, struct Node_t* to_connect )
{   
    assert( to_connect );

    if( parent != nullptr )
    {
        if( branch == LEFT )
        {
            if(parent->left == nullptr)
            {
                parent->left = to_connect;
                to_connect->parent = parent;

                return GOOD_INSERT;
            } 
            else 
            {
                printf(RED "your parent node have leaves\n"
                "So call InsertNode function instead of this function\n"
                "And your node deleted for programm to not catch seg-fault\n"
                "So create it again\n" DELETE_COLOR);

                free(to_connect);

                return BAD_INSERT;
            }
        }   
        else if( branch == RIGHT )
        {
            if( parent->right == nullptr)
            {
                parent->right = to_connect;
                to_connect->parent = parent;
            }
            else 
            {
                printf(RED "your parent node have leaves\n"
                "So call InsertNode function instead of this function\n"
                "And your node deleted for programm to not catch seg-fault\n"
                "So create it again\n" DELETE_COLOR);
                
                free(to_connect);

                return BAD_INSERT;
            }

            return GOOD_INSERT;
        }
    }
    else //случай, когда вставляем корень 
    {
        tree->root = to_connect;
        to_connect->parent = nullptr;
        ON_DEBUG( printf(RED "root inserted in beginning of tree\n" DELETE_COLOR); )
    }

    return GOOD_INSERT;
}

//insert already existing node
enum TreeErrors InsertNode( struct Node_t* left, struct Node_t* right, struct Node_t* node )
{
    if( (left != nullptr) && (right != nullptr) )
    {
        ON_DEBUG( printf(SINIY "inserting in the middle of tree\n" DELETE_COLOR); )

        if( left->parent == right->parent )
        {
            left->parent->left = node;
            left->parent->right = nullptr; // всегда освобождаем правый элемент при вставке

            node->parent = left->parent;

            left->parent = node;
            right->parent = node;

            node->left = left;
            node->right = right;

            return GOOD_INSERT;
        }
        else
        {
            // tree->status = BAD_TREE;
            ON_DEBUG( printf(YELLOW "Pointers don't connect\n" DELETE_COLOR); )
        }   
    }
    if( (left == nullptr) && (right == nullptr) ) 
    {
        printf(RED "Two null pointers pasted -- means yout parent node have no leaves.\n"
        "free is used to avoid memory leaks\n"
        "Call InsertLeave To create leaves\n" DELETE_COLOR);

        free( node );

        return GOOD_INSERT;
    }
    if( (left != nullptr) && (right == nullptr) )
    {
        ON_DEBUG( printf(SINIY "insert, left exists, right doesn't\n" DELETE_COLOR); )
        if( left->parent->left == left)
        {
            left->parent->left = node;
        }
        else if( left->parent->right == left)
        {
            left->parent->right = node;
        }

        node->parent = left->parent;

        left->parent = node;

        node->left = left;


        return GOOD_INSERT;
    }
    if( (left == nullptr) && (right != nullptr) )
    {
        ON_DEBUG( printf(SINIY "insert, right exists, left doesn't\n" DELETE_COLOR); )

        if( right->parent->left == right)
        {
            right->parent->left = node;
        }
        else if( right->parent->right == right)
        {
            right->parent->right = node;
        }

        node->parent = right->parent;

        right->parent = node;

        node->right = right;

        return GOOD_INSERT;
    }

    // tree->status = BAD_TREE;
    return BAD_INSERT;  
}


void FreeTree( struct Tree* tree, struct Node_t* node )
{   
    assert(tree);
    assert(node);
    
    struct Node_t* left = node->left;
    struct Node_t* right = node->right;

    if( left != nullptr)
    {
        FreeTree(tree, left );
    }

    if( right != nullptr )
    {
        FreeTree(tree, right);
    }

    if( node->type == VAR )
    {
        DeleteString( tree, node->data.var );
    }

    free( node );
    return;
}


enum TreeErrors BranchDelete( struct Tree* tree, struct Node_t* node, enum Node_types node_type ) 
{
    if( node )
    {
        if( (node->left == nullptr) && (node->right == nullptr) )
        {
            ON_DEBUG( printf(SINIY "deleting \"leave\" node\n" DELETE_COLOR); )

            struct Node_t* tmp_parent = node->parent;

            if( tmp_parent->left == node )
            {
                tmp_parent->left = nullptr;
                
                if( node_type == VAR )
                    DeleteString( tree, node->data.var );

                free(node);
            }
            else if( tmp_parent->right == node )
            {
                tmp_parent->right = nullptr;

                if( node_type == VAR )
                    DeleteString( tree, node->data.var );

                free(node);
            }
        }
        else if( (node->left != nullptr) && (node->right != nullptr) )
        {
            ON_DEBUG( printf(SINIY "deleting node with both branches\n( causes deletion of all nodes under )\n" DELETE_COLOR); )
 
            struct Node_t* tmp_parent = node->parent;

            if( tmp_parent->left == node )
            {
                tmp_parent->left = nullptr;

                if( node_type == VAR )
                    DeleteString( tree, node->data.var );
            }
            else if( tmp_parent->right == node )
            {
                tmp_parent->right = nullptr;

                if( node_type == VAR )
                    DeleteString( tree, node->data.var );
            }
            
            FreeTree( tree, node );

            return GOOD_EXTRACT;
        }
        else if( (node->left != nullptr) && (node->right == nullptr) ) 
        {
            ON_DEBUG( printf(SINIY "deleting node with only left branch\n" DELETE_COLOR); )

            if( node->parent->left == node )
            {
                node->parent->left = nullptr;

                if( node_type == VAR )
                    DeleteString( tree, node->data.var );
            }
            else if(  node->parent->right == node )
            {
                node->parent->right = nullptr;

                if( node_type == VAR )
                    DeleteString( tree, node->data.var );
            }

            FreeTree( tree, node );

            return GOOD_EXTRACT;
        }
        else if( (node->left == nullptr) && (node->right != nullptr) )
        {
            ON_DEBUG( printf(SINIY "extracting node with only right branch\n" DELETE_COLOR); )

            if( node->parent->left == node )
            {
                node->parent->left = nullptr;

                if( node_type == VAR )
                    DeleteString( tree, node->data.var );
            }
            else if(  node->parent->right == node )
            {
                node->parent->right = nullptr;

                if( node_type == VAR )
                    DeleteString( tree, node->data.var );
            }

            FreeTree( tree, node );

            return GOOD_EXTRACT;    
        }
    }
    else 
    {   
        printf(RED "wrong node pointer pasted to extract");
        return BAD_EXTRACT;
    }

    return BAD_EXTRACT;
}


enum TreeErrors Find( struct Tree* tree, union Data_t to_find, struct Node_t** answer, enum Node_types node_type )
{
    ON_DEBUG( printf("need to find: %s\n", to_find); )

    switch( (int)node_type )
    {
        case NUM:
        {
            FindNumNode( tree->root, to_find.num, answer );
            break;
        }
        case VAR:
        {
            FindVarNode( tree->root, to_find.var, answer );
            break;
        }
        case OP:
        {
            FindOpNode( tree->root, to_find.op, answer );
            break;
        }
        default:
        {
            printf(RED "Smth wrong in node_type in FindNode\n" DELETE_COLOR);
            break;
        }
    }

    return GOOD_FIND;
}


enum TreeErrors FindNumNode( struct Node_t* node_search, double to_find, struct Node_t** answer )
{
    struct Node_t* left_search = node_search->left;
    struct Node_t* right_search = node_search->right;

    if( fabs( node_search->data.num - to_find ) > EPSILON )
    {
        if( left_search != nullptr )
        {
            FindNumNode( left_search, to_find, answer );
        }

        if( right_search != nullptr )
        {
            FindNumNode( right_search, to_find, answer );
        }
        return GOOD_FIND;
    }
    else 
    {
        *answer = node_search;
        return GOOD_FIND;
    }

    printf(YELLOW "Haven't found rhis number\n" DELETE_COLOR);
    return BAD_FIND;
}


enum TreeErrors FindOpNode( struct Node_t* node_search, enum Operations to_find, struct Node_t** answer )
{
    struct Node_t* left_search = node_search->left;
    struct Node_t* right_search = node_search->right;


    if( (int)node_search->data.op != (int)to_find )
    {
        if( left_search != nullptr )
        {
            FindOpNode( left_search, to_find, answer );
        }

        if( right_search != nullptr )
        {
            FindOpNode( right_search, to_find, answer );
        }
        return GOOD_FIND;
    }
    else 
    {
        printf("\n%d %d\n\n", node_search->data.op, to_find );
        *answer = node_search;
        return GOOD_FIND;
    }

    printf(YELLOW "Haven't found this operation\n" DELETE_COLOR);
    return BAD_FIND;
}


enum TreeErrors FindVarNode( struct Node_t* node_search, char* to_find, struct Node_t** answer )
{
    assert( node_search );
    assert( to_find );
    assert( answer );
    
        ON_DEBUG( printf(YELLOW "====== Start of FindNode ======\n" DELETE_COLOR); )
    struct Node_t* left_search = node_search->left;
        ON_DEBUG( printf(PURPLE "    left: %p\n" DELETE_COLOR, node_search->left); )
        ON_DEBUG( printf(PURPLE "    right: %p\n" DELETE_COLOR, node_search->right); )
    struct Node_t* right_search = node_search->right;

    if( strcmp( node_search->data.var, to_find ) != 0 )
    {
        if( left_search != nullptr)
        {
            ON_DEBUG( printf(PURPLE "        going left\n" DELETE_COLOR); )
            FindVarNode( left_search, to_find, answer);
        }

        if( right_search != nullptr )
        {   
            ON_DEBUG( printf(PURPLE "        going right\n" DELETE_COLOR); )
            FindVarNode( right_search, to_find, answer );
        }
        return GOOD_FIND;
    }
    else 
    {
        ON_DEBUG( printf("node: %p, data: %s\n", node_search, node_search->data); )
        *answer = node_search;
            ON_DEBUG( printf(YELLOW "====== End of FindNode ======\n" DELETE_COLOR); )
        return GOOD_FIND;
    }

    printf(YELLOW "Haven't found this variable\n" DELETE_COLOR);
    return BAD_FIND;
}


struct Node_t* CopyBranch( struct Tree* tree, struct Node_t* to_copy )
{
    struct Node_t* tmp_node = nullptr;

    tmp_node = CopyNode( tree, to_copy );
    
    if( to_copy->left != nullptr )
    {
        tmp_node->left = CopyBranch( tree, to_copy->left );
    }

    if( to_copy->right != nullptr )
    {
        tmp_node->right = CopyBranch( tree, to_copy->right );
    }

    return tmp_node;
}


enum TreeErrors ReplaceNode( struct Node_t* to_replace, struct Node_t* src )
{
    assert( to_replace );
    assert( src );

    /* //TODO: доделать кагда исправлю CreateNode()
    if( to_replace->parent->left == to_replace )
    {
        to_replace->parent->left = src;
    }
    else if( to_replace->parent->right == to_replace )
    {
        to_replace->parent->right = src;    
    }
    else
    {
        printf(RED "unknown problem in %s in %s on line %d " DELETE_COLOR, __FILE__, __PRETTY_FUNCTION__, __LINE__ );
        exit(0);
    }
    */
    src->parent = to_replace->parent;
    src->left = to_replace->left;
    src->right = to_replace->right;

    if( to_replace->left != nullptr )
    {
        to_replace->left->parent = src;
    }

    if( to_replace->right != nullptr )
    {
        to_replace->right->parent = src;
    }

    //Deletion of node
    free( to_replace );

    return GOOD_INSERT;
}
    