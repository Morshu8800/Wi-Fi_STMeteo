/*
 * uart.c
 *
 *  Created on: Mar 31, 2025
 *      Author: FlY DeN
 */

#include "uart.h"


void CMSIS_USART_Init(void) {
	SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN);//Enable GPIOA clock
	//PA2
	MODIFY_REG(GPIOA->AFR[0], GPIO_AFRL_AFSEL2_Msk, 0b0001 << GPIO_AFRL_AFSEL2_Pos);//Enable AF
	MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE2_Msk, 0b10 << GPIO_MODER_MODE2_Pos);//AF mode PA2
	MODIFY_REG(GPIOA->OTYPER, GPIO_OTYPER_OT2_Msk, 0b0 << GPIO_OTYPER_OT2_Pos);//type PA2
	MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED2_Msk, 0b11 << GPIO_OSPEEDR_OSPEED2_Pos);//very high speed
	MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPD2_Msk, 0b00 << GPIO_PUPDR_PUPD2_Pos);//no pu/pd
	//PA3
	MODIFY_REG(GPIOA->AFR[0], GPIO_AFRL_AFSEL3_Msk, 0b0001 << GPIO_AFRL_AFSEL3_Pos);//Enable AF
	MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE3_Msk, 0b10 << GPIO_MODER_MODE3_Pos);//AF mode PA3
	MODIFY_REG(GPIOA->OTYPER, GPIO_OTYPER_OT3_Msk, 0b0 << GPIO_OTYPER_OT3_Pos);//type PA3
	MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED3_Msk, 0b11 << GPIO_OSPEEDR_OSPEED3_Pos);//very high speed
	MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPD3_Msk, 0b00 << GPIO_PUPDR_PUPD3_Pos);//no pu/pd
	//u(s)art2 configuration
	SET_BIT(RCC->APBENR1, RCC_APBENR1_USART2EN);//Enable UASRT2 clock
	CLEAR_BIT(USART2->CR1, USART_CR1_UE);//disable uart
	CLEAR_BIT(USART2->CR1, USART_CR1_OVER8);// set oversampling to 16
	USART2->BRR = 0x8b;//set baud rate
	MODIFY_REG(USART2->CR1, USART_CR1_M1_Msk, 0b00 << USART_CR1_M1_Pos);//1 start bit 8 bit data n bit stop
	CLEAR_BIT(USART2->CR1, USART_CR1_WAKE);//Wake as Idle line
	CLEAR_BIT(USART2->CR1, USART_CR1_PCE);//Parity control disable
	//u(s)art interrupts
	CLEAR_BIT(USART2->CR1, USART_CR1_PEIE);//PEI disable
	CLEAR_BIT(USART2->CR1, USART_CR1_TXEIE_TXFNFIE);//TXIE disable
	CLEAR_BIT(USART2->CR1, USART_CR1_TCIE);//TCIE disable
	SET_BIT(USART2->CR1, USART_CR1_RXNEIE_RXFNEIE);//RXIE enable
	SET_BIT(USART2->CR1, USART_CR1_IDLEIE);//IDLEIE enable
	SET_BIT(USART2->CR1, USART_CR1_TE);//Transmitter enable
	SET_BIT(USART2->CR1, USART_CR1_RE);//Receiver enable
	CLEAR_BIT(USART2->CR1, USART_CR1_MME);//only in active mode

	USART2->CR2 = 0;//Control Register 2 = 0
	USART2->CR3 = 0;//Control Register 3 = 0
	USART2->GTPR = 0;//Prescaler = 0

	SET_BIT(USART2->CR1, USART_CR1_UE);//enable uart

	NVIC_EnableIRQ(USART2_IRQn);//enable USART2 IRQ

}

bool CMSIS_USART_Transmit(USART_TypeDef *USART, uint8_t *data, uint16_t Size, uint32_t Timeout){
	static uint32_t uart_tim = 0;
	uart_tim = HAL_GetTick();
	for(uint16_t i = 0; i < Size; i++){
		while(READ_BIT(USART->ISR, USART_ISR_TXE_TXFNF) == 0){
			if(HAL_GetTick() - uart_tim >= Timeout){
				uart_tim = HAL_GetTick();
				return false;
			}
		}
		USART->TDR = *data++;
	}
	uart_tim = HAL_GetTick();
	return true;
}



