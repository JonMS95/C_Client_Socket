#ifndef CLIENT_SOCKET_FSM_H
#define CLIENT_SOCKET_FSM_H

/************************************/
/******** Include statements ********/
/************************************/

#include "ClientSocket_api.h"

/************************************/

/***********************************/
/******** Define statements ********/
/***********************************/

#define CLIENT_SOCKET_WAIT_RECONNECT_SEC    1

#define CLIENT_SOCKET_SET_SIGINT_ERR            "Error while trying to set up SIGINT handler."
#define CLIENT_SOCKET_MSG_SIGINT_RECEIVED       "Received Ctrl+C (SIGINT). Cleaning up and exiting."
#define CLIENT_SOCKET_MSG_CLEANING_UP_SSL_CTX   "Cleaning up server SSL context."
#define CLIENT_SOCKET_MSG_CLEANING_UP_SSL       "Cleaning up server SSL data."
#define CLIENT_SOCKET_MSG_CREATION_NOK          "Socket file descriptor creation failed."
#define CLIENT_SOCKET_MSG_CREATION_OK           "Socket file descriptor created."
#define CLIENT_SOCKET_MSG_SETUP_SSL_NOK         "SSL setup failed."
#define CLIENT_SOCKET_MSG_SETUP_SSL_OK          "SSL setup succeeded."
#define CLIENT_SOCKET_MSG_CONNECT_NOK           "Failed to connect to server with IP <%s> and port <%d>."
#define CLIENT_SOCKET_MSG_CONNECT_OK            "Successfully connected to server with IP <%s> and port <%d>."
#define CLIENT_SOCKET_MSG_SSL_HANDSHAKE_NOK     "SSL handshake failed."
#define CLIENT_SOCKET_MSG_SSL_HANDSHAKE_OK      "SSL handshake succeeded."
#define CLIENT_SOCKET_MSG_CLOSE_NOK             "An error happened while closing the socket."
#define CLIENT_SOCKET_MSG_CLOSE_OK              "Socket successfully closed."

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

static void SocketFreeResources(void);
static void SocketSIGINTHandler(int signum);
static int SocketStateCreate(void);
static int SocketStateConnect(int socket_desc, char* server_addr, int server_port);
static int SocketStateClose(int new_socket);

/*************************************/

#endif