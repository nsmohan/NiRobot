/** 
 *  @file      NMT_sock.cpp
 *  @brief     Wrapper Library sockets
 *  @details   This library is designed to facilate in socket Communication
 *  @author    Nitin Mohan
 *  @date      Feb 22, 2019
 *  @copyright 2020 - NM Technologies
 */
/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <iostream>
#include <tuple>
#include <string.h> 
#include <sys/time.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_sock_base.hpp"
#include "NMT_stdlib.h"
#include "NMT_log.h"

/*--------------------------------------------------/
/                   Constants                       /
/--------------------------------------------------*/

using namespace std;
NMT_sock::NMT_sock(unsigned int port, string ip_address,
                   SOCK_MODE socket_mode, SOCK_TYPE socket_type,
                   unsigned int socket_timeout) : 
                   port(port), ip_address(ip_address), mode(socket_mode), 
                   socket_timeout(socket_timeout)
{
    /*!
     *  @brief     Constructor for NMT_sock
     *  @param[in] port
     *  @param[in] ip_address
     *  @param[in] socket_mode
     *  @param[in] socket_timeout
     *  @param[in] max_connect
     *  @return    NMT_result
     */

    NMT_log_write(DEBUG, (char *)"> port=%u, ip_address=%s, mode=%s, type=%s, time_out=%u",
                  port, ip_address.c_str(), sock_mode_e2s[socket_mode].c_str(), sock_type_e2s[socket_type].c_str(), socket_timeout);

    /* Check Socket Type */
    socket_type == SOCK_TCP ? this->type = SOCK_STREAM : this->type = SOCK_DGRAM;

    /* Initialize socket as Client/Server */
    socket_mode == SOCK_SERVER ? this->result = NMT_init_server() : this->result = NMT_init_client();

    NMT_log_write(DEBUG, (char *)"< ");
}

/*------------------------------------------------------------------------/
/                                Public Methods                           /
/------------------------------------------------------------------------*/
NMT_result NMT_sock::NMT_init_client()
{
    /*!
     *  @brief     Initialize the multicast protocol in server mode
     *  @return    NMT_result
     */

    NMT_log_write(DEBUG, (char *)"> ");

    /* Initialize Varibles */
    unsigned int opt = 1; 
    NMT_result result = OK;

    /* Create socket for sending/receiving datagrams */
    if ((this->sock = socket(AF_INET, this->type,
                             (this->type == SOCK_STREAM ? 0 : IPPROTO_UDP))) < 0)
    {
        result = NOK;
        NMT_log_write(ERROR, (char *)"Error when creating socket!");
    }

    if (result == OK)
    {
        /* Construct local address structure */
        memset(&my_address, 0, sizeof(this->my_address));
        this->my_address.sin_family = AF_INET;
        this->my_address.sin_port = htons(this->port);
        this->my_address.sin_addr.s_addr = inet_addr(this->ip_address.c_str());
    }

    if (result == OK)
    {
        if (this->type == SOCK_DGRAM)
        {
            /* Set TTL of multicast packet */
            if (setsockopt(this->sock, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &opt, sizeof(opt)) < 0)
            {
                result = NOK;
                NMT_log_write(ERROR, (char *)"Error when setting TTL packet for multi-cast");
            }
        }
        else
        {
            if (connect(sock, (struct sockaddr *)&(this->my_address), sizeof(this->my_address)) < 0) 
               { 
                   result = NOK;
                   NMT_log_write(ERROR, (char *)"Failed to establish TCP connection");
               } 
        }
    }

    /* Exit the function */
    NMT_log_write(DEBUG, (char *)"< result=%s", result_e2s[result]);
    return result;
}

