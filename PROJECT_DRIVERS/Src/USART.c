/*
 * USART.c
 *
 *  Created on: 11 Kas 2023
 *      Author: recep
 */

#include "USART.h"
#include "stm32f407xx.h"


static void closeUSART_ISR(USART_HandleTypeDef_t *USART_Handle)
{
	USART_Handle->TxBufferSize = 0;
	USART_Handle->pTxBuffer = NULL;
	USART_Handle->TxStatus = USART_BUS_FREE;

	USART_Handle->Instance->CR1 &= ~(0x1U << USART_CR1_TxEIE);

}
static void closeUSART_ISR_Rx(USART_HandleTypeDef_t *USART_Handle)
{
	USART_Handle->RxBufferSize = 0;
	USART_Handle->pRxBuffer = NULL;
	USART_Handle->RxStatus = USART_BUS_FREE;

	USART_Handle->Instance->CR1 &= ~(0x1U << USART_CR1_RxNEIE); // ilgili bolum 0'lanir.
}



static void USART_SendWith_IT(USART_HandleTypeDef_t *USART_Handle)
{
	if((USART_Handle->Init.WordLength == USART_WORDLENGTH_9Bits) && (USART_Handle->Init.Parity == USART_PARITY_NONE))
	{
		uint16_t *p16BitsData = (uint16_t*)(USART_Handle->pTxBuffer);

		USART_Handle->Instance->DR = (uint16_t)(*p16BitsData & (uint16_t)0x01FF); // ilgili verinin 2bitini oku
		USART_Handle->pTxBuffer += sizeof(uint16_t);
		USART_Handle->TxBufferSize -= 2;
	}
	else
	{
		USART_Handle->Instance->DR = (uint8_t)(*(USART_Handle->pTxBuffer) & (uint8_t)0x00FF); // tek bitini oku & ile
		USART_Handle->pTxBuffer++;
		USART_Handle->TxBufferSize--;
	}
	if(USART_Handle->TxBufferSize == 0)
	{
		closeUSART_ISR(USART_Handle);
	}
}

static void USART_ReceiveWith_IT(USART_HandleTypeDef_t *USART_Handle)
{
	uint16_t *p16BitsBuffer;
	uint8_t *p8BitsBuffer;

	if((USART_Handle->Init.WordLength == USART_WORDLENGTH_9Bits) && (USART_Handle->Init.Parity == USART_PARITY_NONE))
	{
		p16BitsBuffer = (uint16_t*)USART_Handle->pRxBuffer;
		p8BitsBuffer = NULL; // Dangling pointer onlemek icin
	}
	else
	{
		p8BitsBuffer = (uint8_t*)USART_Handle -> pRxBuffer;
		p16BitsBuffer = NULL;
	}

	if(p8BitsBuffer == NULL)
	{
		*p16BitsBuffer = (uint16_t)(USART_Handle->Instance->DR & 0x01FFU);
		USART_Handle->pRxBuffer += sizeof(uint16_t);
		USART_Handle->RxBufferSize -= 2;
	}
	else
	{
		if((USART_Handle->Init.WordLength == USART_WORDLENGTH_9Bits) && (USART_Handle->Init.Parity != USART_PARITY_NONE))
		{
			*p8BitsBuffer = (uint8_t)(USART_Handle->Instance->DR & 0x00FFU);
			USART_Handle -> pRxBuffer++;
			USART_Handle-> RxBufferSize--;
		}
		else if((USART_Handle->Init.WordLength == USART_WORDLENGTH_8Bits) && (USART_Handle->Init.Parity == USART_PARITY_NONE))
		{
			*p8BitsBuffer = (uint8_t)(USART_Handle->Instance->DR & 0x00FFU);
			USART_Handle -> pRxBuffer++;
			USART_Handle-> RxBufferSize--;
		}
		else
		{
			*p8BitsBuffer = (uint8_t)(USART_Handle->Instance->DR & 0x007FU);
			USART_Handle -> pRxBuffer++;
			USART_Handle-> RxBufferSize--;
		}
	}
	if(USART_Handle->RxBufferSize == 0)
	{
		closeUSART_ISR_Rx(USART_Handle);
	}
}


/*
 * @brief USART_Init, Configures the USART Peripheral
 *
 * @param USART_Handle = User config structure
 *
 * @retval Void
 */

