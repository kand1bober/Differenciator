#include "../Headers/tree_output.h"

int CheckSize_t( size_t number )
{
    if( number < 1000000 )
        return 1;
    else
        return -1;
}


char* GetFilePath( char* filepath, const char* filename )
{
    const char* directory = "/home/vyacheslav/Differenciator/Tree/";
    sprintf(filepath, "%s%s", directory, filename);

    return filepath;
}

void MakeOffset( struct File_text* file, int count )
{
    for(int i = 0; i < count; i++)
    {
        fprintf(file->stream, "\t");
    }
}
