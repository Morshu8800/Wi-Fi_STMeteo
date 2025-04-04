/*
 * timers.c
 *
 *  Created on: Apr 2, 2025
 *      Author: FlY DeN
 */


#include "timers.h"


void CMSIS_TIM2_CH2_Init(void){
	//PA1
	SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN); //Enable clock GPIOA
	MODIFY_REG(GPIOA->AFR[0], GPIO_AFRL_AFSEL1_Msk, 0b0010 << GPIO_AFRL_AFSEL1_Pos); //Enable AF
	MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE1_Msk, 0b10 << GPIO_MODER_MODE1_Pos); //MODE AF
	MODIFY_REG(GPIOA->OTYPER, GPIO_OTYPER_OT1_Msk, 0b0 << GPIO_OTYPER_OT1_Pos); //TYPE PP
	MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED1_Msk, 0b11 << GPIO_OSPEEDR_OSPEED1_Pos); //Very high speed
	MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPD1_Msk, 0b00 << GPIO_PUPDR_PUPD1_Pos); //No PUPD
	//TIM2CH2 Configuration
	SET_BIT(RCC->APBENR1, RCC_APBENR1_TIM2EN); //Enable clock TIM2

	CLEAR_BIT(TIM2->CR1, TIM_CR1_CEN); //Disable counter
	CLEAR_BIT(TIM2->CR1, TIM_CR1_UDIS); //Enable update req
	CLEAR_BIT(TIM2->CR1, TIM_CR1_URS); //Enable update
	CLEAR_BIT(TIM2->CR1, TIM_CR1_OPM); //Disable One-pulse
	CLEAR_BIT(TIM2->CR1, TIM_CR1_DIR); //UpCounter
	MODIFY_REG(TIM2->CR1, TIM_CR1_CMS_Msk, 0b00 << TIM_CR1_CMS_Pos); //Edge-aligned Mode
	//SET_BIT(TIM2->CR1, TIM_CR1_ARPE); //Auto-reload preload enable  !!!!THIS IS F*СKING SHIT!!!!
	MODIFY_REG(TIM2->CR1, TIM_CR1_CKD_Msk, 0b00 << TIM_CR1_CKD_Pos); //No division
	SET_BIT(TIM2->DIER, TIM_DIER_UIE); //Update interrupt enable

	TIM2->PSC = 0; //TIM2 Prescaler
	TIM2->ARR = 511; //TIM2 Auto Reload REG
}
void CMSIS_TIM2_CH2_PWM_Start(void){
	NVIC_EnableIRQ(TIM2_IRQn);
	SET_BIT(TIM2->CR1, TIM_CR1_CEN); //Enable counter
	CLEAR_BIT(TIM2->CCER, TIM_CCER_CC2E); //Disable PWM Output
	MODIFY_REG(TIM2->CCMR1, TIM_CCMR1_CC2S_Msk, 0b00 << TIM_CCMR1_CC2S_Pos); // CC2 as Output
	CLEAR_BIT(TIM2->CCMR1, TIM_CCMR1_OC2FE); //Fast Mode disable
	SET_BIT(TIM2->CCMR1, TIM_CCMR1_OC2PE); //Preload enable
	MODIFY_REG(TIM2->CCMR1, TIM_CCMR1_OC2M_Msk, 0b0110 << TIM_CCMR1_OC2M_Pos); //PWM Mode 1
	CLEAR_BIT(TIM2->CCMR1, TIM_CCMR1_OC2CE); //Not affected by ETRF input
	CLEAR_BIT(TIM2->CCER, TIM_CCER_CC2P); //PWM active high
	SET_BIT(TIM2->CCER, TIM_CCER_CC2E); //Enable PWM Output
}

void CMSIS_TIM2_CH2_PWM_Stop(void){
	NVIC_DisableIRQ(TIM2_IRQn);
	CLEAR_BIT(TIM2->CR1, TIM_CR1_CEN); //Disable counter
	CLEAR_BIT(TIM2->CCER, TIM_CCER_CC2E); //Disable PWM Output
}

