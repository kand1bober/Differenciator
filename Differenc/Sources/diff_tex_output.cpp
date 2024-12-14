
#include "../Headers/diff_tex_output.h"

struct OpNames
{
    enum Operations op;
    char name[10];

    enum OpPlace pos;
    int arity;
};

enum BraceAction
{
    kOpen = 77,
    kClose = 78,
};

const int OP_ARR_SIZE = 13;

//======== Инструкция обхода дерева ========
static struct OpNames list[OP_ARR_SIZE] = 
{
    kNoOp, ".",      MID, NULLNARY,
    kAdd,  "+",      MID, BINARY,
    kSub,  "-",      MID, BINARY,
    kMul,  "\\cdot", MID, BINARY,
    kDiv,  "\\frac", BEG, BINARY,
    kDeg,  "^",      MID, BINARY,
    kSin,  "\\sin",  BEG, UNARY,
    kCos,  "\\cos",  BEG, UNARY,
    kTg,   "\\tan",  BEG, UNARY,
    kCtg,  "\\cot",  BEG, UNARY,
    kLog,  "\\log",  BEG, BINARY,
    kLn,   "\\ln",   BEG, UNARY,
    kExp,  "\\exp",  BEG, UNARY,
};


enum TexErrors TexOutput( struct Tree* origin_tree, struct Tree* diff_tree )
{
    struct FileOutput src = {};

    StertTexOutput( &src );

    char statement[1000] = "";

    sprintf( statement + strlen( statement ), "\\section{Входные данные:}\n"
                                              "\\begin{center}\n"
                                              "Функция:\n$$\n");
    TexWrite( &src, origin_tree->root, statement );
    sprintf( statement + strlen( statement ), "$$\n"
                                              "\\end{center}\n" );

    sprintf( statement + strlen( statement ), "\\section{Ответ:}\n"
                                              "\\begin{center}\n"
                                              "Производная:\n$$\n");                           
    TexWrite( &src, diff_tree->root, statement );
    sprintf( statement + strlen( statement ), "$$\n"
                                              "\\end{center}\n");

    sprintf( statement + strlen( statement ), "\\section{Вывод:}\n"
                                              "Лень думать, я пошёл спать\n");

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
    "\\usepackage[russian]{babel}\n"
    "\\title{Лабораторная работа на тему\nвзятия приозводной}\n"
    "\\author{Вячеслав Курневич}\n"
    "\\date{Декабрь 2024}\n"
    "\\begin{document}\n"
    "\\maketitle\n"
    );

    return GOOD;
}


enum TexErrors TexWrite( struct FileOutput* src, Node_t* node, char* statement )
{   
    switch( (int)node->type )
    {
        case NUM:
        {
            sprintf( statement + strlen( statement ), "%.2lf", node->data.num );

            break;
        }

        case VAR:
        {
            sprintf( statement + strlen( statement ), "%s", node->data.var );
            
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
                        sprintf( statement  + strlen( statement ), "%s", op->name );

                        switch( op->arity )
                        {
                            case UNARY:
                            {
                                if( node->right != nullptr )
                                {
                                    enum NeedBrace brace_status = NO_NEED;
                                    OpenBrace( statement, node->right, &brace_status );
                                    TexWrite( src, node->right, statement );
                                    CloseBrace( statement, node, &brace_status );
                                }

                                break;
                            }
                            case BINARY:
                            {
                                if( node->left != nullptr )
                                {
                                    enum NeedBrace brace_status = NO_NEED;
                                    OpenBrace( statement, node->left, &brace_status );
                                    TexWrite( src, node->left, statement );
                                    CloseBrace( statement, node, &brace_status );
                                }                        

                                if( node->right != nullptr )
                                {
                                    enum NeedBrace brace_status = NO_NEED;
                                    OpenBrace( statement, node->right, &brace_status );
                                    TexWrite( src, node->right, statement );
                                    CloseBrace( statement, node, &brace_status );
                                }

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

                    //already binary
                    case MID:
                    {
                        if( node->left != nullptr )
                        {
                            enum NeedBrace brace_status = NO_NEED;
                            OpenBrace( statement, node->left, &brace_status );
                            TexWrite( src, node->left, statement );
                            CloseBrace( statement, node, &brace_status );
                        }

                        sprintf( statement + strlen( statement ), "%s", op->name );

                        if( node->right != nullptr )
                        {
                            enum NeedBrace brace_status = NO_NEED;
                            OpenBrace( statement, node->right, &brace_status );
                            TexWrite( src, node->right, statement );
                            CloseBrace( statement, node, &brace_status );
                        }

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
                printf(RED "No such operation\n file: %s\n func: %s\n line: %d\n" DELETE_COLOR, __FILE__, __PRETTY_FUNCTION__, __LINE__  );
                exit(0);
            }

            break;
        }
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

void OpenBrace( char* statement, Node_t* node, enum NeedBrace* status )
{
    sprintf( statement + strlen( statement ), "{" );

    DoesNeedBraces( node, status );

    if( *status == NEED ) 
    {
        sprintf( statement + strlen( statement ), "(" );
    }
}

void CloseBrace( char* statement, Node_t* node, enum NeedBrace* status )
{
    if( *status == NEED ) 
    {
        sprintf( statement + strlen( statement ), ")" );
    }

    sprintf( statement + strlen( statement ), "}" );
}

void DoesNeedBraces( Node_t* node, enum NeedBrace* status )  //Анализ ветки
{
    switch( (int)node->type )
    {
        case NUM:
        {
            return;
        }   

        case VAR:
        {
            break;
        }
        case OP:
        {
            *status = NEED;
            return;
        }

        default:
        {
            printf(RED "Smth strange in\n file: %s\n func: %s\n line: %d\n" DELETE_COLOR, __FILE__, __PRETTY_FUNCTION__, __LINE__  );
            break;
        }
    }

    return;
}
