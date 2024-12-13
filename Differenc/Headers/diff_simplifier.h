#include <math.h>

#include "diff_functions.h"

#ifndef DIFFERENCIATOR_SIMPLIFIER
#define DIFFERENCIATOR_SIMPLIFIER

const int NO_CHANGE = 57;
const int CHANGE = 58;

struct SimpleSrc
{
    int change_count;

    struct Tree* tree;
    enum Node_types type;
    Data_t data;
};

void TreeSimplifie( struct Tree* tree );
void VarSimplifie( struct SimpleSrc* src, Node_t* node );
void NumSimplifie( struct SimpleSrc* src, Node_t* node );
void OpSimplifie( struct SimpleSrc* src, Node_t* node );
void NodeSimplifie( struct SimpleSrc* src, Node_t** node );

enum TreeErrors ReplaceNode( Node_t** to_replace, Node_t** new_node, struct SimpleSrc* src );

#endif