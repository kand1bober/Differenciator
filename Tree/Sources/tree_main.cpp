#include "../Headers/tree_functions.h"
#include "../Headers/tree_output.h"
#include "../Headers/tree_input.h"

// /*
int main()
{
    //------------Initialize------------
    struct File_input tree_input = {}; //for input
    struct File_text tree_output = {};   // for output
    struct File_text graph_file = {};  //for inpit
    struct Tree my_tree = {};

    MakeTreeData( &graph_file, &tree_input, &my_tree );


//----------------------------------------- WORK WIHOUT FILE INPUT ---------------------------------

    //--------------------------INITIALIZING WORK WITH NODES---------------------
        printf(RED "--------- Init -----------\n" DELETE_COLOR);
        union Data_t data = {};
        struct Node_t* find_node = nullptr; //
        struct Node_t* tmp_node = nullptr;  //
    //---------------------------------------------------------------------------
/*
    //---------------------------------
    data.op = k_Add;
    CreateNode( &my_tree, data, &tmp_node, OP );


        data.num = 7.77;
    CreateNode( &my_tree, data, &tmp_node, NUM );     
        data.op = k_Add;
    Find( &my_tree, data, &find_node, OP );
    InsertLeave( &my_tree, find_node, RIGHT, tmp_node );


       data.var = (char*)"x";
    CreateNode( &my_tree, data, &tmp_node, VAR );
        data.op = k_Add;
    Find( &my_tree, data, &find_node, OP );
    InsertLeave( &my_tree, find_node, LEFT, tmp_node );
*/    
//----------------------------------------------------------------------------------------------

    //-------------SAVING TREE DATA--------------
    // TreeData( &my_tree, &tree_output );
    // -------------------------------------------

    //-----------------PRINTING------------------
    if(my_tree.status == GOOD_TREE)
    {
        // Output( &graph_file, &my_tree);
    }
    else 
    {
        printf(RED "Bad Tree :(\n" DELETE_COLOR);
    }
    //-------------------------------------------
    // TreeDtor( &my_tree );

    return 0;
}

// */