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
#include <string.h> 
#include <unistd.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_sock.hpp"
#include "NMT_stdlib.h"
#include "NMT_log.h"

/*--------------------------------------------------/
/                   Constants                       /
/--------------------------------------------------*/
const unsigned int MAX_BUFFER_SIZE = 10240;

using namespace std;
NMT_sock_multicast::NMT_sock_multicast(unsigned int port, string multicast_ip,
                                       sock_mode socket_mode)
{
    /*!
     *  @brief     Constructor for NMT_sock_multicast
     *  @param[in] port
     *  @param[in] multicast_ip
     *  @param[in] socket_mode
     *  @return    NMT_result
     */

    this->port = port;
    this->multicast_ip = multicast_ip;
    this->mode = socket_mode;

    /* Initialize socket as Client/Server */
    if (socket_mode == SOCK_SERVER) {this->result = NMT_init_multicast_server();}
    else {this->result = NMT_init_multicast_client();}
}

NMT_result NMT_sock_multicast::NMT_init_multicast_server()
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
    if ((this->sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        result = NOK;

    if (result == OK)
    {
        /* Set TTL of multicast packet */
        if (setsockopt(this->sock, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &opt, sizeof(opt)) < 0)
            result = NOK;
    }

    if (result == OK)
    {
        /* Construct local address structure */
        memset(&my_address, 0, sizeof(this->my_address));
        this->my_address.sin_family = AF_INET;
        this->my_address.sin_port = htons(this->port);
        this->my_address.sin_addr.s_addr = inet_addr(this->multicast_ip.c_str());
    }

    /* Exit the function */
    NMT_log_write(DEBUG, (char *)"< result=%s", result_e2s[result]);
    return result;
}

NMT_result NMT_sock_multicast::NMT_init_multicast_client()
{
    /*!
     *  @brief     Initialize the multicast protocol in client mode
     *  @return    NMT_result
     */

    NMT_log_write(DEBUG, (char *)"> ");
    NMT_result result = OK;
    struct timeval tv; 

    if ((this->sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        result = NOK;

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
        tv.tv_sec = 0;
        tv.tv_usec = 100000;
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
            NMT_log_write(ERROR, (char *)"Socket Bind  Failed");     
            result = NOK;
        }
    }

    /* Use setsockopt() to request that the kernel join a multicast group */
    if (result == OK)
    {
        struct ip_mreq mreq;
        mreq.imr_multiaddr.s_addr = inet_addr(this->multicast_ip.c_str());
        mreq.imr_interface.s_addr = htonl(INADDR_ANY);
        if (setsockopt(this->sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                      (char*) &mreq, sizeof(mreq)) < 0)
        {
            
            NMT_log_write(ERROR, (char *)"Failed to join multicast group!");
            result = NOK;
        }
    }

    /* Exit the function */
    NMT_log_write(DEBUG, (char *)"< result=%s", result_e2s[result]);
    return result;
}

NMT_result NMT_sock_multicast::NMT_write_socket(char *message)
{
    /*!
     *  @brief     Function to send message on socket
     *  @param[in[ message
     *  @return    NMT_result
     */

    NMT_log_write(DEBUG, (char *)"> ");
    /* Initialize Varibles */
    int messagelen = strlen(message);
    NMT_result result = OK;

    /* Send the message on the socket */
    if (sendto(this->sock, message, messagelen, 0, (struct sockaddr *) 
          &(this->my_address), sizeof(this->my_address)) != messagelen)
            result = NOK;

    /* Exit the function */
    NMT_log_write(DEBUG, (char *)"< result=%s", result_e2s[result]);
    return result;
}

NMT_result NMT_sock_multicast::NMT_read_socket(char **message)
{

    /*!
     *  @brief     Function to send message on socket
     *  @param[out] message
     *  @return    NMT_result
     */

    NMT_log_write(DEBUG, (char *)"> ");

    /* Initialize Varibles */
    NMT_result result = OK;
    char msgbuf[MAX_BUFFER_SIZE];
    socklen_t adder_len = sizeof(this->my_address);

    int nbytes = recvfrom(this->sock, msgbuf, MAX_BUFFER_SIZE,
        0,
        (struct sockaddr *) &(this->my_address),
        &(adder_len)
    );
    if (nbytes < 0) 
    {
        result = NOK;
        NMT_log_write(WARNING, (char *)"No Message recived in socket");
    }
    else
    {
        msgbuf[nbytes] = '\0';
        *message = (char *)malloc((sizeof(char) * strlen(msgbuf)) + 1);
        strcpy(*message, msgbuf);
    }

    NMT_log_write(DEBUG, (char *)"< result=%s", result_e2s[result]);
    return result;
}
