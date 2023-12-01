#include "RCC.h"


const uint8_t AHB_Prescaler[] = {0,0,0,0,0,0,0,0,1,2,3,4,6,7,8,9};
const uint8_t APB_Prescaler[] = {0,0,0,0,1,2,3,4};


uint32_t RCC_GetSystemClock(void)
{
	uint32_t SystemCoreClock = 0;

	uint8_t clkSource = 0;

	clkSource = (( RCC->CFGR >> 2U )	 & 0x3U );

	switch(clkSource)
	{
		case 0:
			SystemCoreClock = 16000000;		break;
		case 1:
			SystemCoreClock = 8000000;		break;
	//	case 2: // PLL ile alakali

		default:
			SystemCoreClock = 16000000;
	}
	return SystemCoreClock;
}

uint32_t RCC_GetHClock(void)
{
	uint32_t AHB_PeriphClock = 0;
	uint32_t SystemCoreClock = 0;
	uint8_t HPRE_Value = 0;
	uint8_t tempValue = 0;

	SystemCoreClock = RCC_GetSystemClock();

	HPRE_Value = ((RCC->CFGR >> 4U ) & 0xFU);
	tempValue = AHB_Prescaler[HPRE_Value];

	AHB_PeriphClock = ( SystemCoreClock >> tempValue );

	return AHB_PeriphClock;
}

uint32_t RCC_GetPClock1(void)
{
	uint32_t APB1_PeriphClk = 0;
	uint32_t Hclock = 0;
	uint8_t HPRE1_Value = 0;
	uint8_t tempValue = 0;


	Hclock = RCC_GetHClock();

	HPRE1_Value = ((RCC->CFGR >> 10U) & 0x7U );

	tempValue = APB_Prescaler[HPRE1_Value];

	APB1_PeriphClk = ( Hclock >> tempValue );

	return APB1_PeriphClk;
}

uint32_t RCC_GetPClock2(void)
{
	uint32_t APB2_PeriphClk = 0;
	uint32_t Hclock = 0;
	uint8_t HPRE2_Value = 0;
	uint8_t tempValue = 0;

	Hclock = RCC_GetHClock();

	HPRE2_Value = ((RCC->CFGR >> 13U) & 0x7U);

	tempValue = APB_Prescaler[HPRE2_Value];

	APB2_PeriphClk = (Hclock >> tempValue);



	return APB2_PeriphClk;
}
















