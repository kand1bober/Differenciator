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
        // DotOutput( &my_tree);
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
Node_t* MakeDifferentiation( struct Tree* diff_tree, Node_t* origin_node )
{
    Node_t* ready_node = nullptr; //создаётся на каждой стадии рекурсии и возвращается в конце
    Data_t data = {};

    switch( (int)origin_node->type )
    {
        case NUM:
        {
            ready_node = CreateNodeDSL( diff_tree, NULL, NULL, (Data_t){.num = 0}, NUM );
            break;      
        }
        case VAR:
        {   
            ready_node = CreateNodeDSL( diff_tree, NULL, NULL, (Data_t){.num = 1}, NUM );
            break;
        }
        case OP:
        {
            Node_t* tmp_node = nullptr;

            switch( (int)origin_node->data.op )
            {
                case kAdd:
                {   
                    ready_node = CreateNodeDSL( diff_tree, MakeDifferentiation( diff_tree, origin_node->left ), 
                                                           MakeDifferentiation( diff_tree, origin_node->right ), 
                                                           (Data_t){.op = kAdd}, OP );
                    break;
                }
                case kSub:
                {
                    ready_node = CreateNodeDSL( diff_tree, MakeDifferentiation( diff_tree, origin_node->left ), 
                                                           MakeDifferentiation( diff_tree, origin_node->right ), 
                                                           (Data_t){.op = kSub}, OP );
                    break;
                }   
                case kMul:
                {   
                    ready_node = CreateNodeDSL( diff_tree, CreateNodeDSL( diff_tree, 
                                                                                MakeDifferentiation( diff_tree, origin_node->left ), 
                                                                                CopyBranchDSL( diff_tree, origin_node->right ), 
                                                                                (Data_t){.op = kMul}, OP ), 
                                                            CreateNodeDSL( diff_tree, 
                                                                                CopyBranchDSL( diff_tree, origin_node->left ), 
                                                                                MakeDifferentiation( diff_tree, origin_node->right ), 
                                                                                (Data_t){.op = kMul}, OP ), 
                                                        (Data_t){.op = kAdd}, OP );
                    break;
                }
                case kDiv:
                {
                    ready_node = CreateNodeDSL( diff_tree, 
                                                        CreateNodeDSL( diff_tree, 
                                                                        CreateNodeDSL( diff_tree, 
                                                                                                MakeDifferentiation( diff_tree, origin_node->left ), 
                                                                                                CopyBranchDSL( diff_tree, origin_node->right ), 
                                                                                                (Data_t){.op = kMul}, OP ), 
                                                                        CreateNodeDSL( diff_tree, 
                                                                                                CopyBranchDSL( diff_tree, origin_node->left ), 
                                                                                                MakeDifferentiation( diff_tree, origin_node->right ), 
                                                                                                (Data_t){.op = kMul}, OP ),
                                                    (Data_t){.op = kSub}, OP ), 
                                                    CreateNodeDSL( diff_tree, 
                                                                        CopyBranchDSL( diff_tree, origin_node->right ), 
                                                                        CopyBranchDSL( diff_tree, origin_node->right ), 
                                                                        (Data_t){.op = kMul}, OP ), 
                                                    (Data_t){.op = kDiv}, OP ); 
                    break;
                }

                case kDeg:
                {   
                    ready_node = CreateNodeDSL( diff_tree, 
                                                        CopyBranchDSL( diff_tree, origin_node ), 
                                                        CreateNodeDSL( diff_tree, 
                                                                                CreateNodeDSL( diff_tree, 
                                                                                                    MakeDifferentiation( diff_tree, origin_node->right ), 
                                                                                                    CreateNodeDSL( diff_tree, 
                                                                                                            NULL, 
                                                                                                            CopyBranchDSL( diff_tree, origin_node->left ), 
                                                                                                            (Data_t){.op = kLn}, OP ),  
                                                                                                    (Data_t){.op = kMul}, OP ), 
                                                                                CreateNodeDSL( diff_tree, 
                                                                                                    CreateNodeDSL( diff_tree, 
                                                                                                                        CopyBranchDSL( diff_tree, origin_node->right ), 
                                                                                                                        CopyBranchDSL( diff_tree, origin_node->left ), 
                                                                                                                        (Data_t){.op = kDiv}, OP ), 
                                                                                                    MakeDifferentiation( diff_tree, origin_node->left ), 
                                                                                                    (Data_t){.op = kMul}, OP ), 
                                                                                (Data_t){.op = kAdd}, OP ), 
                                                        (Data_t){.op = kMul}, OP );
                    break;
                }

                case kSin:
                {
                    ready_node = CreateNodeDSL( diff_tree, 
                                                        CreateNodeDSL( diff_tree, 
                                                                                NULL, 
                                                                                CopyBranchDSL( diff_tree, origin_node->right ), (Data_t){.op = kCos}, OP ), 
                                                        MakeDifferentiation( diff_tree, origin_node->right ), 
                                                        (Data_t){.op = kMul}, OP );
                    break;
                }

                case kCos:
                {
                    ready_node = CreateNodeDSL( diff_tree, 
                                                            CreateNodeDSL( diff_tree, 
                                                                                    NULL, 
                                                                                    NULL, 
                                                                                    (Data_t){.num = -1}, NUM ), 
                                                            CreateNodeDSL( diff_tree, 
                                                                                    CreateNodeDSL( diff_tree, NULL, CopyBranchDSL( diff_tree, origin_node->right ), (Data_t){.op = kSin}, OP ), 
                                                                                    MakeDifferentiation( diff_tree, origin_node->right ), 
                                                                                    (Data_t){.op = kMul}, OP ), 
                                                            (Data_t){.op = kMul}, OP );                    
                    break;
                }

                case kTg:
                {
                    ready_node = CreateNodeDSL( diff_tree, NULL, NULL, (Data_t){.op = kMul}, OP );

                    InsertLeave( diff_tree, ready_node, RIGHT, MakeDifferentiation( diff_tree, origin_node->right ) );

                    ready_node->left = CreateNodeDSL( diff_tree, 
                                                                CreateNodeDSL( diff_tree, 
                                                                                        NULL, 
                                                                                        NULL, 
                                                                                        (Data_t){.num = 1}, NUM ), 
                                                                CreateNodeDSL( diff_tree, 
                                                                                        CreateNodeDSL( diff_tree, 
                                                                                                                NULL, 
                                                                                                                CopyBranchDSL( diff_tree, origin_node->right ), 
                                                                                                                (Data_t){.op = kCos}, OP ), 
                                                                                        CreateNodeDSL( diff_tree, 
                                                                                                                NULL, 
                                                                                                                NULL, 
                                                                                                                (Data_t){.num = 2}, NUM ), 
                                                                                        (Data_t){.op = kDeg}, OP ), 
                                                                (Data_t){.op = kDiv}, OP );
                    break;
                }

                case kCtg:
                {
                    ready_node = CreateNodeDSL( diff_tree, 
                                                        CreateNodeDSL( diff_tree, 
                                                                CreateNodeDSL( diff_tree, 
                                                                    NULL, 
                                                                    NULL, 
                                                                    (Data_t){.num = -1}, NUM ), 
                                                                CreateNodeDSL( diff_tree, 
                                                                    CreateNodeDSL( diff_tree,
                                                                            NULL, 
                                                                            CopyBranchDSL( diff_tree, origin_node->right ), 
                                                                            (Data_t){.op = kSin}, OP ), 
                                                                    CreateNodeDSL( diff_tree, 
                                                                            NULL, 
                                                                            NULL, 
                                                                            (Data_t){.num = 2}, NUM ), 
                                                                    (Data_t){.op = kDeg}, OP ), 
                                                                (Data_t){.op = kDiv}, OP ),
                                                        MakeDifferentiation( diff_tree, origin_node->right ), 
                                                        (Data_t){.op = kMul}, OP );
                    break;
                }

                case kLog:
                {
                    ready_node = CreateNodeDSL( diff_tree, 
                                                        CreateNodeDSL( diff_tree, 
                                                                CreateNodeDSL( diff_tree, 
                                                                                        NULL, 
                                                                                        NULL, 
                                                                                        (Data_t){.num = 1}, NUM ), 
                                                                CreateNodeDSL( diff_tree, 
                                                                                        CopyBranchDSL( diff_tree, origin_node->right ), 
                                                                                        CreateNodeDSL( diff_tree, 
                                                                                                                NULL, 
                                                                                                                CopyBranchDSL( diff_tree, origin_node->left ), 
                                                                                                                (Data_t){.op = kLn}, OP ), 
                                                                                        (Data_t){.op = kMul}, OP ), 
                                                                (Data_t){.op = kDiv}, OP ), 
                                                        MakeDifferentiation( diff_tree, origin_node->right ), 
                                                        (Data_t){.op = kMul}, OP );
                    break;
                }

                case kLn:
                {
                    ready_node = CreateNodeDSL( diff_tree, 
                                                        CreateNodeDSL( diff_tree, 
                                                                                CreateNodeDSL( diff_tree, NULL, NULL, (Data_t){.num = 1}, NUM ), 
                                                                                CopyBranchDSL( diff_tree, origin_node->right ), 
                                                                                (Data_t){.op = kDiv}, OP ), 
                                                        MakeDifferentiation( diff_tree, origin_node->right ), 
                                                        (Data_t){.op = kMul}, OP );
                    break;
                }

                case kExp:
                {   
                    data.op = kMul;
                    ready_node = CreateNodeDSL( diff_tree, 
                                                        CopyBranchDSL( diff_tree, origin_node ), 
                                                        MakeDifferentiation( diff_tree, origin_node->right ), 
                                                        (Data_t){.op = kMul}, OP );
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

