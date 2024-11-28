#include "../Headers/diff_functions.h"

int main()
{
    //TODO: все заменится на Run();

    //=========================================================================

    //------------Initialize------------
    struct File_text tree_input = {}; //for input
    struct File_text tree_output = {};   // for output
    struct File_text graph_file = {};  //for inpit
    struct Tree my_tree = {};

    struct File_text diff_tree_input = {}; 
    struct File_text diff_tree_output = {};   
    struct File_text diff_graph_file = {};  
    struct Tree diff_tree = {};

    // MakeTreeData( &graph_file, &tree_input, &my_tree );

    //--------------------------INIT-----------------------------
        printf(RED "--------- Init -----------\n" DELETE_COLOR);
        union Data_t data = {};
        struct Node_t* find_node = nullptr; //
        struct Node_t* tmp_node = nullptr;  //
    //-----------------------------------------------------------

    //---------------------------------
    data.op = kAdd;
    CreateNode( &my_tree, data, &tmp_node, OP );


        data.num = 7.77;
    CreateNode( &my_tree, data, &tmp_node, NUM );     
        data.op = kAdd;
    Find( &my_tree, data, &find_node, OP );
    InsertLeave( &my_tree, find_node, RIGHT, tmp_node );

        data.op = kDiv;
    CreateNode( &my_tree, data, &tmp_node, OP );
        data.op = kAdd;
    Find( &my_tree, data, &find_node, OP );
    InsertLeave( &my_tree, find_node, LEFT, tmp_node );

       data.var = (char*)"x";
    CreateNode( &my_tree, data, &tmp_node, VAR );
        data.op = kDiv;
    Find( &my_tree, data, &find_node, OP );
    InsertLeave( &my_tree, find_node, LEFT, tmp_node );

    //        data.num = 17.7;
    // CreateNode( &my_tree, data, &tmp_node, NUM );
    //     data.op = kMul;
    // Find( &my_tree, data, &find_node, OP );
    // InsertLeave( &my_tree, find_node, LEFT, tmp_node );

        data.var = (char*)"y";
    CreateNode( &my_tree, data, &tmp_node, VAR);
        data.op = kDiv;
    Find( &my_tree, data, &find_node, OP );
    InsertLeave( &my_tree, find_node, RIGHT, tmp_node );


    Differentiate( &my_tree, &diff_tree );


    //-------------SAVING TREE DATA--------------
    // TreeData( &my_tree, &tree_output );
    // -------------------------------------------

    //-----------------PRINTING------------------
    if(my_tree.status == GOOD_TREE)
    {
        Output( &diff_graph_file, &diff_tree);
        // Output( &graph_file, &my_tree);
    }
    else 
    {
        printf(RED "Bad Tree :(\n" DELETE_COLOR);
    }
    //-------------------------------------------

    
    TreeDtor( &my_tree );
    TreeDtor( &diff_tree );

    //=========================================================================


    
    return 0;
}