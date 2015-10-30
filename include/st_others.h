#ifndef __ST_OTHERS_H
#define __ST_OTHERS_H

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


/**
 * For st_epoll
 */
#include <sys/epoll.h>

typedef struct __epoll_event 
{
    struct epoll_event event;
    int listen_socket;
    int max_events;
    int event_fd;
    struct epoll_event* p_events;
} EPOLL_STRUCT, *P_EPOLL_STRUCT;

#endif  //__ST_OTHERS_H
