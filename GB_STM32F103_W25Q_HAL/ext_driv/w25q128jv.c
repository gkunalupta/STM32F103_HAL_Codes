/*
 * w25q128jv.c
 *
 *  Created on: Oct 8, 2022
 *      Author: acer
 */


#include "w25q128jv.h"
#include "stm32f1xx_hal.h"
#include "main.h"

//extern SPI_HandleTypeDef hspi1;
static SPI_HandleTypeDef hspi1;

#define cs_set() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET)
#define cs_reset() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET)

static uint8_t tx_buf[10];
void SPI1_Send (uint8_t *dt, uint16_t cnt)
{
  HAL_SPI_Transmit (&hspi1, dt, cnt, 5000);

}
void SPI1_Recv (uint8_t *dt, uint16_t cnt)
{
  HAL_SPI_Receive (&hspi1, dt, cnt, 5000);
}
void W25_Reset (void)
{
  cs_reset();
  tx_buf[0] = reset1;
  tx_buf[1] = reset2;
  SPI1_Send(tx_buf, 2);
  cs_set();
}
void WriteEnable_flash()
{
	cs_reset();
	tx_buf[0] = WriteEnable;
	SPI1_Send(tx_buf,1);
	cs_set();

}
void W25_Read_Data(uint32_t addr, uint8_t* data, uint32_t sz)
{
  cs_reset();
  tx_buf[0] = ReadData;
  tx_buf[1] = (addr >> 16) & 0xFF;
  tx_buf[2] = (addr >> 8) & 0xFF;
  tx_buf[3] = addr & 0xFF;
  SPI1_Send(tx_buf, 4);
  SPI1_Recv(data, sz);
  cs_set();
}
void W25_Write_Data(uint32_t addr, uint8_t* data, uint32_t sz)
{
	WriteEnable_flash();
	  HAL_Delay(100);
  cs_reset();
  tx_buf[0] = WriteData;
  tx_buf[1] = (addr >> 16) & 0xFF;
  tx_buf[2] = (addr >> 8) & 0xFF;
  tx_buf[3] = addr & 0xFF;
  SPI1_Send(tx_buf, 4);
  SPI1_Send(data, sz);
  cs_set();
}
uint32_t W25_Read_ID(void)
{
  uint8_t dt[4];
  tx_buf[0] = JEDECID;
  cs_reset();
  SPI1_Send(tx_buf, 1);
  SPI1_Recv(dt,3);
  cs_set();
  return (dt[0] << 16) | (dt[1] << 8) | dt[2];
}
void W25_Ini(void)
{
  HAL_Delay(100);
  W25_Reset();
  HAL_Delay(100);
  //unsigned int id = W25_Read_ID();
  // HAL_UART_Transmit(&huart2,(uint8_t*)"\r\n",2,0x1000);
  // sprintf(str1,"ID:0x%X\r\n",id);
  // HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
}
void erase_sector4KB(uint32_t addr)
{

	WriteEnable_flash();
	HAL_Delay(100);
	cs_reset();
	tx_buf[0] = SectErase4KB;
	tx_buf[1] = (addr >> 16) & 0xFF;
	tx_buf[2] = (addr >> 8) & 0xFF;
	tx_buf[3] = addr & 0xFF;
	SPI1_Send(tx_buf,4);
	cs_set();
}
void erase_sector32KB(uint32_t addr)
{
	WriteEnable_flash();
	cs_reset();
	tx_buf[0] = SectErase32KB;
	tx_buf[1] = (addr >> 16) & 0xFF;
	tx_buf[2] = (addr >> 8) & 0xFF;
	tx_buf[3] = addr & 0xFF;
	SPI1_Send(tx_buf,4);
	cs_set();
}
void erase_sector64KB(uint32_t addr)
{
	WriteEnable_flash();
	cs_reset();
	tx_buf[0] = SectErase64KB;
	tx_buf[1] = (addr >> 16) & 0xFF;
	tx_buf[2] = (addr >> 8) & 0xFF;
	tx_buf[3] = addr & 0xFF;
	SPI1_Send(tx_buf,4);
	cs_set();
}
void chip_erase()
{
	WriteEnable_flash();
	cs_reset();
	tx_buf[0] = chiperase;
	SPI1_Send(tx_buf,1);
	cs_set();
}
void Uinque_ID(uint8_t uinque[])
{
	cs_reset();
	tx_buf[0] = UinqueID;

}
void WriteSR(uint8_t SR_address, uint8_t SR_data)
{
	WriteEnable_flash();
	cs_reset();
	tx_buf[0] = SR_address;
	tx_buf[1] = SR_data;
	SPI1_Send(tx_buf,2);
	cs_set();

}
uint8_t ReadSR(uint8_t SR_address)
{
	uint8_t RSR[1] = {0};
	cs_reset();
	tx_buf[0] =  SR_address;
	SPI1_Send(tx_buf,1);
	SPI1_Recv(RSR,1);
	cs_set();
	return RSR[0];
}
void WaitForWriteEnd(void)
{
	uint8_t StatusRegist1[1] = {0};
	cs_reset();
	tx_buf[0] = ReadSR1;
	SPI1_Send(tx_buf,1);
	SPI1_Recv(StatusRegist1,1);
	do
  {
		tx_buf[0] = Dummybyte;
		SPI1_Send(tx_buf,1);
		SPI1_Recv(StatusRegist1,1);
    }
  while ((StatusRegist1[0] & 0x01) == 0x01);
  cs_set();
}
