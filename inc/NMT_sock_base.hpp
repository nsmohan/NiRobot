/** 
 *  @file      NMT_sock_base.hpp
 *  @brief     Header file for NMT_sock_base.cpp
 *  @details   Header file for NMT socket wrapper library
 *  @author    Nitin Mohan
 *  @date      Feb 22, 2020
 *  @copyright 2020 - NM Technologies
 */ 
#ifndef DEF_NMT_sock_base
#define DEF_NMT_sock_base
/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <arpa/inet.h>
#include <tuple>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_stdlib.h"



/*--------------------------------------------------/
/                   Constants                       /
/--------------------------------------------------*/
const int MAX_CLIENTS = 5;

/**
 * @enum sock_mode
 * Socket Mode (Server or Client)
 */
typedef enum sock_mode {SOCK_CLIENT, SOCK_SERVER}sock_mode;

/**
 * @enum sock_type
 */
typedef enum sock_type {SOCK_TCP, SOCK_MULTI_CAST}sock_type;

typedef enum SOCK_STATE {SOCK_OK, SOCK_ERROR, SOCK_DISCONNECTED, SOCK_IDLE}sock_state;
const std::string sock_state_e2s[] {"SOCK_OK", "SOCK_ERROR", "SOCK_DISCONNECTED", "SOCK_IDLE"};

class NMT_sock
{
    public:
        /* Constructor */
        NMT_sock(unsigned int port, 
                 std::string ip_address,
                 sock_mode socket_mode,
                 sock_type socket_type,
                 unsigned int socket_timeout = 60);

        /* Getter to return the result */
        NMT_result NMT_get_result() {return this->result;}

    protected:

        /** @var my_address
         *  Object which contains current systems attributes*/
        struct sockaddr_in my_address; 
        
        socklen_t adder_len;

        /** @var sock
         *  Socket object */
        int sock; 

        /** @var max_buffer_size
         *  Max Size of Buffer */
        const unsigned int max_buffer_size = 10240;

        /** @var tv
         *  Socket TimeOut*/
        struct timeval tv; 

        /* Prototypes */
        SOCK_STATE NMT_sock_check_message(int nbytes, int client=-1);



    private:
        /** @var result
         *  Varible to set the overall state of the object */
        NMT_result result = OK;

        /** @var port
         *  Port to communicate on */
        unsigned int port;

        /** @var multicast_ip
         *  IP Address to send multicast messages to */
       std::string  ip_address;

        /** @var mode
         *  Mode of socket */
        sock_mode mode;

        /** @var s_type
         *  Type of socket */
        int type;

        /** @var socket_timeout
         *  Time out for Client */
        unsigned int socket_timeout;

        /* Initialize the socket as a server */
        NMT_result NMT_init_server();

        /* Initialize the socket as a client */
        NMT_result NMT_init_client();

        /* Prototype Definitions */
        NMT_result NMT_sock_set_multi_cast();
        NMT_result NMT_sock_read_tcp_message(socklen_t adder_len, char **message);
        NMT_result NMT_sock_read_multi_message(socklen_t adder_len, char **message);

};
#endif
