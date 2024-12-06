
#include "../Headers/diff_functions.h"
#include "../Headers/diff_simplifier.h"

void TreeSimplifie( struct Tree* tree )
{
    struct SimpleSrc src = {};
    src.tree = tree;
    src.change_count = CHANGE;

    while( src.change_count == CHANGE )
    {
        src.change_count = NO_CHANGE;

        // VarSimplifie( &src, tree->root );

        // NumSimplifie( &src, tree->root );

        OpSimplifie( &src, tree->root );

        // return;
    }

    return;
}

void VarSimplifie( struct SimpleSrc* src, struct Node_t* node )
{
    // if(  )
    // {
        
    // }
    // else if( )
    // {
    
    // }
    //...
}       

void NumSimplifie( struct SimpleSrc* src, struct Node_t* node )
{
    // if( )
    // {

    // }
    // else if()
    // {
    
    // }
    //...
}

void OpSimplifie( struct SimpleSrc* src, struct Node_t* node )
{
    printf(SINIY "node: %p\n\n" DELETE_COLOR, node );

    if( node->left  != nullptr && node->left->left  == nullptr && node->left->right  == nullptr && 
        node->right != nullptr && node->right->left == nullptr && node->right->right == nullptr )
    {
        if( node->left->type == NUM && node->right->type == NUM )
        {
            src->data = {};
            src->type = NUM;

            switch( (int)node->data.op )
            {
                case kAdd:
                {
                    src->data.num = node->left->data.num + node->right->data.num;
                    break;
                }
                case kSub:
                {
                    src->data.num = node->left->data.num - node->right->data.num;
                    break;
                }
                case kMul:
                {
                    src->data.num = node->left->data.num * node->right->data.num;
                    break;
                }
                case kDiv:
                {
                    src->data.num = node->left->data.num / node->right->data.num;
                    break;
                }
                case kDeg:
                {
                    src->data.num = pow( node->left->data.num, node->right->data.num );
                    break;
                }
                case kSin:
                {
                    src->data.num = sin( node->right->data.num ); //TODO: пересмотреть 
                    break;
                }
                case kCos:
                {
                    src->data.num = cos( node->right->data.num ); //TODO: пересмотреть 
                    break;
                }
                case kTg:
                {
                    src->data.num = tan( node->right->data.num ); //TODO: пересмотреть 
                    break;
                }
                case kCtg:
                {
                    src->data.num = pow( tan( node->right->data.num ), -1 ); //TODO: пересмотреть 
                    break;
                }
                case kLn:
                {
                    src->data.num = log( node->right->data.num ); 
                    break;
                }
                case kLog:
                {
                    src->data.num = log( node->left->data.num ) / log( node->right->data.num ); 
                    break;
                }
                case kExp:
                {
                    src->data.num = exp( node->right->data.num ); 
                    break;
                }
                default:
                {
                    printf(RED "unknown problem in %s in %s on line %d " DELETE_COLOR, __FILE__, __PRETTY_FUNCTION__, __LINE__ );
                    exit(0);
                    break;
                }
            }
            printf(YELLOW "ariphmetic: \n" DELETE_COLOR);
            NodeSimplifie( src, &node );
        }
    } 
    else if( node != nullptr && node->left != nullptr && node->right != nullptr )
    {   
        if( node->type == OP && ( node->data.op == kMul || node->data.op == kDiv ) )
        {
            if( node->right->type == NUM && fabs( node->right->data.num - 1) < EPSILON )
            {
                // Right 1
                printf(YELLOW "mul/div 1: \n" DELETE_COLOR);
                ReplaceNode( src->tree, &node, &(node->left), src );
            }
            else if( node->left->type == NUM && fabs( node->left->data.num - 1) < EPSILON )
            {
                // Left 1
                printf(YELLOW "mul/div 1: \n" DELETE_COLOR);
                ReplaceNode( src->tree, &node, &(node->right), src );
            }
        }

        // ----Add/Sub 0----
        else if(  node->type == OP && ( node->data.op == kAdd || node->data.op == kSub ) )
        {
            if( node->right->type == NUM && fabs( node->right->data.num - 0) < EPSILON )
            {
                // Right 0
                printf(YELLOW "add/sub 0: \n" DELETE_COLOR);
                ReplaceNode( src->tree, &node, &(node->left), src );
            }
            else if( node->left->type == NUM && fabs( node->left->data.num - 0) < EPSILON )
            {
                // Left 0
                printf(YELLOW "add/sub 0: \n" DELETE_COLOR);
                ReplaceNode( src->tree, &node, &(node->right), src );
            }
        }

        // // ----Mul on 0----
        else if( node->type == OP && node->data.op == kMul )
        {
            if( node->left->type == NUM && fabs( node->left->data.num ) < EPSILON )
            {   
                // Left 0
                printf("Simplifying multiplying on null\n");
                src->type = NUM;
                src->data.num = 0;

                NodeSimplifie( src, &node );
            }
            else if( node->right->type == NUM && fabs( node->right->data.num ) < EPSILON )
            {
                // Right 0
                printf("Simplifying multiplying on null\n");
                src->type = NUM;
                src->data.num = 0;

                NodeSimplifie( src, &node );
            }
        }

        //----Warning----
        else if( node->type == OP && ( node->data.op == kDiv) )
        {
            printf(RED "==== Warning: Dividing by zero ====\n"
                       "====          Closing Programm ====\n\n" DELETE_COLOR);
            exit(0);
        }
    }

    printf("Nothing... going to sons\n");

    if( node->left != nullptr )
    {
        OpSimplifie( src, node->left );
    }

    if( node->right != nullptr )
    {
        OpSimplifie( src, node->right );
    }

    return;
}

