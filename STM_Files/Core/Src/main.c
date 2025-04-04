/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include "LCD1602.h"
#include "DS3231.h"
#include "sht3x.h"
#include "BMPXX80.h"
#include "rcc.h"
#include "uart.h"
#include "timers.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

/* USER CODE BEGIN PV */
uint32_t SystemCoreClock = 16000000UL;

/*const uint8_t Connect_online[8] = { 0x1C, 0x1C, 0x08, 0x0C, 0x0E, 0x0F, 0x0F,
 0x0F };

 const uint8_t Connect_offline[8] = { 0x1C, 0x1C, 0x08, 0x08, 0x08, 0x0D, 0x0A,
 0x0D };
 const uint8_t Connect_insettings[8] = { 0x1C, 0x1C, 0x0A, 0x0D, 0x09, 0x0A,
 0x08, 0x0A };
 const uint8_t Connect_restarting[8] = { 0x1C, 0x1C, 0x08, 0x08, 0x08, 0x0F,
 0x0D, 0x0F };*/
const uint8_t H[8] = { 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00 };
const uint8_t L[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F };
const uint8_t LH[8] = { 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F };
const uint8_t Copyright[8] = { 0x0A, 0x11, 0x1D, 0x19, 0x19, 0x1D, 0x11, 0x0A };
bool connected = false, con_atempt = false, insettings =
false, con_rest = false, get_pres = false, get_temp = false, get_humi = false,
		get_all = false, change_dot = false, sleep = false, menu = false;

bool clear_flag = false, time_settings = false, hour_set = false, minute_set =
false, day_set = false, month_set = false, year_set = false, dow_set =
false, time_set = false, date_set = false, get_time = false;

bool led_flag = false, uart_flag = true, about = false;

char GET_COMAND[120] = { 0, }, DEB[120] = { 0, }, MES[120] = { 0, };
char *WEEK[7] = { "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY",
		"SATURDAY", "SUNDAY" };
char *WEK[7] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};
int_fast8_t menu_page = 0, main_page = 0, menu_pos = 0, time_pos = 0, date_pos =
		0, settings_pos = 0, about_pos = 0;
uint32_t change_page_time = 0, Sleep_time = 0, button_time = 0, mes_time = 0,
		backl_time = 0, led_con_time = 0, led_connected_time = 0,
		close_men_time = 0, button_settings_time = 0, display_time = 0;
int_fast8_t Hour = 0, Minute = 0, Day = 1, Month = 1, Dow = 1;
int_fast16_t Year = 2000;
int_fast8_t newmin = -1;

