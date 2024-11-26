#include "tree_functions.h"

#ifndef AKINATOR_OUTPUT_HEADER
#define AKINATOR_OUTPUT_HEADER

const char output_image_name[40] = "TreeGraph.svg";
const char output_graph_name[40] = "TreeGraph.dot";
const char output_html_name[40] =  "TreeGraph.html";

const char tree_data_name_old[40] = "OldData.txt";
const char tree_data_name_new[40] = "NewData.txt";

const size_t START_OUTPUT_FILE_SIZE =  10;
const char default_pointer_color[20] = "#FFE9C7";
const char pointer_left_color[20] =    "#9ACD32"; 
const char pointer_right_color[20] =   "#FF2B00";

const char fillcolor[20] = "#FFA9B8";
const char bgcolor[20] =   "#8B4513";
const char fontcolor[20] = "#FFE9C7";

const char fontname[30] =     "Verdana";
const char graph_header[30] = "Binary tree";


struct File_text
{
    FILE* stream;
    size_t stream_size;
};

//-------------------------------------------------------------------
enum TreeErrors Output( struct File_text* file, struct Tree* tree );
enum TreeErrors StartOutput( struct File_text* file );
enum TreeErrors FinishOutput( struct File_text* file );
enum TreeErrors Print( struct File_text* file, struct Tree* tree );
void WriteEdge( struct File_text* file, struct Node_t* node );
//-------------------------------------------------------------------

//-------------------------------------------------------------------
enum TreeErrors TreeData( struct Tree* tree, struct File_text* file );
enum TreeErrors StartWritingData( struct File_text* file );
enum TreeErrors FinishWritingData( struct File_text* file );
enum TreeErrors WriteTreeData( struct Tree* tree, struct File_text* file );
enum TreeErrors CollectTreeData( struct Node_t* node, struct File_text* file, int count_offset );
//-------------------------------------------------------------------

#endif
