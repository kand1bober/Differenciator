#include "../Headers/diff_functions.h"

enum DiffInfo Run()
{
    // ------------Initialize------------
    struct File_text tree_input = {}; //for input
    struct File_text tree_output = {};   // for output
    struct File_text graph_file = {};  //for inpit
    struct Tree my_tree = {};

    struct File_text diff_tree_input = {}; 
    struct File_text diff_tree_output = {};   
    struct File_text diff_graph_file = {};  
    struct Tree diff_tree = {};

    MakeTreeData( &graph_file, &tree_input, &my_tree );

    Differentiate( &my_tree, &diff_tree );


    //----( probably not needed in differenciator )------
    // TreeData( &my_tree, &tree_output );
    //---------------------------------------------------

    //-----------------PRINTING------------------
    if(my_tree.status == GOOD_TREE)
    {
        // Output( &graph_file, &my_tree);
        Output( &diff_graph_file, &diff_tree);
    }
    else 
    {
        printf(RED "Bad Tree :(\n" DELETE_COLOR);
    }
    //-------------------------------------------
    
    TreeDtor( &my_tree );
    TreeDtor( &diff_tree );

    return GOOD_DIFF;
}


enum DiffInfo Differentiate( struct Tree* origin_tree, struct Tree* diff_tree )
{
    diff_tree->root = MakeDifferentiation( origin_tree->root, diff_tree );

    return GOOD_DIFF;
}

//----Watches on origin node and based on its conten, builds new tree( Doesn't touch origin tree!!! )--
struct Node_t* MakeDifferentiation( struct Node_t* origin_node, struct Tree* diff_tree )
{
    struct Node_t* ready_node = nullptr; //создаётся на каждой стадии рекурсии и возвращается в конце
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

                    CopyNode( diff_tree, origin_node->right, &tmp_node );
                    InsertLeave( diff_tree, ready_node->left, RIGHT, tmp_node );
                    //---------------------------------------------------

                    //----------------------RIGHT------------------------
                    data.op = kMul;
                    CreateNode( diff_tree, data, &tmp_node, OP );
                    InsertLeave( diff_tree, ready_node, RIGHT, tmp_node ); 

                    CopyNode( diff_tree, origin_node->left, &tmp_node );
                    InsertLeave( diff_tree, ready_node->right, LEFT, tmp_node );

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

                    CopyNode( diff_tree, origin_node->right, &tmp_node );
                    InsertLeave( diff_tree, ready_node->left->left, RIGHT, tmp_node );
                    //---------------------------------------------------

                    //---------------------RIGHT-------------------------
                    data.op = kMul;
                    CreateNode( diff_tree, data, &tmp_node, OP );
                    InsertLeave( diff_tree, ready_node->left, RIGHT, tmp_node ); 

                    CopyNode( diff_tree, origin_node->left, &tmp_node );
                    InsertLeave( diff_tree, ready_node->left->right, LEFT, tmp_node );

                    tmp_node = MakeDifferentiation( origin_node->right, diff_tree );
                    InsertLeave( diff_tree, ready_node->left->right, RIGHT, tmp_node );
                    //---------------------------------------------------

                    //--------------------UP RIGHT-----------------------
                    data.op = kMul;
                    CreateNode( diff_tree, data, &tmp_node, OP );
                    InsertLeave( diff_tree, ready_node, RIGHT, tmp_node );

                    //-------------SQUARE OF DENOMINATOR-----------------
                    CopyNode( diff_tree, origin_node->right, &tmp_node );
                    InsertLeave( diff_tree, ready_node->right, LEFT, tmp_node );

                    CopyNode( diff_tree, origin_node->right, &tmp_node );
                    InsertLeave( diff_tree, ready_node->right, RIGHT, tmp_node );
                    //---------------------------------------------------

                    break;
                }

                case kDeg:
                {
                    data.op = kMul;
                    CreateNode( diff_tree, data, &ready_node, OP );


                    //--------------------------LEFT---------------------------
                    CopyNode( diff_tree, origin_node->right, &tmp_node );
                    InsertLeave( diff_tree, ready_node, LEFT, tmp_node );
                    
                    //-------------------------RIGHT---------------------------

                    data.op = kMul;
                    CreateNode( diff_tree, data, &tmp_node, OP );
                    InsertLeave( diff_tree, ready_node, RIGHT, tmp_node );

                    tmp_node = MakeDifferentiation( origin_node->left, diff_tree );
                    InsertLeave( diff_tree, ready_node->right, RIGHT, tmp_node );

                    data.op = kDeg;
                    CreateNode( diff_tree, data, &tmp_node, OP );
                    InsertLeave( diff_tree, ready_node->right, LEFT, tmp_node );

                    CopyNode( diff_tree, origin_node->left, &tmp_node );
                    InsertLeave( diff_tree, ready_node->right->left, LEFT, tmp_node );

                    data.op = kSub;
                    CreateNode( diff_tree, data, &tmp_node, OP );
                    InsertLeave( diff_tree, ready_node->right->left, RIGHT, tmp_node );

                    CopyNode( diff_tree, origin_node->right, &tmp_node );
                    InsertLeave( diff_tree, ready_node->right->left->right, LEFT, tmp_node );

                    data.num = 1;
                    CreateNode( diff_tree, data, &tmp_node, NUM );
                    InsertLeave( diff_tree, ready_node->right->left->right, RIGHT, tmp_node );
                    
                    break;
                }

                case kSin:
                {
                    
                    break;
                }

                case kCos:
                {

                    break;
                }

                case kTg:
                {

                    break;
                }

                case kCtg:
                {

                    break;
                }

                case kLog:
                {

                    break;
                }

                case kLn:
                {

                    break;
                }

                case kExp:
                {

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
