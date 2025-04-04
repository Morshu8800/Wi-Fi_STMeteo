/*
 * uart_receive.c
 *
 *  Created on: Apr 1, 2025
 *      Author: FlY DeN
 */

#include "uart_receive.h"
#include "string.h"
#include "stdbool.h"


extern bool con_atempt, con_rest, connected, insettings, get_temp, get_humi,
		get_all, get_pres;
extern char GET_COMAND[120];

void check_mes(void) {
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
	/*if (strcmp(GET_COMAND, "GTARO\r\n") == 0) {
	 get_taro = true;
	 memset(GET_COMAND, 0, sizeof(GET_COMAND));
	 }  НЕВОЗМОЖНО СОЗДАТЬ ПО ПРИЧИНЕ НЕСТАБИЛЬНОСТИ И ВЫСОКОГО ЭНЕРГОПОТРЕБЛЕНИЯ!!!*/
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



