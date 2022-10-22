/*
 * GB_ESP8266_Basic.c
 *
 *  Created on: Oct 22, 2022
 *      Author: acer
 */


#include "main.h"
#include "stm32f1xx_it.h"
#include "GB_ESP8266_Basic.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;


uint16_t size_of_command( const char* gb_string)
{
	uint16_t counter=0;
	while(*gb_string)
	{
		counter = counter+1;
		*gb_string++;
	}
	return counter;
}
void GB_send_command(const char * gb_command)
{
	//HAL_UART_Transmit(&huart2,(uint8_t *)gb_command,cnt, 100);

	HAL_UART_Transmit(&huart2,(uint8_t *)gb_command,size_of_command(gb_command), 100);
//	GB_printString2(gb_command);
}
void GB_printString1(const char *gb_myString)
{
	HAL_UART_Transmit(&huart1,(uint8_t *)gb_myString,size_of_command(gb_myString), 100);
}

#define gb_RX_BUFFER_SIZE_UART2 255
uint8_t gb_Rx_Buffer_UART2[gb_RX_BUFFER_SIZE_UART2]; // character array (buffer)
uint8_t gb_RX_Wr_Index_UART2 =0; //index of next char to be put into the buffer  // head
uint8_t gb_RX_Rd_Index_UART2=0; //index of next char to be fetched from the buffer    //tail
uint8_t gb_RX_Counter_UART2=0; //a total count of characters in the buffer
uint8_t gb_RX_No_of_byte_UART2=0;
_Bool gb_RX_Buffer_Overflow_UART2; // This flag is set on USART Receiver // buffer overflow
uint8_t gb_rx_byte_UART2;
char gb_RX_DATA_UART2[gb_RX_BUFFER_SIZE_UART2];

void USART2_IRQHandler(void)
{
	/* USER CODE BEGIN USART1_IRQn 0 */

		 if((__NVIC_GetActive(USART2_IRQn)) == 1)
		    //check if we are here because of RXNE interrupt
		         if (USART2->SR & USART_SR_RXNE) //if RX is not empty
		                {
		        	 gb_rx_byte_UART2 = USART2->DR; //fetch the data received
		                      gb_Rx_Buffer_UART2[gb_RX_Wr_Index_UART2]= gb_rx_byte_UART2;   /* put received char in buffer */
		  		              if(++gb_RX_Wr_Index_UART2 > gb_RX_BUFFER_SIZE_UART2)
		  		              gb_RX_Wr_Index_UART2 = 0;
		  		              if(++gb_RX_Counter_UART2 > gb_RX_BUFFER_SIZE_UART2) /* keep a character count */
		  		                   {
		  			                    /* overflow check.. */
		  			                    gb_RX_Counter_UART2 = gb_RX_BUFFER_SIZE_UART2; /* if too many chars came */
		  			                  //  gb_RX_Buffer_Overflow_UART2 = 1;            /* in before they could be used */
		  			                }                                          /* that could cause an error!! */
		  			          gb_RX_No_of_byte_UART2=gb_RX_Counter_UART2;

		  			          //HAL_UART_Transmit(&huart1,&gb_rx_byte_UART2,1, 10);

		  			      //  HAL_UART_Transmit(&huart1,&gb_RX_No_of_byte_UART2,1,10);

		                 }
	  /* USER CODE END USART2_IRQn 0 */
	  /* USER CODE BEGIN USART2_IRQn 1 */
	  /* USER CODE END USART2_IRQn 1 */
	}
