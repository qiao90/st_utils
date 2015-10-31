#ifndef __ST_INTERFACE_H
#define __ST_INTERFACE_H

#include "st_others.h"
/**
 * For st_epoll
 */
#include <sys/epoll.h>
#include "st_threadpool.h"

typedef struct __epoll_event 
{
    struct epoll_event event;
    int listen_socket;
    int max_events;
    int event_fd;
    struct epoll_event* p_events;
} EPOLL_STRUCT, *P_EPOLL_STRUCT;

void st_epoll_test(void);
void st_event_loop(P_EPOLL_STRUCT p_epoll, P_ST_THREAD_MANAGE p_manage, void* handler(void* data));
P_EPOLL_STRUCT st_make_events(int lsocket, size_t maxepoll_size);
static int st_make_nonblock(int lsocket);
static int st_add_new_event(int accepted_socket, P_EPOLL_STRUCT p_epoll);
int st_buildsocket(int listen_cnt, int port);


#endif
