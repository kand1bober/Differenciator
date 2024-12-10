
#include "../Headers/diff_tex_output.h"

struct OpNames
{
    enum Operations op;
    char name[10];

    enum OpPlace pos;
    int arity;
};

const int OP_ARR_SIZE = 13;

//======== Инструкция обхода дерева ========
static struct OpNames list[OP_ARR_SIZE] = 
{
    kNoOp, ".",      MID, 0,
    kAdd,  "+",      MID, 2,
    kSub,  "-",      MID, 2,
    kMul,  "\\cdot", MID, 2,
    kDiv,  "\\frac", BEG, 2,
    kDeg,  "^",      MID, 2,
    kSin,  "\\sin",  BEG, 1,
    kCos,  "\\cos",  BEG, 1,
    kTg,   "\\tan",  BEG, 1,
    kCtg,  "\\cot",  BEG, 1,
    kLog,  "\\log",  BEG, 2,
    kLn,   "\\ln",   BEG, 1,
    kExp,  "\\exp",  BEG, 1,
};


enum TexErrors TexOutput( struct Tree* tree )
{
    struct FileOutput src = {};

    StertTexOutput( &src );

    char statement[200] = "";
    TexWrite( &src, tree->root, statement );
    fprintf( src.stream, "%s", statement );

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
    "\\documentclass[12pt, a4paper]{article}\n"
    "\\usepackage{amsmath}\n"
    "\\title{Laboratory work on the topic of\ntaking a derivative}\n"
    "\\author{Vyacheslav Kurnevich}\n"
    "\\date{November 2024}\n"
    "\\begin{document}\n"
    "\\maketitle\n"
    "\\["
    );

    return GOOD;
}

enum TexErrors TexWrite( struct FileOutput* src, struct Node_t* node, char* statement )
{   
    // printf(YELLOW "__%s__\n\n" DELETE_COLOR, statement );

    switch( (int)node->type )
    {
        case NUM:
        {
            sprintf( statement + strlen( statement ), "%.2lf", node->data.num );

            if( node->left != nullptr )
            {
                TexWrite( src, node->left, statement );
            }

            if( node->right != nullptr )
            {
                TexWrite( src, node->right, statement );
            }

            break;
        }

        case VAR:
        {
            sprintf( statement + strlen( statement ), "%s", node->data.var );

            if( node->left != nullptr )
            {
                TexWrite( src, node->left, statement );
            }

            if( node->right != nullptr )
            {
                TexWrite( src, node->right, statement );
            }

            break;
        }
        
        case OP:
        {   
            struct OpNames* op = nullptr;

            if( ( op = FindOperation( node->data.op ) ) != NULL )
            {
                switch( (int)op->pos )
                {
                    case BEG:
                    {
                        sprintf( statement  + strlen( statement ), "%s"
                                            "{(", op->name );

                        switch( op->arity )
                        {
                            case UNARY:
                            {
                                if( node->right != nullptr )
                                {
                                    TexWrite( src, node->right, statement );
                                }

                                sprintf( statement + strlen( statement ), ")}" );

                                break;
                            }
                            case BINARY:
                            {
                                if( node->left != nullptr )
                                {
                                    TexWrite( src, node->left, statement );
                                }

                                sprintf( statement + strlen( statement ), ")}" );
                                sprintf( statement + strlen( statement ), "{(" );

                                if( node->right != nullptr )
                                {
                                    TexWrite( src, node->right, statement );
                                }

                                sprintf( statement + strlen( statement ), ")}" );

                                break;
                            }
                            default:
                            {
                                printf( RED "No such operation\n file: %s\n func: %s\n line: %d\n" DELETE_COLOR, __FILE__, __PRETTY_FUNCTION__, __LINE__  );
                                exit(0);
                                break;
                            }
                        }
                                            
                        break;
                    }   

                    case MID:
                    {
                        sprintf( statement + strlen( statement ), "{(" );

                        if( node->left != nullptr )
                        {
                            TexWrite( src, node->left, statement );
                        }

                        sprintf( statement + strlen( statement ), ")}" );

                        sprintf( statement + strlen( statement ), "%s"
                                            "{(", op->name );

                        if( node->right != nullptr )
                        {
                            TexWrite( src, node->right, statement );
                        }

                        sprintf( statement + strlen( statement ), ")}" );

                        break;
                    }
                    default:
                    {
                        printf( RED "\n" DELETE_COLOR );
                        exit(0);
                        break;
                    }
                }
            }
            else 
            {
                printf( RED "No such operation\n file: %s\n func: %s\n line: %d\n" DELETE_COLOR, __FILE__, __PRETTY_FUNCTION__, __LINE__  );
                exit(0);
            }

            break;
        }
    }

    return GOOD;    
}

enum TexErrors FinishTexOutput( struct FileOutput* src )
{   
    fprintf( src->stream, "\\]\n"
                          "\\end{document}\n");

    fclose( src->stream );

    char command[300] = "";
    sprintf( command, "pdflatex -output-directory=%s %s", latex_src_dir, src->filepath );

    system( command );

    return GOOD;
}

struct OpNames* FindOperation( enum Operations op )
{
    for( int i = 0; i < OP_ARR_SIZE; i++ )
    {
        if( list[i].op == op )
        {
            return &list[i];
        }
    }
    
    return nullptr;
}
