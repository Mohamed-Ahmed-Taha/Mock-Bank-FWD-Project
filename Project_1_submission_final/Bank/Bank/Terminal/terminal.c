#include "terminal.h"

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	uint8_t date[11] = "";

	if (tm.tm_mon < 9 && tm.tm_mday < 10)
		sprintf(date, "0%d/0%d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
	else if (tm.tm_mon < 9)
		sprintf(date, "%d/0%d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
	else if (tm.tm_mday < 10)
		sprintf(date, "0%d/%d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
	else
		sprintf(date, "%d/%d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

	strcpy(termData->transationDate, date);
	
	return TERMINAL_OK;
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData) {

	uint8_t cardMonDate[3] = "";
	uint8_t cardYearDate[3] = "";
	uint8_t termMonDate[3] = "";
	uint8_t termYearDate[3] = "";

	sprintf(cardMonDate, "%c%c", cardData.cardExpirationDate[0], cardData.cardExpirationDate[1]);
	sprintf(cardYearDate, "%c%c", cardData.cardExpirationDate[3], cardData.cardExpirationDate[4]);
	sprintf(termMonDate, "%c%c", termData.transationDate[3], termData.transationDate[4]);
	sprintf(termYearDate, "%c%c", termData.transationDate[8], termData.transationDate[9]);
	
	uint8_t cardMonDateInt = atoi(cardMonDate);
	uint8_t cardYearDateInt = atoi(cardYearDate);
	uint8_t termMonDateInt = atoi(termMonDate);
	uint8_t termYearDateInt = atoi(termYearDate);

	if ((cardYearDateInt < termYearDateInt) || ((cardYearDateInt == termYearDateInt) && (cardMonDateInt < termMonDateInt))) return EXPIRED_CARD;
	else return TERMINAL_OK;
}

EN_terminalError_t isValidCardPan(ST_cardData_t* cardData) {

	//LUHN check
	uint8_t PAN[20] = "";
	strcpy(PAN, cardData->primaryAccountNumber);
	uint32_t sum = 0;

	for (int i = 0; i < strlen(PAN); i++) {
		uint8_t digit = PAN[i] - '0';
		if (i % 2 == 0) digit *= 2;
		if (digit > 9) digit -= 9;

		sum += digit;
	}

	if (sum % 10 != 0) return INVALID_CARD;	
	return TERMINAL_OK;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {

	float amount;

	printf("\nPlease enter transaction amount (greater than 0): ");
	scanf("%f", &amount);

	if (amount <= 0) return INVALID_AMOUNT;
	
	termData->transAmount = amount;
	return TERMINAL_OK;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {

	if (termData->transAmount > termData->maxTransAmount) return EXCEED_MAX_AMOUNT;
	else return TERMINAL_OK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData) {

	float maxAmount;
	printf("\nPlease enter transaction maximum amount of ATM machine (greater than 0): ");
	scanf("%f", &maxAmount);

	if (maxAmount <= 0) return INVALID_MAX_AMOUNT;

	termData->maxTransAmount = maxAmount;
	return TERMINAL_OK;
}

