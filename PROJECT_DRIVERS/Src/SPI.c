/*
 * SPI.c
 *
 *  Created on: 28 Eki 2023
 *      Author: recep
 */

#include "SPI.h"

/*
 *  @brief SPI_CloseISR_TX, Disables the interrupt for Transmission
 *
 *  @parameters SPI_Handle = User config structures
 *
 *  @return value Void (None)
 */

static void SPI_CloseISR_TX(SPI_HandleTypeDef_t *SPI_Handle)
{
	SPI_Handle -> Instance -> CR2 &= ~(0x1U << SPI_CR2_TXEIE); // interrupt close, flag 0landi
	SPI_Handle -> TxDataSize = 0; 		 // temizledik
	SPI_Handle -> pTxDataAddr = NULL; 	 // temizledik
	SPI_Handle -> busStateTx = SPI_BUS_FREE; //
}


/*
 *  @brief SPI_CloseISR_RX, Disables the interrupt for Reception
 *
 *  @parameters SPI_Handle = User config structures
 *
 *  @return value Void (None)
 */

static void SPI_CLOSEISR_RX(SPI_HandleTypeDef_t *SPI_Handle)
{
	SPI_Handle -> Instance -> CR2 &= ~(0x1U << SPI_CR2_RXNEIE);
	SPI_Handle -> RxDataSize = 0;
	SPI_Handle -> pRxDataAddr = NULL;
	SPI_Handle -> busStateRx = SPI_BUS_FREE;
}

/*
 *  @brief SPI_TransmitHelper_16Bits, Stores the user data into the DR Register 16 bits format
 *
 *  @parameters SPI_Handle = User config structures
 *
 *  @return value Void (None)
 */
static void SPI_TransmitHelper_16Bits(SPI_HandleTypeDef_t *SPI_Handle)
{
	SPI_Handle->Instance->DR= *((uint16_t*)(SPI_Handle->pTxDataAddr));
	SPI_Handle->pTxDataAddr += sizeof(uint16_t);
	SPI_Handle->TxDataSize -= 2;

	if(SPI_Handle->TxDataSize == 0)
	{
		SPI_CloseISR_TX(SPI_Handle);
	}
}

/*
 *  @brief SPI_TransmitHelper_8Bits, Stores the user data into the DR Register for 8 bits formats
 *
 *  @parameters SPI_Handle = User config structures
 *
 *  @return value Void (None)
 */
static void SPI_TransmitHelper_8Bits(SPI_HandleTypeDef_t *SPI_Handle)
{
	SPI_Handle-> Instance->DR= *((uint8_t*)(SPI_Handle->pTxDataAddr));
	SPI_Handle-> pTxDataAddr += sizeof(uint8_t);
	SPI_Handle-> TxDataSize--;

	if(SPI_Handle->TxDataSize == 0)
	{
		SPI_CloseISR_TX(SPI_Handle);
	}
}

/*
 *  @brief SPI_ReceiveHelper_8Bits, Reads the Data Registers and stores into the user variable for 8bits format
 *
 *  @parameters SPI_Handle = User config structures
 *
 *  @return value Void (None)
 */
static void SPI_ReceiveHelper_8Bits(SPI_HandleTypeDef_t *SPI_Handle)
{
	*((uint8_t*)SPI_Handle->pRxDataAddr ) = *((__IO uint8_t *)(&SPI_Handle->Instance->DR));
	SPI_Handle->pRxDataAddr += sizeof(uint8_t);
	SPI_Handle->RxDataSize--;

	if(SPI_Handle->RxDataSize == 0)
	{
		SPI_CLOSEISR_RX(SPI_Handle);
	}

}
/*
 *  @brief SPI_ReceiveHelper_16Bits, Reads the Data Registers and stores into the user variable for 16bits format
 *
 *  @parameters SPI_Handle = User config structures
 *
 *  @return value Void (None)
 */
static void SPI_ReceiveHelper_16Bits(SPI_HandleTypeDef_t *SPI_Handle) // data yükleme saglar bu fonksiyonlar
{
	*((uint16_t*)SPI_Handle->pRxDataAddr ) = (uint16_t)SPI_Handle->Instance->DR;
	SPI_Handle->pRxDataAddr += sizeof(uint16_t);
	SPI_Handle->RxDataSize -= 2;

	if(SPI_Handle->RxDataSize == 0)
	{
		SPI_CLOSEISR_RX(SPI_Handle);
	}
}


