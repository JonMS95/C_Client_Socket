/************************************/
/******** Include statements ********/
/************************************/

#include "SeverityLog_api.h"
#include "ClientSocket_api.h"
#include "GetOptions_api.h"

/************************************/

/***************************************/
/********** Define statements **********/
/***************************************/

#define TEST_LOG_BUFFER_SIZE    10000
#define TEST_LOG_MASK           0XFF

/************ Addr settings ************/

#define SERVER_ADDR_SIZE    16

#define ADDR_OPT_CHAR       'a'
#define ADDR_OPT_LONG       "Address"
#define ADDR_OPT_DETAIL     "Target server address."
#define ADDR_DEFAULT_VALUE  "192.168.1.148"

/************ Port settings ************/

#define PORT_OPT_CHAR       'p'
#define PORT_OPT_LONG       "Port"
#define PORT_OPT_DETAIL     "Target server port."
#define PORT_MIN_VALUE      49152
#define PORT_MAX_VALUE      65535
#define PORT_DEFAULT_VALUE  50000

/********* Secure connection *********/

#define SECURE_CONN_CHAR            's'
#define SECURE_CONN_LONG            "Secure"
#define SECURE_CONN_DETAIL          "Secure connection."
#define SECURE_CONN_DEFAULT_VALUE   false

/***************************************/

int main(int argc, char** argv)
{
    SeverityLogInitWithMask(TEST_LOG_BUFFER_SIZE, TEST_LOG_MASK);

    int server_port;
    bool secure_connection;
    char* server_addr = calloc(SERVER_ADDR_SIZE + 1, 1);

    SetOptionDefinitionBool(SECURE_CONN_CHAR            ,
                            SECURE_CONN_LONG            ,
                            SECURE_CONN_DETAIL          ,
                            SECURE_CONN_DEFAULT_VALUE   ,
                            &secure_connection          );

    SetOptionDefinitionInt( PORT_OPT_CHAR       ,
                            PORT_OPT_LONG       ,
                            PORT_OPT_DETAIL     ,
                            PORT_MIN_VALUE      ,
                            PORT_MAX_VALUE      ,
                            PORT_DEFAULT_VALUE  ,
                            &server_port        );

    SetOptionDefinitionStringNL(ADDR_OPT_CHAR     ,
                                ADDR_OPT_LONG     ,
                                ADDR_OPT_DETAIL   ,
                                ADDR_DEFAULT_VALUE,
                                server_addr);

    int parse_arguments = ParseOptions(argc, argv);
    if(parse_arguments < 0)
    {
        SVRTY_LOG_ERR("Arguments parsing failed!");
        return parse_arguments;
    }

    SVRTY_LOG_INF("Arguments successfully parsed!");

    ClientSocketRun(server_addr, server_port, secure_connection, NULL);

    return 0;
}