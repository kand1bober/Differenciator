#include <sys/stat.h>

#include "tree_output.h"
#include "tree_functions.h"

#ifndef TREE_INPUT_HEADER
#define TREE_INPUT_HEADER

struct ParserSrc
{
    int p;
    int old_p;
    char s[200];
    struct Tree* tree;
};

enum TreeErrors MakeTreeData( struct File_text* dump, struct File_text* file, struct Tree* tree );
void NodeFromData( struct File_text* dump, struct Tree* tree, struct Node_t* node, struct Parser* utility );
void InputFileNameChange( void );

//-------------PARSER BY DED----------------
void GetG( struct ParserSrc* data );
Node_t* GetV( struct ParserSrc* data );
Node_t* GetE( struct ParserSrc* data );
Node_t* GetT( struct ParserSrc* data );
Node_t* GetP( struct ParserSrc* data );
Node_t* GetN( struct ParserSrc* data );
//----------------------------------------------

#endif
