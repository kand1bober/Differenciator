
#include "../../Tree/MyTreeLib.h"

#ifndef DIFFERENCIATOR_FUNCTIONS_HEADER
#define DIFFERENCIATOR_FUNCTIONS_HEADER

enum DiffInfo
{
    GOOD_DIFF = 234,
    BAD_DIFF = 235,
};

enum DiffInfo Run();
enum DiffInfo Differentiate( struct Tree* origin_tree, struct Tree* diff_tree );
Node_t* MakeDifferentiation( struct Tree* diff_tree, Node_t* origin_node );

enum Node_types SubtreeType( Node_t* node );

#endif