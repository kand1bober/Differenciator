#include "../Headers/diff_functions.h"
#include "../Headers/diff_simplifier.h"
#include "../Headers/diff_tex_output.h"

enum DiffInfo Run()
{
    // ------------Initialize------------
    struct File_text tree_input = {}; //for input
    // struct File_text tree_output = {};   // for output
    struct File_text graph_file = {};  //for inpit
    struct Tree my_tree = {};

    // struct File_text diff_tree_input = {}; 
    // struct File_text diff_tree_output = {};   
    // struct File_text diff_graph_file = {};  
    struct Tree diff_tree = {};

    MakeTreeData( &graph_file, &tree_input, &my_tree );

    Differentiate( &my_tree, &diff_tree );

    //----( probably not needed in differenciator )------
    // TreeData( &my_tree, &tree_output );
    //---------------------------------------------------

    //-----------------PRINTING------------------
    if(my_tree.status == GOOD_TREE)
    {
        // Output( &my_tree);
        DotOutput( &diff_tree );
        TexOutput( &diff_tree );
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
    diff_tree->root = MakeDifferentiation( diff_tree, origin_tree->root );

    TreeSimplifie( diff_tree );

    return GOOD_DIFF;
}

//----Watches on origin node and based on its conten, builds new tree( Doesn't touch origin tree!!! )--
struct Node_t* MakeDifferentiation( struct Tree* diff_tree, struct Node_t* origin_node )
{
    struct Node_t* ready_node = nullptr; //создаётся на каждой стадии рекурсии и возвращается в конце
    union Data_t data = {};

