/*
 * I2C.c
 *
 *  Created on: 1 Ara 2023
 *      Author: recep
 */
#include "I2C.h"





void I2C_Init(I2C_HandleTypeDef_t *I2C_Handle)
{
	uint32_t pClockValue = 0x0U;

	pClockValue = RCC_GetPClock1(); // Clock1'e bagli, clock hizimizi almamiz gerekir, icin I2C.

	if(Check_PClock_Value(pClockValue, I2C_Handle->Init.ClockSpeed) != 1U)
	{
		uint32_t tempReg = 0x0U;
		uint32_t freqValue = 0x0U;

		/***** ACK STATE AND STRETCHING MODE *****/

		tempReg = I2C_Handle->Instance->CR1;

		tempReg |= (I2C_Handle->Init.ACK_State) | (I2C_Handle->Init.ClockStretch);

		I2C_Handle->Instance->CR1 = tempReg;

		/***** Frequency Value Of PCLOCK for Clock Generation *****/

		freqValue = I2C_Get_FreqValue(pClockValue);

		tempReg = I2C_Handle->Instance->CR2;

		tempReg |= (freqValue << 0U);

		I2C_Handle->Instance->CR2 = tempReg;

		/***** Addressing Mode Configuration *****/

		tempReg = I2C_Handle -> Instance ->OAR1;

		tempReg |= (I2C_Handle->Init.AdressingMode);

		if(I2C_Handle->Init.AdressingMode == I2C_ADDRMODE_7)
		{
			tempReg |= (I2C_Handle->Init.MyOwnAddress << 1U );
		}
		else
		{
			tempReg |= (I2C_Handle->Init.MyOwnAddress << 0U );
		}



	}
}









/*
 * I2C_PeriphCmd, Enable or Disable I2C Peripheral
 * @param I2Cx = I2C Peripheral
 *
 * @param stateOfI2C = ENABLE or DISABLE
 *
 * @retval Void
 */


void I2C_PeriphCmd(I2C_TypeDef_t *I2Cx,FunctionalState_t stateOfI2C)
{

	if(stateOfI2C == ENABLE)
	{
		I2Cx->CR1 |= (0x1U << I2C_CR1_PE);
	}
	else
	{
		I2Cx->CR1 &= ~(0x1U << I2C_CR1_PE);
	}
}
