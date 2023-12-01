#ifndef INC_RCC_H_
#define INC_RCC_H_


#include "stm32f407xx.h"

/*
 * RCC AHB1 Peripherals Clock Control Macro Definitions
 *
 */

#define RCC_GPIOA_CLK_ENABLE()						do{ uint32_t tempValue =0; 	\
														SET_BIT(RCC->AHB1ENR,RCC_AHB1ENR_GPIOAEN);	\
														tempValue = READ_BIT(RCC->AHB1ENR,RCC_AHB1ENR_GPIOAEN);	\
														UNUSED(tempValue);	\
													}while(0) // while(0) ONE TIME

#define RCC_GPIOB_CLK_ENABLE()						do{ uint32_t tempValue =0; 	\
														SET_BIT(RCC->AHB1ENR,RCC_AHB1ENR_GPIOBEN);	\
														tempValue = READ_BIT(RCC->AHB1ENR,RCC_AHB1ENR_GPIOBEN);	\
														UNUSED(tempValue);	\
													}while(0)

#define RCC_GPIOC_CLK_ENABLE()						do{ uint32_t tempValue =0; 	\
														SET_BIT(RCC->AHB1ENR,RCC_AHB1ENR_GPIOCEN);	\
														tempValue = READ_BIT(RCC->AHB1ENR,RCC_AHB1ENR_GPIOCEN);	\
														UNUSED(tempValue);	\
													}while(0)

#define RCC_GPIOD_CLK_ENABLE()						do{ uint32_t tempValue =0; 	\
														SET_BIT(RCC->AHB1ENR,RCC_AHB1ENR_GPIODEN);	\
														tempValue = READ_BIT(RCC->AHB1ENR,RCC_AHB1ENR_GPIODEN);	\
														UNUSED(tempValue);	\
													}while(0)

#define RCC_SYSCFG_CLK_ENABLE()						do{		uint32_t tempValue = 0;	\
															SET_BIT(RCC->APB2ENR,RCC_APB2ENR_SYSCFGEN);	\
															tempValue = READ_BIT(RCC->APB2ENR,RCC_APB2ENR_SYSCFGEN); \
															UNUSED(tempValue); \
													}while(0)

#define RCC_GPIOA_CLK_DISABLE()						CLEAR_BIT(RCC->AHB1ENR,RCC_AHB1ENR_GPIOAEN)
#define RCC_GPIOB_CLK_DISABLE()						CLEAR_BIT(RCC->AHB1ENR,RCC_AHB1ENR_GPIOBEN)
#define RCC_GPIOC_CLK_DISABLE()						CLEAR_BIT(RCC->AHB1ENR,RCC_AHB1ENR_GPIOCEN)
#define RCC_GPIOD_CLK_DISABLE()						CLEAR_BIT(RCC->AHB1ENR,RCC_AHB1ENR_GPIODEN)



/*
 * RCC APB2 Peripherals Clock Control Macro Definitions
 *
 */


#define RCC_SPI1_CLK_ENABLE()						do{uint32_t tempValue = 0;                               \
													   SET_BIT(RCC->APB2ENR,RCC_APB2ENR_SPI1EN);             \
													   tempValue = READ_BIT(RCC->APB2ENR,RCC_APB2ENR_SPI1EN);\
													   UNUSED(tempValue);                                    \
													  }while(0)

#define RCC_SPI1_DISABLE()						CLEAR_BIT(RCC->APB2ENR,RCC_APB2ENR_SPI1EN)


/*
 * RCC APB1 Peripherals Clock Control Macro Definitions
 *
 */
#define RCC_SPI2_CLK_ENABLE()						do{uint32_t tempValue = 0;                               \
													   SET_BIT(RCC->APB1ENR,RCC_APB2ENR_SPI1EN);             \
													   tempValue = READ_BIT(RCC->APB1ENR,RCC_APB2ENR_SPI1EN);\
													   UNUSED(tempValue);                                    \
													  }while(0)

#define RCC_SPI2_CLK_DISABLE()						CLEAR_BIT(RCC->APB1ENR,RCC_APB2ENR_SPI1EN)


#define RCC_USART2_CLK_ENABLE()						do{uint32_t tempValue = 0;									\
														SET_BIT(RCC->APB1ENR,RCC_APB1ENR_USART2EN);				\
														tempValue = READ_BIT(RCC->APB1ENR,RCC_APB1ENR_USART2EN);\
														UNUSED(tempValue);										\
													}while(0)

#define RCC_USART2_CLK_DISABLE()					CLEAR_BIT(RCC->APB1ENR,RCC_APB1ENR_USART2EN)



#define RCC_I2C1_CLK_ENABLE()						do{uint32_t tempValue = 0;									\
														SET_BIT(RCC->APB1ENR,RCC_APB1EN_I2C1EN);				\
														tempValue = READ_BIT(RCC->APB1ENR,RCC_APB1EN_I2C1EN);	\
														UNUSED(tempValue);										\
													}while(0)

#define RCC_I2C1_CLK_DISABLE()					CLEAR_BIT(RCC->APB1ENR,RCC_APB1EN_I2C1EN)







//#define RCC_SPI2_CLK_ENABLE()
//#define RCC_SPI3_CLK_ENABLE()
//#define RCC_SPI4_CLK_ENABLE()


// Bagli oldugumuz cevresel clocklar icin yazdik
uint32_t RCC_GetSystemClock(void);
uint32_t RCC_GetHClock(void);
uint32_t RCC_GetPClock1(void);
uint32_t RCC_GetPClock2(void);







#endif /* INC_RCC_H_ */