void USART_Init(USART_HandleTypeDef_t *USART_Handle)
{

	uint32_t periphClock     = 0;
	uint32_t mantissaPart    = 0;
	uint32_t fractionPart 	 = 0;
	uint32_t USART_DIV_Value = 0;
	uint32_t tempValue 	     = 0;

	/****  OverSampling WordLength Mode Parity    ******/
	uint32_t tempRegister = 0;

	tempRegister = USART_Handle->Instance->CR1;

	tempRegister |= (USART_Handle->Init.OverSampling) | (USART_Handle->Init.WordLength) | \
			(USART_Handle->Init.Mode) | (USART_Handle->Init.Parity);

	USART_Handle->Instance->CR1 = tempRegister;

	/****  Stop Bits      ******/

	tempRegister = USART_Handle->Instance->CR2;

	tempRegister &=~(0x3U << UART_CR2_STOP);

	tempRegister |= (USART_Handle->Init.StopBits);

	USART_Handle->Instance->CR2 = tempRegister;

	/**** HardWareFlowControl ******/
	tempRegister = USART_Handle->Instance->CR3;

	tempRegister |= (USART_Handle->Init.HardWareFlowControl);

	USART_Handle->Instance->CR3 = tempRegister;

	/**** Baud Rate Configuration ******/

	if(USART_Handle->Instance == USART1 || USART_Handle->Instance==USART6)
	{
		periphClock = RCC_GetPClock2();
	}
	else
	{
		periphClock = RCC_GetPClock1();
	}

	if(USART_Handle->Init.OverSampling == USART_OVERSAMPLE_8 )
	{
		USART_DIV_Value = __USART_DIV_VALUE_8(periphClock,USART_Handle->Init.BaudRate);
		mantissaPart = (USART_DIV_Value / 100U);
		fractionPart = (USART_DIV_Value) - (mantissaPart * 100U);

		fractionPart = (((fractionPart * 8U) + 50U) / 100U ) & (0x07U);
	}
	else
	{
		USART_DIV_Value = __USART_DIV_VALUE_16(periphClock,USART_Handle->Init.BaudRate);
		mantissaPart = (USART_DIV_Value / 100U);
		fractionPart = (USART_DIV_Value) - (mantissaPart * 100U);

		fractionPart = (((fractionPart * 16U) + 50U) / 100U ) & (0xFU);
	}

	tempValue |= (mantissaPart << 4U);
	tempValue |= (fractionPart << 0U);

	USART_Handle->Instance->BRR = tempValue;
}


/*
 * @brief USART_TransmitData, Transmit data
 *
 * @param USART_Handle = User config structure
 *
 * @param pData = Addres of data to send
 *
 * @param dataSize = Length ouf your data in bytes
 *
 * @retval Void
 */



void USART_TransmitData(USART_HandleTypeDef_t *USART_Handle,uint8_t *pData, uint16_t dataSize)
{

	uint16_t *data16Bits; // "Diğerinin" üzerinde gezmek için, DATA'nın...

	if((USART_Handle->Init.WordLength == USART_WORDLENGTH_9Bits) && (USART_Handle->Init.Parity == USART_PARITY_NONE))
	{
		data16Bits = (uint16_t *)pData; // ?
	}
	else
	{
		data16Bits = NULL;


	}

	while(dataSize > 0)
	{
		while(!(USART_GetFlagStatus(USART_Handle, USART_TxE_FLAG))); // Data varsa kilitle, yok ise devam et // Flag Control // wait for txe flag

		/* 9Bits Data No Parity is for ELSE Condition, for others you will be in IF Case */

		if(data16Bits == NULL)
		{
			USART_Handle->Instance->DR = (uint8_t)(*pData & 0xFFU); // ilk 8biti ile işi var, and'leyerek ulaş
			pData++;
			dataSize--;

		}
		else
		{
			USART_Handle->Instance->DR = (*data16Bits & (0x01FFU)); // ilgili yapinin 9 bitinin aldi yazdi // 9bit 9bit karşıyo vermek
			data16Bits++;
			dataSize -= 2;
		}

	}

		while(!(USART_GetFlagStatus(USART_Handle,USART_TC_FLAG))); // Transmission completed flag beklemek // 1 döndüğünde not ile 0 olacak ve TC completed yani 1 ...

}

/*
 * @brief USART_ReceiveData, Receives data
 *
 * @param USART_Handle = User config structure
 *
 * @param pData = Addres of data to store
 *
 * @param dataSize = Length ouf your data in bytes
 *
 * @retval Void
 */


