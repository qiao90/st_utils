#ifndef __ST_OTHERS_H
#define __ST_OTHERS_H

#define DEBUG

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

#endif  //__ST_OTHERS_H
