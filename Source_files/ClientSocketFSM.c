/************************************/
/******** Include statements ********/
/************************************/

#include <unistd.h>
#include "ClientSocketUse.h"
#include "ClientSocketFSM.h"
#include "SeverityLog_api.h"

/************************************/

/*************************************/
/******* Function definitions ********/
/*************************************/

int SocketStateCreate()
{
    int socket_desc = CreateSocketDescriptor(AF_INET, SOCK_STREAM, IPPROTO_IP);

    if(socket_desc < 0)
    {
        LOG_ERR(CLIENT_SOCKET_MSG_CREATION_NOK);
    }
    else
    {
        LOG_INF(CLIENT_SOCKET_MSG_CREATION_OK);
    }

    return socket_desc;
}

int SocketStateConnect(int socket_desc, char* server_addr, int server_port)
{
    struct sockaddr_in server_data = PrepareForConnection(AF_INET, server_addr, server_port);
    int socket_connect = SocketConnect(socket_desc, server_data);

    if(socket_connect < 0)
    {
        LOG_ERR(CLIENT_SOCKET_MSG_CONNECT_NOK, server_addr, server_port);
    }
    else
    {
        LOG_INF(CLIENT_SOCKET_MSG_CONNECT_OK, server_addr, server_port);
    }
    
    return socket_connect;
}

int SocketStateInteract(int socket_desc)
{
    SocketInteract(socket_desc);

    return 0;
}

/// @brief Close socket.
/// @param new_socket Socket file descriptor.
/// @return < 0 if it failed to close the socket.
int SocketStateClose(int new_socket)
{
    int close = CloseSocket(new_socket);
    
    if(close < 0)
    {
        LOG_ERR(CLIENT_SOCKET_MSG_CLOSE_NOK);
    }
    else
    {
        LOG_INF(CLIENT_SOCKET_MSG_CLOSE_OK);
    }

    return close;    
}

int ClientSocketRun(char* server_addr, int server_port)
{
    CLIENT_SOCKET_FSM client_socket_fsm = CREATE_FD;
    int socket_desc;

    while(1)
    {
        switch(client_socket_fsm)
        {
            case CREATE_FD:
            {
                socket_desc = SocketStateCreate();
                if(socket_desc < 0)
                {
                    client_socket_fsm = CREATE_FD;
                }
                else
                {
                    client_socket_fsm = CONNECT;
                }
            }
            break;

            case CONNECT:
            {
                if(SocketStateConnect(socket_desc, server_addr, server_port) < 0)
                {
                    sleep(CLIENT_SOCKET_WAIT_RECONNECT_SEC);
                    client_socket_fsm = CREATE_FD;
                }
                else
                {
                    client_socket_fsm = INTERACT;
                }
            }
            break;

            case INTERACT:
            {
                SocketInteract(socket_desc);

                client_socket_fsm = CLOSE;
            }
            break;

            case CLOSE:
            {
                if(SocketStateClose(socket_desc) < 0)
                {
                    client_socket_fsm = CLOSE;
                }

                return 0;
            }
            break;

            default:
            break;
        }
    }
}

/************************************/
