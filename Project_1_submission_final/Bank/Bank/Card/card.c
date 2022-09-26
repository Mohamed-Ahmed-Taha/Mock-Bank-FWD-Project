#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {

	uint8_t name[100] = "";
	printf("\nPlease enter the card's holder name (20 to 24 characters long with no spaces): ");
	scanf("%s", name);

	if (name == NULL || strlen(name) > 24 || strlen(name) < 20) return WRONG_NAME;

	strcpy(cardData->cardHolderName, name);

	return CARD_OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {

	uint8_t date[100] = "";
	printf("\nPlease enter the card's expiration date (MM/YY): ");
	scanf("%s", date);

	if (date == NULL || strlen(date) != 5) return WRONG_EXP_DATE;
	if (!((date[0] >= '0' && date[0] <= '1') && (date[1] >= '0' && date[1] <= '9') && (date[2] == '/')
		&& (date[3] >= '0' && date[3] <= '9') && (date[4] >= '0' && date[4] <= '9'))) return WRONG_EXP_DATE;
	if ((date[0] == '0' && date[1] == '0') || (date[0] == '1' && date[1] > '2')) return WRONG_EXP_DATE;

	strcpy(cardData->cardExpirationDate, date);

	return CARD_OK;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData) {

	uint8_t PAN[100] = "";
	printf("\nPlease enter the card's Primary Account Number (PAN): ");
	scanf("%s", PAN);

	if (PAN == NULL || strlen(PAN) < 16 || strlen(PAN) > 19) return WRONG_PAN;
	for (int i = 0; i < strlen(PAN); i++) {
		if (PAN[i] < '0' || PAN[i] > '9') return WRONG_PAN;
	}

	strcpy(cardData->primaryAccountNumber, PAN);

	return CARD_OK;
}



