
#include "../Headers/diff_functions.h"
#include <cstdio>
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
    
    else if( node != nullptr && node->left != nullptr && node->right != nullptr )
    {   
        // Left smth mult on right 1
        if( node->type == OP && ( node->data.op == kMul || node->data.op == kDiv ) && ( node->right->type == NUM && fabs( node->right->data.num - 1) < EPSILON ) )
        {
            // printf(YELLOW "Simplifiying mult on right 1\n" DELETE_COLOR);
            ReplaceNode( src->tree, &node, &(node->left) );
        } 
        //Right smth mult on left 1
        else if( node->type == OP && ( node->data.op == kMul || node->data.op == kDiv ) && ( node->left->type == NUM && fabs( node->left->data.num - 1) < EPSILON ) ) 
        {
            // printf(YELLOW "Simplifiying mult on left 1\n" DELETE_COLOR);
            ReplaceNode( src->tree, &node, &(node->right) );
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

void NodeSimplifie( struct SimpleSrc* src, struct Node_t** node )
{
    // ON_DEBUG( printf(YELLOW "Simplifying\n" DELETE_COLOR ) );

    struct Node_t* tmp_node = nullptr;
    tmp_node = CreateNode(src->tree, NULL, NULL, NULL, src->data, src->type);
    ReplaceNode( src->tree, node, &tmp_node );
    src->change_count = CHANGE;

    // ON_DEBUG( printf("%p res: %lf\n", node, src->data.num); );

    return;
}
