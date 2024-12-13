#include <sys/stat.h>

#include "tree_dot_output.h"
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
void NodeFromData( struct File_text* dump, struct Tree* tree, Node_t* node, struct Parser* utility );
void InputFileNameChange( void );

//-------------PARSER BY DED----------------
void GetG( struct ParserSrc* src );
Node_t* GetE( struct ParserSrc* src );
Node_t* GetT( struct ParserSrc* src );
Node_t* GetD( struct ParserSrc* src );
Node_t* GetSL( struct ParserSrc* src );
Node_t* GetP( struct ParserSrc* src );
Node_t* GetN( struct ParserSrc* src );
Node_t* GetV( struct ParserSrc* src );
//----------------------------------------------

#endif
