
#ifndef DIFF_DSL_COMMANDS_HEADER
#define DIFF_DSL_COMMANDS_HEADER

#define CREATE( left, right, low_type, value, hi_type ) CreateNodeDSL( diff_tree, left, right, (Data_t){.low_type = value}, hi_type )
#define DIFF( node_name ) MakeDifferentiation( diff_tree, node_name )    
#define COPY( node_name ) CopyBranchDSL( diff_tree, node_name ) 

#endif