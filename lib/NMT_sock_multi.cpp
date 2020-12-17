/** 
 *  @file      NMT_sock_multi.cpp
 *  @brief     NMT Multi-Cast Sockets
 *  @details   Library to establish multi-cast sockets
 *  @author    Nitin Mohan
 *  @date      June 10, 2020
 *  @copyright 2020 - NM Technologies
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <iostream>
#include <string.h> 
#include <sys/time.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_sock_multi.hpp"
#include "NMT_log.h"
/*--------------------------------------------------/
/                   Constants                       /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                 Start of Program                  /
/--------------------------------------------------*/
using namespace std;

tuple<NMT_result, string> NMT_sock_multi::NMT_read_socket()
{
    /*!
     *  @brief     Read UDP Message
     *  @param[out] message
     *  @return    NMT_result
     */

    NMT_log_write(DEBUG, (char *)"> ");

    /* Initialize Varibles */
    NMT_result result = OK;
    SOCK_STATE state = SOCK_OK;
    char msgbuf[max_buffer_size];
    string message;
    socklen_t adder_len = sizeof(this->my_address);

    /* Read the Message */
    int nbytes = recvfrom(this->sock, msgbuf, max_buffer_size,
        0,
        (struct sockaddr *) &(this->my_address),
        &(adder_len)
    );

    /* Validate the message */
    state = NMT_sock::NMT_sock_check_message(nbytes);

    /* Set Result and Message */
    switch (state)
    {
        case SOCK_OK:
            msgbuf[nbytes] = '\0';
            message = msgbuf;
            break;
        case SOCK_ERROR:
        case SOCK_DISCONNECTED:
            result = NOK;
            message = "";
            break;
        case SOCK_IDLE:
            /* Do Nothing */
            break;
    }

    /* Exit the Function */
    NMT_log_write(DEBUG, (char *)"< result=%s", result_e2s[result]);
    return make_tuple(result, message);
}

NMT_result NMT_sock_multi::NMT_write_socket(char *message)
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
