#include "WINDEF.H"
#include "st_others.h"

/**
 * 主要用于提供一些在Windows下同步操作的Linux实现 
 * 包括 线程内部的  CriticalSection 
 * 进程间的 Mutex 
 *        Event 
 */

/*****************************
 * Windows兼容函数
 */

typedef pthread_mutex_t CRITICAL_SECTION;
typedef CRITICAL_SECTION *LPCRITICAL_SECTION;

void InitializeCriticalSection(LPCRITICAL_SECTION section)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr); 
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);  //可递归
    pthread_mutex_init(section, &attr);
    pthread_mutexattr_destroy(&attr);
}

void EnterCriticalSection(LPCRITICAL_SECTION section)
{
    pthread_mutex_lock(section);
}

void LeaveCriticalSection(LPCRITICAL_SECTION section)
{
    pthread_mutex_unlock(section);
}

void DeleteCriticalSection(LPCRITICAL_SECTION section)
{
	pthread_mutex_destroy(section);
}

/*****************************
 * Windows Mutex互斥锁 
 * 可以跨进程 
 */

HANDLE CreateMutex( void* lpMutexAttributes,
                BOOL bInitialOwner, const char* lpName)
{
	pthread_mutex_t* p_mutex = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
	if(!p_mutex)
		return NULL;

	pthread_mutex_init(p_mutex, NULL);
	
	return (HANDLE)p_mutex;
	
}

DWORD  WaitForSingleObject_Mutex(HANDLE hHandle,
                DWORD dwMilliseconds)
{
	int ret = 0;
	
	if(dwMilliseconds == INFINITE)
		return pthread_mutex_lock((pthread_mutex_t *)hHandle);
	
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += dwMilliseconds/1000; 
	ts.tv_nsec += (dwMilliseconds%1000)*1000*1000;  //ns
	ret = pthread_mutex_timedlock((pthread_mutex_t *)hHandle, &ts);
	
	return ret;
}

BOOL  ReleaseMutex(HANDLE hMutex)
{
	if(!hMutex)
		return TRUE;
	
	return pthread_mutex_unlock((pthread_mutex_t *)hMutex);
}

BOOL  CloseHandle_Mutex( HANDLE hObject)
{
	if(!hObject)
		return TRUE;
	
	pthread_mutex_destroy((pthread_mutex_t *)hObject);
	
	free(hObject);
	return TRUE;
}


/**************** 
 * Windows事件通知，可以跨进程
 */
HANDLE CreateEvent(void* lpEventAttributes,
                BOOL bManualReset,BOOL bInitialState,
                const char* lpName)
{
	return NULL;
}

BOOL ResetEvent( HANDLE hEvent)
{
	return TRUE;
}

BOOL SetEvent( HANDLE hEvent)
{
	return TRUE;
}

DWORD WaitForSingleObject_Event(HANDLE hHandle,
                DWORD dwMilliseconds)
{
	return 0;
}

BOOL WINAPI CloseHandle_Event(HANDLE hObject)
{
    return TRUE;
}


void Sleep(DWORD dwMilliseconds)
{
	if(dwMilliseconds >= 1000 && !(dwMilliseconds % 1000))
		sleep(dwMilliseconds/1000);
	else
		usleep(dwMilliseconds*1000);
	
	return;
}


BOOL get_workdir( char* store)
{
    if(!store)
        return FALSE;
        
    int cnt = readlink("/proc/self/exe", store, PATH_MAX);
    if(cnt == -1)
        return FALSE;
        
    char* ptr = store + strlen(store);
    while(ptr > store && *ptr != '/')
        ptr --;
    
    if(ptr < store)
        return FALSE;
     
     *(ptr+1) = '\0';
        
    return TRUE;
}
