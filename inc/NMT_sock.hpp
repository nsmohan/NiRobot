/** 
 *  @file      NMT_sock.hpp
 *  @brief     Header file for NMT_sock.cpp
 *  @details   Header file for NMT socket wrapper library
 *  @author    Nitin Mohan
 *  @date      Feb 22, 2019
 *  @copyright 2020 - NM Technologies
 */

#ifndef DEF_NMT_sock
#define DEF_NMT_sock
/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <arpa/inet.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_stdlib.h"


typedef enum sock_mode {SOCK_CLIENT, SOCK_SERVER}sock_mode;

class NMT_sock_multicast
{
    public:
        /* Constructor */
        NMT_sock_multicast(unsigned int port, std::string multicast_ip,
                           sock_mode socket_mode, unsigned int sec = 60);

        /* Function to send message over the socket */
        NMT_result NMT_write_socket(char *message);

        /* Function to read message on the socket */
        NMT_result NMT_read_socket(char **message);

        /* Getter function to return the result */
        NMT_result NMT_get_result() {return this->result;}

    private:
        /** @var result
         *  Varible to set the overall state of the object */
        NMT_result result = OK;

        /** @var port
         *  Port to communicate on */
        unsigned int port;

        /** @var multicast_ip
         *  IP Address to send multicast messages to */
       std::string  multicast_ip;

        /** @var sock
         *  Socket object */
        int sock; 

        /** @var sec
         *  Time out for Client */
        unsigned int sec;

        /** @var mode
         *  Mode of socket */
        sock_mode mode;

        /** @var my_address
         *  Object which contains current systems attributes*/
        struct sockaddr_in my_address; 

        /* Function to initialize the multicast protocol */
        NMT_result NMT_init_multicast_server();

        /* Function to initialize the multicast protocol */
        NMT_result NMT_init_multicast_client();

};


#endif
