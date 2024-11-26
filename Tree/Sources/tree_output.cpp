#include "../Headers/tree_functions.h"
#include "../Headers/tree_output.h"
#include "../Headers/tree_input.h"

//--------------------------DUUUUUUMP--------------------------------
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
    char html_filepath[200] = {};
    char image_filepath[200] = {};
    GetFilePath( image_filepath, output_image_name );
    GetFilePath( code_filepath, output_graph_name );
    GetFilePath( html_filepath, output_html_name );

    // printf("Name of output file with code: %s\n", code_filepath); 

    //============================== GRAPH FILE ====================================
    file->stream = fopen(code_filepath, "w");
    fprintf(file->stream, "digraph G\n{\nlabel=\"%s\";\nlabelloc=\"t\";\nfontsize=30\nfontname=\"%s\";\nfontcolor=\"%s\"\n"
    "\nrankdir=TB;size=\"200,300\";bgcolor=\"%s\";\n", graph_header, fontname, fontcolor, bgcolor); //splines=polyline
    //==============================================================================

    
    //============================== HTML FIlE =====================================
    FILE* html_stream = fopen( html_filepath, "w" );
    fprintf(html_stream, "<img src=\"%s\"  alt=\"MyGraph\" width=\"1300px\" height=\"900px\">", image_filepath);
    fclose( html_stream );
    //==============================================================================

    return GOOD_START_OUTPUT;
}


enum TreeErrors FinishOutput( struct File_text* file )
{
    fprintf(file->stream, "\n}\n");
    char cmd[256] = {};
    snprintf(cmd, sizeof(cmd), "dot -Tsvg %s > %s", output_graph_name, output_image_name);
    ON_DEBUG( printf(SINIY "command for graphviz: " YELLOW "%s\n" DELETE_COLOR, cmd); )

    fclose(file->stream);

    system( cmd );

    return GOOD_FINISH_OUTPUT;
}


enum TreeErrors Print( struct File_text* file, struct Tree* tree )
{   
    WriteEdge( file, tree->root );

    return GOOD_PRINT;
}


void WriteEdge( struct File_text* file, struct Node_t* node )
{
    struct Node_t* left = node->left;
    struct Node_t* right = node->right;

    int node_type = node->type;
    // switch( node_type )
    // {
    //     case NUM:
    //     {

    //         break;
    //     }
    //     case VAR:
    //     {

    //         break;
    //     }
    //     case OP:
    //     {

    //         break;
    //     }
    //     default:
    //     {

    //         break;
    //     }
    // }

    fprintf(file->stream, " node_%p [shape=record,style=\"rounded,filled\",fillcolor=\"%s\",color=\"%s\",label=\" { { <curr%p> curr: %p } | { <parent%p> parent: %p } | { <data%p> data: %s} | { { <left%p> L: %p } | { <right%p> R: %p } } } \" ]; ",
                            node, fillcolor, default_pointer_color, node, node, node, node->parent, node, node->data.variable, node, left, node, right );

    if( (node->left) != nullptr )
    {
        fprintf(file->stream, " node_%p [shape=record,style=\"rounded,filled\",fillcolor=\"%s\",color=\"%s\",label=\" { { <curr%p> curr: %p } { <parent%p> parent: %p } | | { <data%p> data: %s} | { { <left%p> L: %p } | { <right%p> R: %p } } } \" ];",
        left, fillcolor, default_pointer_color, left, left, left, left->parent, left, left->data.variable, left, left->left, left, left->right);

        fprintf(file->stream, "node_%p: <left%p> -> node_%p [color = \"%s\", arrowsize = 1] ;\n", node, node, left, pointer_left_color );    
        WriteEdge( file, node->left );
    }

    if( (node->right) != nullptr )
    {
        fprintf(file->stream, " node_%p [shape=record,style=\"rounded,filled\",fillcolor=\"%s\",color=\"%s\",label=\" { { <curr%p> curr: %p } { <parent%p> parent: %p } | | { <data%p> data: %s} | { { <left%p> L: %p } | { <right%p> R: %p } } } \" ];",
        right, fillcolor, default_pointer_color, right, right, right, right->parent, right, right->data.variable, right, right->left, right, right->right );

        fprintf(file->stream, "node_%p: <right%p> -> node_%p [color = \"%s\", arrowsize = 1] ;\n", node, node, right, pointer_right_color );    
        WriteEdge( file, node->right );
    }

    return;
}
//------------------------------------------------------------






//-------------------- TREE DATA -----------------------------
enum TreeErrors TreeData( struct Tree* tree, struct File_text* file )
{
    StartWritingData( file );

    WriteTreeData( tree, file );

    FinishWritingData( file ); 

    InputFileNameChange();

    if( !file )
    {
        return GOOD_DATA;
    }
    else
    {
        return BAD_DATA;
    }
    return GOOD_DATA;
}


enum TreeErrors StartWritingData( struct File_text* file )
{
    char data_path[256] = {};
    GetFilePath( data_path, tree_data_name_new );
    file->stream = fopen( data_path, "w+" ); 

    return GOOD_DATA;
}


enum TreeErrors FinishWritingData( struct File_text* file )
{   
    fclose( file->stream );

    if( !file )
    {
        return GOOD_DATA;
    }
    else 
    {
        return BAD_DATA;
    }
}


enum TreeErrors WriteTreeData( struct Tree* tree, struct File_text* file )
{
    int count_offset = 0;

    fprintf(file->stream, "\n");
    CollectTreeData( tree->root, file, count_offset );

    return GOOD_DATA;
}


enum TreeErrors CollectTreeData( struct Node_t* node, struct File_text* file, int count_offset )
{
    struct Node_t* left = node->left;
    struct Node_t* right = node->right;

    MakeOffset( file, count_offset );
    fprintf( file->stream, "{\n");
    MakeOffset( file, count_offset );
    fprintf( file->stream, "\"%s\"\n", node->data);

    if( left != nullptr )
    {
        CollectTreeData( left, file, count_offset + 1 );
    }
    else 
    {
        MakeOffset( file, count_offset + 1);
        fprintf( file->stream, "{\n");
        MakeOffset( file, count_offset + 1);
        fprintf( file->stream, "\"nuull\"\n");
        MakeOffset( file, count_offset + 1);
        fprintf( file->stream, "}\n");
    }

    if( right != nullptr )
    {
        CollectTreeData( right, file, count_offset + 1 );
    }
    else 
    {
        MakeOffset( file, count_offset + 1);
        fprintf( file->stream, "{\n");
        MakeOffset( file, count_offset + 1);
        fprintf( file->stream, "\"nuull\"\n");
        MakeOffset( file, count_offset + 1);
        fprintf( file->stream, "}\n");
    }

    MakeOffset( file, count_offset );
    fprintf( file->stream, "}\n");
    return GOOD_DATA;
}
//------------------------------------------------------------
