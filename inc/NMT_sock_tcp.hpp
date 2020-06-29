/** 
 *  @file      NMT_sock_tcp.hpp
 *  @brief     Header file for NMT_sock_tcp.cpp
 *  @details   Header file for NMT socket wrapper library
 *  @author    Nitin Mohan
 *  @date      June 10, 2020
 *  @copyright 2020 - NM Technologies
 */

#ifndef DEF_NMT_sock_tcp
#define DEF_NMT_sock_tcp
/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_stdlib.h"
#include "NMT_sock_base.hpp"

/*--------------------------------------------------/
/                   Constants                       /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                   Start of Program                /
/--------------------------------------------------*/
class NMT_sock_tcp: public NMT_sock
{
    public:
        /* Constructor */
        NMT_sock_tcp(unsigned int port,
                     std::string ip_address,
                     sock_mode sock_mode,
                     unsigned int socket_timeout = 60)
            : NMT_sock(port, ip_address, sock_mode, SOCK_TCP, socket_timeout) {}; 
        std::tuple<NMT_result, std::string, int> NMT_read_socket();
        NMT_result NMT_write_socket(std::string message, int sock_id=-1);
    private: 
            
        /** @var client_sockets
         *  Client Sockets Array */
        std::vector <int> client_sockets;

        int max_sd;

        /** @var readfds
         *  Select File Descriptor */
        fd_set readfds;

        void init_select();
        std::tuple<NMT_result, bool> listen();
        std::tuple<NMT_result, bool> accept_new_connection();
        std::tuple<SOCK_STATE, std::string, int> read_message();
};
#endif
