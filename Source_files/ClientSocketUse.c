/************************************/
/******** Include statements ********/
/************************************/

#include <sys/socket.h>     // socket, connect functions.
#include <arpa/inet.h>      // sockaddr_in, inet_addr
#include <unistd.h>         // Write socket.
#include "SeverityLog_api.h" // Severity Log.

/************************************/

/*************************************/
/******* Function definitions ********/
/*************************************/

/// @brief Create socket descriptor.
/// @param domain Use AF_INET if the socket is meant to be serving to another computer in the same net,
/// AF_LOCAL if the purpose is to communicate different processes within the same host.
/// @param type Use SOCK_STREAM for TCP, SOCK_DGRAM for UDP.
/// @param protocol Use IPPROTO_IP (0) to use the internet protocol (IP).
/// @return  a file descriptor > 0 number if the socket descriptor was succesfully created, -1 if any error happened. 
int CreateSocketDescriptor(int domain, int type, int protocol)
{
    return socket(domain, type, protocol);
}

/// @brief Prepare the sockaddr_in struct for the binding process to be completed properly.
/// @param address_family Address family the server is going to work with.
/// @param server_addr Target server IP address.
/// @param server_port Target server port number.
/// @return A struct which contains all the parameters related to a socket descriptor. 
struct sockaddr_in PrepareForConnection(sa_family_t address_family, char* server_addr, uint16_t server_port)
{
    // Prepare the sockaddr_in structure for the binding process.
    struct sockaddr_in server;
    server.sin_family = address_family;                 // IPv4.
    server.sin_addr.s_addr = inet_addr(server_addr);    // Any address is allowed to connect to the socket.
    server.sin_port = htons(server_port);               // The htons() function makes sure that numbers are stored in memory
                                                        // in network byte order, which is with the most significant byte first.

    return server;
}

/// @brief Connects to the target server.
/// @param socket_desc previously created client socket descriptor.
/// @param server tagrget server client is meant to connect to.
/// @return 0 if everything went to plan, <0 otherwise.
int SocketConnect(int socket_desc, struct sockaddr_in server)
{
    // socklen_t file_desc_len = (socklen_t)sizeof(struct sockaddr_in);
    return connect(socket_desc, (struct sockaddr*)&server, sizeof(server));
}

/// @brief Closes the socket.
/// @param new_socket ID of the socket that is meant to be closed.
/// @return < 0 if close failed.
int CloseSocket(int new_socket)
{
    // Close the socket.
    int close_socket = close(new_socket);

    return close_socket;
}

/*************************************/
