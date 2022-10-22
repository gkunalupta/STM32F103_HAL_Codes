/*
 * GB_ESP8266_client.h
 *
 *  Created on: Oct 23, 2022
 *      Author: acer
 */

#ifndef GB_ESP8266_CLIENT_H_
#define GB_ESP8266_CLIENT_H_

		void GB_esp8266_initialise_client();
		//AT+CIPSTART
		void GB_esp8266_connectTCPserver(const char * gb_url,int gb_port);
		//FOR CALCULATING LENGTH OF GET COMMAND
	    char * GB_esp8266_command(const char * gb_urlpath);
	    //FOR CALCULATING LENGTH OF GET COMMAND
	    const char * GB_esp8266_commandint(const char * gb_urlpath, int gb_val);
        //FOR CALCULATING LENGTH OF GET COMMAND
	    const char * GB_esp8266_commandchar(const char * gb_urlpath, char * gb_val);

	    void GB_esp8266_cipsendint(int  gb_val, const char *  gb_urlpath);

	    void GB_esp8266_cipsendchar(char *  gb_val, const char *  gb_urlpath);

		void GB_esp8266_cipsendfloat(float  gb_val, const char *  gb_urlpath);

		//void GB_(float val, const char * gb_urlpath);

		//GET COMMAND
		void GB_esp8266_tcpgetcommand(const char * gb_urlpath);    //when no field value has to be updated like path for json file
		//GET COMMAND
		void GB_esp8266_tcpgetcommandint(const char * gb_urlpath, int gb_val); //  //when field has to be updated like updating the sendor value.
	    //GET COMMAND
		void GB_esp8266_tcpgetcommandchar(const char * gb_urlpath, char * gb_val);   //when field has to be updated like updating the sendor value.






#endif /* GB_ESP8266_CLIENT_H_ */
