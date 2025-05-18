#ifndef CLIENT_SOCKET_DEFAULT_INTERACT_H
#define CLIENT_SOCKET_DEFAULT_INTERACT_H

/************************************/
/******** Include statements ********/
/************************************/

#include <stdbool.h>
#include <openssl/ssl.h>

/*************************************/

/*************************************/
/******** Function prototypes ********/
/*************************************/

/// @brief Reads from server, then sends a response.
/// @param server_socket Server socket.
/// @param secure True if TLS security is wanted, false otherwise.
/// @param ssl SSL data. 
void SocketDefaultInteractFn(int server_socket, bool secure, SSL** ssl);

/*************************************/

#endif
