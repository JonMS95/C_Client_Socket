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

// #define SERVER_ADDR_SIZE    16
#define SERVER_ADDR         "192.168.1.143"

#define PORT_OPT_CHAR       'p'
#define PORT_OPT_LONG       "Port"
#define PORT_OPT_DETAIL     "Target server port."
#define PORT_MIN_VALUE      49152
#define PORT_MAX_VALUE      65535
#define PORT_DEFAULT_VALUE  50000

/***************************************/

int main(int argc, char** argv)
{
    SetSeverityLogMask(SVRTY_LOG_MASK_ALL);

    int server_port;

    SetOptionDefinitionInt( PORT_OPT_CHAR       ,
                            PORT_OPT_LONG       ,
                            PORT_OPT_DETAIL     ,
                            PORT_MIN_VALUE      ,
                            PORT_MAX_VALUE      ,
                            PORT_DEFAULT_VALUE  ,
                            &server_port        );

    int parse_arguments = ParseOptions(argc, argv);
    if(parse_arguments < 0)
    {
        LOG_ERR("Arguments parsing failed!");
        return parse_arguments;
    }

    LOG_INF("Arguments successfully parsed!");

    ClientSocketRun(SERVER_ADDR, server_port);

    return 0;
}