/************************************/
/******** Include statements ********/
/************************************/

#include <stdbool.h>        // Bool type
#include <openssl/ssl.h>    // SSL / TLS
#include <fcntl.h>          // Set socket flags
#include <arpa/inet.h>      // sockaddr_in, inet_addr
#include <unistd.h>         // read, write
#include "SeverityLog_api.h"

/************************************/

/************************************/
/********* Define statements ********/
/************************************/

#define CLIENT_SOCKET_LEN_RX_BUFFER             256     // RX buffer size.
#define CLIENT_SOCKET_LEN_TX_BUFFER             256     // TX buffer size.

#define CLIENT_SOCKET_MSG_ERR_GET_SOCKET_FLAGS  "Error while getting socket flags."
#define CLIENT_SOCKET_MSG_ERR_SET_SOCKET_FLAGS  "Error while setting O_NONBLOCK flag."

#define CLIENT_SOCKET_MSG_GREETING              "Hello Server! It's a fine day today, \"innit\"?"
#define CLIENT_SOCKET_MSG_DATA_READ_FROM_SERVER "Data read from server: <%s>"
#define CLIENT_SOCKET_MSG_SERVER_DISCONNECTED   "Server with IP <%s> disconnected."

/************************************/

/// @brief Set the socket as non-blocking.
/// @param client_socket Client socket.
/// @return < 0 is getting / settings flags failed 0, otherwise.
static int ClientSocketSetNonBlocking(int server_socket)
{
    // Fisrt, try to get socket's current flag set.
    int flags = fcntl(server_socket, F_GETFL, 0);
    if(flags < 0)
    {
        SVRTY_LOG_ERR(CLIENT_SOCKET_MSG_ERR_GET_SOCKET_FLAGS);
        return flags;
    }

    // Then, set the O_NONBLOCK flag (which, as the name suggests, makes the socket non-blocking).
    flags |= O_NONBLOCK;
    if(fcntl(server_socket, F_SETFL, flags) < 0)
    {
        SVRTY_LOG_ERR(CLIENT_SOCKET_MSG_ERR_SET_SOCKET_FLAGS);
        return flags;
    }

    return 0;
}

/// @brief Get server's IP address.
/// @param server_socket Server socket.
/// @param server_IPv4 Target string to which server IPv4 address is meant to be copied.
static void ClientSocketGetServerIPv4(int server_socket, char* server_IPv4)
{
    struct sockaddr_in server;
    socklen_t server_len = sizeof(server);
    getpeername(server_socket, (struct sockaddr*)&server, &server_len);
    inet_ntop(AF_INET, &server.sin_addr, server_IPv4, INET_ADDRSTRLEN);
}

/// @brief Reads from server, then sends a response.
/// @param server_socket Server socket.
/// @param secure True if TLS security is wanted, false otherwise.
/// @param ssl SSL data.
void SocketDefaultInteractFn(int server_socket, bool secure, SSL** ssl)
{
    // Make the socket read non-blocking.
    int set_non_blocking = ClientSocketSetNonBlocking(server_socket);
    if(set_non_blocking < 0)
        return;

    // Get server's IP address (IPv4).
    char server_IP_addr[INET_ADDRSTRLEN] = {};
    ClientSocketGetServerIPv4(server_socket, server_IP_addr);

    char tx_buffer[CLIENT_SOCKET_LEN_TX_BUFFER] = {};
    memset(tx_buffer, 0, sizeof(tx_buffer));
    strcpy(tx_buffer, CLIENT_SOCKET_MSG_GREETING);
    
    if(!secure)
        write(server_socket, tx_buffer, strlen(tx_buffer));
    else
        SSL_write(*ssl, tx_buffer, sizeof(tx_buffer));

    char rx_buffer[CLIENT_SOCKET_LEN_RX_BUFFER] = {};
    memset(rx_buffer, 0, sizeof(rx_buffer));

    ssize_t read_from_socket = 0;
    bool something_read = false;

    while( (read_from_socket >= 0) || (something_read == false) )
    {
        if(!secure)
            read_from_socket = read(server_socket, rx_buffer, sizeof(rx_buffer));
        else
            read_from_socket = SSL_read(*ssl, rx_buffer, sizeof(rx_buffer));

        if(read_from_socket == 0)
        {
            SVRTY_LOG_WNG(CLIENT_SOCKET_MSG_SERVER_DISCONNECTED, server_IP_addr);
            break;
        }

        // If no data has been found in the rx buffer but it has been found previously, then exit.
        // It is assumed that all the data that was meant to be read has already been read.
        if(read_from_socket < 0)
        {
            if(something_read)
                break;
            
            continue;
        }

        something_read = true;
        SVRTY_LOG_INF(CLIENT_SOCKET_MSG_DATA_READ_FROM_SERVER, rx_buffer);
        
        // Clean the buffer after reading.
        memset(rx_buffer, 0, read_from_socket);

        // Wait for a millisecond before trying to read again.
        usleep(1000);
    }
}
