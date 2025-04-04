/*
 * uart.h
 *
 *  Created on: Mar 31, 2025
 *      Author: FlY DeN
 */

#pragma once

#include "main.h"
#include "stdbool.h"


void CMSIS_USART_Init(void);
bool CMSIS_USART_Transmit(USART_TypeDef *USART, uint8_t *data, uint16_t Size, uint32_t Timeout);