void USART_ReceiveData(USART_HandleTypeDef_t *USART_Handle,uint8_t *pBuffer,uint16_t dataSize)
{
	uint16_t *p16BitsBuffer;
	uint8_t  *p8BitsBuffer;

	if((USART_Handle->Init.WordLength == USART_WORDLENGTH_9Bits) && (USART_Handle->Init.Parity == USART_PARITY_NONE ))
	{
		p16BitsBuffer =(uint16_t*)pBuffer;
		p8BitsBuffer = NULL;
	}
	else
	{
		p8BitsBuffer = (uint8_t*)pBuffer;
		p16BitsBuffer = NULL;
	}

	while(dataSize > 0)
	{
		// Datayi okumak icin musait mi ? RXNE, Read data not emty register

		while(!(USART_GetFlagStatus(USART_Handle, USART_RxNE_FLAG) )); // Veri geldiginde 1 oldu , not ile 0 oldu ve cikti

		if(p8BitsBuffer == NULL) // 9bitin 9uda veri olarak geliyor
		{
			*p16BitsBuffer = (uint16_t)(USART_Handle->Instance->DR & 0x01FFU); // 9 biti 1 olan sayi
			p16BitsBuffer++;
			dataSize-=2;

		}
		else
		{
			if((USART_Handle->Init.WordLength == USART_WORDLENGTH_9Bits) && (USART_Handle->Init.Parity != USART_PARITY_NONE ))
			{
				*p8BitsBuffer = (uint8_t)(USART_Handle->Instance->DR & 0x00FFU);
				p8BitsBuffer++;
				dataSize--;
			}
			else if((USART_Handle->Init.WordLength == USART_WORDLENGTH_8Bits) && (USART_Handle->Init.Parity == USART_PARITY_NONE )) // 8bits?
			{
				*p8BitsBuffer = (uint8_t)(USART_Handle->Instance->DR & 0x00FFU);
				p8BitsBuffer++;
				dataSize--;
			}
			else
			{
				*p8BitsBuffer = (uint8_t)(USART_Handle->Instance->DR & 0x007FU); // 7 biti 1 olan sayi
				p8BitsBuffer++;
				dataSize--;
			}
		}
	}

}


// Ilgili degiskenler assigned to global
void USART_TransmitData_IT(USART_HandleTypeDef_t *USART_Handle,uint8_t *pData, uint16_t dataSize)
{
	USART_BusState_t usartBusState = USART_Handle -> TxStatus;

	if(usartBusState != USART_BUS_Tx)
	{
		USART_Handle -> pTxBuffer = (uint8_t *)pData;
		USART_Handle -> TxBufferSize = (uint16_t)dataSize;

		USART_Handle->TxStatus = USART_BUS_Tx;
		USART_Handle->TxISR_Function = USART_SendWith_IT;


		USART_Handle->Instance->CR1 |= (0x1U << USART_CR1_TxEIE);
	}


}

// Ilgili degiskenler assigned to global
void USART_ReceiveData_IT(USART_HandleTypeDef_t *USART_Handle, uint8_t *pBuffer,uint16_t dataSize)
{
	USART_BusState_t usartBusState = USART_Handle->RxStatus;

	if(usartBusState != USART_BUS_Rx)
	{
		USART_Handle -> pRxBuffer = (uint8_t*)pBuffer;
		USART_Handle -> RxBufferSize = (uint16_t)dataSize;
		USART_Handle -> RxStatus = USART_BUS_Rx;

		USART_Handle->RxISR_Function = USART_ReceiveWith_IT;

		USART_Handle->Instance->CR1 |= (0x1U << USART_CR1_RxNEIE );
	}
}

/*
 * @brief USART_PeriphCmd, Enable or Disable USART Peripheral
 *
 * @param USART_Handle = User config structure
 * @param stateOfUSART = ENABLE or DISABLE
 *
 * @retval Void
 */

void USART_PeriphCmd(USART_HandleTypeDef_t *USART_Handle, FunctionalState_t stateOfUSART)
{
	if(stateOfUSART == ENABLE)
	{
		USART_Handle->Instance->CR1 |= (0x1U << USART_CR1_UE);
	}
	else
	{
		USART_Handle->Instance->CR1 &= ~(0x1U << USART_CR1_UE);
	}
}

/*
 * @brief USART_GetFlagStatus, Return the flag of SR Register
 *
 * @param USART_Handle = User config structure
 * @param flagName = flagName of SR Register
 *
 * retval USART_FlagStatus_t
 */

USART_FlagStatus_t USART_GetFlagStatus(USART_HandleTypeDef_t *USART_Handle, uint16_t flagName)
{
	return ((USART_Handle->Instance->SR & (flagName)) ? USART_FLAG_SET : USART_FLAG_RESET);
}

void USART_InterruptHandler(USART_HandleTypeDef_t *USART_Handle)
{
	uint8_t flagValue = 0;
	uint8_t interruptValue = 0;

	flagValue = (uint8_t)((USART_Handle -> Instance->SR >> 7U) & 0x1U );
	interruptValue = (uint8_t)((USART_Handle -> Instance->CR1 >> 7U) & 0x1U );

	if(flagValue && interruptValue) // interrupt gelmistir
	{
		USART_Handle -> TxISR_Function(USART_Handle);
	}

	flagValue = (uint8_t)((USART_Handle -> Instance -> SR >> 5U ) & 0x1U );
	interruptValue = (uint8_t)((USART_Handle -> Instance -> CR1 >> 5U ) & 0x1U );

	if(flagValue && interruptValue)
	{
		USART_Handle -> RxISR_Function(USART_Handle);
	}
}



















