#ifndef __ST_THREADPOOL_H
#define __ST_THREADPOOL_H


enum THREAD_STATUS {
    THREAD_SPARE,
    THREAD_RUNNING,
    THREAD_DEAD,
    THREAD_STATUS_MAX,
};

typedef struct _st_thread {
    SLIST_HEAD list;
    enum THREAD_STATUS status; //
    pthread_t  pid;
} ST_THREAD, *P_ST_THREAD;

typedef struct _st_tasks {
    SLIST_HEAD list;
    void *(*handler) (void *);
    void *arg;
}ST_TASK, *P_ST_TASK;

typedef struct _st_thread_manage {
    volatile int thread_spare;
    volatile int thread_running;
    volatile int thread_dead;
    pthread_t    manage_pid;
    SLIST_HEAD   threads;   // threads的结构不会被外部访问，不需要保护
    SLIST_HEAD   tasks;     // 需要保护操作，任何线程都可以添加任务
    pthread_mutex_t tk_mutex;
} ST_THREAD_MANAGE, *P_ST_THREAD_MANAGE;

P_ST_THREAD_MANAGE st_threadpool_init(P_ST_THREAD_MANAGE p_manage, int cnt);
void st_threadpool_destroy(P_ST_THREAD_MANAGE p_manage);
void st_threadpool_statistic(P_ST_THREAD_MANAGE p_manage);
int st_threadpool_push_task(P_ST_THREAD_MANAGE p_manage, void *(*start_routine) (void *), void *arg);
int st_threadpool_pop_task(P_ST_THREAD_MANAGE p_manage, P_ST_TASK p_task_store);

static void* threadpool_run(void *data);
static void* threadpool_manage(void *data);

int st_threadpool_test(void);

#endif  //__ST_THREADPOOL_H