void NodeSimplifie( struct SimpleSrc* src, struct Node_t** node )
{
    // ON_DEBUG( printf(YELLOW "Simplifying\n" DELETE_COLOR ) );

    struct Node_t* tmp_node = nullptr;
    tmp_node = CreateNode(src->tree, NULL, NULL, NULL, src->data, src->type);
    ReplaceNode( src->tree, node, &tmp_node, src );
    src->change_count = CHANGE;

    // ON_DEBUG( printf("%p res: %lf\n", node, src->data.num); );

    return;
}


enum TreeErrors ReplaceNode( struct Tree* tree, struct Node_t** to_replace, struct Node_t** src, struct SimpleSrc* simple_status )
{   

    assert( *to_replace );
    assert( *src );

    // //-----------DEBUG--------------
    // printf(SINIY "%p %p\n\n" DELETE_COLOR, (*to_replace), (*src) );
    // struct File_text graph_file = {}; 
    // Output( &graph_file, tree);
    // getchar();
    // //------------------------------

    const int ROOT = 53;
    const int NOT_ROOT = 52;
    int type_status = NOT_ROOT;

    if( (*to_replace)->parent != nullptr )
    {
        if( (*to_replace)->parent->left == (*to_replace) )
        {
            (*to_replace)->parent->left = (*src);
        }
        else if( (*to_replace)->parent->right == (*to_replace) )
        {
            (*to_replace)->parent->right = (*src);    
        }
        else
        {
            printf(RED "unknown problem in %s in %s on line %d " DELETE_COLOR, __FILE__, __PRETTY_FUNCTION__, __LINE__ );
            exit(0);
        }

        type_status = NOT_ROOT;
    }
    else 
    {
        type_status = ROOT;
    }   
    (*src)->parent = (*to_replace)->parent;


    if( (*to_replace)->left == (*src) )
    {
        if( type_status == ROOT )
        {
            (*to_replace)->parent = nullptr;
    
            struct Node_t* tmp_node = nullptr;
            tmp_node = (*src);

            (*to_replace)->left = nullptr;
            FreeTree( tree, (*to_replace) );

            //----exchange-------     //TODO: скорее всего, такого даже не бывает, пока не проверено, скорее всего работает
            (*to_replace) = tmp_node;
            tree->root = tmp_node;
        }
        else 
        {
            (*to_replace)->parent = nullptr;
    
            struct Node_t* tmp_node = nullptr;
            tmp_node = (*src);

            (*to_replace)->left = nullptr;
            FreeTree( tree, (*to_replace) );

            (*to_replace) = tmp_node;
        }
    }
    else if( (*to_replace)->right == (*src) )
    {
        if( type_status == ROOT )
        {
            struct Node_t* tmp_node = nullptr;
            tmp_node = (*src);
            
            (*to_replace)->right = nullptr;
            FreeTree( tree, (*to_replace) );

            //----exchange-------     //TODO: скорее всего, такого даже не бывает, пока не проверено, скорее всего работает
            (*to_replace) = tmp_node;
            tree->root = tmp_node;
        }   
        else 
        {
            (*to_replace)->parent = nullptr;

            struct Node_t* tmp_node = nullptr;
            tmp_node = (*src);
            
            (*to_replace)->right = nullptr;
            FreeTree( tree, (*to_replace) );

            //----exchange-------
            (*to_replace) = tmp_node;
        }
    }
    else if(  (*to_replace)->left != (*src) && (*to_replace)->right != (*src) )
    {
        if( type_status == ROOT )
        {
            FreeTree( tree, (*to_replace) );

            //----exchange-------
            (*to_replace) = (*src);
            tree->root = (*src);
        }
        else 
        {
            FreeTree( tree, (*to_replace) );

            //----exchange-------
            (*to_replace) = (*src);
        }
    }
    
    simple_status->change_count = CHANGE;

    return GOOD_INSERT;
}