/*
 *  @brief SPI_Init; Configures the SPI Peripheral
 *
 *  @parameters SPI_Handle = User config structures
 *
 *  @return value Void (None)
 */

void SPI_Init(SPI_HandleTypeDef_t *SPI_Handle)
{
	uint32_t tempValue = 0;

	tempValue = SPI_Handle->Instance->CR1;

	tempValue |= (SPI_Handle->Init.BaudRate) |	(SPI_Handle->Init.CPHA) | (SPI_Handle->Init.CPOL) \
				| (SPI_Handle->Init.DFF_Format) | (SPI_Handle->Init.Mode) | (SPI_Handle->Init.FrameFormat) \
				| (SPI_Handle->Init.BusConfig) | (SPI_Handle->Init.SSM_Cmd);

	SPI_Handle->Instance->CR1 = tempValue;


}

/*
 *  @brief SPI_PeriphCmd; Enable or Disable SPI Peripheral
 *
 *  @parameters SPI_Handle = User config structures
 *	@parameters stateOfSPI = ENABLE or DISABLE
 *
 *  @return value Void (None)
 */

void SPI_PeriphCmd(SPI_HandleTypeDef_t *SPI_Handle, FunctionalState_t stateOfSPI)
{
	if(stateOfSPI == ENABLE)
	{
		SPI_Handle->Instance->CR1 |= (0x1U << SPI_CR1_SPE);
	}
	else
	{
		SPI_Handle->Instance->CR1 &= ~(0x1U << SPI_CR1_SPE);
	}
}


/*
 *  @brief SPI_TransmitData, Transmit Data to the SLAVE
 *
 *  @parameters SPI_Handle = User config structures
 *
 *	@parameters pData = Addres of data to send
 *
 *	@parameters sizeOfData = Length of your data in bytes
 *
 *  @return value Void (None)
 */
void SPI_TransmitData(SPI_HandleTypeDef_t *SPI_Handle, uint8_t *pData,uint16_t sizeOfData)
{
	if(SPI_GetFlagStatus(SPI_Handle, SPI_TxE_FLAG))
	{
		while(sizeOfData > 0) 										// Veri var ise yazmaya devam et
		{
			if((SPI_Handle->Instance->SR >> 1U) & 0x1U) 			// Flag control, TXE & RXE and
			{
				SPI_Handle->Instance->DR = *((uint16_t*) pData); 	// 2byte yollamak icin
				pData += sizeof(uint16_t); 							// 2byte'lik arttirma yaptik
				sizeOfData -= 2;
			}
		}
	}
	else
	{
		while(sizeOfData > 0)
		{
			if((SPI_Handle->Instance->SR >> 1U) & 0x1U) // Flag control, TXE & RXE and
			{
				SPI_Handle->Instance->DR = *pData;
				pData++; // Otekini yollamak icin arttir, yada pData+=sizeof(uint8_t);
				sizeOfData--; // Bir tanesi yollandiysa dataBoyutu azalt
			}
		}
	}
	while(SPI_GetFlagStatus(SPI_Handle, SPI_Busy_FLAG)); // Wait for BUSY FLAG.
}

/*
 *  @brief SPI_TransmitData_IT, send the data to external world with Interrupt method
 *
 *  @parameters SPI_Handle = User config structures
 *
 *	@parameters pData = Carries the user data
 *
 *	@parameters sizeOfData = Bytes number that we will send
 *
 *  @return value Void (None)
 */
// SPI IRQ Transmit Data
void SPI_TransmitData_IT(SPI_HandleTypeDef_t *SPI_Handle,uint8_t *pData, uint16_t sizeOfData)
{
	SPI_BusStatus_t busState = SPI_Handle->busStateTx;
	if(busState != SPI_BUS_BUSY_TX) // If ise interrupt cagrilmistir, tx icin
	{
		// Busy degil ise degerleri globale cek
		SPI_Handle -> pTxDataAddr = (uint8_t*)pData; // Bu datanin adresinde pTx ile gezecegiz // yani data ve size'i globale çekti
		SPI_Handle ->TxDataSize = (uint16_t)sizeOfData; // Same thing with the above one, for data size
		SPI_Handle->busStateTx = SPI_BUS_BUSY_TX;

		if(SPI_Handle -> Instance->CR1 & (0x1U << SPI_CR1_DFF))
		{
			SPI_Handle -> TxISRFunction = SPI_TransmitHelper_16Bits;
		}
		else
		{
			SPI_Handle -> TxISRFunction = SPI_TransmitHelper_8Bits;
		}
		SPI_Handle -> Instance-> CR2 |= (0x1U << SPI_CR2_TXEIE); // Interrupt enable
	}
}


