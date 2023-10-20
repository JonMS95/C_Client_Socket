#ifndef CLIENT_SOCKET_USE_H
#define CLIENT_SOCKET_USE_H

#include <sys/socket.h>
#include <arpa/inet.h>

int CreateSocketDescriptor(int domain, int type, int protocol);
struct sockaddr_in PrepareForConnection(sa_family_t address_family, char* server_addr, uint16_t server_port);
int SocketConnect(int socket_desc, struct sockaddr_in server);
void SocketWrite(int new_socket);
int CloseSocket(int new_socket);

#endif