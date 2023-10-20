#include <stdio.h>
#include <string.h>
#include <sys/socket.h>     // socket, connect functions.
#include <arpa/inet.h>      // sockaddr_in, inet_addr
#include "../../Source_files/ClientSocketUse.h"

#define SERVER_ADDR "192.168.1.143"
#define SERVER_PORT 50000

int main()
{
    int socket_desc = CreateSocketDescriptor(AF_INET, SOCK_STREAM, IPPROTO_IP);

    if(socket_desc < 0)
    {
        printf("ERROR CREATING SOCKET FD.\r\n");
    }
    else
    {
        printf("SOCKET FD CREATED SUCCESSFULLY.\r\n");
    }

    struct sockaddr_in server_data = PrepareForConnection(AF_INET, SERVER_ADDR, SERVER_PORT);

    SocketConnect(socket_desc, server_data);
    SocketWrite(socket_desc);
    
    CloseSocket(socket_desc);

    return 0;
}