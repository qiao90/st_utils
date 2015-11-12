#include "st_openssl.h"
#include "st_others.h"


P_ST_TLS_STRUCT st_tls_create_ctx(P_ST_TLS_STRUCT p_st_tls)
{
    P_SSL_CTX p_ctx = NULL;

    OpenSSL_add_ssl_algorithms();
    SSL_load_error_strings();
    SSL_library_init();     //SSL_library_init() always returns "1"

    tls_rand_seed();

    if ( p_st_tls->work_status == WORK_SERVER )
    {
        st_print("Initialize with TLSv1_server_method() \n");
        EXIT_IF_TRUE(!(p_ctx = SSL_CTX_new(TLSv1_server_method()))); 
    }
    else if ( p_st_tls->work_status == WORK_CLIENT )
    {
        st_print("Initialize with TLSv1_client_method() \n");
        EXIT_IF_TRUE(!(p_ctx = SSL_CTX_new(TLSv1_client_method())));
    }
    else
    {
        st_print("Initialize with TLSv1_method() \n");
        EXIT_IF_TRUE(!(p_ctx = SSL_CTX_new(TLSv1_method())));
    }


    //SSL_CTX_set_default_passwd_cb(p_ctx, no_passphrase_callback);
    SSL_CTX_set_mode(p_ctx, SSL_MODE_ENABLE_PARTIAL_WRITE);

    if ( strlen(p_st_tls->ca_file) )
    {
        EXIT_IF_TRUE( !SSL_CTX_load_verify_locations(p_ctx, p_st_tls->ca_file, NULL));
    }

    EXIT_IF_TRUE( !SSL_CTX_use_PrivateKey_file(p_ctx, p_st_tls->key_file, SSL_FILETYPE_PEM)); 

    EXIT_IF_TRUE( !SSL_CTX_use_certificate_chain_file(p_ctx, p_st_tls->cert_file));

    // 判定私钥是否正确  
    EXIT_IF_TRUE( !SSL_CTX_check_private_key(p_ctx));

    p_st_tls->p_ctx = p_ctx;

    return p_st_tls;
}


SSL* st_tls_create_ssl(P_ST_TLS_STRUCT p_st_tls, int sock)
{
    char*   str = NULL;
    X509*   peer_cert = NULL;
    SSL*    p_ssl;

    if ( !p_st_tls || !p_st_tls->p_ctx || sock < 0)
    {
        st_d_print("Invalid argument!\n");
        return NULL;
    }

    RET_NULL_IF_TRUE( !(p_ssl = SSL_new (p_st_tls->p_ctx)) );
    RET_NULL_IF_TRUE( !SSL_set_fd (p_ssl, sock) );

    if ( p_st_tls->work_status == WORK_SERVER)
    {
        RET_NULL_IF_TRUE((SSL_accept(p_ssl) != 1)); 
    }
    else if ( p_st_tls->work_status == WORK_CLIENT)
    {
        RET_NULL_IF_TRUE((SSL_connect(p_ssl) != 1));
    }
    else
    {
        SYS_ABORT("YOU SHOULD NOT CALL THIS FUNC!!!\n");
    }
	
#if 1
    peer_cert = SSL_get_peer_certificate (p_ssl);

    if (peer_cert != NULL) 
    {
        str = X509_NAME_oneline (X509_get_subject_name (peer_cert), 0, 0);
        if(str)
        {
            st_print("OBJECT:%s\n", str);
            OPENSSL_free (str);
        }
    
        str = X509_NAME_oneline (X509_get_issuer_name  (peer_cert), 0, 0);
        if(str)
        {
            st_print("ISSUER:%s\n", str);
            OPENSSL_free (str);
        }
    
        /* We could do all sorts of certificate verification stuff here before
           deallocating the certificate. */
    
        X509_free (peer_cert);
    } 
    else
        st_d_print("Peer does not have certificate!!!\n");
#endif

    return p_ssl;
}

void st_tls_destroy(P_ST_TLS_STRUCT p_st_tls)
{
    if ( ! p_st_tls)
        return;

    if ( ! p_st_tls->p_ctx)
        SSL_CTX_free(p_st_tls->p_ctx); 
}
