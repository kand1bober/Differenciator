/*

#include "../Headers/tree_tex_output.h"


enum TreeErrors Output( struct File_text* file, struct Tree* tree )
{
    StartOutput( file );

    Print( file, tree );

    FinishOutput( file );

    return GOOD_OUTPUT;
}


enum TreeErrors StartOutput( struct File_text* file )
{
    char code_filepath[200] = {};
    GetFilePath( code_filepath, output_graph_name );
    char html_filepath[200] = {};
    GetFilePath( html_filepath, output_html_name );
    char image_filepath[200] = {};
    GetFilePath( image_filepath, output_image_name );

    // printf("Name of output file with code: %s\n", code_filepath); 

    //---------------GRAPH FILE------------------
    file->stream = fopen(code_filepath, "w");
    //-------------------------------------------

    //---------------------Write node settings------------------------
    fprintf(file->stream, "digraph G\n{\nlabel=\"%s\";\nlabelloc=\"t\";\nfontsize=30\nfontname=\"%s\";\nfontcolor=\"%s\"\n"
    "\nrankdir=TB;size=\"200,300\";bgcolor=\"%s\";\n", graph_header, fontname, fontcolor, bgcolor); //splines=polyline
    //----------------------------------------------------------------

    
    //--------------------------- HTML FIlE --------------------------
    static int count_files = 0;
    switch( count_files )
    {
        case 0:
        {
            FILE* html_stream = fopen( html_filepath, "w" );
            fprintf(html_stream, "<img src=\"%s\"  alt=\"MyGraph\" width=\"1300px\" height=\"900px\">", image_filepath);
            fclose( html_stream );
            
            break;
        }
        case 1:
        {
            FILE* html_stream = fopen( html_filepath, "a" );
            fprintf(html_stream, "<img src=\"%s\"  alt=\"MyGraph\" width=\"1300px\" height=\"900px\">", image_filepath);
            fclose( html_stream );      
            break;
        }
        default:
        {
            printf(RED "The case of this file is not described\n" DELETE_COLOR);
            break;
        }
        count_files++;
    }
    //----------------------------------------------------------------

    return GOOD_START_OUTPUT;
}


enum TreeErrors FinishOutput( struct File_text* file )
{
    fprintf(file->stream, "\n}\n");
    char cmd[256] = {};
    snprintf(cmd, sizeof(cmd), "dot -Tsvg %s%s > %s%s", project_directory, output_graph_name, project_directory, output_image_name);
    ON_DEBUG( printf(SINIY "command for graphviz: " YELLOW "%s\n" DELETE_COLOR, cmd); )

    fclose(file->stream);

    system( cmd );

    return GOOD_FINISH_OUTPUT;
}

*/