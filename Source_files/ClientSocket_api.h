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

/// @brief Runs client socket.
/// @param server_addr Target server address
/// @param server_port Target server port.
/// @param secure Enable secure communication (TLS).
/// @param CustomSocketStateInteract Custom function to interact with server once connection is established.
/// @return 0 always, exit sending failure signal if SIGINT signal handler could not be properly set.
int ClientSocketRun(char* server_addr, int server_port, bool secure, void (*CustomSocketStateInteract)(int client_socket, bool secure, SSL** ssl));

/*************************************/

#ifdef __cplusplus
}
#endif

#endif