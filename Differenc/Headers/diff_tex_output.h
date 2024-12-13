#include "diff_functions.h"

#ifndef DIFFERENCIATOR_OUTPUT_HEADER
#define DIFFERENCIATOR_OUTPUT_HEADER

const char latex_src_dir[100] = "/home/vyacheslav/Differenciator/Differenc/diff_Latex_src";
const char latex_dump_filename[100] = "LaTexDump.tex";

enum TexErrors
{
    GOOD = 65,
    BAD = 66,

    GOOD_OPEN = 77,
    BAD_OPEN = 76,
};

enum OpPlace 
{
    BEG,
    MID,
    END,
};

enum Arity
{
    NULLNARY = 0,
    UNARY = 1,
    BINARY = 2,
    TERNARY = 3,
};

enum NeedBrace
{
    NEED = 69,
    NO_NEED = 70,
};

struct FileOutput
{
    FILE* stream;
    char filepath[200];
};

enum TexErrors TexOutput( struct Tree* tree );
enum TexErrors StertTexOutput( struct FileOutput* src ); 
enum TexErrors FinishTexOutput( struct FileOutput* src );
enum TexErrors TexWrite( struct FileOutput* src, Node_t* node, char* statement );

struct OpNames* FindOperation( enum Operations op );

void OpenBrace( char* statement, Node_t* node, enum NeedBrace* status );
void CloseBrace( char* statement, Node_t* node, enum NeedBrace* status );
void DoesNeedBraces( Node_t* node, enum NeedBrace* status );

#endif
