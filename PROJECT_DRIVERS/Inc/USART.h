/*
 * USART.h
 *
 *  Created on: 11 Kas 2023
 *      Author: recep
 */

#ifndef INC_USART_H_
#define INC_USART_H_
#include "stm32f407xx.h"
/**********************/


/*
 *
 *  @def_group MODE_Types
 */
#define USART_MODE_Tx					((uint32_t)(0x00000008) ) // 3.biti 1 olan sayi
#define USART_MODE_Rx					((uint32_t)(0x00000004) )
#define USART_MODE_Tx_rx				((uint32_t)(0x0000000C) )

/*
 *
 *  @def_group WordLength_Types
 */
#define USART_WORDLENGTH_8Bits			((uint32_t)(0x00000000) )
#define USART_WORDLENGTH_9Bits			((uint32_t)(0x00001000) )

/*
 *
 * @def_group Parity_Modes
 */
#define USART_PARITY_NONE				((uint32_t)(0x00000000) )
#define USART_PARITY_EVEN				((uint32_t)(0x00000400) )
#define USART_PARITY_ODD				((uint32_t)(0x00000600) )

/*
 *
 * @def_group	Stop Bits
 */
#define USART_STOPBITS_1				((uint32_t)(0x00000000) )
#define USART_STOPBITS_Half				((uint32_t)(0x00001000) )
#define USART_STOPBITS_2				((uint32_t)(0x00002000) )
#define USART_STOPBITS_1_Half			((uint32_t)(0x00003000) )

/*
 *
 * @def_group OverSampling_Modes
 */
#define USART_OVERSAMPLE_16				((uint32_t)(0x00000000) )
#define USART_OVERSAMPLE_8				((uint32_t)(0x00008000) ) // 15.BİTİ 1 OLAN SAYI

/*
 *
 * @def_group HardWareFlowControl_Modes
 */
#define USART_HW_NONE					((uint32_t)(0x00000000) )
#define USART_HW_CTS					((uint32_t)(0x00000200) )
#define USART_HW_RTS					((uint32_t)(0x00000100) )
#define USART_HW_CTS_RTS				((uint32_t)(0x00000300) )

/*USART FLAG STATUS*/
typedef enum
{
	USART_FLAG_RESET = 0x0U,
	USART_FLAG_SET = !USART_FLAG_RESET
}USART_FlagStatus_t;


#define __USART_DIV_VALUE_16(__CLOCK__,__BAUDRATE__)				( ( 25U * (uint32_t)(__CLOCK__) ) / (4U * __BAUDRATE__) )
#define __USART_DIV_VALUE_8(__CLOCK__,__BAUDRATE__ )				( ( 25U * (uint32_t)(__CLOCK__) ) / (2U * __BAUDRATE__) )

typedef enum
{

	USART_BUS_FREE = 0x0U,
	USART_BUS_Tx = 0x1U,
	USART_BUS_Rx = 0x2U

}USART_BusState_t;


typedef struct
{
	uint32_t Mode;						/*!< Transmission and Reception Modes @def_group MODE_Types 			*/
	uint32_t BaudRate;					/*!< User Value for UART Baudrate										*/
	uint32_t WordLength;				/*!< 8 Bits & 9 Bits Modes @def_group WordLength_Types      			*/
	uint32_t Parity;					/*!< Even & Odd Parity Modes @def_group Parity_Modes        			*/
	uint32_t StopBits;					/*!< Stop Bits Modes		 @def_group	StopBits_Modes					*/
	uint32_t OverSampling;				/*!< Over Sampling Modes @def_group OverSampling_Modes					*/
	uint32_t HardWareFlowControl;		/*!< HardWareFlowControl Modes 	@def_group HardWareFlowControl_Modes	*/


}USART_InitTypeDef_t;

typedef struct __USART_HandleTypeDef_t
{
	USART_TypeDef_t *Instance;
	USART_InitTypeDef_t Init;

	uint8_t *pTxBuffer; // ilgili adresi globale atacagiz burada tutalim
	uint16_t TxBufferSize;
	uint8_t TxStatus;
	void (*TxISR_Function)(struct __USART_HandleTypeDef_t *USART_Handle);

	uint8_t *pRxBuffer;
	uint16_t RxBufferSize;
	uint8_t RxStatus;
	void(*RxISR_Function)(struct __USART_HandleTypeDef_t *USART_Handle);

}USART_HandleTypeDef_t;


void USART_Init(USART_HandleTypeDef_t *USART_Handle);
void USART_TransmitData(USART_HandleTypeDef_t *USART_Handle,uint8_t *pData, uint16_t dataSize);
void USART_ReceiveData(USART_HandleTypeDef_t *USART_Handle,uint8_t *pBuffer,uint16_t dataSize);
void USART_TransmitData_IT(USART_HandleTypeDef_t *USART_Handle,uint8_t *pData, uint16_t dataSize);
void USART_ReceiveData_IT(USART_HandleTypeDef_t *USART_Handle, uint8_t *pBuffer,uint16_t dataSize);
void USART_PeriphCmd(USART_HandleTypeDef_t *USART_Handle, FunctionalState_t stateOfUSART);

USART_FlagStatus_t USART_GetFlagStatus(USART_HandleTypeDef_t *USART_Handle, uint16_t flagName);
void USART_InterruptHandler(USART_HandleTypeDef_t *USART_Handle);





















#endif /* INC_USART_H_ */
