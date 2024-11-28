#include "../Headers/diff_functions.h"

/*
enum DiffInfo Run()
{
    //------------Initialize------------
    struct File_text origin_tree_input = {}; 
    struct File_text origin_tree_output = {};   
    struct File_text origin_graph_file = {};  
    struct Tree origin_tree = {};

    struct File_text diff_tree_input = {}; 
    struct File_text diff_tree_output = {};   
    struct File_text diff__graph_file = {};  
    struct Tree diff_tree = {};
    //----------------------------------

    // MakeTreeData( &graph_file, &tree_input, &oringin_tree );

    

    //-------------SAVING ORIGINAL TREE DATA--------------
    // TreeData( &my_tree, &tree_output );
    // -------------------------------------------

    //-----------------PRINTING------------------
    if(origin_tree.status == GOOD_TREE)
    {
        Output( &origin_graph_file, &origin_tree);
    }
    else 
    {
        printf(RED "Bad Tree :(\n" DELETE_COLOR);
    }
    //-------------------------------------------




    // Differentiate( );

    
    TreeDtor( &origin_tree );
    TreeDtor( &diff_tree );
}   

*/
enum DiffInfo Differentiate( struct Tree* origin_tree, struct Tree* diff_tree )
{
    diff_tree->root = MakeDifferentiation( origin_tree->root, diff_tree );

    return GOOD_DIFF;
}


//----Watches on origin node and based on its conten, builds new tree( Doesn't touch origin tree!!! )--
struct Node_t* MakeDifferentiation( struct Node_t* origin_node, struct Tree* diff_tree )
{
    struct Node_t* ready_node = nullptr;
    union Data_t data = {};

    switch( (int)origin_node->type )
    {
        case NUM:
        {
            data.num = 0;
            CreateNode( diff_tree, data, &ready_node, NUM );
            break;      
        }
        case VAR:
        {   
            data.num = 1;
            CreateNode( diff_tree, data, &ready_node, NUM );
            break;
        }
        case OP:
        {
            struct Node_t* tmp_node = nullptr;

            switch( (int)origin_node->data.op )
            {
                case kAdd:
                {   
                    data.op = kAdd;
                    CreateNode( diff_tree, data, &ready_node, OP );

                    tmp_node = MakeDifferentiation( origin_node->left, diff_tree );
                    InsertLeave( diff_tree, ready_node, LEFT, tmp_node );

                    tmp_node = MakeDifferentiation( origin_node->right,  diff_tree );
                    InsertLeave( diff_tree, ready_node, RIGHT, tmp_node );
                    
                    break;
                }
                case kSub:
                {
                    data.op = kSub;
                    CreateNode( diff_tree, data, &ready_node, OP );

                    tmp_node = MakeDifferentiation( origin_node->left, diff_tree );
                    InsertLeave( diff_tree, ready_node, LEFT, tmp_node );

                    tmp_node = MakeDifferentiation( origin_node->right,  diff_tree );
                    InsertLeave( diff_tree, ready_node, RIGHT, tmp_node );

                    break;
                }   
                case kMul:
                {   
                    data.op = kAdd;
                    CreateNode( diff_tree, data, &ready_node, OP );

                    //-----------------------LEFT------------------------
                    data.op = kMul;
                    CreateNode( diff_tree, data, &tmp_node, OP );
                    InsertLeave( diff_tree, ready_node, LEFT, tmp_node );

                    tmp_node = MakeDifferentiation( origin_node->left, diff_tree );
                    InsertLeave( diff_tree, ready_node->left, LEFT, tmp_node );

                    InsertLeave( diff_tree, ready_node->left, RIGHT, origin_node->right );
                    //---------------------------------------------------

                    //----------------------RIGHT------------------------
                    data.op = kMul;
                    CreateNode( diff_tree, data, &tmp_node, OP );
                    InsertLeave( diff_tree, ready_node, RIGHT, tmp_node ); 

                    InsertLeave( diff_tree, ready_node->right, LEFT, origin_node->left );

                    tmp_node = MakeDifferentiation( origin_node->right, diff_tree );
                    InsertLeave( diff_tree, ready_node->right, RIGHT, tmp_node );
                    //---------------------------------------------------

                    break;
                }
                case kDiv:
                {
                    data.op = kDiv;
                    CreateNode( diff_tree, data, &ready_node, OP ); 

                    //--------------------UP LEFT------------------------
                    data.op = kSub;
                    CreateNode( diff_tree, data, &tmp_node, OP );
                    InsertLeave( diff_tree, ready_node, LEFT, tmp_node );
                    //---------------------------------------------------

                    //----------------------LEFT-------------------------
                    data.op = kMul;
                    CreateNode( diff_tree, data, &tmp_node, OP );
                    InsertLeave( diff_tree, ready_node->left, LEFT, tmp_node ); 

                    tmp_node = MakeDifferentiation( origin_node->left, diff_tree );
                    InsertLeave( diff_tree, ready_node->left->left, LEFT, tmp_node );

                    InsertLeave( diff_tree, ready_node->left->left, RIGHT, origin_node->right );
                    //---------------------------------------------------

                    //---------------------RIGHT-------------------------
                    data.op = kMul;
                    CreateNode( diff_tree, data, &tmp_node, OP );
                    InsertLeave( diff_tree, ready_node->left, RIGHT, tmp_node ); 

                    InsertLeave( diff_tree, ready_node->left->right, LEFT, origin_node->left );

                    tmp_node = MakeDifferentiation( origin_node->right, diff_tree );
                    InsertLeave( diff_tree, ready_node->left->right, RIGHT, tmp_node );
                    //---------------------------------------------------

                    //--------------------UP RIGHT-----------------------
                    data.op = kMul;
                    CreateNode( diff_tree, data, &tmp_node, OP );
                    InsertLeave( diff_tree, ready_node, RIGHT, tmp_node );

                    //-------------SQUARE OF DENOMINATOR-----------------
                    data.var = origin_node->left->data.var;
                    CreateNode( diff_tree, data, &tmp_node, VAR );
                    InsertLeave( diff_tree, ready_node->right, LEFT, tmp_node );
                    CreateNode( diff_tree, data, &tmp_node, VAR );
                    InsertLeave( diff_tree, ready_node->right, RIGHT, tmp_node );
                    //---------------------------------------------------

                    break;
                }
            }
        }
        default:
        {
            break;
        }
    }

    //финальный, возвращает указатель, который фактически указатель на diff_tree->root
    return ready_node;
}
