/**
 * 使用epoll进行大流量服务器设计的实现
 */

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <string.h>

#include <stdio.h>
#include "st_others.h"

int st_buildsocket(int listen_cnt, int port)
{
    int listen_socket; 
    int sfd;

    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_socket == -1)
    {
        st_d_print("Create Socket Error!\n");
        return -1;
    }


	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(port);
	svraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(listen_socket, (struct sockaddr *)&svraddr, sizeof(struct sockaddr_in)) < 0)
	{
		st_d_print("Socket Bind Error!\n");
        close(listen_socket);
		return -1;
	}


	if(listen(listen_socket, listen_cnt) != 0)
	{
		st_d_print("Socket Set Listen Error!\n");
        close(listen_socket);
		return -1;
	}
	
	return listen_socket;
}

static void st_add_new_event(int socket)
{
    int flags = 0;

    flags = fcntl (socket, F_GETFL, 0);
	flags |= O_NONBLOCK;
    fcntl (socket, F_SETFL, flags);

    return;
}

P_EPOLL_STRUCT st_make_events(int lsocket, size_t maxepoll_size)
{
    P_EPOLL_STRUCT p_epoll = 
        (P_EPOLL_STRUCT)malloc(sizeof(EPOLL_STRUCT));

    if (!p_epoll)
        return NULL;

    p_epoll->p_events = 
        (struct epoll_event*)calloc (maxepoll_size, sizeof(struct epoll_event));

    if (! p_epoll->p_events)
    {
        free(p_epoll);
        return NULL;
    }
	
    memset(p_epoll->p_events, 0, maxepoll_size*sizeof(struct epoll_event));
    p_epoll->max_events = maxepoll_size;
	p_epoll->event_fd = epoll_create (maxepoll_size);
	p_epoll->event.data.fd = lsocket;
    p_epoll->event.events = EPOLLIN | EPOLLET;
    if(epoll_ctl (p_epoll->event_fd, EPOLL_CTL_ADD, lsocket, &p_epoll->event) == -1)
    {
        free(p_epoll->p_events);
        free(p_epoll);
        return NULL;
    }

    return p_epoll;
}

void st_event_loop(P_EPOLL_STRUCT p_epoll)
{
    if (!p_epoll)
        return;
    struct epoll_event* p_events = p_epoll->p_events;
    int listen_socket = p_epoll->event.data.fd;

    int ready = 0;

    struct epoll_event new_event;

    for ( ; ; )
    {
        ready = epoll_wait(p_epoll->event_fd, p_events, p_epoll->max_events, -1); 
        for (int i = 0; i < ready; i++)
        {
            if( (p_events[i].events & EPOLLERR) ||
                (p_events[i].events & EPOLLHUP) ||
              (!(p_events[i].events & EPOLLIN)))
            {
                /* 异常情况 */
                st_d_print("Epoll Error!\n");
                close(p_events[i].data.fd);
                continue;
            }
            
            if (listen_socket == p_events[i].data.fd)
            {
                 /* 新链接到了（可能会有多个连接同时到来） */
                while (1)
                {
                    struct sockaddr in_addr;
                    socklen_t in_len;
                    int infd;
                    char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

                    in_len = sizeof(in_addr);
                    infd = accept (listen_socket, &in_addr, &in_len);
                    if (infd == -1)
                        break;

                    int sk = getnameinfo (&in_addr, in_len,
                                       hbuf, sizeof(hbuf),
                                       sbuf, sizeof(sbuf),
                                       NI_NUMERICHOST | NI_NUMERICSERV);
                    if (sk == 0)
                    {
                        st_d_print("Accepted connection on descriptor %d "
                                 "(host=%s, port=%s)\n", infd, hbuf, sbuf);
                    }

                    st_make_nonblock(infd);
                    new_event.data.fd = infd;
                    new_event.events = EPOLLIN | EPOLLET;
                    sk = epoll_ctl (p_epoll->event_fd, EPOLL_CTL_ADD, infd, &new_event);
                    if (sk == -1)
                    {
                        perror ("epoll_ctl");
                        abort ();
                    }
                    
                    continue;       //可能有多个连接
                }
            }
            else
            {
                  /* We have data on the fd waiting to be read. Read and
                     display it. We must read whatever data is available
                     completely, as we are running in edge-triggered mode
                     and won't get a notification again for the same
                     data. */
                int done = 0;

                while (1)
                {
                    //读取数据，分发给线程处理
                    break;

                }
            }
        }
    }
}

static void st_epoll_test(void)
{

}
