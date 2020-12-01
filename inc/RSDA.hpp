/** 
 *  @file      
 *  @brief     
 *  @details   
 *  @author    
 *  @date      
 *  @copyright 2020
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "RSDA_handler.hpp"
#include "NMT_sock_multi.hpp"
#include "NMT_sock_tcp.hpp"

/*--------------------------------------------------/
/                    Macros                         /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                Structs/Classes/Enums              /
/--------------------------------------------------*/

/*--------------------------------------------------------------------/
/                             Start of Program                        /
/--------------------------------------------------------------------*/
class RSDA : public SensorDataHandler
{
    public:
        RSDA(RSXA hw_settings, DataMode mode);
        ~RSDA() {};
        void get_and_transmit_sensor_data(int interval);
        void server_listener();

    private:
        NMT_sock_multi *client_sock;
        NMT_sock_tcp   *server_sock;
        int multi_cast_port;
        std::string multi_cast_ip;
        int tcp_port;
        std::string tcp_ip;
        void get_RSDA_parameters(RSXA_procs *procs, int nrof_procs);
};

