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

    // //--------------------------INITIALIZING WORK WITH NODES---------------------
        printf(RED "--------- Init -----------\n" DELETE_COLOR);
        char data[STRING_SIZE] = "";
        struct Node_t* find_node = nullptr; //
        struct Node_t* tmp_node = nullptr;  //shold be used everytime you need to create a new node, otherwise it will be seg-fault
    // //---------------------------------------------------------------------------

    // //---------------------------------
    //     strcpy( data, "14.88" );
    // CreateNode( &my_tree, data, &tmp_node );
    // InsertLeave( &my_tree, my_tree.root, LEFT, tmp_node );
    // //---------------------------------
    // printf(RED "--------- 1 -----------\n" DELETE_COLOR);
    //     strcpy( data, "7.77" );
    // CreateNode( &my_tree, data, &tmp_node );     
    //     strcpy( data, "14.88" );
    // Find( &my_tree, data, &find_node );
    // printf("find_node: %p\n", find_node);
    // InsertLeave( &my_tree, find_node, RIGHT, tmp_node );
    // // --------------------------------
    // printf(RED "--------- 2 -----------\n" DELETE_COLOR);
    //     strcpy( data, "6.66" );
    // CreateNode( &my_tree, data, &tmp_node );
    //     strcpy( data, "14.88" );
    // Find( &my_tree, data, &find_node );
    // printf("find_node: %p\n", find_node);
    // InsertNode( find_node->right, nullptr, tmp_node);
    // // InsertNode( nullptr, find_node->right, tmp_node);
    // // -------------------------------

    // //-------------------------------
    // printf(RED "--------- 6 -----------\n" DELETE_COLOR);
    // strcpy( data, "Doesn't exist" );
    // printf("%p\n", find_node );
    // Find( &my_tree, data, &find_node );
    // printf("%p\n", find_node );
    // printf("find_node: %p\n", find_node);
    // NodeDelete( &my_tree, find_node );
    // //-------------------------------
//----------------------------------------------------------------------------------------------



    //-------------SAVING TREE DATA--------------
    TreeData( &my_tree, &tree_output );
    // -------------------------------------------

    //-----------------PRINTING------------------
    if(my_tree.status == GOOD_TREE)
    {
        Output( &graph_file, &my_tree);
    }
    else 
    {
        printf(RED "Bad Tree :(\n" DELETE_COLOR);
    }
    //-------------------------------------------
    TreeDtor( &my_tree );

    return 0;
}

// */