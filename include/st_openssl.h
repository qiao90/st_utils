#ifndef _ST_OPENSSL_H_
#define _ST_OPENSSL_H_

#include <openssl/crypto.h>
#include <openssl/dh.h>
#include <openssl/dsa.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/opensslv.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>


#include <sys/utsname.h>
#include <unistd.h>
#include <sys/types.h>

typedef struct ssl_ctx_st SSL_CTX, *P_SSL_CTX;

enum _ST_WORK_STATUS {
    WORK_SERVER, 
    WORK_CLIENT, 
    WORK_GENERAL
};

typedef struct _st_tls_ctx_struct {
    char    cert_file[PATH_MAX];
    char    key_file[PATH_MAX];
    char    ca_file[PATH_MAX];
    enum    _ST_WORK_STATUS work_status;
    P_SSL_CTX    p_ctx;  //唯一的对象
} ST_TLS_STRUCT, *P_ST_TLS_STRUCT;


P_ST_TLS_STRUCT st_tls_create_ctx(P_ST_TLS_STRUCT p_st_tls);
SSL* st_tls_create_ssl(P_ST_TLS_STRUCT p_st_tls, int sock);
void st_tls_destroy(P_ST_TLS_STRUCT p_st_tls);

static inline void tls_rand_seed_uniquely(void)
{
    struct {
        pid_t pid;
        time_t time;
        void *stack;
    } data;

    data.pid = getpid();
    data.time = time(NULL);
    data.stack = (void *)&data;

    RAND_seed((const void *)&data, sizeof data);
}

static inline void tls_rand_seed(void)
{
    struct {
        struct utsname uname;
        int uname_1;
        int uname_2;
        uid_t uid;
        uid_t euid;
        gid_t gid;
        gid_t egid;
    } data;

    data.uname_1 = uname(&data.uname);
    data.uname_2 = errno;       /* Let's hope that uname fails randomly :-) */

    data.uid = getuid();
    data.euid = geteuid();
    data.gid = getgid();
    data.egid = getegid();

    RAND_seed((const void *)&data, sizeof data);
    tls_rand_seed_uniquely();
}

#define st_ssl_error(...) \
	do{ fprintf( stderr,"!!!ERROR:%s|%s<%d>:",__FILE__, __FUNCTION__,__LINE__); \
		fprintf( stderr , __VA_ARGS__ ); \
        ERR_print_errors_fp(stderr);    \
	}while(0)

#define EXIT_IF_TRUE(x) if (x)                                  \
    do {                                                        \
            fprintf(stderr, "!!!ASSERT,CHECK '%s' IS TRUE\n", #x);    \
            ERR_print_errors_fp(stderr);                    \
            SYS_ABORT(#x);                                  \
    }while(0)  

#define RET_NULL_IF_TRUE(x) if (x)                                  \
    do {                                                        \
            fprintf(stderr, "!!!ASSERT,CHECK '%s' IS TRUE\n", #x);    \
            ERR_print_errors_fp(stderr);                    \
            return NULL;                                  \
    }while(0)  

#endif //_ST_OPENSSL_H_
