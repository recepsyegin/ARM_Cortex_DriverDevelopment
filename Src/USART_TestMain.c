/*
 * USART_TestMain.c
 *
 *  Created on: 18 Kas 2023
 *      Author: recep
 */
#include "stm32f407xx.h"

USART_HandleTypeDef_t USART_Handle;


//I2C_HandleTypeDef_t I2C_Handle;

static void GPIO_Config(void);
static void UART_Config(void);


void USART2_IRQHandler()
{
	//Interrupt geldiginde bu fonksiyon cagrilir

	USART_InterruptHandler(&USART_Handle);

}



int main(void)
{
	char msgToSend[] = "Hey interrupt send ?\n";
	char receiveData[30];
	GPIO_Config();
	UART_Config();

	//USART_ReceiveData(&USART_Handle, (uint8_t*)msgToSend,12);
	//USART_TransmitData(&USART_Handle, (uint8_t*)msgToSend, strlen(msgToSend));

	USART_TransmitData_IT(&USART_Handle, (uint8_t*)msgToSend, strlen(msgToSend));
	USART_ReceiveData_IT(&USART_Handle,(uint8_t*)receiveData ,30);

	/************I2C****************/
	//I2C_Handle.Instance = I2C1;
	//I2C_PeriphCmd(I2C_Handle.Instance, ENABLE);



	while(1);


}

static void UART_Config(void)
{

	RCC_USART2_CLK_ENABLE(); // Don't forget!

	USART_Handle.Instance = USART2;
	USART_Handle.Init.BaudRate = 115200;
	USART_Handle.Init.HardWareFlowControl = USART_HW_NONE;
	USART_Handle.Init.Mode = USART_MODE_Tx_rx;
	USART_Handle.Init.OverSampling = USART_OVERSAMPLE_16;
	USART_Handle.Init.Parity = USART_PARITY_NONE;
	USART_Handle.Init.StopBits = USART_STOPBITS_1;
	USART_Handle.Init.WordLength = USART_WORDLENGTH_8Bits;

	USART_Init(&USART_Handle);

	NVIC_EnableInterrupt(USART2_IRQn);


	USART_PeriphCmd(&USART_Handle,ENABLE);// USART ENABLE

}



static void GPIO_Config(void)
{
	// DataSheet -> Pin Description -> USART2_Tx : PA2, Alternate Function 7

	GPIO_InitTypeDef_t GPIO_InitStruct = { 0 } ; // lokal degisken ilk atama 0 yap, global degisken otomatik olarak 0 baslatilir

	RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.pinNumber = GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_AF;
	GPIO_InitStruct.Otype = GPIO_OTYPE_PP;
	GPIO_InitStruct.PuPd = GPIO_PUPD_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_VERY;
	GPIO_InitStruct.Alternate = GPIO_AF7;

	GPIO_Init(GPIOA,&GPIO_InitStruct);


}

