    switch( (int)origin_node->type )
    {
        case NUM:
        {
            data.num = 0;
            ready_node = CreateNode( diff_tree, NULL, NULL, NULL, data, NUM );
            break;      
        }
        case VAR:
        {   
            data.num = 1;
            ready_node = CreateNode( diff_tree, NULL, NULL, NULL, data, NUM );
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
                    ready_node = CreateNode( diff_tree, NULL, NULL, NULL, data, OP );

                    tmp_node = MakeDifferentiation( diff_tree, origin_node->left );
                    InsertLeave( diff_tree, ready_node, LEFT, tmp_node );

                    tmp_node = MakeDifferentiation( diff_tree, origin_node->right );
                    InsertLeave( diff_tree, ready_node, RIGHT, tmp_node );
                    
                    break;
                }
                case kSub:
                {
                    data.op = kSub;
                    ready_node = CreateNode( diff_tree, NULL, NULL, NULL, data, OP );

                    tmp_node = MakeDifferentiation( diff_tree, origin_node->left );
                    InsertLeave( diff_tree, ready_node, LEFT, tmp_node );

                    tmp_node = MakeDifferentiation( diff_tree, origin_node->right );
                    InsertLeave( diff_tree, ready_node, RIGHT, tmp_node );

                    break;
                }   
                case kMul:
                {   
                    data.op = kAdd;
                    ready_node = CreateNode( diff_tree, NULL, NULL, NULL, data, OP );

                    //-----------------------LEFT------------------------
                    data.op = kMul;
                    ready_node->left = CreateNode( diff_tree, NULL, NULL, ready_node, data, OP );

                    tmp_node = MakeDifferentiation( diff_tree, origin_node->left );
                    InsertLeave( diff_tree, ready_node->left, LEFT, tmp_node );

                    ready_node->left->right = CopyBranch( diff_tree, origin_node->right, ready_node->left );
                    //---------------------------------------------------

                    //----------------------RIGHT------------------------
                    data.op = kMul;
                    ready_node->right = CreateNode( diff_tree, NULL, NULL, ready_node, data, OP );

                    ready_node->right->left = CopyBranch( diff_tree, origin_node->left, ready_node->right );
                    // InsertLeave( diff_tree,);

                    tmp_node = MakeDifferentiation( diff_tree, origin_node->right );
                    InsertLeave( diff_tree, ready_node->right, RIGHT, tmp_node );
                    //---------------------------------------------------

                    break;
                }
                case kDiv:
                {
                    data.op = kDiv; 
                    ready_node = CreateNode( diff_tree, NULL, NULL, NULL, data, OP ); 

                    //--------------------UP LEFT------------------------
                    data.op = kSub;
                    ready_node->left = CreateNode( diff_tree, NULL, NULL, ready_node, data, OP );
                    //---------------------------------------------------

                    //----------------------LEFT-------------------------
                    data.op = kMul;
                    ready_node->left->left = CreateNode( diff_tree, NULL, NULL, ready_node->left, data, OP );

                    tmp_node = MakeDifferentiation( diff_tree, origin_node->left );
                    InsertLeave( diff_tree, ready_node->left->left, LEFT, tmp_node );

                    ready_node->left->left->right = CopyBranch( diff_tree, origin_node->right, ready_node->left->left );
                    //---------------------------------------------------

                    //---------------------RIGHT-------------------------
                    data.op = kMul;
                    ready_node->left->right = CreateNode( diff_tree, NULL, NULL, ready_node->left, data, OP );

                    ready_node->left->right->left = CopyBranch( diff_tree, origin_node->left, ready_node->left->right ); 

                    tmp_node = MakeDifferentiation( diff_tree, origin_node->right );
                    InsertLeave( diff_tree, ready_node->left->right, RIGHT, tmp_node );
                    //---------------------------------------------------

                    //--------------------UP RIGHT-----------------------
                    data.op = kMul;
                    ready_node->right = CreateNode( diff_tree, NULL, NULL, ready_node, data, OP );

                    //-------------SQUARE OF DENOMINATOR-----------------
                    ready_node->right->left = CopyBranch( diff_tree, origin_node->right, ready_node->right ); 
 
                    ready_node->right->right = CopyBranch( diff_tree, origin_node->right, ready_node->right ); 
                    //---------------------------------------------------

                    break;
                }

                case kDeg:
                {
                    data.op = kMul;
                    ready_node = CreateNode( diff_tree, NULL, NULL, NULL, data, OP );

                    //--------------------------LEFT---------------------------
                    ready_node->left = CopyBranch( diff_tree, origin_node->right, ready_node );
                    
                    //-------------------------RIGHT---------------------------
                    data.op = kMul;
                    ready_node->right = CreateNode( diff_tree, NULL, NULL, ready_node, data, OP );
                    // InsertLeave( diff_tree, &ready_node, RIGHT, );

                    tmp_node = MakeDifferentiation( diff_tree, origin_node->left );
                    InsertLeave( diff_tree, ready_node->right, RIGHT, tmp_node );

                    data.op = kDeg;
                    ready_node->right->left = CreateNode( diff_tree, NULL, NULL, ready_node->right, data, OP );
 
                    ready_node->right->left->left = CopyBranch( diff_tree, origin_node->left, ready_node->right->left ); 

                    data.op = kSub;
                    ready_node->right->left->right = CreateNode( diff_tree, NULL, NULL, ready_node->right->left, data, OP );

                    ready_node->right->left->right->left = CopyBranch( diff_tree, origin_node->right, ready_node->right->left->right ); 

                    data.num = 1;
                    ready_node->right->left->right->right = CreateNode( diff_tree, NULL, NULL, ready_node->right->left->right, data, NUM );
                    
                    break;
                }

                case kSin:
                {
                    data.op = kMul;
                    ready_node = CreateNode( diff_tree, NULL, NULL, NULL, data, OP );

                    data.op = kCos;
                    ready_node->left = CreateNode( diff_tree, NULL, NULL, ready_node, data, OP );

                    ready_node->left->right = CopyBranch( diff_tree, origin_node->right, ready_node->left );

                    tmp_node = MakeDifferentiation( diff_tree, origin_node->right );
                    InsertLeave( diff_tree, ready_node, RIGHT, tmp_node );

                    break;
                }

                case kCos:
                {
                    data.op = kMul;
                    ready_node = CreateNode( diff_tree, NULL, NULL, NULL, data, OP );

                    data.num = -1;
                    ready_node->left = CreateNode( diff_tree, NULL, NULL, ready_node, data, NUM );
                    
                    data.op = kMul;
                    ready_node->right = CreateNode( diff_tree, NULL, NULL, ready_node, data, OP );

                    data.op = kSin;
                    ready_node->right->left = CreateNode( diff_tree, NULL, NULL, ready_node->right, data, OP );

                    ready_node->right->left->right = CopyBranch( diff_tree, origin_node->right, ready_node->right->left ); 

                    tmp_node = MakeDifferentiation( diff_tree, origin_node->right );
                    InsertLeave( diff_tree, ready_node->right, RIGHT, tmp_node );

                    break;
                }

                case kTg:
                {
                    data.op = kMul;
                    ready_node = CreateNode( diff_tree, NULL, NULL, NULL, data, OP );

                    tmp_node = MakeDifferentiation( diff_tree, origin_node->right );
                    InsertLeave( diff_tree, ready_node, RIGHT, tmp_node );
                    data.op = kDiv;
                    ready_node->left = CreateNode( diff_tree, NULL, NULL, ready_node, data, OP );

                    data.num = 1;
                    ready_node->left->left = CreateNode( diff_tree, NULL, NULL, ready_node->left, data, NUM );

                    data.op = kDeg;
                    ready_node->left->right = CreateNode( diff_tree, NULL, NULL, ready_node->left->right, data, OP );

                    data.op = kCos;
                    ready_node->left->right->left = CreateNode( diff_tree, NULL, NULL, ready_node->left->right, data, OP );
                    ready_node->left->right->left->right = CopyBranch( diff_tree, origin_node->right, ready_node->left->right->left ); 

                    data.num = 2;
                    ready_node->left->right->right = CreateNode( diff_tree, NULL, NULL, ready_node->left->right, data, NUM );

                    break;
                }

                case kCtg:
                {
                    data.op = kMul;
                    ready_node = CreateNode( diff_tree, NULL, NULL, NULL, data, OP );

                    tmp_node = MakeDifferentiation( diff_tree, origin_node->right );
                    InsertLeave( diff_tree, ready_node, RIGHT, tmp_node );

                    data.op = kDiv;
                    ready_node->left = CreateNode( diff_tree, NULL, NULL, ready_node, data, OP );

                    data.num = -1;
                    ready_node->left->left = CreateNode( diff_tree, NULL, NULL, ready_node->left, data, NUM );

                    data.op = kDeg;
                    ready_node->left->right = CreateNode( diff_tree, NULL, NULL, ready_node->left, data, OP );

                    data.op = kSin;
                    ready_node->left->right->left = CreateNode( diff_tree, NULL, NULL, ready_node->left->right, data, OP );

                    ready_node->left->right->left->right = CopyBranch( diff_tree, origin_node->right, ready_node->left->right->left ); 

                    data.num = 2;
                    ready_node->left->right->right = CreateNode( diff_tree, NULL, NULL, ready_node->left->right, data, NUM );

                    break;
                }

                case kLog:
                {
                    data.op = kMul;
                    ready_node = CreateNode( diff_tree, NULL, NULL, NULL, data, OP );

                    tmp_node = MakeDifferentiation( diff_tree, origin_node->right );
                    InsertLeave( diff_tree, ready_node, RIGHT, tmp_node );

                    data.op = kDiv;
                    ready_node->left = CreateNode( diff_tree, NULL, NULL, ready_node, data, OP );

                    data.num = 1;
                    ready_node->left->left = CreateNode( diff_tree, NULL, NULL, ready_node->left, data, NUM );

                    data.op = kMul;
                    ready_node->left->right =  CreateNode( diff_tree, NULL, NULL, ready_node->left, data, OP );

                    ready_node->left->right->left = CopyBranch( diff_tree, origin_node->right, ready_node->left->right );

                    data.op = kLn;
                    ready_node->left->right->right = CreateNode( diff_tree, NULL, NULL, ready_node->left->right, data, OP );

                    ready_node->left->right->right->right = CopyBranch( diff_tree, origin_node->left, ready_node->left->right->right );

                    break;
                }

                case kLn:
                {
                    data.op = kMul;
                    ready_node = CreateNode( diff_tree, NULL, NULL, NULL, data, OP );

                    tmp_node = MakeDifferentiation( diff_tree, origin_node->right );
                    InsertLeave( diff_tree, ready_node, RIGHT, tmp_node );

                    data.op = kDiv;
                    ready_node->left = CreateNode( diff_tree, NULL, NULL, ready_node, data, OP );

                    data.num = 1;
                    ready_node->left->left = CreateNode( diff_tree, NULL, NULL, ready_node->left, data, NUM );
                    ready_node->left->right = CopyBranch( diff_tree, origin_node->right, ready_node->left );

                    break;
                }

                case kExp:
                {   
                    data.op = kMul;
                    ready_node = CreateNode( diff_tree, NULL, NULL, NULL, data, OP );

                    tmp_node = MakeDifferentiation( diff_tree, origin_node->right );
                    InsertLeave( diff_tree, ready_node, RIGHT, tmp_node );

                    ready_node->left = CopyBranch( diff_tree, origin_node, ready_node ); 

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


