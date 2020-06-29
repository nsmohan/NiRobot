/** 
 *  @file      NMT_sock_multi.hpp
 *  @brief     Header file for NMT_sock_multi.cpp
 *  @details   Header file for NMT socket wrapper library
 *  @author    Nitin Mohan
 *  @date      June 10, 2020
 *  @copyright 2020 - NM Technologies
 */

#ifndef DEF_NMT_sock_multi
#define DEF_NMT_sock_multi
/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_stdlib.h"
#include "NMT_sock_base.hpp"

/*--------------------------------------------------/
/                   Start of Program                /
/--------------------------------------------------*/
class NMT_sock_multi : public NMT_sock
{
    public:
        /* Constructor */
        NMT_sock_multi(unsigned int port,
                       std::string ip_address,
                       sock_mode sock_mode,
                       unsigned int socket_timeout = 60)
            : NMT_sock(port, ip_address, sock_mode, SOCK_MULTI_CAST, socket_timeout) {};

        std::tuple<NMT_result, std::string> NMT_read_socket();
        NMT_result NMT_write_socket(char *message);
};
#endif
