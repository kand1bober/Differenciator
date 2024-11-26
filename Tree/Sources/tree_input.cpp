#include "../Headers/tree_input.h"
#include "../Headers/tree_functions.h"
// #include <mutex>
// #include <features.h>

//---------------------------------------------------------------------------------------
enum TreeErrors MakeTreeData( struct File_text* dump, struct File_input* file, struct Tree* tree )
{
    ON_DEBUG( printf(RED "====== START MakeTreeData ======\n" DELETE_COLOR); )

    //-------------------FILE WORK-------------------
    char filepath[256] = {};
    GetFilePath( filepath, tree_data_name_old);
    FILE* stream = fopen( filepath, "r" );
    struct stat file_info = {};
    stat( filepath, &file_info );
    unsigned long int size_of_stream = (unsigned long int)file_info.st_size;
    ON_DEBUG( printf(YELLOW "size of file: %lu\n" DELETE_COLOR, size_of_stream); )
    file->size_of_file = size_of_stream;
    if( file->size_of_file > 0 )
    {
        file->buffer = (char*)calloc( size_of_stream + 1, sizeof(char) );
        fseek(stream, sizeof(char) * 0L,  SEEK_SET);
        fread( (void*)file->buffer, sizeof(char), size_of_stream, stream);
        fclose(stream);
        //-----------------------------------------------

        //-----------------PARSER START------------------
        struct Parser utility = {};
        utility.string = file->buffer;
        utility.delim_1 = "{}";
        utility.delim_2 = "\"";
        utility.delim_3 = " }";
        utility.result_1 = nullptr;
        utility.result_2 = nullptr;
        utility.track_1 = utility.string;
        utility.track_2 = nullptr;

        NodeFromData( dump, tree, tree->root, &utility );
        //-----------------------------------------------

        free( file->buffer );
        
        ON_DEBUG( printf(RED "====== END MakeTreeData ======\n" DELETE_COLOR); )
        return GOOD_INPUT;

    }
    else 
    {
        printf(RED "File with tree description should\n"
        "at least contain first node desription" DELETE_COLOR);
        fclose( stream );
        return BAD_INPUT;
    }

}
//---------------------------------------------------------------------------------------


//--------------------------------RECURSIVE----------------------------------------------
void NodeFromData( struct File_text* dump, struct Tree* tree, struct Node_t* node, struct Parser* utility )
{
    struct Node_t* left = nullptr;
    struct Node_t* right = nullptr;

    //----------------------------------------------------------------------
    utility->track_2 = nullptr;
    utility->result_1 = nullptr;
    utility->result_2 = nullptr;
    //---------------------------LEFT SON-----------------------------------
    //-------GETTING STRING------
    if ( (utility->result_1 = strtok_r( utility->track_1, utility->delim_1, &utility->track_1)) != nullptr )
    {
            ON_DEBUG( printf(YELLOW "track_1: %s\n" DELETE_COLOR, utility->track_1); )
        strtok_r( utility->track_1, utility->delim_2, &utility->track_2 );
        utility->result_2 = strtok_r( nullptr, utility->delim_2, &utility->track_2);
            ON_DEBUG( printf(PURPLE "left branch command: %s\ntrack_2: %s\n" DELETE_COLOR, utility->result_2, utility->track_2); )
        //---------------------------

        if( strcmp( utility->result_2, "nuull" ) != 0 )
        {
            ON_DEBUG( printf(YELLOW "again left\n" DELETE_COLOR); )
            CreateNode( tree, utility->result_2, &left, VAR );
            InsertLeave( tree, node, LEFT, left );
            ON_DEBUG(
                        Output( dump, tree ); 
                        PAUSE    
                    )
            //----------------------------------
            utility->track_1 = utility->track_2;
            //----------------------------------
            NodeFromData( dump, tree, left, utility );

        }
        else 
        {
            node->left = nullptr;
            strtok_r( utility->track_2, utility->delim_3, &utility->track_2 );
        }
    }
    else 
    {
        return;
    }

    //---------------------------------------
    utility->track_1 = utility->track_2;
    utility->track_2 = nullptr;
    utility->result_1 = nullptr;
    utility->result_2 = nullptr;
    //---------------------------------------

    //---------------------------RIGHT SON----------------------------------
    //-------GETTING STRING------
    if ( (utility->result_1 = strtok_r( utility->track_1, utility->delim_1, &utility->track_1)) != nullptr ) 
    {
        //---------------
        if( utility->track_1[0] == '\0') 
        {
            ON_DEBUG( printf("\n\nEEENNNDDD\n\n"); )
            return;
        }
        //---------------
        strtok_r( utility->track_1, utility->delim_2, &utility->track_2 );
        utility->result_2 = strtok_r( nullptr, utility->delim_2, &utility->track_2);
            ON_DEBUG( printf(PURPLE "rught branch command: %s\ntrack_2: %s\n" DELETE_COLOR, utility->result_2, utility->track_2); )
        //---------------------------
        if( strcmp( utility->result_2, "nuull" ) != 0)
        {
            ON_DEBUG( printf(YELLOW "again left\n" DELETE_COLOR); )
            CreateNode( tree, utility->result_2, &right, VAR );
            InsertLeave( tree, node, RIGHT, right );
            ON_DEBUG(
                        Output( dump, tree ); 
                        PAUSE    
                    )
            //----------------------------------
            utility->track_1 = utility->track_2;
            //----------------------------------
            NodeFromData( dump, tree, right, utility );

        }
        else 
        {
            node->right = nullptr;
            strtok_r( utility->track_2, utility->delim_3, &utility->track_2 );
            ON_DEBUG( printf(SINIY "huy: %s\n" DELETE_COLOR, utility->track_2 ); )
        }
    }
    else 
    {   
        return;
    }
    strtok_r( utility->track_2, utility->delim_3, &utility->track_2 );

    return;
}
//---------------------------------------------------------------------------------------


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
