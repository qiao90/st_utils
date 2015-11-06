#ifndef __ST_OTHERS_H
#define __ST_OTHERS_H

#define DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

#ifdef DEBUG
#define st_d_print(...) \
	do{ fprintf( stderr,"DEBUG:%s|%s<%d>\n",__FILE__, __FUNCTION__,__LINE__); \
		fprintf( stderr , __VA_ARGS__ ); \
	}while(0)
#define st_print(...) fprintf( stderr , __VA_ARGS__ )
#else
#define st_d_print(...) \
	do {} while(0)
#define st_print(...) fprintf( stderr , __VA_ARGS__ )
#endif

#define WAIT_FOR_ENTER  fprintf( stderr, "Press ENTER\n" );getchar()

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
    do{ fprintf( stderr,"DIE:%s|%s<%d>\n",__FILE__, __FUNCTION__,__LINE__); \
		fprintf( stderr , __VA_ARGS__ ); \
        backtrace_info(); abort();    \
    }while(0)

#endif  //__ST_OTHERS_H