/***********Niche Ring Buffer Functions *****/
/****reading from Interrupted Ring Buffer*/
char  GB_getchar_UART2()             // <--->   Serial.read()
{
	char gb_c;
	gb_c = gb_Rx_Buffer_UART2[gb_RX_Rd_Index_UART2]; /* get one from the buffer..*/
	if(++gb_RX_Rd_Index_UART2 > gb_RX_BUFFER_SIZE_UART2) /* wrap the pointer */
	gb_RX_Rd_Index_UART2 = 0;
	if(gb_RX_Counter_UART2)
	gb_RX_Counter_UART2--; /* keep a count (buffer size) */
	//GB_UART_TxChar1(gb_c);
	return gb_c ;//return char *
	//printString0("n \n ");
}
void GB_getstring_UART2()
{
	GB_printString1(" Number of characters received : ");
	//GB_decimel1(gb_RX_No_of_byte_UART2);
	HAL_UART_Transmit(&huart1,&gb_RX_No_of_byte_UART2,2, 10);
	GB_printString1("\n");
	uint8_t gb_x=0;
	memset(gb_RX_DATA_UART2, '\0',gb_RX_BUFFER_SIZE_UART2);
	while (gb_RX_Counter_UART2)
	{
		gb_RX_DATA_UART2[gb_x]= GB_getchar_UART2();
		gb_x++;
	}
	GB_printString1(gb_RX_DATA_UART2);
}
char* GB_search_buffer(const char* gb_search)
{
	char* gb_p;
	int gb_bufferlen = strlen((const char * )gb_RX_DATA_UART2);
	if(gb_bufferlen < gb_RX_BUFFER_SIZE_UART2)
	gb_p= strstr ((const char *)gb_RX_DATA_UART2,gb_search);
	return (gb_p);
}

