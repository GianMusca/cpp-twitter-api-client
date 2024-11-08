#pragma once

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <chrono>
#include <iostream>
#include <cstdio>
#include <windows.h>
#include <chrono>
#include <thread>
#define FTD2XX_EXPORTS
#include "ftd2xx.h"
#include "Ports.h"
#include <bitset>

using namespace std;

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define MY_LCD_DESCRIPTION "Dual RS232 B" 
#define CONNECTING_TIME 5 //in seconds


/*******************************************************************************
 * GLOBAL FUNCTIONS DECLARATION
 ******************************************************************************/

//Inicializa el FTDI y el LCD
FT_HANDLE * lcdInit(char* description, FT_STATUS& status);

//Destruye el LCD
void lcdDestroy(FT_HANDLE* ftHandle);

//Escribe un byte al IR
bool lcdWriteIR(FT_HANDLE deviceHandler, BYTE valor, DWORD ms = 2);

//Escribe un byte al DR
bool lcdWriteDR(FT_HANDLE deviceHandler, BYTE valor, DWORD ms = 2);

