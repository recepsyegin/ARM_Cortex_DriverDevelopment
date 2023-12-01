#include "GPIO.h"




/*
 *  @brief GPIO_Init; Configures the port and pin
 *
 *  @parameters GPIOx = GPIO Port Base Address
 *  @parameters GPIO_InitTypeDef_t = User Config Structures
 *
 *  @return value Void (None)
 */

void GPIO_Init(GPIO_TypeDef_t *GPIOx, GPIO_InitTypeDef_t *GPIO_ConfigStruct)
{
	uint32_t position;
	uint32_t fakePosition = 0;
	uint32_t lastPosition = 0;

	for(position = 0; position < 16; position++)
	{
		fakePosition = (0x1U << position);
		lastPosition = (uint32_t)(GPIO_ConfigStruct->pinNumber) & fakePosition;

		if(fakePosition == lastPosition)
		{
			/* MODE CONFIG */
			uint32_t tempValue = GPIOx->MODER;

			tempValue &=~(0x3U << (position*2));
			tempValue |= (GPIO_ConfigStruct->Mode << (position * 2) );

			GPIOx -> MODER = tempValue;

			if(GPIO_ConfigStruct->Mode == GPIO_MODE_OUTPUT || GPIO_ConfigStruct -> Mode == GPIO_MODE_AF)
			{
				/* OUTPUT TYPE CONFIG */
				tempValue = GPIOx->OTYPER;
				tempValue &=~ (0x1U << position);
				tempValue |= (GPIO_ConfigStruct->Otype << position);
				GPIOx->OTYPER = tempValue;

				/* OUTPUT SPEED CONFIG */
				tempValue = GPIOx->OSPEEDR;
				tempValue &=~ (0x3U << (position * 2 ));
				tempValue |= (GPIO_ConfigStruct->Speed << (position*2));
				GPIOx->OSPEEDR = tempValue;
			}

			/*PUSH PULL CONFIG*/
			tempValue = GPIOx->PUPDR;
			tempValue &=~ (0x3U << (position *2 ));
			tempValue |= (GPIO_ConfigStruct->PuPd << (position *2 ));
			GPIOx -> PUPDR = tempValue;

			if(GPIO_ConfigStruct -> Mode == GPIO_MODE_AF)
			{
				tempValue = GPIOx->AFR[position >> 3U]; // pos >> 3 , 8e bolmek demek, 0 yada 1, AFR Low yada AFR High
				tempValue &= ~(0xFU << ((position & 0x7U) * 4)); // mod almak, 3 biti 1 olan sayi 0x7
				tempValue |= (GPIO_ConfigStruct->Alternate << ((position & 0x7U) * 4) );
				GPIOx->AFR[position >> 3U] = tempValue;
			}
		}

	}
}









/*
 *  @brief GPIO_Write_Pin; Makes pin HIGH or LOW
 *
 *  @parameters GPIOx = GPIO Port Base Address
 *  @parameters pinNumber = GPIO Pin Numbers 0 - 15
 *  @parameters pinState = GPIO_Pin_Set OR GPIO_Pin_Reset
 *
 *  @return value Void (None)
 */


void GPIO_WritePin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber,GPIO_PinState_t pinState)
{
	if(pinState == GPIO_Pin_Set)
	{
		GPIOx -> BSRR = pinNumber;
	}
	else
	{
		GPIOx -> BSRR = (pinNumber << 16);
	}
}


/*
 *  @brief GPIO_Read_Pin; reads the pin of GPIOx Port
 *
 *  @parameters GPIOx = GPIO Port Base Address
 *  @parameters pinNumber = GPIO Pin Numbers 0 - 15
 *
 *
 *  @return value GPIO_PinState_t
 */

GPIO_PinState_t GPIO_ReadPin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber)
{
	GPIO_PinState_t bitStatus = GPIO_Pin_Reset; // Initializes with reset at first, doesn't matter

	if((GPIOx -> IDR & pinNumber)!= GPIO_Pin_Reset)
	{
		bitStatus = GPIO_Pin_Set;
	}

	return bitStatus;
}

/*
 *  @brief GPIO_Lock_Pin; locks the pin of GPIOx Port
 *
 *  @parameters GPIOx = GPIO Port Base Address
 *  @parameters pinNumber = GPIO Pin Numbers 0 - 15
 *
 *
 *  @return value Void
 */
void GPIO_LockPin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber)
{
	uint32_t tempValue = (0x1U << 16 ) | pinNumber; /*16.bit 1 ve kendi istedigim pinlerim 1 digerleri 0*/

	GPIOx->LCKR = tempValue;	// LCKRR[16] = '1'		LCKRR[15:0] = DATA
	GPIOx->LCKR = pinNumber;	// LCKRR[16] = '0'		LCKRR[15:0] = DATA
	GPIOx->LCKR = tempValue;	// LCKRR[16] = '1'		LCKRR[15:0] = DATA
	tempValue = GPIOx->LCKR;	// Read Lock Register
}



void GPIO_TogglePin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber)
{
	uint32_t tempODRRegister = GPIOx -> ODR; // ODR'leri atamak,islenecek pinleri belirlemek

	GPIOx -> BSRR = ((tempODRRegister & pinNumber)<< 16U ) | (~tempODRRegister & pinNumber); // on ise off, off ise on
}