/* @brief variable to store the number of characters received*/
uint8_t gb_received =0;
int GB_RFR_UART2(const char * gb_rsp,unsigned int gb_timeout)
{
	uint32_t gb_timeIn = HAL_GetTick();
	gb_received = 0;
	memset(gb_RX_DATA_UART2, '\0',gb_RX_BUFFER_SIZE_UART2);
	while (HAL_GetTick() - gb_timeIn < gb_timeout) // While we haven't timed out
	{
		if(gb_RX_Counter_UART2)
		{
			gb_RX_DATA_UART2[gb_received]= GB_getchar_UART2();
			gb_received++;
			if(GB_search_buffer(gb_rsp)) return 1;
		}
	}
	if (gb_received > 0) // If we received any characters
	return 0; // Return unkown response error code
	else // If we haven't received any characters
	return 2; // Return the timeout error code

}
int gb_CWMODE=0;
void GB_esp8266_getmode()
{
	GB_send_command("AT+CWMODE?\r\n");
	int gb_rsp = GB_RFR_UART2("OK", 1000);
	if(gb_rsp==1)
	{
		char * gb_p = strchr(gb_RX_DATA_UART2, ':');
		if(gb_p!=NULL)
		{
			char gb_mode = *(gb_p+1);
			gb_CWMODE= gb_mode-48;
			if(gb_CWMODE==1) GB_printString1("Station mode selected\n");
			if(gb_CWMODE==2) GB_printString1("SoftAP mode selected\n");
			if(gb_CWMODE==3) GB_printString1("SoftAP+Station mode selected\n");
		}
	}
	else
	{
		GB_printString1("Error in receiving response (check the command) or timeout has happened\n");
		//esp8266_rst();
		GB_printString1(gb_RX_DATA_UART2);
	}

	//GB_printString1(gb_RX_DATA_UART2);
}
void GB_esp8266_connectiontype()
{
	GB_send_command("AT+CIPMUX?\r\n");
	int gb_rsp = GB_RFR_UART2("OK", 1000);
	if(gb_rsp==1)
	{
		// printString0("command is successfully send\n");
		char * gb_p = strchr(gb_RX_DATA_UART2, ':');
		if(gb_p!=NULL)
		{
			char gb_mode = *(gb_p+1);
			int gb_num = gb_mode-48;
			if(gb_num==0) GB_printString1("Single connection - configured as client\n");
			if(gb_num==1) GB_printString1("multiple connection - configured as server\n");
		}
	}
	else
	{
		GB_printString1("Error in receiving response (check the command) or timeout has happened\n");
		GB_printString1(gb_RX_DATA_UART2);
	}
//	GB_printString1(gb_RX_DATA_UART2);
}
void GB_esp8266_getAP()
{
	char gb_ssid[20];
	GB_send_command("AT+CWJAP?\r\n");
	int gb_rsp = GB_RFR_UART2("OK", 2000);
	if(gb_rsp==1)
	{

		if (strstr(gb_RX_DATA_UART2, "No AP") != NULL)   GB_printString1("No AP is connected\n");
		char * gb_p = strstr(gb_RX_DATA_UART2,"+CWJAP:");
		if(gb_p!=NULL)
		{
			gb_p +=  strlen("+CWJAP") +2;
			char * gb_q=strchr(gb_p,'"');
			strncpy(gb_ssid,gb_p,gb_q-gb_p);
			GB_printString1("Connected to Wifi:");
			GB_printString1(gb_ssid);
			GB_printString1("\n");
		}

	}else
	{
		//printString0("Error in receiving response (check the command ) or timeout has happened\n");
		GB_printString1(gb_RX_DATA_UART2);
	}

}
void GB_esp8266_setAP(const char * gb_ssid, const char * gb_pass)
{
	char gb_setAP[40];
	sprintf(gb_setAP,"AT+CWJAP=\"%s\",\"%s\"\r\n",gb_ssid,gb_pass);


	GB_send_command(gb_setAP);
	int gb_rsp = GB_RFR_UART2("OK",16000);


	if(gb_rsp==1)
	{

		GB_printString1("WiFi is  connected to :");
		GB_printString1(gb_ssid);
		GB_printString1("\n");

	}
	else
	{
		GB_printString1("Error in connecting to :");
		GB_printString1(gb_ssid);
		GB_printString1("\n");
		char * gb_p = strstr(gb_RX_DATA_UART2,"+CWJAP:");
		if(gb_p!=NULL)
		{
			gb_p+= strlen("+CWJAP:");
			char gb_error_code = *gb_p;
			int gb_num = gb_error_code-48;
			if(gb_num==1) GB_printString1("Connection timeout \n");
			if(gb_num==2) GB_printString1("Wrong password \n");
			if(gb_num==3) GB_printString1("cannot find the target AP\n");
			if(gb_num==4) GB_printString1("connection failed \n");
		}
	}
}
//void GB_esp8266_disconnect()
//{
//
//	GB_send_command("AT+CWQAP\r\n");
//	//printString0("Sending \n");
//	int gb_rsp = GB_RFR_UART2("WIFI DISCONNECT",1000);
//	if(gb_rsp==1) GB_printString1("WIFI is  disconnected  \n");
//	if(gb_rsp==0) GB_printString1("WIFI is already disconnected \n");
//}
void GB_esp8266_IPadress()
{

	GB_send_command("AT+CIFSR\r\n");
	int gb_rsp = GB_RFR_UART2("OK",1000);
	char gb_staip[20],gb_stamac[20],gb_apip[20],gb_apmac[20];
	memset(gb_staip, '\0',sizeof(gb_staip));
	memset(gb_stamac, '\0',sizeof(gb_stamac));
	memset(gb_apip, '\0',sizeof(gb_apip));
	memset(gb_apmac, '\0',sizeof(gb_apmac));
	while(gb_rsp==1)
	{
		GB_printString1("IP address is :\n");
		if(gb_CWMODE==1)    //if station mode is selected
		{
			//Station IP
			char * gb_p = strstr(gb_RX_DATA_UART2,"+CIFSR:STAIP");
			gb_p += strlen("+CWJAP:STAIP")+2;
			char * gb_q=strchr(gb_p,'"');
			strncpy(gb_staip,gb_p,gb_q-gb_p);
			GB_printString1("IP address of Station is :");
			GB_printString1(gb_staip);

			// Station MAC
			char * gb_r = strstr(gb_RX_DATA_UART2,"+CIFSR:STAMAC");
			gb_r += strlen("+CIFSR:STAMAC")+2;
			char * gb_s =strchr(gb_r,'"');
			strncpy(gb_stamac,gb_r,gb_s-gb_r);
			GB_printString1("\n Mac address of Station is :");
			GB_printString1(gb_stamac);
			break;
		}
		if(gb_CWMODE==2)
		{
			// Soft AP IP
			char * gb_p = strstr(gb_RX_DATA_UART2,"+CIFSR:APIP");
			gb_p += strlen("+CIFSR:APIP")+2;
			char * gb_q=strchr(gb_p,'"');
			strncpy(gb_apip,gb_p,gb_q-gb_p);
			GB_printString1("IP address of SoftAP is :");
			GB_printString1(gb_apip);

			// Soft AP MAC
			char * gb_r = strstr(gb_RX_DATA_UART2,"+CIFSR:APMAC");
			gb_r += strlen("+CIFSR:APMAC")+2;
			char * gb_s =strchr(gb_r,'"');
			strncpy(gb_apmac,gb_r,gb_s-gb_r);
			GB_printString1("\n Mac address of SoftAP is :");
			GB_printString1(gb_apmac);
			break;
		}
		if(gb_CWMODE==3)
		{
			//Station IP
			char * gb_p = strstr(gb_RX_DATA_UART2,"+CIFSR:STAIP");
			gb_p += strlen("+CWJAP:STAIP")+2;
			char * gb_q=strchr(gb_p,'"');
			strncpy(gb_staip,gb_p,gb_q-gb_p);
			GB_printString1("IP address of Station is :");
			GB_printString1(gb_staip);
			GB_printString1("\n");

			// Station MAC
			char * gb_r = strstr(gb_RX_DATA_UART2,"+CIFSR:STAMAC");
			gb_r += strlen("+CIFSR:STAMAC")+2;
			char * gb_s =strchr(gb_r,'"');
			strncpy(gb_stamac,gb_r,gb_s-gb_r);
			GB_printString1("Mac address of Station is :");
			GB_printString1(gb_stamac);
			GB_printString1("\n");


			// Soft AP IP
			char * gb_a = strstr(gb_RX_DATA_UART2,"+CIFSR:APIP");
			gb_a += strlen("+CIFSR:APIP")+2;
			char * gb_b=strchr(gb_a,'"');
			strncpy(gb_apip,gb_a,gb_b-gb_a);
			GB_printString1("IP address of SoftAP is :");
			GB_printString1(gb_apip);
			GB_printString1("\n");


			// Soft AP MAC
			char * gb_c = strstr(gb_RX_DATA_UART2,"+CIFSR:APMAC");
			gb_c += strlen("+CIFSR:APMAC")+2;
			char * gb_d =strchr(gb_c,'"');
			strncpy(gb_apmac,gb_c,gb_d-gb_c);
			GB_printString1("Mac address of SoftAP is :");
			GB_printString1(gb_apmac);
			GB_printString1("\n");
			break;
		}
	}
}
void GB_esp8266_listAP()
{

	GB_send_command("AT+CWLAP\r\n");
	int gb_rsp = GB_RFR_UART2("OK",7000);
	//printString0(RX_DATA_UART1);
	//printString0("\n");
	if(gb_rsp==0)
	{
		GB_printString1("WiFi networks near are: \n");
		GB_printString1(gb_RX_DATA_UART2);
		GB_printString1("\n");
	}else
	GB_printString1(gb_RX_DATA_UART2);
}
void GB_esp8266_rst()
{

	GB_send_command("AT+RST\r\n");
	int gb_rsp = GB_RFR_UART2("OK",4000);
	if(gb_rsp==1) GB_printString1(gb_RX_DATA_UART2);
}

void GB_esp8266_initialise_client()
	{

		GB_esp8266_getmode();
		GB_esp8266_connectiontype();
		GB_esp8266_getAP();
	GB_esp8266_setAP("kunal2.4ghz","Mansi5481");
//
//		GB_printString1("\nSETAP Command\n");
		//esp8266_setAP("JioFiber 2.4ghz","Mansi5481");
		// esp8266_setAP("KG","12345678");
		//esp8266_disconnect();
//	GB_esp8266_listAP();
	GB_esp8266_IPadress();
		//esp8266_rst();
	}


