#ifndef CLIENT_SOCKET_API_H
#define CLIENT_SOCKET_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*************************************/
/******** Function prototypes ********/
/*************************************/

int ClientSocketRun(char* server_addr, int server_port);

/*************************************/

#ifdef __cplusplus
}
#endif

#endif