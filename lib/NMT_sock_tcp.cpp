/** *  @file      NMT_sock_tcp.cpp
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
#include <vector>
#include <string> 
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_sock_tcp.hpp"
#include "NMT_log.h"

/*--------------------------------------------------/
/                   Constants                       /
/--------------------------------------------------*/


using namespace std;

/*-------------------------------------------------------------------------------------/
/                                  Public Interfaces                                  /
/-----------------------------------------------------------------------------------*/
NMT_sock_tcp::~NMT_sock_tcp()
{
    NMT_log_write(DEBUG, (char *)"> ");

    shutdown(this->sock, SHUT_RDWR);
    close(this->sock);

    NMT_log_write(DEBUG, (char *)"< ");
}

tuple<NMT_result, string, int> NMT_sock_tcp::NMT_read_socket()
{
    /*!
     *  @brief     Read TCP Message
     *  @param[out] message
     *  @return    NMT_result
     */

    NMT_log_write(DEBUG, (char *)"> ");

    /* Initialize Varibles */
    bool time_out = false; 
    NMT_result result = OK;
    SOCK_STATE state = SOCK_IDLE;
    string rx_message = "";
    bool new_connection = false;
    int client;

    while (((state == SOCK_DISCONNECTED) || (state == SOCK_IDLE)) && 
            (!time_out) && (result == OK))
    {
        if (this->mode == SOCK_SERVER)
        {
            /* Initialize Select */
            NMT_sock_tcp::init_select();

            /* Listen on Socket */
            tie(result, time_out) = NMT_sock_tcp::listen();

            /* Check for New Connection */
            if ((result == OK) && (!time_out))
                tie(result, new_connection) = NMT_sock_tcp::accept_new_connection();

            /* Read the message */
            if ((result == OK) && (!time_out) && (!new_connection))
                tie(state, rx_message, client) = NMT_sock_tcp::read_client_message();
        }
        else
        {
            /* Read the message */
            tie(state, rx_message) = NMT_sock_tcp::read_server_message();
            client = this->sock;
        }
            
    }

    /* Do we have a failure ? */
    if (state == SOCK_ERROR)
    {
        result = NOK;
        NMT_log_write(ERROR, (char *)"Socket Read Error");
    }
        
    /* Exit the Function */
    NMT_log_write(DEBUG, (char *)"< result=%s", result_e2s[result]);
    return make_tuple(result, rx_message, client);
} 

NMT_result NMT_sock_tcp::NMT_write_socket(string message, int sock_id)
{
    /*!
     *  @brief     Function to send message on socket
     *  @param[in[ message
     *  @return    NMT_result
     */

    NMT_log_write(DEBUG, (char *)"> ");

    /* Initialize Varibles */
    int sock_result;
    NMT_result result = OK;
    if (sock_id < 0) sock_id = this->sock;
    int messagelen = strlen((char *)message.c_str());

    /* Send the message on the socket */
    sock_result = send(sock_id, (char *)message.c_str(), messagelen, 0);

    if (sock_result != messagelen)
    {
        result = NOK;
        NMT_log_write(ERROR, (char *)"ERROR sending message to: %d", sock_id);
    }

    /* Exit the function */
    NMT_log_write(DEBUG, (char *)"< result=%s", result_e2s[result]);
    return result;
}

/*-------------------------------------------------------------------------------------/
/                                  Private Methods                                    /
/-----------------------------------------------------------------------------------*/

void NMT_sock_tcp::init_select()
{
    /*!
     *  @brief     Initialize Select for Multi-Clients
     *  @return    void
     */

    NMT_log_write(DEBUG, (char *)"> ");

    this->max_sd = this->sock;

    /* Clear the socket set */
    FD_ZERO(&this->readfds);   
 
    /* Add sock socket to set */
    FD_SET(this->sock, &this->readfds);   

    /* Add child sockets to set */
    for (int sd : this->client_sockets)
    {   
        /* Add to Read List of Valid */
        if(sd > 0)   
            FD_SET(sd, &this->readfds);   
             
        /* Get Highest fd number */
        if(sd > this->max_sd)   
            this->max_sd = sd;   
    }

    /* End of Method */
    NMT_log_write(DEBUG, (char *)"< ");
}

