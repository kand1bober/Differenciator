#include <sys/stat.h>

#include "tree_output.h"
#include "tree_functions.h"

#ifndef TREE_INPUT_HEADER
#define TREE_INPUT_HEADER

enum TreeErrors MakeTreeData( struct File_text* dump, struct File_text* file, struct Tree* tree );
void NodeFromData( struct File_text* dump, struct Tree* tree, struct Node_t* node, struct Parser* utility );
void InputFileNameChange( void );

//-------------PARSER BY DED----------------
int GetG();
int GetN();
int GetE();
int GetT();
int GetP();
//----------------------------------------------

#endif