sht3x_handle_t handle = { .i2c_handle = &hi2c2, .device_address =
SHT3X_I2C_DEVICE_ADDRESS_ADDR_PIN_LOW };
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
/* USER CODE BEGIN PFP */
//extern void CMSIS_Clock_Config(void);
//extern void CMSIS_USART_Init(void);
//extern void CMSIS_TIM2_CH2_Init(void);
void Print_Time(void);
void Draw_Dig(uint_fast8_t x, uint_fast8_t y, uint_fast8_t dig);
void Check_Mes(void);
void Check_Con(void);
void Startup(void);
void Load_Clock_seg(void);
//void Load_Con_seg(void);
void Print_Date(void);
void Print_Meteo(void);
//void Sleep(void);
void Backlight_change(void);
void Controls(void);
void Display_menu(void);
void Menu_Controls(void);
void Display_Settings(void);
void Settings_Controls(void);
void Display_About(void);
void About_controls(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Startup(void) {
	//HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	//TIM3->CCR1 = 256;
	TIM2->CCR2 = 64;
	TIM3->CCR1 = 256;
	//CMSIS_TIM2_CH2_PWM_Start();
	CMSIS_TIM3_CH1_PWM_Start();
	DS3231_Init(&hi2c1);
	HAL_Delay(100);
	DS3231_EnableOscillator(DS3231_ENABLED);
	if (!LCD_Connect_test()) {
		NVIC_SystemReset();
	}
	LCD_Init();
	LCD_Clear();
	LCD_CreateChar((uint8_t*) Copyright, 4);
	LCD_SetCursor(0, 0);
	LCD_PrintString("Wi-Fi STMeteo by");
	LCD_SetCursor(0, 1);
	LCD_PrintString("Morshu8800 ");
	LCD_PrintMyChar(4);
	LCD_PrintString("2025");
	HAL_Delay(2000);
	LCD_Clear();
	BMP280_Init(&hi2c2, BMP280_TEMPERATURE_16BIT, BMP280_HIGHRES,
	BMP280_FORCEDMODE);
	BMP280_SetConfig( BMP280_STANDBY_MS_20, BMP280_FILTER_X4);
	if (!sht3x_init(&handle)) {
		for (uint8_t i = 0; i < 5; i++) {
			if (sht3x_init(&handle))
				break;
			else {
				HAL_Delay(100);
				continue;
			}
		}
		if (!sht3x_init(&handle)) {
			while (1)
				NVIC_SystemReset();
		}
	}
	GPIOA->BSRR = GPIO_BSRR_BS5; //HAL_GPIO_WritePin(ESP_RST_GPIO_Port, ESP_RST_Pin, 1);
	GPIOA->BSRR = GPIO_BSRR_BS6; //HAL_GPIO_WritePin(stm_check_GPIO_Port, stm_check_Pin, 1);
	//Load_Con_seg();
	Load_Clock_seg();
	//HAL_UART_Receive_IT(&huart2, (uint8_t*) GET_COMAND, 1);
	/*HAL_UARTEx_ReceiveToIdle_IT(&huart2, (uint8_t*) GET_COMAND,
	 sizeof(GET_COMAND));*/
	HAL_Delay(1500);
	if (READ_BIT(GPIOA->IDR, GPIO_IDR_ID4) == 1) {
		sprintf(MES, "CONC;");
		//memcpy(DEB, MES, sizeof(DEB));
		uart_flag = CMSIS_USART_Transmit(USART2, (uint8_t*) MES, sizeof(MES),
				100);
//		HAL_UART_Transmit(&huart2, (uint8_t*) MES, sizeof(MES), 150);
		memset(MES, 0, sizeof(MES));
	}
}
void Print_Time(void) {
	static int_fast8_t second, newsec = -1, /*dig1, dig2, dig3, dig4,*/hour,
			minute;
	second = DS3231_GetSecond();
	if (newsec != second) {
		newsec = second;
		change_dot = !change_dot;
		hour = DS3231_GetHour();
		minute = DS3231_GetMinute();
		if (newmin != minute) {
			newmin = minute;
			Draw_Dig(0, 0, hour * 0.1);
			Draw_Dig(4, 0, hour % 10);
			Draw_Dig(9, 0, minute * 0.1);
			Draw_Dig(13, 0, minute % 10);
		}

		if (change_dot) {
			LCD_SetCursor(7, 1);
			LCD_SendByte(111, 1);
			LCD_SetCursor(8, 0);
			LCD_SendByte(111, 1);
		} else if (!change_dot) {
			LCD_SetCursor(7, 1);
			LCD_PrintString(" ");
			LCD_SetCursor(8, 0);
			LCD_PrintString(" ");
		}
		/*LCD1_CreateChar(UP, 1);
		 LCD1_CreateChar(UPHI, 2);
		 LCD1_CreateChar(UPDOWN, 3);
		 LCD1_CreateChar(FULL, 4);
		 LCD1_CreateChar(DOWNHI, 5);
		 LCD1_CreateChar(DOWN, 6);

		 LCD_CreateChar((uint8_t*) F, 4);
		 LCD_CreateChar((uint8_t*) LH, 5);
		 LCD_CreateChar((uint8_t*) H, 6);
		 LCD_CreateChar((uint8_t*) L, 7);*/
	}
}
void Draw_Dig(uint_fast8_t x, uint_fast8_t y, uint_fast8_t dig) {
	switch (dig) {
	case 0:
		LCD_SetCursor(x, y);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		LCD_PrintMyChar(6);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		LCD_SetCursor(x, y + 1);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		LCD_PrintMyChar(7);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		break;
	case 1:
		LCD_SetCursor(x, y);
		LCD_PrintMyChar(6);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		LCD_PrintString(" ");
		LCD_SetCursor(x, y + 1);
		LCD_PrintMyChar(7);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		LCD_PrintMyChar(7);
		break;
	case 2:
		LCD_SetCursor(x, y);
		LCD_PrintMyChar(5);
		LCD_PrintMyChar(5);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		LCD_SetCursor(x, y + 1);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		LCD_PrintMyChar(7);
		LCD_PrintMyChar(7);
		break;
	case 3:
		LCD_SetCursor(x, y);
		LCD_PrintMyChar(6);
		LCD_PrintMyChar(5);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		LCD_SetCursor(x, y + 1);
		LCD_PrintMyChar(7);
		LCD_PrintMyChar(7);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		break;
	case 4:
		LCD_SetCursor(x, y);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		LCD_PrintMyChar(7);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		LCD_SetCursor(x, y + 1);
		LCD_PrintString(" ");
		LCD_PrintString(" ");
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		break;
	case 5:
		LCD_SetCursor(x, y);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		LCD_PrintMyChar(5);
		LCD_PrintMyChar(5);
		LCD_SetCursor(x, y + 1);
		LCD_PrintMyChar(7);
		LCD_PrintMyChar(7);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		break;
	case 6:
		LCD_SetCursor(x, y);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		LCD_PrintMyChar(5);
		LCD_PrintMyChar(5);
		LCD_SetCursor(x, y + 1);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		LCD_PrintMyChar(7);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		break;
	case 7:
		LCD_SetCursor(x, y);
		LCD_PrintMyChar(6);
		LCD_PrintMyChar(6);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		LCD_SetCursor(x, y + 1);
		LCD_PrintString(" ");
		LCD_PrintString(" ");
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		break;
	case 8:
		LCD_SetCursor(x, y);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		LCD_PrintMyChar(5);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		LCD_SetCursor(x, y + 1);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		LCD_PrintMyChar(7);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		break;
	case 9:
		LCD_SetCursor(x, y);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		LCD_PrintMyChar(5);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		LCD_SetCursor(x, y + 1);
		LCD_PrintMyChar(7);
		LCD_PrintMyChar(7);
		LCD_SendByte(255, 1); //LCD_PrintMyChar(4);
		break;
	}
}

void Check_Mes(void) {
	if (get_pres) {
		float pr;
		uint32_t pressu;
		pr = (float) BMP280_ReadPressure();
		pr = pr * 0.0075006;
		pressu = (uint32_t) pr;
		sprintf(MES, "Давление: %lu мм.Рт.Ст.;", pressu);
		//memcpy(DEB, MES, sizeof(DEB));
		uart_flag = CMSIS_USART_Transmit(USART2, (uint8_t*) MES, sizeof(MES),
				100);
		//HAL_UART_Transmit(&huart2, (uint8_t*) MES, sizeof(MES), 150);
		memset(MES, 0, sizeof(MES));
		get_pres = false;
	}
	if (get_temp) {
		float temp, tempfl, tempdec, humidity;
		int_fast8_t tempfdec, tempu;
		sht3x_read_temperature_and_humidity(&handle, &temp, &humidity);
		tempfl = modff(temp, &tempdec);
		tempfl = tempfl * 100;
		tempfdec = (int_fast8_t) tempfl;
		tempu = (int_fast8_t) tempdec;
		sprintf(MES, "Температура: %d.%d `c;", tempu, tempfdec);
		//memcpy(DEB, MES, sizeof(DEB));
		uart_flag = CMSIS_USART_Transmit(USART2, (uint8_t*) MES, sizeof(MES),
				100);
		//HAL_UART_Transmit(&huart2, (uint8_t*) MES, sizeof(MES), 150);
		memset(MES, 0, sizeof(MES));
		get_temp = false;
	}
	if (get_humi) {
		uint_fast8_t humid = 0;
		float shttemp, humidity;
		sht3x_read_temperature_and_humidity(&handle, &shttemp, &humidity);
		humid = (uint_fast8_t) humidity;
		sprintf(MES, "Влажность: %d %%;", humid);
		//memcpy(DEB, MES, sizeof(DEB));
		uart_flag = CMSIS_USART_Transmit(USART2, (uint8_t*) MES, sizeof(MES),
				100);
		//HAL_UART_Transmit(&huart2, (uint8_t*) MES, sizeof(MES), 150);
		memset(MES, 0, sizeof(MES));
		get_humi = false;
	}
	/*if (get_taro) {
	 sprintf(MES, "Rain: %d %%;", taro);
	 //memcpy(DEB, MES, sizeof(DEB));
	 HAL_UART_Transmit(&huart2, (uint8_t*) MES, sizeof(MES), 150);
	 memset(MES, 0, sizeof(MES));
	 get_taro = false;
	 }  НЕВОЗМОЖНО СОЗДАТЬ ПО ПРИЧИНЕ НЕСТАБИЛЬНОСТИ И ВЫСОКОГО ЭНЕРГОПОТРЕБЛЕНИЯ!!!*/
	if (get_all) {
		float pr;
		uint32_t pressu;
		uint_fast8_t humid = 0;
		float temp, tempfl, tempdec, humidity;
		int_fast8_t tempfdec, tempu;
		pr = (float) BMP280_ReadPressure();
		pr = pr * 0.0075006;
		pressu = (uint32_t) pr;
		sht3x_read_temperature_and_humidity(&handle, &temp, &humidity);
		tempfl = modff(temp, &tempdec);
		tempfl = tempfl * 100;
		tempfdec = (int_fast8_t) tempfl;
		tempu = (int_fast8_t) tempdec;
		humid = (uint_fast8_t) humidity;
		sprintf(MES,
				"Температура: %d.%d `c\r\nВлажность: %d %%\r\nДавление: %lu мм.Рт.Ст.;",
				tempu, tempfdec, humid, pressu);
		//memcpy(DEB, MES, sizeof(DEB));
		uart_flag = CMSIS_USART_Transmit(USART2, (uint8_t*) MES, sizeof(MES),
				100);
		//HAL_UART_Transmit(&huart2, (uint8_t*) MES, sizeof(MES), 150);
		memset(MES, 0, sizeof(MES));
		get_all = false;
	}
}
void Check_Con(void) {
	if (!connected && !insettings && !con_rest && !sleep /*&& !change_dot && main_page == 0
	 && !menu*/) {
		/*LCD_SetCursor(8, 0);
		 LCD_PrintMyChar(2);
		 LCD_SetCursor(7, 1);
		 LCD_PrintString(" ");*/

		CMSIS_TIM2_CH2_PWM_Stop();
		//LED_OFF
		//GPIOA->BSRR = GPIO_BSRR_BR1;

	} /*else if (!connected && !insettings && !con_rest && change_dot
	 && main_page == 0 && !menu) {
	 LCD_SetCursor(7, 1);
	 LCD_PrintMyChar(2);
	 LCD_SetCursor(8, 0);
	 LCD_PrintString(" ");
	 }*/
	if (insettings && !con_rest
			&& !sleep && !connected /*&& !change_dot && main_page == 0 && !menu*/) {
		/*LCD_SetCursor(8, 0);
		 LCD_PrintMyChar(1);
		 LCD_SetCursor(7, 1);
		 LCD_PrintString(" ");*/
		//insettings = false;
		if (HAL_GetTick() - led_con_time >= 1500) {
			led_con_time = HAL_GetTick();
			led_flag = !led_flag;
		}
		if (!led_flag)
			CMSIS_TIM2_CH2_PWM_Stop();
//			LED_OFF
		//GPIOA->BSRR = GPIO_BSRR_BR1;
		else if (led_flag)
			CMSIS_TIM2_CH2_PWM_Start();
//			LED_ON
		//GPIOA->BSRR = GPIO_BSRR_BS1;

	} /*else if (insettings && !con_rest && change_dot && main_page == 0
	 && !menu) {
	 LCD_SetCursor(7, 1);
	 LCD_PrintMyChar(1);
	 LCD_SetCursor(8, 0);
	 LCD_PrintString(" ");
	 //insettings = false;
	 }*/
	if (connected && !con_rest && !insettings && !sleep /*&& !change_dot && main_page == 0
	 && !menu*/) {
		/*LCD_SetCursor(8, 0);
		 LCD_PrintMyChar(0);
		 LCD_SetCursor(7, 1);
		 LCD_PrintString(" ");*/

		if (HAL_GetTick() - led_connected_time >= 500) {
			led_connected_time = HAL_GetTick();
			led_flag = !led_flag;
		}
		if (!led_flag)
			CMSIS_TIM2_CH2_PWM_Stop();
//			LED_OFF
		//GPIOA->BSRR = GPIO_BSRR_BR1;
		else if (led_flag)
			CMSIS_TIM2_CH2_PWM_Start();
//			LED_ON
		//GPIOA->BSRR = GPIO_BSRR_BS1;

		//connected = false;
	} /*else if (connected && !con_rest && !insettings && change_dot
	 && main_page == 0 && !menu) {
	 LCD_SetCursor(7, 1);
	 LCD_PrintMyChar(0);
	 LCD_SetCursor(8, 0);
	 LCD_PrintString(" ");
	 //connected = false;
	 }*/
	if (con_rest && !sleep && !connected /*&& !change_dot && main_page == 0 && !menu*/) {
		CMSIS_TIM2_CH2_PWM_Start();
		//LED_ON
		//GPIOA->BSRR = GPIO_BSRR_BS1;

		/*LCD_SetCursor(8, 0);
		 LCD_PrintMyChar(3);
		 LCD_SetCursor(7, 1);
		 LCD_PrintString(" ");*/
	} /*else if (con_rest && change_dot && main_page == 0 && !menu) {
	 LCD_SetCursor(7, 1);
	 LCD_PrintMyChar(3);
	 LCD_SetCursor(8, 0);
	 LCD_PrintString(" ");
	 }*/
	if (READ_BIT(GPIOA->IDR, GPIO_IDR_ID4) == 0) {
		connected = false;
		insettings = false;
	}
	/*if ((!connected && !insettings && !con_rest && main_page == 1 && !menu)
	 || (!connected && !insettings && !con_rest && main_page == 2
	 && !menu)) {
	 LCD_SetCursor(0, 0);
	 LCD_PrintMyChar(2);
	 }
	 if ((insettings && !con_rest && main_page == 1 && !menu)
	 || (insettings && !con_rest && main_page == 2 && !menu)) {
	 LCD_SetCursor(0, 0);
	 LCD_PrintMyChar(1);
	 }
	 if ((connected && !con_rest && !insettings && main_page == 1 && !menu)
	 || (connected && !con_rest && !insettings && main_page == 2 && !menu)) {
	 LCD_SetCursor(0, 0);
	 LCD_PrintMyChar(0);
	 }
	 if ((con_rest && main_page == 1 && !menu)
	 || (con_rest && main_page == 2 && !menu)) {
	 LCD_SetCursor(0, 0);
	 LCD_PrintMyChar(3);
	 }*/
	if (sleep) {
		CMSIS_TIM2_CH2_PWM_Stop();
		//LED_OFF
		//GPIOA->BSRR = GPIO_BSRR_BR1;
	}
}
void Load_Clock_seg(void) {
	LCD_CreateChar((uint8_t*) LH, 5);
	LCD_CreateChar((uint8_t*) H, 6);
	LCD_CreateChar((uint8_t*) L, 7);
}
/*void Load_Con_seg(void) {
 LCD_CreateChar((uint8_t*) Connect_online, 0);
 LCD_CreateChar((uint8_t*) Connect_insettings, 1);
 LCD_CreateChar((uint8_t*) Connect_offline, 2);
 LCD_CreateChar((uint8_t*) Connect_restarting, 3);
 }*/
void Print_Date(void) {
	static int_fast8_t day, week, month;
	int_fast8_t newday = -1;
	static uint16_t year;
	static char DATE[20];
	day = DS3231_GetDate();
	if (newday != day) {
		newday = day;
		month = DS3231_GetMonth();
		year = DS3231_GetYear();
		week = DS3231_GetDayOfWeek();
		sprintf(DATE, "%02d/%02d/%04u", day, month, year);
		LCD_SetCursor(2, 0);
		LCD_PrintString(DATE);
		switch (week) {
		case 1:
			LCD_SetCursor(2, 1);
			LCD_PrintString(WEEK[0]);
			break;
		case 2:
			LCD_SetCursor(2, 1);
			LCD_PrintString(WEEK[1]);
			break;
		case 3:
			LCD_SetCursor(2, 1);
			LCD_PrintString(WEEK[2]);
			break;
		case 4:
			LCD_SetCursor(2, 1);
			LCD_PrintString(WEEK[3]);
			break;
		case 5:
			LCD_SetCursor(2, 1);
			LCD_PrintString(WEEK[4]);
			break;
		case 6:
			LCD_SetCursor(2, 1);
			LCD_PrintString(WEEK[5]);
			break;
		case 7:
			LCD_SetCursor(2, 1);
			LCD_PrintString(WEEK[6]);
			break;
		}
	}
}
void Print_Meteo(void) {
	static float pr;
	static char TEMP[10], HUMI[10], PRES[10];
	static uint32_t pressu;
	static uint_fast8_t humid = 0;
	static float temp, tempfl, tempdec, humidity;
	static int_fast8_t tempfdec, tempu;
	if (HAL_GetTick() - mes_time >= 500) {
		mes_time = HAL_GetTick();
		pr = (float) BMP280_ReadPressure();
		pr = pr * 0.0075006;
		pressu = (uint32_t) pr;
		sht3x_read_temperature_and_humidity(&handle, &temp, &humidity);
		tempfl = modff(temp, &tempdec);
		tempfl = tempfl * 100;
		tempfdec = (int_fast8_t) tempfl;
		tempu = (int_fast8_t) tempdec;
		humid = (uint_fast8_t) humidity;
		sprintf(TEMP, "%02d.%02d", tempu, tempfdec);
		sprintf(HUMI, "H:%02d%%", humid);
		sprintf(PRES, "%lu mmHg", pressu);
		LCD_SetCursor(2, 0);
		LCD_PrintString(TEMP);
		LCD_SendByte(223, 1);
		LCD_PrintString("C");
		LCD_SetCursor(10, 0);
		LCD_PrintString(HUMI);
		LCD_SetCursor(2, 1);
		LCD_PrintString(PRES);
	}

}

//Функция оказалась ненужной.
/*void Sleep(void) {
 HAL_GPIO_WritePin(LCD_GND_GPIO_Port, LCD_GND_Pin, 0);
 HAL_GPIO_WritePin(stm_check_GPIO_Port, stm_check_Pin, 0);
 HAL_Delay(100);
 GPIO_InitTypeDef GPIO_InitStruct = { 0 };
 Configure GPIO pin : stm_check_Pin
 GPIO_InitStruct.Pin = stm_check_Pin;
 GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
 GPIO_InitStruct.Pull = GPIO_PULLDOWN;
 //GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
 HAL_GPIO_Init(stm_check_GPIO_Port, &GPIO_InitStruct);

 Configure GPIO pin : LCD_GND_Pin
 GPIO_InitStruct.Pin = LCD_GND_Pin;
 GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
 GPIO_InitStruct.Pull = GPIO_PULLDOWN;
 //GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
 HAL_GPIO_Init(LCD_GND_GPIO_Port, &GPIO_InitStruct);
 HAL_Delay(100);
 HAL_UART_AbortReceive_IT(&huart2);
 HAL_UART_AbortTransmit_IT(&huart2);
 HAL_Delay(100);

 GPIO_InitStruct.Pin = GPIO_PIN_2;
 GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
 GPIO_InitStruct.Pull = GPIO_PULLDOWN;
 HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

 GPIO_InitStruct.Pin = GPIO_PIN_3;
 GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
 GPIO_InitStruct.Pull = GPIO_PULLDOWN;
 HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

 HAL_SuspendTick();
 HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
 CMSIS_Clock_Config();
 HAL_ResumeTick();

 MX_GPIO_Init();
 //MX_USART2_UART_Init();
 HAL_Init();
 GPIO_InitStruct.Pin = GPIO_PIN_2;
 GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
 HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

 GPIO_InitStruct.Pin = GPIO_PIN_3;
 GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
 HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

 Sleep_time = HAL_GetTick();
 change_page_time = HAL_GetTick();
 main_page = 0;
 HAL_Delay(100);
 HAL_GPIO_WritePin(LCD_GND_GPIO_Port, LCD_GND_Pin, 1);
 HAL_GPIO_WritePin(stm_check_GPIO_Port, stm_check_Pin, 1);
 HAL_Delay(100);
 if (!LCD_Connect_test()) {
 HAL_Delay(1000);
 NVIC_SystemReset();
 }
 LCD_Init();
 LCD_Clear();
 Load_Clock_seg();
 Load_Con_seg();
 if (HAL_GPIO_ReadPin(esp_check_GPIO_Port, esp_check_Pin) == 1) {
 sprintf(MES, "CONC;");
 //memcpy(DEB, MES, sizeof(DEB));
 HAL_UART_Transmit(&huart2, (uint8_t*) MES, sizeof(MES), 150);
 memset(MES, 0, sizeof(MES));
 }
 HAL_UARTEx_ReceiveToIdle_IT(&huart2, (uint8_t*) GET_COMAND,
 sizeof(GET_COMAND));
 }*/

void Backlight_change(void) {
	if (HAL_GetTick() - backl_time > 60000) {
		sleep = true;
		CMSIS_TIM3_CH1_PWM_Stop();
		//HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
	}
	if (READ_BIT(GPIOA->IDR, GPIO_IDR_ID0) == 0 && sleep == true
			&& HAL_GetTick() - button_time > 150) {
		sleep = false;
		CMSIS_TIM3_CH1_PWM_Start();
		//HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
		backl_time = HAL_GetTick();
		button_time = HAL_GetTick();
		HAL_Delay(100);
	}
}
void Controls(void) {
	if (READ_BIT(GPIOA->IDR, GPIO_IDR_ID0) == 0 && sleep == false
			&& HAL_GetTick() - button_time > 150) {
		button_time = HAL_GetTick();
		backl_time = HAL_GetTick();
		change_page_time = HAL_GetTick();
		close_men_time = HAL_GetTick();
		menu = true;
		newmin = -1;
		//clear_flag = true;
		HAL_Delay(50);
		LCD_Clear();
	}
}

void Display_menu(void) {
	switch (menu_pos) {
	case 0:
		LCD_SetCursor(0, 0);
		LCD_SendByte(126, 1);
		LCD_SetCursor(0, 1);
		LCD_PrintString(" ");
		LCD_SetCursor(1, 0);
		LCD_PrintString("Time Settings");
		LCD_SetCursor(1, 1);
		LCD_PrintString("Restart MCU  ");
		break;
	case 1:
		LCD_SetCursor(0, 1);
		LCD_SendByte(126, 1);
		LCD_SetCursor(0, 0);
		LCD_PrintString(" ");
		LCD_SetCursor(1, 0);
		LCD_PrintString("Time Settings");
		LCD_SetCursor(1, 1);
		LCD_PrintString("Restart MCU  ");
		break;
	case 2:
		LCD_SetCursor(0, 1);
		LCD_SendByte(126, 1);
		LCD_SetCursor(0, 0);
		LCD_PrintString(" ");
		LCD_SetCursor(1, 0);
		LCD_PrintString("Restart MCU  ");
		LCD_SetCursor(1, 1);
		LCD_PrintString("Factory Reset");
		break;
	case 3:
		LCD_SetCursor(0, 1);
		LCD_SendByte(126, 1);
		LCD_SetCursor(0, 0);
		LCD_PrintString(" ");
		LCD_SetCursor(1, 0);
		LCD_PrintString("Factory Reset");
		LCD_SetCursor(1, 1);
		LCD_PrintString("About        ");
		break;
	case 4:
		LCD_SetCursor(0, 1);
		LCD_SendByte(126, 1);
		LCD_SetCursor(0, 0);
		LCD_PrintString(" ");
		LCD_SetCursor(1, 0);
		LCD_PrintString("About        ");
		LCD_SetCursor(1, 1);
		LCD_PrintString("Exit menu    ");
		break;
	}
}

void Menu_Controls(void) {
	if (READ_BIT(GPIOA->IDR, GPIO_IDR_ID7) == 0 && sleep == false
			&& HAL_GetTick() - button_time > 100) {
		button_time = HAL_GetTick();
		close_men_time = HAL_GetTick();
		menu_pos++;

	}
	if (READ_BIT(GPIOB->IDR, GPIO_IDR_ID0) == 0 && sleep == false
			&& HAL_GetTick() - button_time > 100) {
		button_time = HAL_GetTick();
		close_men_time = HAL_GetTick();
		menu_pos--;

	}

	if (READ_BIT(GPIOA->IDR, GPIO_IDR_ID0) == 0 && sleep == false
			&& HAL_GetTick() - button_time > 100) {
		button_time = HAL_GetTick();
		close_men_time = HAL_GetTick();
		switch (menu_pos) {
		case 0:
			time_settings = true;
			get_time = true;
			LCD_Clear();
			break;
		case 1:
			NVIC_SystemReset();
			break;
		case 2:
			DS3231_SetFullTime(22, 0, 0);
			DS3231_SetFullDate(25, 3, 2, 2025);
			DS3231_EnableOscillator(DS3231_ENABLED);
			NVIC_SystemReset();
			break;
		case 3:
			about = true;
			menu_pos = 0;
			HAL_Delay(50);
			LCD_Clear();
			break;
		case 4:
			backl_time = HAL_GetTick();
			change_page_time = HAL_GetTick();
			menu = false;
			//clear_flag = true;
			menu_pos = 0;
			HAL_Delay(50);
			LCD_Clear();
			break;
		}
	}
	if (menu_pos > 4)
		menu_pos = 4;
	else if (menu_pos < 0)
		menu_pos = 0;
}

void Display_Settings(void) {
	static char TIME1[5], TIME2[5], DATE1[5], DATE2[5], DATE3[9];
	if (get_time) {
		Hour = DS3231_GetHour();
		Minute = DS3231_GetMinute();
		Day = DS3231_GetDate();
		Month = DS3231_GetMonth();
		Year = DS3231_GetYear();
		Dow = DS3231_GetDayOfWeek();
		get_time = false;
	}
//	if (HAL_GetTick() - display_time > 10) {
//		display_time = HAL_GetTick();
	sprintf(TIME1, "%02d", Hour);
	sprintf(TIME2, "%02d", Minute);
	sprintf(DATE1, "%02d", Day);
	sprintf(DATE2, "%02d", Month);
	sprintf(DATE3, "%04d", Year);
	LCD_SetCursor(1, 0);
	LCD_PrintString(TIME1);
	LCD_SetCursor(4, 0);
	LCD_PrintString(TIME2);
	LCD_SetCursor(9, 0);
	LCD_PrintString("Ap");
	LCD_SetCursor(13, 0);
	LCD_PrintString("Ex");
	LCD_SetCursor(1, 1);
	LCD_PrintString(DATE1);
	LCD_SetCursor(4, 1);
	LCD_PrintString(DATE2);
	LCD_SetCursor(7, 1);
	LCD_PrintString(DATE3);
	LCD_SetCursor(12, 1);
	switch(Dow){
	case 1:
		LCD_PrintString(WEK[0]);
		break;
	case 2:
		LCD_PrintString(WEK[1]);
		break;
	case 3:
		LCD_PrintString(WEK[2]);
		break;
	case 4:
		LCD_PrintString(WEK[3]);
		break;
	case 5:
		LCD_PrintString(WEK[4]);
		break;
	case 6:
		LCD_PrintString(WEK[5]);
			break;
	case 7:
		LCD_PrintString(WEK[6]);
			break;
	}

	switch (time_pos) {
	case 0:
		LCD_SetCursor(3, 0);
		LCD_PrintString(":");
		break;
	case 1:
		LCD_SetCursor(6, 0);
		LCD_PrintString(" ");
		if (!hour_set) {
			LCD_SetCursor(3, 0);
			LCD_SendByte(127, 1);
		} else if (hour_set) {
			LCD_SetCursor(3, 0);
			LCD_SendByte(63, 1);
		}
		break;
	case 2:
		LCD_SetCursor(3, 0);
		LCD_PrintString(":");
		if (!minute_set) {
			LCD_SetCursor(6, 0);
			LCD_SendByte(127, 1);
		} else if (minute_set) {
			LCD_SetCursor(6, 0);
			LCD_SendByte(63, 1);
		}
		break;
	}

	switch (settings_pos) {
	case 0:
		if (!time_set) {
			LCD_SetCursor(0, 0);
			LCD_SendByte(126, 1);
		} else if (time_set) {
			LCD_SetCursor(0, 0);
			LCD_SendByte(63, 1);
		}
		LCD_SetCursor(0, 1);
		LCD_PrintString(" ");
		LCD_SetCursor(8, 0);
		LCD_PrintString(" ");
		LCD_SetCursor(12, 0);
		LCD_PrintString(" ");
		break;
	case 1:
		if (!date_set) {
			LCD_SetCursor(0, 1);
			LCD_SendByte(126, 1);
		} else if (date_set) {
			LCD_SetCursor(0, 1);
			LCD_SendByte(63, 1);
		}
		LCD_SetCursor(0, 0);
		LCD_PrintString(" ");
		LCD_SetCursor(8, 0);
		LCD_PrintString(" ");
		LCD_SetCursor(12, 0);
		LCD_PrintString(" ");
		break;
	case 2:
		LCD_SetCursor(0, 0);
		LCD_PrintString(" ");
		LCD_SetCursor(0, 1);
		LCD_PrintString(" ");
		LCD_SetCursor(8, 0);
		LCD_SendByte(126, 1);
		LCD_SetCursor(12, 0);
		LCD_PrintString(" ");
		break;
	case 3:
		LCD_SetCursor(0, 0);
		LCD_PrintString(" ");
		LCD_SetCursor(0, 1);
		LCD_PrintString(" ");
		LCD_SetCursor(8, 0);
		LCD_PrintString(" ");
		LCD_SetCursor(12, 0);
		LCD_SendByte(126, 1);
		break;
	}

	switch (date_pos) {
	case 0:
		LCD_SetCursor(3, 1);
		LCD_PrintString("/");
		LCD_SetCursor(6, 1);
		LCD_PrintString("/");
		LCD_SetCursor(11, 1);
		LCD_PrintString(",");
		LCD_SetCursor(15, 1);
		LCD_PrintString(" ");
		break;
	case 1:
		if (!day_set) {
			LCD_SetCursor(3, 1);
			LCD_SendByte(127, 1);
		} else if (day_set) {
			LCD_SetCursor(3, 1);
			LCD_SendByte(63, 1);
		}
		LCD_SetCursor(6, 1);
		LCD_PrintString("/");
		LCD_SetCursor(11, 1);
		LCD_PrintString(",");
		LCD_SetCursor(15, 1);
		LCD_PrintString(" ");
		break;
	case 2:
		LCD_SetCursor(3, 1);
		LCD_PrintString("/");
		if (!month_set) {
			LCD_SetCursor(6, 1);
			LCD_SendByte(127, 1);
		} else if (month_set) {
			LCD_SetCursor(6, 1);
			LCD_SendByte(63, 1);
		}
		LCD_SetCursor(11, 1);
		LCD_PrintString(",");
		LCD_SetCursor(15, 1);
		LCD_PrintString(" ");
		break;
	case 3:
		LCD_SetCursor(3, 1);
		LCD_PrintString("/");
		LCD_SetCursor(6, 1);
		LCD_PrintString("/");
		if (!year_set) {
			LCD_SetCursor(11, 1);
			LCD_SendByte(127, 1);
		} else if (year_set) {
			LCD_SetCursor(11, 1);
			LCD_SendByte(63, 1);
		}
		LCD_SetCursor(15, 1);
		LCD_PrintString(" ");
		break;
	case 4:
		LCD_SetCursor(3, 1);
		LCD_PrintString("/");
		LCD_SetCursor(6, 1);
		LCD_PrintString("/");
		LCD_SetCursor(11, 1);
		LCD_PrintString(",");
		if (!dow_set) {
			LCD_SetCursor(15, 1);
			LCD_SendByte(127, 1);
		} else if (dow_set) {
			LCD_SetCursor(15, 1);
			LCD_SendByte(63, 1);
		}
		break;
	}
//	}
}

void Settings_Controls(void) {
	if (READ_BIT(GPIOA->IDR, GPIO_IDR_ID7) == 0
			&& HAL_GetTick() - button_settings_time > 150 && !time_set
			&& !date_set) {
		button_settings_time = HAL_GetTick();
		close_men_time = HAL_GetTick();
		settings_pos++;

	}
	if (READ_BIT(GPIOB->IDR, GPIO_IDR_ID0) == 0
			&& HAL_GetTick() - button_settings_time > 150 && !time_set
			&& !date_set) {
		button_settings_time = HAL_GetTick();
		close_men_time = HAL_GetTick();
		settings_pos--;

	}
	if (READ_BIT(GPIOA->IDR, GPIO_IDR_ID0) == 0
			&& HAL_GetTick() - button_settings_time > 150) {
		button_settings_time = HAL_GetTick();
		close_men_time = HAL_GetTick();
		switch (settings_pos) {
		case 0:
			if (time_pos == 0)
				time_set = !time_set;
			HAL_Delay(50);
			break;
		case 1:
			if (date_pos == 0)
				date_set = !date_set;
			HAL_Delay(50);
			break;
		case 2:
			DS3231_SetFullTime(Hour, Minute, 0);
			DS3231_SetFullDate(Day, Month, Dow, Year);
			DS3231_EnableOscillator(DS3231_ENABLED);
			HAL_Delay(50);
			LCD_Clear();
			break;
		case 3:
			time_settings = false;
			settings_pos = 0;
			time_pos = 0;
			date_pos = 0;
			HAL_Delay(50);
			LCD_Clear();
			break;
		}
		switch (time_pos) {
		case 1:
			if (time_set && !date_set)
				hour_set = !hour_set;
			break;
		case 2:
			if (time_set && !date_set)
				minute_set = !minute_set;
			break;
		}
		switch (date_pos) {
		case 1:
			if (date_set && !time_set)
				day_set = !day_set;
			break;
		case 2:
			if (date_set && !time_set)
				month_set = !month_set;
			break;
		case 3:
			if (date_set && !time_set)
				year_set = !year_set;
			break;
		case 4:
			if (date_set && !time_set)
				dow_set = !dow_set;
			break;
		}
	}
	if (READ_BIT(GPIOA->IDR, GPIO_IDR_ID7) == 0
			&& HAL_GetTick() - button_settings_time > 75) {
		button_settings_time = HAL_GetTick();
		close_men_time = HAL_GetTick();
		if (time_set && !hour_set && !minute_set)
			time_pos++;
		if (date_set && !day_set && !month_set && !year_set && !dow_set)
			date_pos++;
		if (hour_set && !minute_set)
			Hour++;
		if (!hour_set && minute_set)
			Minute++;
		if (day_set && !month_set && !year_set && !dow_set)
			Day++;
		if (!day_set && month_set && !year_set && !dow_set)
			Month++;
		if (!day_set && !month_set && year_set && !dow_set)
			Year++;
		if (!day_set && !month_set && !year_set && dow_set)
			Dow++;
	}

	if (READ_BIT(GPIOB->IDR, GPIO_IDR_ID0) == 0
			&& HAL_GetTick() - button_settings_time > 75) {
		button_settings_time = HAL_GetTick();
		close_men_time = HAL_GetTick();
		if (time_set && !hour_set && !minute_set)
			time_pos--;
		if (date_set && !day_set && !month_set && !year_set && !dow_set)
			date_pos--;
		if (hour_set && !minute_set)
			Hour--;
		if (!hour_set && minute_set)
			Minute--;
		if (day_set && !month_set && !year_set && !dow_set)
			Day--;
		if (!day_set && month_set && !year_set && !dow_set)
			Month--;
		if (!day_set && !month_set && year_set && !dow_set)
			Year--;
		if (!day_set && !month_set && !year_set && dow_set)
			Dow--;
	}

	if (Dow < 1)
		Dow = 7;
	else if (Dow > 7)
		Dow = 1;
	if (Year < 2000)
		Year = 2999;
	else if (Year > 2999)
		Year = 2000;
	if (Month < 1)
		Month = 12;
	else if (Month > 12)
		Month = 1;
	if (Minute < 0)
		Minute = 59;
	else if (Minute > 59)
		Minute = 0;
	if (Hour < 0)
		Hour = 23;
	else if (Hour > 23)
		Hour = 0;
	if (date_pos < 0)
		date_pos = 0;
	else if (date_pos > 4)
		date_pos = 4;
	if (time_pos < 0)
		time_pos = 0;
	else if (time_pos > 2)
		time_pos = 2;
	if (settings_pos < 0)
		settings_pos = 0;
	else if (settings_pos > 3)
		settings_pos = 3;

	if (Year % 4) {
		switch (Month) {
		case 1:
			if (Day > 31)
				Day = 1;
			else if (Day < 1)
				Day = 31;
			break;
		case 2:
			if (Day > 28)
				Day = 1;
			else if (Day < 1)
				Day = 28;
			break;
		case 3:
			if (Day > 31)
				Day = 1;
			else if (Day < 1)
				Day = 31;
			break;
		case 4:
			if (Day > 30)
				Day = 1;
			else if (Day < 1)
				Day = 30;
			break;
		case 5:
			if (Day > 31)
				Day = 1;
			else if (Day < 1)
				Day = 31;
			break;
		case 6:
			if (Day > 30)
				Day = 1;
			else if (Day < 1)
				Day = 30;
			break;
		case 7:
			if (Day > 31)
				Day = 1;
			else if (Day < 1)
				Day = 31;
			break;
		case 8:
			if (Day > 31)
				Day = 1;
			else if (Day < 1)
				Day = 31;
			break;
		case 9:
			if (Day > 30)
				Day = 1;
			else if (Day < 1)
				Day = 30;
			break;
		case 10:
			if (Day > 31)
				Day = 1;
			else if (Day < 1)
				Day = 31;
			break;
		case 11:
			if (Day > 30)
				Day = 1;
			else if (Day < 1)
				Day = 30;
			break;
		case 12:
			if (Day > 31)
				Day = 1;
			else if (Day < 1)
				Day = 31;
			break;
		}
	}

	else {
		switch (Month) {
		case 1:
			if (Day > 31)
				Day = 1;
			else if (Day < 1)
				Day = 31;
			break;
		case 2:
			if (Day > 29)
				Day = 1;
			else if (Day < 1)
				Day = 29;
			break;
		case 3:
			if (Day > 31)
				Day = 1;
			else if (Day < 1)
				Day = 31;
			break;
		case 4:
			if (Day > 30)
				Day = 1;
			else if (Day < 1)
				Day = 30;
			break;
		case 5:
			if (Day > 31)
				Day = 1;
			else if (Day < 1)
				Day = 31;
			break;
		case 6:
			if (Day > 30)
				Day = 1;
			else if (Day < 1)
				Day = 30;
			break;
		case 7:
			if (Day > 31)
				Day = 1;
			else if (Day < 1)
				Day = 31;
			break;
		case 8:
			if (Day > 31)
				Day = 1;
			else if (Day < 1)
				Day = 31;
			break;
		case 9:
			if (Day > 30)
				Day = 1;
			else if (Day < 1)
				Day = 30;
			break;
		case 10:
			if (Day > 31)
				Day = 1;
			else if (Day < 1)
				Day = 31;
			break;
		case 11:
			if (Day > 30)
				Day = 1;
			else if (Day < 1)
				Day = 30;
			break;
		case 12:
			if (Day > 31)
				Day = 1;
			else if (Day < 1)
				Day = 31;
			break;
		}
	}
}

void Display_About(void) {
	switch (about_pos) {
	case 0:
		LCD_SetCursor(0, 0);
		LCD_PrintString("Wi-Fi STMeteo by");
		LCD_SetCursor(0, 1);
		LCD_PrintString("Morshu8800 ");
		LCD_PrintMyChar(4);
		LCD_PrintString("2025");
		break;
	case 1:
		LCD_SetCursor(0, 0);
		LCD_PrintString("Version 1.0     ");
		LCD_SetCursor(0, 1);
		LCD_PrintString("Library authors:");
		break;
	case 2:
		LCD_SetCursor(0, 0);
		LCD_PrintString("ST Electronics  ");
		LCD_SetCursor(0, 1);
		LCD_PrintString("K. Golinskiy    ");
		break;
	case 3:
		LCD_SetCursor(0, 0);
		LCD_PrintString("Ilya Prikhodko  ");
		LCD_SetCursor(0, 1);
		LCD_PrintString("Arseniy. Borisov");
		break;
	case 4:
		LCD_SetCursor(0, 0);
		LCD_PrintString("Pan Eepj        ");
		LCD_SetCursor(0, 1);
		LCD_PrintString("Henri Heimann   ");
		break;
	case 5:
		LCD_SetCursor(0, 0);
		LCD_PrintString("https://github  ");
		LCD_SetCursor(0, 1);
		LCD_PrintString(".com/Morshu8800 ");
		break;
	}

}

void About_controls(void) {
	if (READ_BIT(GPIOA->IDR, GPIO_IDR_ID7) == 0 && sleep == false
			&& HAL_GetTick() - button_time > 100) {
		button_time = HAL_GetTick();
		close_men_time = HAL_GetTick();
		about_pos++;

	}
	if (READ_BIT(GPIOB->IDR, GPIO_IDR_ID0) == 0 && sleep == false
			&& HAL_GetTick() - button_time > 100) {
		button_time = HAL_GetTick();
		close_men_time = HAL_GetTick();
		about_pos--;

	}

	if (READ_BIT(GPIOA->IDR, GPIO_IDR_ID0) == 0 && sleep == false
			&& HAL_GetTick() - button_time > 100) {
		button_time = HAL_GetTick();
		close_men_time = HAL_GetTick();
		about = false;
		about_pos = 0;
		HAL_Delay(50);
		LCD_Clear();

	}
	if (about_pos > 5)
		about_pos = 5;
	else if (about_pos < 0)
		about_pos = 0;
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* USER CODE BEGIN SysInit */
	CMSIS_Clock_Config();
	//SystemClock_Config();
	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_I2C2_Init();
	/* USER CODE BEGIN 2 */
	CMSIS_USART_Init();
	CMSIS_TIM2_CH2_Init();
	CMSIS_TIM3_CH1_Init();
	//NVIC_EnableIRQ(USART2_IRQn);//enable USART2 IRQ
	Startup();
	change_page_time = Sleep_time = button_time = led_con_time =
			led_connected_time = HAL_GetTick();
	//HAL_UART_Transmit(&huart2, (uint8_t*)CON_MESS, strlen(CON_MESS), 10);
	//__HAL_UART_CLEAR_IDLEFLAG(&huart2);
	//DS3231_SetHour(19);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		// while (HAL_GetTick() - Sleep_time <= 35000) {
		while (!menu) {
			/*if (clear_flag) {
			 LCD_Clear();
			 clear_flag = false;
			 }*/
//			HAL_UARTEx_ReceiveToIdle_IT(&huart2, (uint8_t*) GET_COMAND,
//					sizeof(GET_COMAND));
			Check_Mes();
			Check_Con();
			Backlight_change();
			if (HAL_GetTick() - change_page_time >= 10000) {
				change_page_time = HAL_GetTick();
				main_page++;
				newmin = -1;
				if (main_page > 2) {
					main_page = 0;
				}
				LCD_Clear();
			}
			switch (main_page) {
			case 0:
				Print_Time();
				break;
			case 1:
				Print_Date();
				break;
			case 2:
				Print_Meteo();
				break;
			}
			Controls();
		}
		while (menu) {
			//GPIOA->BSRR = GPIO_BSRR_BR1;
			/*if (clear_flag) {
			 LCD_Clear();
			 clear_flag = false;
			 }*/
			if (menu && !time_settings && !about) {
				Display_menu();
				Menu_Controls();
				if (HAL_GetTick() - close_men_time >= 60000) {
					close_men_time = HAL_GetTick();
					backl_time = HAL_GetTick();
					change_page_time = HAL_GetTick();
					menu = false;
					menu_pos = 0;
					//HAL_Delay(50);
					LCD_Clear();
				}
			}
			if (menu && time_settings && !about) {
				Display_Settings();
				Settings_Controls();
				if (HAL_GetTick() - close_men_time >= 60000) {
					close_men_time = HAL_GetTick();
					time_settings = false;
					settings_pos = 0;
					time_pos = 0;
					date_pos = 0;
					date_set = time_set = hour_set = minute_set = day_set =
							month_set = year_set = dow_set = false;
					HAL_Delay(50);
					LCD_Clear();
				}
			}
			if (menu && !time_settings && about) {
				Display_About();
				About_controls();
				if (HAL_GetTick() - close_men_time >= 60000) {
					close_men_time = HAL_GetTick();
					about = false;
					about_pos = 0;
					HAL_Delay(50);
					LCD_Clear();
				}
			}
			/*HAL_UARTEx_ReceiveToIdle_IT(&huart2, (uint8_t*) GET_COMAND,
			 sizeof(GET_COMAND));*/
			Check_Mes();
			Check_Con();

		}

		//		}
		/*LCD_Clear();
		 Sleep();*/
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	return 0;
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x00503D58;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}

	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE)
			!= HAL_OK) {
		Error_Handler();
	}

	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief I2C2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C2_Init(void) {

	/* USER CODE BEGIN I2C2_Init 0 */

	/* USER CODE END I2C2_Init 0 */

	/* USER CODE BEGIN I2C2_Init 1 */

	/* USER CODE END I2C2_Init 1 */
	hi2c2.Instance = I2C2;
	hi2c2.Init.Timing = 0x00503D58;
	hi2c2.Init.OwnAddress1 = 0;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c2) != HAL_OK) {
		Error_Handler();
	}

	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE)
			!= HAL_OK) {
		Error_Handler();
	}

	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C2_Init 2 */

	/* USER CODE END I2C2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */

	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, ESP_RST_Pin | STM_Check_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : WAKE_OK_Pin DOWN_Pin */
	GPIO_InitStruct.Pin = WAKE_OK_Pin | DOWN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : ESP_Check_Pin */
	GPIO_InitStruct.Pin = ESP_Check_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(ESP_Check_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : ESP_RST_Pin */
	GPIO_InitStruct.Pin = ESP_RST_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(ESP_RST_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : STM_Check_Pin */
	GPIO_InitStruct.Pin = STM_Check_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(STM_Check_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : UP_Pin */
	GPIO_InitStruct.Pin = UP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(UP_GPIO_Port, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */

	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/*void HAL_UART_RxCplitCallback(UART_HandleTypeDef *huart) {
 //memcpy(DEB, GET_COMAND, sizeof(DEB));
 if (huart->Instance == USART2){
 if (strcmp(GET_COMAND, "ST\r\n") == 0) {
 con_atempt = true;
 con_rest = false;
 connected = false;
 insettings = false;
 memset(GET_COMAND, 0, sizeof(GET_COMAND));
 }
 if (strcmp(GET_COMAND, "AP\r\n") == 0) {
 insettings = true;
 connected = false;
 con_rest = false;
 memset(GET_COMAND, 0, sizeof(GET_COMAND));
 }
 if (strcmp(GET_COMAND, "CP\r\n") == 0) {
 connected = true;
 insettings = false;
 con_rest = false;
 memset(GET_COMAND, 0, sizeof(GET_COMAND));
 }
 if (strcmp(GET_COMAND, "RS\r\n") == 0) {
 connected = false;
 insettings = false;
 con_rest = true;
 memset(GET_COMAND, 0, sizeof(GET_COMAND));
 }
 if (strcmp(GET_COMAND, "CL\r\n") == 0) {
 connected = false;
 insettings = false;
 con_rest = false;
 memset(GET_COMAND, 0, sizeof(GET_COMAND));
 }
 if (strcmp(GET_COMAND, "GT\r\n") == 0) {
 get_temp = true;
 memset(GET_COMAND, 0, sizeof(GET_COMAND));
 }
 if (strcmp(GET_COMAND, "GH\r\n") == 0) {
 get_humi = true;
 memset(GET_COMAND, 0, sizeof(GET_COMAND));
 }
 if (strcmp(GET_COMAND, "GTARO\r\n") == 0) {
 get_taro = true;
 memset(GET_COMAND, 0, sizeof(GET_COMAND));
 }  НЕВОЗМОЖНО СОЗДАТЬ ПО ПРИЧИНЕ НЕСТАБИЛЬНОСТИ И ВЫСОКОГО ЭНЕРГОПОТРЕБЛЕНИЯ!!!
 if (strcmp(GET_COMAND, "GA\r\n") == 0) {
 get_all = true;
 memset(GET_COMAND, 0, sizeof(GET_COMAND));
 }
 if (strcmp(GET_COMAND, "GP\r\n") == 0) {
 get_pres = true;
 memset(GET_COMAND, 0, sizeof(GET_COMAND));
 } else {
 memset(GET_COMAND, 0, sizeof(GET_COMAND));
 }
 }
 //HAL_UART_Receive_IT(&huart2, (uint8_t*) GET_COMAND, 1);
 }*/
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
