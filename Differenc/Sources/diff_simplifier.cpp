
#include "../Headers/diff_functions.h"
#include "../Headers/diff_simplifier.h"

void TreeSimplifie( struct Tree* tree )
{
    struct SimpleSrc src = {};
    src.tree = tree;
    src.change_count = CHANGE;

    int i = 0;
    while( src.change_count == CHANGE )
    {
        src.change_count = NO_CHANGE;

        // //------------------------------
        // printf(GREEN "iteration: %d\n" DELETE_COLOR, i );
        // struct File_text graph_file = {}; 
        // Output( &graph_file, tree);
        // getchar();
        // //------------------------------

        OpSimplifie( &src, tree->root );

        // return;
        i++;
    }

    return;
}

void OpSimplifie( struct SimpleSrc* src, struct Node_t* node )
{
    // printf(SINIY "node: %p\n" DELETE_COLOR, node );

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
            NodeSimplifie( src, &node );      
        }

    } 

    //-----------------ЛЮБОЙ ДРУГОЙ УЗЕЛ!!!!!------------------
    if( node->left != nullptr && node->right != nullptr ) 
    {   
        if( ( node->type == OP ) && ( node->data.op == kMul || node->data.op == kDiv ) && ( node->right->type == NUM ) && ( fabs( node->right->data.num - 1 ) < EPSILON ) )
        {
            ReplaceNode( &node, &(node->left), src );
        }
        else if( ( node->type == OP ) && ( node->data.op == kMul || node->data.op == kDiv ) && ( node->left->type == NUM ) && ( fabs( node->left->data.num - 1 ) < EPSILON ) )
        {
            ReplaceNode( &node, &(node->right), src );
        }
        
        // ----Add/Sub 0----
        else if( ( node->type == OP ) && ( node->data.op == kAdd || node->data.op == kSub ) && ( node->right->type == NUM ) && ( fabs( node->right->data.num - 0 ) < EPSILON ) ) 
        {
            ReplaceNode( &node, &(node->left), src );
        }
        else if( ( node->type == OP ) && ( node->data.op == kAdd || node->data.op == kSub ) && ( node->left->type == NUM ) && ( fabs( node->left->data.num - 0 ) < EPSILON ) )
        {
            ReplaceNode( &node, &(node->right), src );
        }
    
        // ----Mul on 0----
        else if( ( node->type == OP ) && ( node->data.op == kMul ) && ( node->left->type == NUM ) && ( fabs( node->left->data.num - 0 ) < EPSILON ) )
        {
            ReplaceNode( &node, &(node->left), src );
        }
    
        else if( ( node->type == OP ) && ( node->data.op == kMul ) && ( node->right->type == NUM ) && ( fabs( node->right->data.num - 0 ) < EPSILON ) )
        {
            ReplaceNode( &node, &(node->right), src );
        }
        // ----Deg 1----
        else if( ( node->type == OP ) && ( node->data.op == kDeg ) && ( node->right->type == NUM ) && ( fabs( node->right->data.num - 1 ) < EPSILON ) )
        {       
            ReplaceNode( &node, &(node->left), src );
        }
        // ----Deg 0----
        else if( ( node->type == OP ) && ( node->data.op == kDeg ) && ( node->right->type == NUM ) && ( fabs( node->right->data.num - 0 ) < EPSILON ) )
        {
            src->type = NUM;
            src->data.num = 1;

            NodeSimplifie( src, &node );
        }
    }

        // printf(" going to sons\n\n");
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
    struct Node_t* tmp_node = nullptr;
    tmp_node = CreateNode(src->tree, NULL, NULL, NULL, src->data, src->type);
    ReplaceNode( node, &tmp_node, src );

    return;
}


enum TreeErrors ReplaceNode( struct Node_t** to_replace, struct Node_t** new_node, struct SimpleSrc* src )
{   

    assert( *to_replace );
    assert( *new_node );

    // //-----------DEBUG--------------
    // printf(SINIY "%p %p\n\n" DELETE_COLOR, (*to_replace), (*new_node) );
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
            (*to_replace)->parent->left = (*new_node);
        }
        else if( (*to_replace)->parent->right == (*to_replace) )
        {
            (*to_replace)->parent->right = (*new_node);    
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
    (*new_node)->parent = (*to_replace)->parent;


    if( (*to_replace)->left == (*new_node) )
    {
        if( type_status == ROOT )
        {
            (*to_replace)->parent = nullptr;
    
            struct Node_t* tmp_node = nullptr;
            tmp_node = (*new_node);

            (*to_replace)->left = nullptr;
            FreeTree( src->tree, (*to_replace) );

            //----exchange-------     //TODO: скорее всего, такого даже не бывает, пока не проверено, скорее всего работает
            (*to_replace) = tmp_node;
            src->tree->root = tmp_node;
        }
        else 
        {
            (*to_replace)->parent = nullptr;
    
            struct Node_t* tmp_node = nullptr;
            tmp_node = (*new_node);

            (*to_replace)->left = nullptr;
            FreeTree( src->tree, (*to_replace) );

            (*to_replace) = tmp_node;
        }
    }
    else if( (*to_replace)->right == (*new_node) )
    {
        if( type_status == ROOT )
        {
            struct Node_t* tmp_node = nullptr;
            tmp_node = (*new_node);
            
            (*to_replace)->right = nullptr;
            FreeTree( src->tree, (*to_replace) );

            //----exchange-------     //TODO: скорее всего, такого даже не бывает, пока не проверено, скорее всего работает
            (*to_replace) = tmp_node;
            src->tree->root = tmp_node;
        }   
        else 
        {
            (*to_replace)->parent = nullptr;

            struct Node_t* tmp_node = nullptr;
            tmp_node = (*new_node);
            
            (*to_replace)->right = nullptr;
            FreeTree( src->tree, (*to_replace) );

            //----exchange-------
            (*to_replace) = tmp_node;
        }
    }
    else if(  (*to_replace)->left != (*new_node) && (*to_replace)->right != (*new_node) )
    {
        if( type_status == ROOT )
        {
            FreeTree( src->tree, (*to_replace) );

            //----exchange-------
            (*to_replace) = (*new_node);
            src->tree->root = (*new_node);
        }
        else 
        {
            FreeTree( src->tree, (*to_replace) );

            //----exchange-------
            (*to_replace) = (*new_node);
        }
    }
    
    // printf(RED "    changing    \n" DELETE_COLOR);
    src->change_count = CHANGE;

    return GOOD_INSERT;
}
