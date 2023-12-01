/*
 * I2C.h
 *
 *  Created on: 1 Ara 2023
 *      Author: recep
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

#include "stm32f407xx.h"


#define Check_PClock_Value(__pClock__ , __I2C_Clock__)		((__I2C_Clock__ <= I2C_SPEED_STANDART) ? (__pClock__ <= 2000000U ) : ( __pClock__ <= 4000000U) )
#define I2C_Get_FreqValue(__pClock__)						((__pClock__) / (1000000U ))


/*
 *
 *  @def_group I2C_ClockSpeeds
 *
 */
#define I2C_SPEED_STANDART			(100000U) // 100 kHz
#define I2C_SPEED_FASTMODE			(400000U) // 400 kHz

/*
 *
 * @def_group I2C_ACK_State
 *
 */
#define I2C_ACK_DISABLE				(0x00000000U)
#define I2C_ACK_ENABLE				(0x00000400U) // 10.biti 1 olan sayi

/*
 *
 * @def_group I2C_Stretch_State
 */
#define I2C_STRETCH_ENABLE			(0x00000000U)
#define I2C_STRETCH_DISABLE			(0x00000080U) // 7.biti 1 olan sayi

/*
 *
 * @def_group I2C_Address_Mode
 *
 */

#define I2C_ADDRMODE_7				(0x0004000U)
#define I2C_ADDRMODE_10				(0x000C000U)

/*
 *
 * @def_group I2C_Duty_Mode
 *
 */
#define I2C_DUTY_STANDART			(0x00000000U)
#define I2C_DUTY_FM_2				(0x00008000U)
#define I2C_DUTY_FM_16_9			(0x0000C000U)

typedef struct
{
	uint32_t ClockSpeed;		/*!< I2C Clock Speed Choice @def_group I2C_ClockSpeeds			*/
	uint32_t ACK_State;			/*!< I2C ACK State  Choice  @def_group I2C_ACK_State			*/
	uint32_t ClockStretch;		/*!< I2C Stretch State Choice @def_group I2C_Stretch_State		*/
	uint32_t AdressingMode;		/*!< I2C Address Mode Choice @def_group I2C_Address_Mode		*/
	uint32_t DutyCycle;			/*!< I2C Duty Mode Choice @def_group I2C_Duty_Mode				*/
	uint32_t MyOwnAddress;		/*!< I2C Slave Mode Address										*/

}I2C_InitTypeDef_t;

typedef struct
{
	I2C_InitTypeDef_t Init; 			// Ustteki yapiya ulasmak icin ...
	I2C_TypeDef_t	*Instance; 			// Registerlere dokunmak icin  ...


}I2C_HandleTypeDef_t;



void I2C_Init(I2C_HandleTypeDef_t *I2C_Handle);
void I2C_PeriphCmd(I2C_TypeDef_t *I2Cx,FunctionalState_t stateOfI2C);








#endif /* INC_I2C_H_ */
