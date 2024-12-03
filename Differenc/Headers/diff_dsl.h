#define DIFF_DSL

#ifdef DIFF_DSL
#define DIFF_DSL

    #define CreateNode( TREE_TYPE, NODE_NAME, NODE_TYPE ) CreateNode( type##_tree, data, &#NODE_NAME##_node, NODE_TYPE )
            // CreateNode( diff, ready, tmp, OP );
    
    
#endif