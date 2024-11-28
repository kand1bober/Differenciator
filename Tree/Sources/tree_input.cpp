#include "../Headers/tree_input.h"
#include "../Headers/tree_functions.h"

int p = 0;
int old_p = 0;
char s[200] = "";

//---------------------------------------------------------------------------------------
enum TreeErrors MakeTreeData( struct File_text* dump, struct File_text* file, struct Tree* tree )
{
    ON_DEBUG( printf(RED "====== START MakeTreeData ======\n" DELETE_COLOR); )

    //-------------------FILE WORK-------------------
    char filepath[256] = {};
    GetFilePath( filepath, tree_data_name_old);
    FILE* stream = fopen( filepath, "r" );

    struct stat file_info = {};
    stat( filepath, &file_info );
    unsigned long int size_of_stream = (unsigned long int)file_info.st_size;
    file->stream_size = size_of_stream;

    if( file->stream_size > 0 )
    {
        file->buffer = (char*)calloc( size_of_stream + 1, sizeof(char) );
        fseek(stream, sizeof(char) * 0L,  SEEK_SET);
        fread( (void*)file->buffer, sizeof(char), size_of_stream, stream);

        strcpy( s, file->buffer );
        printf("%s\n\n", s);

        int val = 0;
        val = GetG();

        printf(YELLOW "val: %d\n" DELETE_COLOR, val );


        fclose(stream);
        free( file->buffer );
    }

    return GOOD_INPUT;
}
//---------------------------------------------------------------------------------------

int GetG()
{
    int val = GetE();
    
    if( s[p] != '$' )
    {
        printf(RED "Syntax error in GetG\n" DELETE_COLOR);
        exit(0);
    }
    p++;
    return val;
}

int GetE()
{
    int val = GetT();
 
    while( (s[p] == '+') || (s[p] == '-') )
    {
        int op = p;
            printf(" p: %d OP: '%d'\n", p, s[op] );
        p++;
        int val2 = GetT();
        if( s[op] == '+' )
        {
            printf("va1: %d val2: %d \n", val, val2 );
            val += val2;
        }
        else
        {
            printf("va1: %d val2: %d \n", val, val2 );
            val -= val2;
        }
    }

    return val;
}

int GetT( )
{
    int val = GetP();

    while( s[p] == '*' || s[p] == '/' )
    {
        int op = p;
        p++;
        int val2 = GetP();
        if( s[op] == '*' )
        {
            val *= val2;
        }
        else 
        {
            val /= val2;
        }
    }
    return val;
}

int GetP()
{
    if( s[p] == '(' )
    {
        p++;
        int val = GetE();
        if( s[p] != ')' )
        {
            printf(RED "Syntax error in GetP\n" DELETE_COLOR);
            exit(0);
        }
        p++;
        return val;
    }
    else 
    {
        return GetN();
    }
}

int GetN()
{
    int val = 0;
    old_p = p;

    while( '0' <= s[p] && s[p] <= '9' )
    {   
        val = val*10 + s[p] - '0';
        p++;
    }
    assert( old_p != p );
    return val;
}


//---------------------------------------------------------------------------------------
void InputFileNameChange( void )
{
    ON_DEBUG( printf(YELLOW "====== Start Changing Names ======\n" DELETE_COLOR); )
    char tmp_name[20] = "tmp.txt";

    char command_1[300] = "";
    sprintf(command_1, "mv %s %s", tree_data_name_old, tmp_name);
    system( command_1 );
    ON_DEBUG( printf(YELLOW "%s\n" DELETE_COLOR, command_1); )

    char command_2[300] = "";
    sprintf( command_2, "mv %s %s", tree_data_name_new, tree_data_name_old);
    system( command_2 );
    ON_DEBUG( printf(YELLOW "%s\n" DELETE_COLOR, command_2); )

    char command_3[300] = "";
    sprintf( command_3, "mv %s %s", tmp_name, tree_data_name_new);
    system( command_3 );
    ON_DEBUG( printf(YELLOW "%s\n" DELETE_COLOR, command_3); )

    ON_DEBUG( printf(YELLOW "====== Finished Changing Names ======\n" DELETE_COLOR); )
}
