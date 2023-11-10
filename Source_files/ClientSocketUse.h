#ifndef CLIENT_SOCKET_USE_H
#define CLIENT_SOCKET_USE_H

/************************************/
/******** Include statements ********/
/************************************/

#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <stdbool.h>

/************************************/

/*************************************/
/******* Function definitions ********/
/*************************************/

int CreateSocketDescriptor(int domain, int type, int protocol);
struct sockaddr_in PrepareForConnection(sa_family_t address_family, char* server_addr, uint16_t server_port);
int SocketConnect(int socket_desc, struct sockaddr_in server);
void SocketInteract(int server_socket, bool secure, SSL** ssl);
int CloseSocket(int new_socket);

/*************************************/

#endif