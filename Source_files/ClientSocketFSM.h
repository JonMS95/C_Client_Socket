#ifndef CLIENT_SOCKET_FSM_H
#define CLIENT_SOCKET_FSM_H

/************************************/
/******** Include statements ********/
/************************************/

#include <stdbool.h>
#include <openssl/ssl.h>
#include "ClientSocket_api.h"

/************************************/

/***********************************/
/******** Define statements ********/
/***********************************/

#define CLIENT_SOCKET_WAIT_RECONNECT_SEC    1

#define CLIENT_SOCKET_MSG_CREATION_NOK      "Socket file descriptor creation failed."
#define CLIENT_SOCKET_MSG_CREATION_OK       "Socket file descriptor created."
#define CLIENT_SOCKET_MSG_SETUP_SSL_NOK     "SSL setup failed."
#define CLIENT_SOCKET_MSG_SETUP_SSL_OK      "SSL setup succeeded."
#define CLIENT_SOCKET_MSG_CONNECT_NOK       "Failed to connect to server with IP <%s> and port <%d>."
#define CLIENT_SOCKET_MSG_CONNECT_OK        "Successfully connected to server with IP <%s> and port <%d>."
#define CLIENT_SOCKET_MSG_SSL_HANDSHAKE_NOK "SSL handshake failed."
#define CLIENT_SOCKET_MSG_SSL_HANDSHAKE_OK  "SSL handshake succeeded."
#define CLIENT_SOCKET_MSG_CLOSE_NOK         "An error happened while closing the socket."
#define CLIENT_SOCKET_MSG_CLOSE_OK          "Socket successfully closed."

/***********************************/

/**********************************/
/******** Type definitions ********/
/**********************************/

typedef enum
{
    CREATE_FD = 0   ,
    SETUP_SSL       ,
    CONNECT         ,
    SSL_HANDSHAKE   ,
    INTERACT        ,
    CLOSE           ,

} CLIENT_SOCKET_FSM;

/**********************************/

/*************************************/
/******** Function prototypes ********/
/*************************************/

int SocketStateCreate(void);
int SocketStateConnect(int socket_desc, char* server_addr, int server_port);
int SocketStateInteract(int socket_desc, bool secure, SSL** ssl);
int SocketStateClose(int new_socket);

/*************************************/

#endif