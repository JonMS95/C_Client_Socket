#ifndef CLIENT_SOCKET_API_H
#define CLIENT_SOCKET_API_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************/
/******** Include statements ********/
/************************************/

#include <stdbool.h>

/************************************/

/*************************************/
/******** Function prototypes ********/
/*************************************/

int ClientSocketRun(char* server_addr, int server_port, bool secure);

/*************************************/

#ifdef __cplusplus
}
#endif

#endif