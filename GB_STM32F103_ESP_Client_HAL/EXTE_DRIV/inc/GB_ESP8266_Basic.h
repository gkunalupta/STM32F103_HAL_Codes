/*
 * GB_ESP8266_Basic.h
 *
 *  Created on: Oct 22, 2022
 *      Author: acer
 */

#ifndef GB_ESP8266_BASIC_H_
#define GB_ESP8266_BASIC_H_

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

uint16_t size_of_command( const char* gb_string);
void GB_send_command(const char * gb_command);
void GB_printString1(const char *gb_myString);

/***********Niche Ring Buffer Functions *****/
/****reading from Interrupted Ring Buffer*/
char  GB_getchar_UART2();             // <--->   Serial.read();
void GB_getstring_UART2();
char* GB_search_buffer(const char* gb_search);
int GB_RFR_UART2(const char * gb_rsp,unsigned int gb_timeout);



void GB_esp8266_getmode();
void GB_esp8266_connectiontype();
void GB_esp8266_getAP();
void GB_esp8266_setAP(const char * gb_ssid, const char * gb_pass);
void GB_esp8266_disconnect();
void GB_esp8266_IPadress();
void GB_esp8266_listAP();
void GB_esp8266_rst();
void GB_esp8266_initialise_client();
#endif /* GB_ESP8266_BASIC_H_ */
