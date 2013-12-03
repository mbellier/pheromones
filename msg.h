#ifndef MSG_H_
#define MSG_H_

#include <stdio.h>

/* Uncomment compilator version : */
//#define CPP
#define C99

/* NB : Variadic macros
   (C99) ... => __VA_ARGS__
   (CPP) args... => args
*/

/* ------ Messages ------ */
#ifdef CPP
#define DEBUG_MSG(MODULE, args...) if(MODULE ## __DEBUG_MSG){printf("[%s] ", #MODULE); printf(args); printf("\n");}
#define ERROR_MSG(MODULE, args...) if(MODULE ## __ERROR_MSG){fprintf(stderr, "[%s] Error : ", #MODULE); fprintf(stderr, args); fprintf(stderr, "\n");}
#elif defined C99
#define DEBUG_MSG(MODULE, ...) if(MODULE ## __DEBUG_MSG){printf("[%s] ",#MODULE); printf(__VA_ARGS__); printf("\n");}
#define ERROR_MSG(MODULE, ...) if(MODULE ## __ERROR_MSG){fprintf(stderr, "[%s] Error : ", #MODULE); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");}
#endif

/* ------ Usage : ------

At the top of a module :
    #define <MODULE>__DEBUG_MSG 1 // boolean

Then :
    DEBUG_MSG(<MODULE>, "text")
    ERROR_MSG(<MODULE>, "n°%d", 404)

*/

#endif