/*
 *  @brief SPI_ReceiveData_IT,Read the data from external world with Interrupt method
 *
 *  @parameters SPI_Handle = User config structures
 *
 *	@parameters pData = Stores the data in this variable
 *
 *	@parameters sizeOfData = Bytes number that we will read
 *
 *  @return value Void (None)
 */
void SPI_ReceiveData_IT(SPI_HandleTypeDef_t *SPI_Handle,uint8_t *pData, uint16_t sizeOfData)
{
	SPI_BusStatus_t busState = SPI_Handle -> busStateRx;

	if(busState != SPI_BUS_BUSY_RX)
	{// İçerideki verileri globale atamak:

		SPI_Handle -> pRxDataAddr = (uint8_t*)pData;
		SPI_Handle -> RxDataSize = (uint16_t)sizeOfData;
		SPI_Handle-> busStateRx = SPI_BUS_BUSY_RX; // busy yapiyoruz ki mesgulde kalsın, rahatsız edilmesin

		if(SPI_Handle->Instance->CR1 & (0x1U << SPI_CR1_DFF))
		{
			SPI_Handle -> RxISRFunction = SPI_ReceiveHelper_16Bits;
		}
		else
		{
			SPI_Handle -> RxISRFunction = SPI_ReceiveHelper_8Bits;
		}

		SPI_Handle -> Instance-> CR2 |= (0x1U << SPI_CR2_RXNEIE); // Receive icin Interrupt enable
	}
}

void SPI_InterruptHandler(SPI_HandleTypeDef_t *SPI_Handle)
{
	uint8_t interruptSource = 0;
	uint8_t interruptFlag = 0;

	interruptSource = SPI_Handle->Instance->CR2 & (0x1U << SPI_CR2_TXEIE);
	interruptFlag = SPI_Handle->Instance->SR & (0x1U << SPI_SR_TxE);

	if((interruptSource != 0 ) && (interruptFlag != 0)) // interrupt gerçekten gelmistir,fonk. cagir
	{
		SPI_Handle->TxISRFunction(SPI_Handle);
	}

	interruptSource = SPI_Handle->Instance->CR2 & (0x1U << SPI_CR2_RXNEIE);
	interruptFlag = SPI_Handle->Instance->SR & (0x1U << SPI_SR_RxNE);

	if((interruptSource != 0 ) && (interruptFlag != 0)) // interrupt gerçekten gelmistir,fonk. cagir, receive için
		{
			SPI_Handle->RxISRFunction(SPI_Handle);
		}
}

/*
 *  @brief SPI_ReceiveData, Receive Data from the slave
 *
 *  @parameters SPI_Handle = User config structures
 *
 *	@parameters pData = Addres of data to store to the values that I get
 *
 *	@parameters sizeOfData = Length of your data in bytes
 *
 *  @return value Void (None)
 */

void SPI_ReceiveData(SPI_HandleTypeDef_t *SPI_Handle, uint8_t *pBuffer,uint16_t sizeOfData)
{
	if(SPI_Handle->Init.DFF_Format == SPI_DFF_16BITS)
	{
		while(sizeOfData > 0)
		{
			if(SPI_GetFlagStatus(SPI_Handle, SPI_RxNE_FLAG))
			{
				*((uint16_t*)pBuffer) = (uint16_t)SPI_Handle->Instance->DR;
				pBuffer+= sizeof(uint16_t);
				sizeOfData -= 2;
			}
		}
	}

	else
	{
		while(sizeOfData > 0)
		{
			if(SPI_GetFlagStatus(SPI_Handle, SPI_RxNE_FLAG))
			{
				*(pBuffer) = *((__IO uint8_t*)&SPI_Handle->Instance->DR );
				pBuffer+= sizeof(uint8_t);
				sizeOfData -= 1;
			}

		}
	}
}

/*
 *  @brief SPI_FlagStatus, Return the flag of SR register
 *
 *  @parameters SPI_Handle = User config structures
 *
 *	@parameters SPI_Flag = Flag name of SR register
 *
 *	@
 *
 *  @return value : SPI_FlagStatus_t
 */

SPI_FlagStatus_t SPI_GetFlagStatus(SPI_HandleTypeDef_t *SPI_Handle,uint16_t SPI_Flag)
{
	return (SPI_Handle->Instance->SR & SPI_Flag) ? SPI_FLAG_SET : SPI_FLAG_RESET;
}




