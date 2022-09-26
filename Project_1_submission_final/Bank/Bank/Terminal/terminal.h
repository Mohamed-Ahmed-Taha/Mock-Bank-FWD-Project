#pragma once


#ifndef TERMINAL_H
#define TERMINAL_H

#define _CRT_SECURE_NO_WARNINGS

	#ifndef UINT8_T
	#define UINT8_T

		typedef unsigned char uint8_t;

	#endif // !UINT8_t

	#ifndef UINT32_T
	#define UINT32_T

		typedef unsigned int uint32_t;

	#endif // !UINT32_T


#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "../Card/card.h"

typedef enum EN_terminalError_t {
	TERMINAL_OK, WRONG_DATE, EXPIRED_CARD, INVALID_CARD, INVALID_AMOUNT, EXCEED_MAX_AMOUNT, INVALID_MAX_AMOUNT
} EN_terminalError_t;

typedef struct ST_terminalData_t {
	float transAmount;
	float maxTransAmount;
	uint8_t transationDate[11];
} ST_terminalData_t;

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData);
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData);
EN_terminalError_t isValidCardPan(ST_cardData_t* cardData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData);

#endif // !TERMINAL_H

