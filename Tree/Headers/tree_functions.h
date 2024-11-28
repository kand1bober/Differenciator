#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "tree_configuration.h"
#include "tree_macros.h"
#include "tree_decoration.h"
#include "tree_utilities.h"

#ifndef TREE_FUNCTIONS_HEADER
#define TREE_FUNCTIONS_HEADER

const int STRING_SIZE = 100;
const int STRING_ARRAY_SIZE = 100;
const uint64_t POISON = 0xBADBABA;

// typedef char TreeElem[STRING_SIZE]; // строка

//--------NODE---------
enum Node_types
{
    NUM = 1,
    VAR = 2,
    OP = 3,
};

enum Operations
{
    k_Add = '+',
    k_Sub = '-',
    k_Mul = '*',
    k_Div = '/',
};

struct SmartString
{
    char string[STRING_SIZE];
    int links_amount;
};

union Data_t
{
    double num;
    char* var; //только указатель на нужную строку
    Operations op;
};  

struct Node_t
{   
    union Data_t data;
    enum Node_types type; 

    Node_t* left;
    Node_t* right;
    Node_t* parent;
};
//--------------------

//--------TREE-----------
enum TreeErrors
{
    GOOD_TREE,
    BAD_TREE,

    GOOD_PRINT,
    BAD_PRINT,

    GOOD_OUTPUT,
    BAD_OUTPUT,

    GOOD_START_OUTPUT,
    BAD_START_OUTPUT,

    GOOD_FINISH_OUTPUT,
    BAD_FINISH_OUTPUT,

    GOOD_CTOR,
    BAD_CTOR,

    GOOD_DTOR,
    BAD_DTOR,

    GOOD_CREATE,
    BAD_CREATE,

    FOUND_EMPTY_STRING,
    NOT_FOUND_EMPTY_STRING,
    SAME_STRING_EXISTS,
    SAME_STRING_NOT_EXISTS,
    GOOD_STRING_DELETE,
    BAD_STRING_DELETE,

    GOOD_INSERT,
    BAD_INSERT,

    GOOD_EXTRACT,
    BAD_EXTRACT,

    GOOD_DELETE,
    BAD_DELETE,

    GOOD_FIND,
    BAD_FIND,

    GOOD_DATA,
    BAD_DATA,

    GOOD_INPUT,
    BAD_INPUT
};

enum Direction
{
    LEFT = 1,
    RIGHT = 2,
    NO_BRANCH = 3,
};

struct Tree 
{
    union Data_t data;
    enum Node_types type;

    Node_t* root;

    SmartString variables[STRING_ARRAY_SIZE]; //база данных строк(переменных) и кол-ввом ссылок на неё
    TreeErrors status;
};
//----------------------

//-------------------------------OPERATIONS WITH TREE------------------------------------
enum TreeErrors TreeCtor( struct Tree* tree );
enum TreeErrors TreeDtor( struct Tree* tree );
void FreeTree( struct Tree* tree, struct Node_t* node ); //-------- recursive

//--------------------------------OPERATIONS WITH NODES----------------------------------
enum TreeErrors CreateNode( struct Tree* tree, Data_t data, struct Node_t** new_node, enum Node_types type );
enum TreeErrors CreateNumNode( struct Tree* tree, double number, struct Node_t** new_node );
enum TreeErrors CreateOpNode( struct Tree* tree, enum Operations operation, struct Node_t** new_node );
enum TreeErrors CreateVarNode( struct Tree* tree, char* variable, struct Node_t** new_node );

enum TreeErrors NodeDelete( struct Tree* tree, struct Node_t* node, enum Node_types node_type );
enum TreeErrors InsertNode( struct Node_t* left, struct Node_t* right, struct Node_t* node );
enum TreeErrors InsertLeave( struct Tree* tree, struct Node_t* parent, enum Direction branch, struct Node_t* to_connect );

//----------------------------FIND FUNCTIONS----------------------------------------
enum TreeErrors Find( struct Tree* tree, union Data_t to_find, struct Node_t** answer, enum Node_types node_type ); //TODO: переделать 
enum TreeErrors FindNumNode( struct Node_t* node_search, double to_find, struct Node_t** answer );
enum TreeErrors FindOpNode( struct Node_t* node_search, enum Operations to_find, struct Node_t** answer );
enum TreeErrors FindVarNode( struct Node_t* node_search, char* to_find, struct Node_t** answer );

//---------------------------- STRING FUCTIONS ------------------------------------------
enum TreeErrors FindString( struct Tree* tree, char* to_find, int* string_position );
enum TreeErrors FindSameString( struct Tree* tree, char* to_find, int* string_position );
enum TreeErrors FindEmptyString( struct Tree* tree, int* string_position );
enum TreeErrors DeleteString( struct Tree* tree, char* string );
void StringDump( struct Tree* tree );

#endif
