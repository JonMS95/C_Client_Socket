/************************************/
/******** Include statements ********/
/************************************/

#include <unistd.h>         // Sleep
#include <signal.h>         // Shutdown signal.
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "ClientSocketUse.h"
#include "ClientSocketSSL.h"
#include "ClientSocketFSM.h"
#include "ClientSocketDefaultInteract.h"
#include "SeverityLog_api.h"

/************************************/

/***********************************/
/******** Private variables ********/
/***********************************/

static volatile int ctrlCPressed    = 0;

static SSL_CTX* ctx                 = NULL;
static SSL*     ssl                 = NULL;

/// @brief Pointer to a function which is meant to interact with the server.
/// @param server_socket Server socket.
/// @param secure True if TLS security is wanted, false otherwise.
/// @param ssl SSL data.
/// @return < 0 if any error happened, 0 otherwise.
static void (*SocketStateInteract)(int server_socket, bool secure, SSL** ssl)  = &SocketDefaultInteractFn;

/***********************************/

/*************************************/
/******* Function definitions ********/
/*************************************/

/// @brief Frees previously heap allocated memory before exiting the program.
static void SocketFreeResources(void)
{
    if(ssl != NULL)
    {
        LOG_DBG(CLIENT_SOCKET_MSG_CLEANING_UP_SSL);
        SSL_free(ssl);
    }

    if(ctx != NULL)
    {
        LOG_DBG(CLIENT_SOCKET_MSG_CLEANING_UP_SSL_CTX);
        SSL_CTX_free(ctx);
    }

    ERR_free_strings();
    EVP_cleanup();
    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();
    CONF_modules_unload(1);
    CONF_modules_free();
    EVP_cleanup();
    CRYPTO_cleanup_all_ex_data();
}

/// @brief Handle SIGINT signal (Ctrl+C).
/// @param signum Signal number (SIGINT by default).
static void SocketSIGINTHandler(int signum)
{
    LOG_WNG(CLIENT_SOCKET_MSG_SIGINT_RECEIVED);
    ctrlCPressed = 1; // Set the flag to indicate Ctrl+C was pressed

    SocketFreeResources();

    exit(EXIT_SUCCESS);
}

/// @brief Create socket descriptor.
/// @return < 0 if it failed to create the socket.
static int SocketStateCreate(void)
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

/// @brief Setup SSL context.
/// @param ctx SSL context.
/// @return 0 if succeeded, < 0 otherwise.
static int SocketStateSetupSSL(SSL_CTX** ctx)
{
    int client_socket_SSL_setup = ClientSocketSSLSetup(ctx);

    if(client_socket_SSL_setup != CLIENT_SOCKET_SETUP_SSL_SUCCESS)
        LOG_ERR(CLIENT_SOCKET_MSG_SETUP_SSL_NOK);
    else
        LOG_INF(CLIENT_SOCKET_MSG_SETUP_SSL_OK);

    return (client_socket_SSL_setup == CLIENT_SOCKET_SETUP_SSL_SUCCESS ? 0 : -1);
}

/// @brief Connects to the server sicket with specified address and port.
/// @param socket_desc Socket file descriptor.
/// @param server_addr Server IP address.
/// @param server_port Server port.
/// @return < 0 if it failed to connect.
static int SocketStateConnect(int socket_desc, char* server_addr, int server_port)
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

/// @brief Perform SSL handshake if needed.
/// @param server_socket Server socket instance.
/// @param ctx SSL context.
/// @param ssl SSL data.
/// @return 0 if handhsake was successfully performed, < 0 otherwise.
static int SocketStateSSLHandshake(int server_socket, SSL_CTX** ctx, SSL** ssl)
{
    int ssl_handshake = ClientSocketSSLHandshake(server_socket, ctx, ssl);

    if(ssl_handshake != CLIENT_SOCKET_SSL_HANDSHAKE_SUCCESS)
        LOG_ERR(CLIENT_SOCKET_MSG_SSL_HANDSHAKE_NOK);
    else
        LOG_INF(CLIENT_SOCKET_MSG_SSL_HANDSHAKE_OK);

    return (ssl_handshake == CLIENT_SOCKET_SSL_HANDSHAKE_SUCCESS ? 0 : -1);
}

/// @brief Close socket.
/// @param new_socket Socket file descriptor.
/// @return < 0 if it failed to close the socket.
static int SocketStateClose(int new_socket)
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

/// @brief Runs client socket.
/// @param server_addr Target server address
/// @param server_port Target server port.
/// @param secure Enable secure communication (TLS).
/// @param CustomSocketStateInteract Custom function to interact with server once connection is established.
/// @return 0 always, exit sending failure signal if SIGINT signal handler could not be properly set.
int ClientSocketRun(char* server_addr, int server_port, bool secure, void (*CustomSocketStateInteract)(int client_socket, bool secure, SSL** ssl))
{
    CLIENT_SOCKET_FSM client_socket_fsm = CREATE_FD;
    int socket_desc;

    if(CustomSocketStateInteract != NULL)
        SocketStateInteract = CustomSocketStateInteract;

    if(signal(SIGINT, SocketSIGINTHandler) == SIG_ERR)
    {
        LOG_ERR(CLIENT_SOCKET_SET_SIGINT_ERR);
        exit(EXIT_FAILURE);
    }

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
                SocketStateInteract(socket_desc, secure, &ssl);

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

    return 0;
}

/************************************/
