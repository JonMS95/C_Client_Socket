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

/// @brief 
/// @param server_socket 
/// @param secure 
/// @param ssl 
void SocketDefaultInteractFn(int server_socket, bool secure, SSL** ssl);

/*************************************/

#endif
