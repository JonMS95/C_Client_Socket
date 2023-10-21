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

#define CLIENT_SOCKET_MSG_CREATION_NOK  "Socket file descriptor creation failed."
#define CLIENT_SOCKET_MSG_CREATION_OK   "Socket file descriptor created."
#define CLIENT_SOCKET_MSG_CONNECT_NOK   "Failed to connect to server with IP <%s> and port <%d>."
#define CLIENT_SOCKET_MSG_CONNECT_OK    "Successfully connected to server with IP <%s> and port <%d>."
#define CLIENT_SOCKET_MSG_CLOSE_NOK     "An error happened while closing the socket."
#define CLIENT_SOCKET_MSG_CLOSE_OK      "Socket successfully closed."

/***********************************/

/**********************************/
/******** Type definitions ********/
/**********************************/

typedef enum
{
    CREATE_FD = 0   ,
    CONNECT         ,
    INTERACT        ,
    CLOSE           ,

} CLIENT_SOCKET_FSM;

/**********************************/

/*************************************/
/******** Function prototypes ********/
/*************************************/

int SocketStateCreate(void);
int SocketStateConnect(int socket_desc, char* server_addr, int server_port);
int SocketStateInteract(int socket_desc);
int SocketStateClose(int new_socket);

/*************************************/

#endif