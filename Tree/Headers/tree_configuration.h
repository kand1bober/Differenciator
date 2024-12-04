#ifndef CONFIGURATION_LIST_HEADER
#define CONFIGURATION_LIST_HEADER

// #define DEBUG
// #define RELIZ

#ifdef DEBUG
    #ifdef RELIZ
        #undef DEBUG
        #undef RELIZ
    #endif
#endif

#ifdef DEBUG
    #undef RELIZ
#endif

#ifdef RELIZ
    #undef DEBUG
#endif


#endif