tuple<NMT_result, bool> NMT_sock_tcp::listen()
{
    /*!
     *  @brief     Initialize Select for Multi-Clients
     *  @return    NMT_result, time_out
     */

    NMT_log_write(DEBUG, (char *)"> ");

    /* Initialize Varibles */
    NMT_result result = OK;
    bool time_out = false;
    int activity;

    /* Wait for Activity on Socket */
    activity = select(this->max_sd + 1, &this->readfds , NULL , NULL , &(this->tv));

    if (activity < 0)
    {
        result = NOK;
        NMT_log_write(ERROR, (char *)"Select Error!");
    }
    else if (activity == 0)
    {
        time_out = true;
    }
    else
    {
        /* All is OK */
        /* Do Nothing */
    }

    NMT_log_write(DEBUG, (char *)"< result=%s time_out=%s", result_e2s[result], btoa(time_out));
    return make_tuple(result, time_out);
}

tuple<NMT_result, bool> NMT_sock_tcp::accept_new_connection()
{
    NMT_log_write(DEBUG, (char *)"> ");

    /* Initialize Varibles */
    int client_fd;
    bool new_connection = false;
    NMT_result result = OK;

    if (FD_ISSET(this->sock, &this->readfds))  
    {
        if ((client_fd = accept(this->sock,
                               (struct sockaddr*) &this->my_address,
                               &this->adder_len)) < 0)
        {
            result = NOK;
            NMT_log_write(ERROR, (char *)"Failed to accept Socket Connection!");
        }

        /* Add to Known Clients */
        if (result == OK)
        {
            NMT_log_write(DEBUG, (char *)"Adding: %d to clients", client_fd);
            this->client_sockets.push_back(client_fd);
            new_connection = true;
        }
    }

    NMT_log_write(DEBUG, (char *)"< result=%s", result_e2s[result]);
    return make_tuple(result, new_connection);
}

tuple<SOCK_STATE, string, int> NMT_sock_tcp::read_client_message()
{
    NMT_log_write(DEBUG, (char *)"> ");

    /* Initialize Varibles */
    int nbytes;
    SOCK_STATE state = SOCK_IDLE;
    char msgbuf[max_buffer_size];
    int client = -1;
    string message = "";
    bool msgrecv = false;

    /* Check who sent the message and read it */
    for (auto client_p = this->client_sockets.begin(); 
         client_p != this->client_sockets.end(); client_p++)
        {   
            if (FD_ISSET(*client_p, &(this->readfds)))
            {   
                /* Hold Client */
                client = *client_p;

                /* Read Incoming Message */
                nbytes = read(*client_p, msgbuf, max_buffer_size); 

                /* Check State */
                state = NMT_sock::NMT_sock_check_message(nbytes, *client_p);

                /* Set Flag */
                msgrecv = true;

                switch (state)
                {
                    case SOCK_OK:
                        msgbuf[nbytes] = '\0';
                        message = msgbuf;
                        break;
                    case SOCK_DISCONNECTED:
                        //Close the socket and mark as 0 in list for reuse  
                        close(*client_p);   
                        this->client_sockets.erase(client_p);
                        break;
                    case SOCK_ERROR:
                    case SOCK_IDLE:
                        /* Do Nothing */
                        break;
                }
            }   

            /* Break Out of Loop */
            if (msgrecv)
                break;
        }   

    /* Exit the Function */
    NMT_log_write(DEBUG, (char *)"< state=%s, client=%d", 
                         (char *)sock_state_e2s[state].c_str(), client);
    return make_tuple(state, message, client);
}

tuple<SOCK_STATE, string> NMT_sock_tcp::read_server_message()
{
    NMT_log_write(DEBUG, (char *)"> ");

    /* Initialize Varibles */
    int nbytes;
    SOCK_STATE state = SOCK_IDLE;
    char msgbuf[max_buffer_size];
    string message;

    /* Read Incoming Message */
    nbytes = read(this->sock, msgbuf, max_buffer_size); 

    /* Check State */
    state = NMT_sock::NMT_sock_check_message(nbytes, this->sock);

    switch (state)
    {
        case SOCK_OK:
            msgbuf[nbytes] = '\0';
            message = msgbuf;
            break;
        case SOCK_DISCONNECTED:
        case SOCK_ERROR:
        case SOCK_IDLE:
            /* Do Nothing */
            break;
    }

    NMT_log_write(DEBUG, (char *)"< state=%s",
                         (char *)sock_state_e2s[state].c_str());
    return make_tuple(state, message);
}

