/************************************/
/******** Include statements ********/
/************************************/

#include <unistd.h>
#include <openssl/ssl.h>
#include "ClientSocketUse.h"
#include "ClientSocketSSL.h"
#include "ClientSocketFSM.h"
#include "SeverityLog_api.h"

/************************************/

/***********************************/
/******** Private variables ********/
/***********************************/

static SSL_CTX* ctx = NULL;
static SSL*     ssl = NULL;

/***********************************/

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

int SocketStateSetupSSL(SSL_CTX** ctx)
{
    int client_socket_SSL_setup = ClientSocketSSLSetup(ctx);

    if(client_socket_SSL_setup != CLIENT_SOCKET_SETUP_SSL_SUCCESS)
        LOG_ERR(CLIENT_SOCKET_MSG_SETUP_SSL_NOK);
    else
        LOG_INF(CLIENT_SOCKET_MSG_SETUP_SSL_OK);

    return (client_socket_SSL_setup == CLIENT_SOCKET_SETUP_SSL_SUCCESS ? 0 : -1);
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

int SocketStateSSLHandshake(int server_socket, SSL_CTX** ctx, SSL** ssl)
{
    int ssl_handshake = ClientSocketSSLHandshake(server_socket, ctx, ssl);

    if(ssl_handshake != CLIENT_SOCKET_SSL_HANDSHAKE_SUCCESS)
        LOG_ERR(CLIENT_SOCKET_MSG_SSL_HANDSHAKE_NOK);
    else
        LOG_INF(CLIENT_SOCKET_MSG_SSL_HANDSHAKE_OK);

    return (ssl_handshake == CLIENT_SOCKET_SSL_HANDSHAKE_SUCCESS ? 0 : -1);
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

int ClientSocketRun(char* server_addr, int server_port, bool secure)
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
                    client_socket_fsm = SETUP_SSL;
                }
            }
            break;

            case SETUP_SSL:
            {
                if(!secure)
                {
                    client_socket_fsm = CONNECT;
                    continue;
                }

                if(SocketStateSetupSSL(&ctx) < 0)
                    client_socket_fsm = CLOSE;
                else
                    client_socket_fsm = CONNECT;
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
                    client_socket_fsm = SSL_HANDSHAKE;
                }
            }
            break;

            case SSL_HANDSHAKE:
            {
                if(!secure)
                {
                    client_socket_fsm = INTERACT;
                    continue;
                }

                if(SocketStateSSLHandshake(socket_desc, &ctx, &ssl) >= 0)
                    client_socket_fsm = INTERACT;
                else
                    client_socket_fsm = CONNECT;
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
