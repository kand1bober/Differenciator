#include <math.h>

#ifndef DIFFERENCIATOR_SIMPLIFIER
#define DIFFERENCIATOR_SIMPLIFIER

const int NO_CHANGE = 57;
const int CHANGE = 58;

struct SimpleSrc
{
    int change_count;
    struct Tree* tree;
};

void TreeSimplifie( struct Tree* tree );
void VarSimplifie( struct SimpleSrc* src, struct Node_t* node );
void NumSimplifie( struct SimpleSrc* src, struct Node_t* node );
void OpSimplifie( struct SimpleSrc* src, struct Node_t* node );
void NodeSimplifie( struct SimpleSrc* src, struct Node_t* node, union Data_t data );

#endif