/************************************/
/******** Include statements ********/
/************************************/

#include <openssl/err.h>    // OpenSSL errors.
#include "ClientSocketSSL.h"
#include "SeverityLog_api.h"

/************************************/

/*************************************/
/******* Function definitions ********/
/*************************************/

int ClientSocketSSLSetup(SSL_CTX** ctx)
{
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    *ctx = SSL_CTX_new(TLS_client_method());

    if(*ctx == NULL)
    {
        ERR_print_errors_fp(stderr);
        return CLIENT_SOCKET_SETUP_SSL_NULL_CTX;
    }

    return CLIENT_SOCKET_SETUP_SSL_SUCCESS;
}

int ClientSocketSSLHandshake(int server_socket, SSL_CTX** ctx, SSL** ssl)
{
    *ssl = SSL_new(*ctx);
    SSL_set_fd(*ssl, server_socket);

    int ssl_connect = SSL_connect(*ssl);

    if(ssl_connect <= 0)
    {
        ERR_print_errors_fp(stderr);
    }

    return ssl_connect;
}

/*************************************/