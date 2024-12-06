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

struct FileOutput
{
    FILE* stream;
    char filepath[200];
};

enum TexErrors TexOutput( struct Tree* tree );
enum TexErrors StertTexOutput( struct FileOutput* src ); 
enum TexErrors FinishTexOutput( struct FileOutput* src );
enum TexErrors TexWrite( struct FileOutput* src, struct Tree* tree  );

#endif