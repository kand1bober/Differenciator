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
    union Data_t data;
};

void TreeSimplifie( struct Tree* tree );
void VarSimplifie( struct SimpleSrc* src, struct Node_t* node );
void NumSimplifie( struct SimpleSrc* src, struct Node_t* node );
void OpSimplifie( struct SimpleSrc* src, struct Node_t* node );
void NodeSimplifie( struct SimpleSrc* src, struct Node_t** node );

enum TreeErrors ReplaceNode( struct Tree* tree, struct Node_t** to_replace, struct Node_t** src, struct SimpleSrc* simple_status );

#endif