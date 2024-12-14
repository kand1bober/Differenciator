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
        TexOutput( &my_tree, &diff_tree );
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
    #define DIFF_DSL
    #include "../Headers/diff_dsl.h"
    Node_t* ready_node = nullptr; //создаётся на каждой стадии рекурсии и возвращается в конце
    Data_t data = {};

    switch( (int)origin_node->type )
    {
        case NUM:
        {
            ready_node = CREATE( NULL, NULL, num, 0, NUM );
            break;      
        }
        case VAR:
        {   
            ready_node = CREATE( NULL, NULL, num, 1, NUM );
            break;
        }
        case OP:
        {
            switch( (int)origin_node->data.op )
            {
                case kAdd:
                {   
                    ready_node = CREATE( 
                                        DIFF( origin_node->left ), 
                                        DIFF( origin_node->right ), 
                                        op, kAdd, OP );

                    break;
                }
                case kSub:
                {
                    ready_node = CREATE( 
                                        DIFF( origin_node->left ), 
                                        DIFF( origin_node->right ), 
                                        op, kSub, OP );

                    break;
                }   
                case kMul:
                {   
                    ready_node = CREATE( 
                                        CREATE( 
                                                DIFF( origin_node->left ), 
                                                COPY( origin_node->right ), 
                                                op, kMul, OP ), 
                                        CREATE( 
                                                COPY( origin_node->left ), 
                                                DIFF( origin_node->right ), 
                                                op, kMul, OP ), 
                                        op, kAdd, OP );

                    break;
                }
                case kDiv:
                {
                    ready_node = CREATE( 
                                        CREATE( 
                                                CREATE( 
                                                        DIFF( origin_node->left ), 
                                                        COPY( origin_node->right ), 
                                                        op, kMul, OP ), 
                                                CREATE( 
                                                        COPY( origin_node->left ), 
                                                        DIFF( origin_node->right ), 
                                                        op, kMul, OP ),
                                                op, kSub, OP ), 
                                        CREATE( 
                                                COPY( origin_node->right ), 
                                                COPY( origin_node->right ), 
                                                op, kMul, OP ), 
                                        op, kDiv, OP ); 
                    break;
                }

                case kDeg:
                {   
                    ready_node = CREATE(
                                        COPY( origin_node ), 
                                        CREATE(
                                                CREATE( 
                                                        DIFF( origin_node->right ), 
                                                        CREATE( 
                                                                NULL, 
                                                                COPY( origin_node->left ), 
                                                                op, kLn, OP ),  
                                                        op, kMul, OP ), 
                                                CREATE( 
                                                        CREATE( 
                                                                COPY( origin_node->right ), 
                                                                COPY( origin_node->left ), 
                                                                op, kDiv, OP ), 
                                                        DIFF( origin_node->left ), 
                                                        op, kMul, OP ), 
                                                op, kAdd, OP ), 
                                        op, kMul, OP );
                    break;
                }

                case kSin:
                {
                    ready_node = CREATE( 
                                        CREATE( 
                                                NULL, 
                                                COPY( origin_node->right ), 
                                                op, kCos, OP ), 
                                        DIFF( origin_node->right ), 
                                        op, kMul, OP );
                    break;
                }

                case kCos:
                {
                    ready_node = CREATE( 
                                        CREATE( 
                                                NULL, 
                                                NULL, 
                                                num, -1, NUM ), 
                                        CREATE( 
                                                CREATE( 
                                                        NULL, 
                                                        COPY( origin_node->right ), 
                                                        op, kSin, OP ), 
                                                DIFF( origin_node->right ), 
                                                op, kMul, OP ), 
                                        op, kMul, OP );                    
                    break;
                }

                case kTg:
                {
                    ready_node = CREATE( 
                                        CREATE(
                                                CREATE( 
                                                        NULL, 
                                                        NULL, 
                                                        num, 1, NUM ), 
                                                CREATE( 
                                                        CREATE( 
                                                                NULL, 
                                                                COPY( origin_node->right ), 
                                                                op, kCos, OP ), 
                                                        CREATE( 
                                                                NULL, 
                                                                NULL, 
                                                                num, 2, NUM ), 
                                                        op, kDeg, OP ), 
                                                op, kDiv, OP ), 
                                        DIFF( origin_node->right ), 
                                        op, kMul, OP );
                    break;
                }

                case kCtg:
                {
                    ready_node = CREATE( 
                                        CREATE( 
                                                CREATE( 
                                                        NULL, 
                                                        NULL, 
                                                        num, -1, NUM ), 
                                                CREATE( 
                                                        CREATE( 
                                                                NULL, 
                                                                COPY( origin_node->right ), 
                                                                op, kSin, OP ), 
                                                        CREATE( 
                                                                NULL, 
                                                                NULL, 
                                                                num, 2, NUM ), 
                                                        op, kDeg, OP ), 
                                                    op, kDiv, OP ),
                                        DIFF( origin_node->right ), 
                                        op, kMul, OP );
                    break;
                }

                case kLog:
                {
                    ready_node = CREATE( 
                                        CREATE( 
                                                CREATE( 
                                                        NULL, 
                                                        NULL, 
                                                        num, 1, NUM ), 
                                                CREATE( 
                                                        COPY( origin_node->right ), 
                                                        CREATE( 
                                                                NULL, 
                                                                COPY( origin_node->left ), 
                                                                op, kLn, OP ), 
                                                        op, kMul, OP ), 
                                                op, kDiv, OP ), 
                                        DIFF( origin_node->right ), 
                                        op, kMul, OP );
                    break;
                }

                case kLn:
                {
                    ready_node = CREATE( 
                                        CREATE( 
                                                CREATE( 
                                                        NULL, 
                                                        NULL, 
                                                        num, 1, NUM ), 
                                                COPY( origin_node->right ), 
                                                op, kDiv, OP ), 
                                        DIFF( origin_node->right ), 
                                        op, kMul, OP );
                    break;
                }

                case kExp:
                {   
                    data.op = kMul;
                    ready_node = CREATE( 
                                        COPY( origin_node ), 
                                        DIFF( origin_node->right ), 
                                        op, kMul, OP );
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

    #undef DIFF_DSL
}

