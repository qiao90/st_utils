#ifndef __ST_OTHERS_H
#define __ST_OTHERS_H

#define DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

typedef unsigned long ulong;
typedef unsigned int  uint;
typedef unsigned char uchar;

#ifdef DEBUG
#define st_d_print(...) \
	do{ fprintf( stderr,"DEBUG:%s|%s<%d>:",__FILE__, __FUNCTION__,__LINE__); \
		fprintf( stderr , __VA_ARGS__ ); \
	}while(0)
#define st_print(...) fprintf( stderr , __VA_ARGS__ )
#else
#define st_d_print(...) \
	do {} while(0)
#define st_print(...) fprintf( stderr , __VA_ARGS__ )
#endif

typedef struct _st_small_obj {
    char    data[2048];
    size_t  len;
} ST_SMALL_OBJ, *P_ST_SMALL_OBJ;

typedef struct _st_small_pobj {
    char*   data;
    size_t  len;
} ST_SMALL_POBJ, *P_ST_SMALL_POBJ;

#define WAIT_FOR_ENTER  fprintf( stderr, "Press ENTER\n" );getchar()


#define EXIT_IF_TRUE(x) if (x)                                  \
    do {                                                        \
            fprintf(stderr, "!!!%s:%d ASSERT '%s' IS TRUE\n",   \
            __FILE__, __LINE__, #x);                            \
            SYS_ABORT(#x);                                      \
    }while(0)  

#define RET_NULL_IF_TRUE(x) if (x)                              \
    do {                                                        \
            fprintf(stderr, "!!!%s:%d ASSERT '%s' IS TRUE\n",   \
            __FILE__, __LINE__, #x);                            \
            return NULL;                                        \
    }while(0)  

#define GOTO_IF_TRUE(x, flag) if (x)                            \
    do {                                                        \
            fprintf(stderr, "!!!%s:%d ASSERT '%s' IS TRUE\n",   \
            __FILE__, __LINE__, #x);                            \
            goto flag;                                          \
    }while(0) 

static inline void backtrace_info(void)
{
	int j, nptrs;
#define BT_SIZE 100
	char **strings;
    void *buffer[BT_SIZE];
	
	nptrs = backtrace(buffer, BT_SIZE);
	fprintf(stderr, "backtrace() returned %d addresses\n", nptrs);
	
	strings = backtrace_symbols(buffer, nptrs);
	if (strings == NULL) 
	{
		perror("backtrace_symbols");
		exit(EXIT_FAILURE);
    }

    for (j = 0; j < nptrs; j++)
        fprintf(stderr, "%s\n", strings[j]);

    free(strings);
}

#define SYS_ABORT(...)  \
    do{ fprintf( stderr,"!!!DIE:%s|%s<%d>\n",__FILE__, __FUNCTION__,__LINE__); \
		fprintf( stderr , __VA_ARGS__ ); \
        backtrace_info(); abort();    \
    }while(0)

#endif  //__ST_OTHERS_H
