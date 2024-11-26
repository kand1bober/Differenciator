#include <sys/stat.h>

#include "tree_output.h"
#include "tree_functions.h"

#ifndef TREE_INPUT_HEADER
#define TREE_INPUT_HEADER

struct File_input
{
    int size_of_file;
    char* buffer;
    int lines_amount;
};

struct Parser
{
    char* string;                   // целевая строка
    char* track_1= nullptr;        // внешние скобки
    char* track_2 = nullptr;      // внутренние кавычки 
    const char* delim_1;         // просто пропускаем 
    const char* delim_2;        // каллочим, если не "nuull" или придумать другой пойзон
    const char* delim_3;
    char* result_1 = nullptr; // строка с кавычками 
    char* result_2 = nullptr;// целевая строка
};

enum TreeErrors MakeTreeData( struct File_text* dump, struct File_input* file, struct Tree* tree );
void NodeFromData( struct File_text* dump, struct Tree* tree, struct Node_t* node, struct Parser* utility );
void InputFileNameChange( void );

#endif
