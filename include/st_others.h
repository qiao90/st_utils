#ifndef __ST_OTHERS_H
#define __ST_OTHERS_H

#define DEBUG

#include <stdio.h>
#include <stdlib.h>

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

#define SYS_ABORT(...)  \
    do{ fprintf( stderr,"DIE:%s|%s<%d>\n",__FILE__, __FUNCTION__,__LINE__); \
		fprintf( stderr , __VA_ARGS__ ); \
        abort();    \
    }while(0)

#endif  //__ST_OTHERS_H
