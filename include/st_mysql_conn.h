#ifndef _ST_MYSQL_CONN_POOL_H
#define _ST_MYSQL_CONN_POOL_H

// For Database
#include <mysql/mysql.h>  
#include <pthread.h>

#ifdef __cplusplus 
extern "C" {
#endif //__cplusplus 

typedef struct _mysql_data
{
    char hostname[128];
    char username[128];
    char password[128];
    char database[128];
} MYSQL_Data, *P_MYSQL_Data;
extern P_MYSQL_Data p_mysql_data;


typedef struct  _mysql_conn 
{
    MYSQL mysql;
    unsigned free;  // =0 表示空闲
} MYSQL_CONN, *P_MYSQL_CONN;

typedef struct  _mysql_conns 
{
    pthread_mutex_t mutex;
    int len;
    P_MYSQL_CONN conns;
} MYSQL_CONNS, *P_MYSQL_CONNS;

P_MYSQL_CONN mysql_get_conn(P_MYSQL_CONNS p_conns);
void mysql_free_conn(P_MYSQL_CONNS p_conns, P_MYSQL_CONN p_conn);
P_MYSQL_CONNS mysql_conns_init(unsigned int conn_num, P_MYSQL_Data pdata);
int mysql_conns_destroy(P_MYSQL_CONNS p_conns);


int mysql_fetch_string(MYSQL *mysql,const char *create_definition,
    char* result_store);
int mysql_fetch_num(MYSQL *mysql,const char *create_definition,    
    int* result_store);
// return 0 if not exist
int mysql_check_exist(MYSQL *mysql,const char *create_definition);
int mysql_exec_sql(MYSQL *mysql,const char *create_definition);

int st_mysql_conn_test(void);

#ifdef __cplusplus 
}
#endif //__cplusplus 

#endif //_ST_MYSQL_CONN_POOL_H
