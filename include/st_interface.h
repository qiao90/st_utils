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


/**
 * For st_memmap
 */

#include <limits.h>

typedef struct _st_memmap_t
{    	
    void*   location;   //映射得到的内存地址
    int	    fd;	        //文件操作句柄
    size_t	size;	    //最大映射大小
    char    filename[PATH_MAX]; //映射到物理磁盘文件
    char    mapname[PATH_MAX];  //map的名字，例如 RPCShare等
} ST_MEMMAP_T, * P_ST_MEMMAP_T;

void* st_memmap_create(const char* filename, const char* share_name, size_t max_size);
void* st_memmap_open(const char* share_name, int fixaddr, int writable);
void st_memmap_close(P_ST_MEMMAP_T p_token);
void st_memmap_destroy(P_ST_MEMMAP_T p_token);
void st_memmap_test(void);

#endif
