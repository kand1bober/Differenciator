
#include "../Headers/diff_tex_output.h"


enum TexErrors TexOutput( struct Tree* tree )
{
    struct FileOutput src = {};

    StertTexOutput( &src );

    TexWrite( &src, tree->root );

    FinishTexOutput( &src );

    return GOOD;
}

enum TexErrors StertTexOutput( struct FileOutput* src )
{
    sprintf( src->filepath, "%s/%s", latex_src_dir, latex_dump_filename );

    src->stream = fopen( src->filepath, "w+");
    if( src->stream == nullptr )
    {
        printf(RED "No such file, creating new\n" DELETE_COLOR);
    }

    fprintf( src->stream, 
    "\\documentclass[12pt, letterpaper]{article}\n"
    "\\usepackage{amsmath}\n"
    "\\title{My first LaTeX document}\n"
    "\\author{Vyacheslav Kurnevich}\n"
    "\\date{November 2024}\n"
    "\\begin{document}\n"
    "\\maketitle\n"
    "First try using \\LaTeX{}\n"
    );

    return GOOD;
}

enum TexErrors TexWrite( struct FileOutput* src, struct Node_t* node )
{   
    char statement[50] = "";

    switch( (int)node->type )
    {
        case NUM:
        {

            break;
        }

        case VAR:
        {

            break;
        }
        
        case OP:
        {

            break;
        }
    }

    fprintf(src->stream, "%s", "huy\n" );

    if( node->left != nullptr )
    {
        TexWrite( src, node->left );
    }

    if( node->right != nullptr )
    {
        TexWrite( src, node->right );
    }

    return GOOD;    
}

enum TexErrors FinishTexOutput( struct FileOutput* src )
{   
    fprintf( src->stream, "\\end{document}\n");
    fclose( src->stream );

    char command[300] = "";
    sprintf( command, "pdflatex -output-directory=%s %s", latex_src_dir, src->filepath );

    system( command );

    return GOOD;
}

