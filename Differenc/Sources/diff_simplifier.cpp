
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
    printf(YELLOW "HUY: %d\n" DELETE_COLOR, node->type );

    if( node->left  != nullptr && node->left->left  == nullptr && node->left->right  == nullptr && 
        node->right != nullptr && node->right->left == nullptr && node->right->right == nullptr )
    {
        if( node->left->type == NUM && node->right->type == NUM )
        {
            union Data_t data = {};

            printf(YELLOW "HUY: %d\n" DELETE_COLOR, node->type );

            switch( (int)node->data.op )
            {
                case kAdd:
                {
                    data.num = node->left->data.num + node->right->data.num;
                    break;
                }
                case kSub:
                {
                    data.num = node->left->data.num - node->right->data.num;
                    break;
                }
                case kMul:
                {
                    data.num = node->left->data.num * node->right->data.num;
                    break;
                }
                case kDiv:
                {
                    data.num = node->left->data.num / node->right->data.num;
                    break;
                }
                case kDeg:
                {
                    data.num = pow( node->left->data.num, node->right->data.num );
                    break;
                }
                case kSin:
                {
                    data.num = sin( node->right->data.num ); //TODO: пересмотреть 
                    break;
                }
                case kCos:
                {
                    data.num = cos( node->right->data.num ); //TODO: пересмотреть 
                    break;
                }
                case kTg:
                {
                    data.num = tan( node->right->data.num ); //TODO: пересмотреть 
                    break;
                }
                case kCtg:
                {
                    data.num = pow( tan( node->right->data.num ), -1 ); //TODO: пересмотреть 
                    break;
                }
                case kLn:
                {
                    data.num = log( node->right->data.num ); 
                    break;
                }
                case kLog:
                {
                    data.num = log( node->left->data.num ) / log( node->right->data.num ); 
                    break;
                }
                case kExp:
                {
                    data.num = exp( node->right->data.num ); 
                    break;
                }
                default:
                {
                    printf(RED "unknown problem in %s in %s on line %d " DELETE_COLOR, __FILE__, __PRETTY_FUNCTION__, __LINE__ );
                    exit(0);
                    break;
                }
            }

            NodeSimplifie( src, node, data );
        }
    }

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

void NodeSimplifie( struct SimpleSrc* src, struct Node_t* node, union Data_t data )
{
        printf(YELLOW "Simplifying\n" DELETE_COLOR );
        struct Node_t* tmp_node = nullptr;
        tmp_node = CreateNode( src->tree, data, NUM );
        ReplaceNode( node, tmp_node );

        src->change_count = CHANGE;
}
