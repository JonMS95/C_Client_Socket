#ifndef CLIENT_SOCKET_API_H
#define CLIENT_SOCKET_API_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************/
/******** Include statements ********/
/************************************/

#include <stdbool.h>
#include <openssl/ssl.h>

/************************************/

/*************************************/
/******** Function prototypes ********/
/*************************************/

int ClientSocketRun(char* server_addr, int server_port, bool secure, void (*CustomSocketStateInteract)(int client_socket, bool secure, SSL** ssl));

/*************************************/

#ifdef __cplusplus
}
#endif

#endif