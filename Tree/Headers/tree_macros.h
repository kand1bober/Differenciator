#include "tree_configuration.h"

#ifndef MACROS_LIST_HEADER
#define MACROS_LIST_HEADER


#ifdef DEBUG
    #define ON_DEBUG( expr ) expr
    #define PAUSE //getchar();
#else
    #define ON_DEBUG( expr )
    #define PAUSE
#endif

#ifdef RELIZ
    #define ON_RELIZ( expr ) expr
#else
    #define ON_RELIZ( expr )
#endif



#endif
