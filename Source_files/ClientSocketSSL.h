/************************************/
/******** Include statements ********/
/************************************/

#include <openssl/ssl.h>    // OpenSSL

/************************************/

/************************************/
/********* Define statements ********/
/************************************/

#define CLIENT_SOCKET_SETUP_SSL_NULL_CTX    -1
#define CLIENT_SOCKET_SETUP_SSL_SUCCESS     1

#define CLIENT_SOCKET_SSL_HANDSHAKE_SUCCESS 1

/************************************/

/*************************************/
/******** Function prototypes ********/
/*************************************/

int ClientSocketSSLSetup(SSL_CTX** ctx);
int ClientSocketSSLHandshake(int server_socket, SSL_CTX** ctx, SSL** ssl);

/*************************************/