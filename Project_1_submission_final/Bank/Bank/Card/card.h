#pragma once

#ifndef CARD_H
#define CARD_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>



	#ifndef UINT8_T
	#define UINT8_T

		typedef unsigned char uint8_t;


	#endif // !UINT8_T


typedef struct ST_cardData_t {
	uint8_t cardHolderName[25];
	uint8_t primaryAccountNumber[20];
	uint8_t cardExpirationDate[6];
} ST_cardData_t;

typedef enum EN_cardError_t {
	CARD_OK, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
}EN_cardError_t;

EN_cardError_t getCardHolderName(ST_cardData_t* cardData);
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData);
EN_cardError_t getCardPAN(ST_cardData_t* cardData);


#endif // !CARD_H
