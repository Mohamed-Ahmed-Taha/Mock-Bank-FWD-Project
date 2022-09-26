#include "server.h"

EN_transState_t recieveTransactionData(ST_transaction_t* transData, ST_cardData_t cardData, ST_terminalData_t termData, ST_accountDB_t** accountRefrence) {

	transData->cardHolderData = cardData;
	transData->terminalData = termData;

	//is Valid Account
	if (isValidAccount(&transData->cardHolderData, accountRefrence) == ACCOUNT_NOT_FOUND)
		return (transData->transState = FRAUD_CARD);
	
	//is Blocked Account
	if (isBlockedAccount(*accountRefrence) == BLOCKED_ACCOUNT)
		return (transData->transState = DECLINED_STOLEN_CARD);

	//is Amount Available
	if (isAmountAvailable(&transData->terminalData, *accountRefrence) == LOW_BALANCE)
		return (transData->transState = DECLINED_INSUFFECIENT_FUND);


	//update database
	//save transaction
	if (saveTransaction(transData) == SERVER_OK) {
		(*accountRefrence)->balance -= (transData->terminalData.transAmount);
	}
	return transData->transState;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountDB_t** accountRefrence) {

	uint8_t taken = 0;
	for (int i = 0; i < NELEMS(accountsDB) && !taken; i++) {
		if (strcmp(cardData->primaryAccountNumber, accountsDB[i].primaryAccountNumber) == 0) {
			taken = 1;
			*accountRefrence = &accountsDB[i];
		}
	}

	if (!taken) return ACCOUNT_NOT_FOUND;
	else return SERVER_OK;
}

EN_serverError_t isBlockedAccount(ST_accountDB_t* accountRefrence) {

	if (accountRefrence->accountState == BLOCKED) return BLOCKED_ACCOUNT;
	else return SERVER_OK;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountDB_t* accountRefrence) {

	if (termData->transAmount > accountRefrence->balance) return LOW_BALANCE;
	else return SERVER_OK;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData) {

	uint32_t randomSequenceNumber = 0;
	uint8_t flag = 1;
	uint8_t index = -1;
	do {
		flag = 1;
		randomSequenceNumber = rand() % 1000000 + 100000;

		for (int i = 0; i < NELEMS(transactionsDB) && flag; i++) {
			if (transactionsDB[i].transactionSequenceNumber == randomSequenceNumber) flag = 0;
			else if (transactionsDB[i].transactionSequenceNumber == 0) index = i;
		}
	} while (!flag);

	if (index == -1) {
		transData->transState = INTERNAL_SERVER_ERROR;
		return SAVING_FAILED;
	}
	
	// if connection lost return SAVING_FAILED

	transData->transactionSequenceNumber = randomSequenceNumber;
	transData->transState = APPROVED;

	transactionsDB[index] = *transData;

	if (getTransaction(transData->transactionSequenceNumber, transData) == TRANSACTION_NOT_FOUND)
		return SAVING_FAILED;

	return SERVER_OK;
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData) {

	for (int i = 0; i < NELEMS(transactionsDB); i++) {
		if (transactionsDB[i].transactionSequenceNumber == transactionSequenceNumber) {
			*transData = transactionsDB[i];

			return SERVER_OK;
		}
	}

	return TRANSACTION_NOT_FOUND;

}