NMT_result NMT_sock::NMT_init_server()
{
    /*!
     *  @brief     Initialize the multicast protocol in client mode
     *  @return    NMT_result
     */

    NMT_log_write(DEBUG, (char *)"> ");
    NMT_result result = OK;

    if ((this->sock = socket(AF_INET, this->type, 0)) <= 0)
    {
        NMT_log_write(ERROR, (char *)"Failed to create Socket!");
        result = NOK;
        cout << "Error=" << errno << endl;
    }

    if (result == OK)
    {
        /* Allow multiple sockets to use the same PORT number */
        unsigned int opt = 1;
        if (setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) 
        {
            NMT_log_write(ERROR, (char *)"Failed to set Reuse Address for socket! errno=%d", errno);
            result = NOK;
        }
    }

    if (result == OK)
    {
        /* Set socket timeout */
        tv.tv_sec = this->socket_timeout;
        tv.tv_usec = 0;
        if (setsockopt(this->sock, SOL_SOCKET, SO_RCVTIMEO,&tv, sizeof(tv)) < 0)
        {
            NMT_log_write(ERROR, (char *)"Failed to set socket timeout errno=%d", errno);
            result = NOK;
        }
    }

    if (result == OK)
    {
        /* Set up destination address */
        memset(&(this->my_address), 0, sizeof(this->my_address));
        this->my_address.sin_family = AF_INET;
        this->my_address.sin_addr.s_addr = htonl(INADDR_ANY);
        this->my_address.sin_port = htons(this->port);

        /* Bind to receive address */
        if (bind(this->sock, (struct sockaddr*) &(this->my_address), 
                 sizeof(this->my_address)) < 0) 
        {
            NMT_log_write(ERROR, (char *)"Socket Bind Failed");     
            result = NOK;
        }
    }

    if (result == OK)
    {
        if (this->type == SOCK_DGRAM)
        {
            result = NMT_sock::NMT_sock_set_multi_cast();
        }
        else
        {
            /* Listen for TCP Connection */
            listen(this->sock, MAX_CLIENTS);
            NMT_log_write(DEBUG, (char *)"Listening on TCP Socket");
        }
    }


    /* Exit the function */
    NMT_log_write(DEBUG, (char *)"< result=%s", result_e2s[result]);
    return result;
}

/*------------------------------------------------------------------------/
/                                Private Methods                          /
/------------------------------------------------------------------------*/
NMT_result NMT_sock::NMT_sock_set_multi_cast()
{
    /*!
     *  @brief     Join the Multi-Cast Group
     *  @return    NMT_result
     */

    NMT_log_write(DEBUG, (char *)"> ");

    /* Initialize Varibles */
    NMT_result result = OK;

    /* Use setsockopt() to request that the kernel join a multicast group */
    if (result == OK)
    {
        struct ip_mreq mreq;
        mreq.imr_multiaddr.s_addr = inet_addr(this->ip_address.c_str());
        mreq.imr_interface.s_addr = htonl(INADDR_ANY);
        if (setsockopt(this->sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                      (char*) &mreq, sizeof(mreq)) < 0)
        {
            NMT_log_write(ERROR, (char *)"Failed to join multicast group! errno=%d", errno);
            result = NOK;
        }
    }

    /* Exit the function */
    NMT_log_write(DEBUG, (char *)"< result=%s", result_e2s[result]);
    return result;
}


SOCK_STATE NMT_sock::NMT_sock_check_message(int nbytes, int client)
{
    /*!
     *  @brief     Validate Message recieved on socket
     *  @param[in] nbytes
     *  @param[in] msgbuf
     *  @param[out] message
     *  @return    NMT_result
     */

    NMT_log_write(DEBUG, (char *)"> ");

    /* Initialize Varibles */
    SOCK_STATE state = SOCK_OK;

    if (nbytes < 0) 
    {
        state = SOCK_ERROR;
        NMT_log_write(WARNING, (char *)"No Message recived on socket");
    }
    else if ((nbytes == 0) && (this->type = SOCK_STREAM))
    {
        /*Set State */
        state = SOCK_DISCONNECTED;

        /* Check which client disconnected */
        getpeername(client,
                    (struct sockaddr*)&this->my_address,
                    (socklen_t*)&adder_len);  

        NMT_log_write(WARNING,
                      (char *)"Host disconnected , ip %s , port %d \n",  
                      inet_ntoa(this->my_address.sin_addr),
                      ntohs(this->my_address.sin_port));   
    }

    /* Exit the Function */
    NMT_log_write(DEBUG, (char *)"< state=%s", result_e2s[result]);
    return state;
}