void CMSIS_TIM3_CH1_Init(void){
	//PB4
	SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOBEN); //Enable clock GPIOB
	MODIFY_REG(GPIOB->AFR[0], GPIO_AFRL_AFSEL4_Msk, 0b0001 << GPIO_AFRL_AFSEL4_Pos); //Enable AF
	MODIFY_REG(GPIOB->MODER, GPIO_MODER_MODE4_Msk, 0b10 << GPIO_MODER_MODE4_Pos); //MODE AF
	MODIFY_REG(GPIOB->OTYPER, GPIO_OTYPER_OT4_Msk, 0b0 << GPIO_OTYPER_OT4_Pos); //TYPE PP
	MODIFY_REG(GPIOB->OSPEEDR, GPIO_OSPEEDR_OSPEED4_Msk, 0b11 << GPIO_OSPEEDR_OSPEED4_Pos); //Very high speed
	MODIFY_REG(GPIOB->PUPDR, GPIO_PUPDR_PUPD4_Msk, 0b00 << GPIO_PUPDR_PUPD4_Pos); //No PUPD
	//TIM3CH1 Configuration
	SET_BIT(RCC->APBENR1, RCC_APBENR1_TIM3EN); //Enable clock TIM3

	CLEAR_BIT(TIM3->CR1, TIM_CR1_CEN); //Disable counter
	CLEAR_BIT(TIM3->CR1, TIM_CR1_UDIS); //Enable update req
	CLEAR_BIT(TIM3->CR1, TIM_CR1_URS); //Enable update
	CLEAR_BIT(TIM3->CR1, TIM_CR1_OPM); //Disable One-pulse
	CLEAR_BIT(TIM3->CR1, TIM_CR1_DIR); //UpCounter
	MODIFY_REG(TIM3->CR1, TIM_CR1_CMS_Msk, 0b00 << TIM_CR1_CMS_Pos); //Edge-aligned Mode
	//SET_BIT(TIM3->CR1, TIM_CR1_ARPE); //Auto-reload preload enable  !!!!THIS IS F*CKING SHIT!!!!
	MODIFY_REG(TIM3->CR1, TIM_CR1_CKD_Msk, 0b00 << TIM_CR1_CKD_Pos); //No division
	SET_BIT(TIM3->DIER, TIM_DIER_UIE); //Update interrupt enable

	TIM3->PSC = 0; //TIM3 Prescaler
	TIM3->ARR = 255; //TIM3 Auto Reload REG
}

void CMSIS_TIM3_CH1_PWM_Start(void){
	NVIC_EnableIRQ(TIM3_IRQn);
	SET_BIT(TIM3->CR1, TIM_CR1_CEN); //Enable counter
	CLEAR_BIT(TIM3->CCER, TIM_CCER_CC1E); //Disable PWM Output
	MODIFY_REG(TIM3->CCMR1, TIM_CCMR1_CC1S_Msk, 0b00 << TIM_CCMR1_CC1S_Pos); // CC1 as Output
	CLEAR_BIT(TIM3->CCMR1, TIM_CCMR1_OC1FE); //Fast Mode disable
	SET_BIT(TIM3->CCMR1, TIM_CCMR1_OC1PE); //Preload enable
	MODIFY_REG(TIM3->CCMR1, TIM_CCMR1_OC1M_Msk, 0b0110 << TIM_CCMR1_OC1M_Pos); //PWM Mode 1
	CLEAR_BIT(TIM3->CCMR1, TIM_CCMR1_OC1CE); //Not affected by ETRF input
	CLEAR_BIT(TIM3->CCER, TIM_CCER_CC1P); //PWM active high
	SET_BIT(TIM3->CCER, TIM_CCER_CC1E); //Enable PWM Output
}

void CMSIS_TIM3_CH1_PWM_Stop(void){
	NVIC_DisableIRQ(TIM3_IRQn);
	CLEAR_BIT(TIM3->CR1, TIM_CR1_CEN); //Disable counter
	CLEAR_BIT(TIM3->CCER, TIM_CCER_CC1E); //Disable PWM Output
}



