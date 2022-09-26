#pragma once

#ifndef SERVER_H
#define SERVER_H

#define _CRT_SECURE_NO_WARNINGS

	#ifndef NELEMS
	#define NELMSS
	#define NELEMS(X) (sizeof(X)/sizeof((X[0])))

	#endif // !NELEMS


	#ifndef UINT8_T
	#define UINT8_T

		typedef unsigned char uint8_t;

	#endif // !UINT8_t

	#ifndef UINT32_T
	#define UINT32_T

		typedef unsigned int uint32_t;

	#endif // !UINT32_T

#include <stdlib.h>
#include "../Card/card.h"
#include "../Terminal/terminal.h"


typedef enum EN_transState_t {
	APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, FRAUD_CARD, INTERNAL_SERVER_ERROR	
} EN_transState_t;

typedef enum EN_serverError_t {
	SERVER_OK, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE, BLOCKED_ACCOUNT, TRANSACTION_DECLINED
} EN_serverError_t;

typedef enum EN_accountState_t {
	RUNNING, BLOCKED
} EN_accountState_t;

typedef struct ST_transaction_t {
	ST_cardData_t cardHolderData;
	ST_terminalData_t terminalData;
	EN_transState_t transState;
	uint32_t transactionSequenceNumber;
} ST_transaction_t;

typedef struct ST_accountDB_t {
	float balance;
	EN_accountState_t accountState;
	uint8_t primaryAccountNumber[20];
} ST_accountDB_t;

ST_accountDB_t accountsDB[255];
ST_transaction_t transactionsDB[255];

EN_transState_t recieveTransactionData(ST_transaction_t* transData, ST_cardData_t cardData, ST_terminalData_t termData, ST_accountDB_t** accountRefrence); 
EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountDB_t** accountRefrene);
EN_serverError_t isBlockedAccount(ST_accountDB_t* accountRefrence);
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountDB_t* accountRefrence);
EN_serverError_t saveTransaction(ST_transaction_t* transData);
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData);

#endif // !SERVER_H
