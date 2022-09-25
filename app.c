#include"app.h"

void initializeAccountsDatabase(void);
void initializeTransactionsDatabase(void);

int main() {

    //initialization example
    initializeAccountsDatabase();
    initializeTransactionsDatabase();



    uint8_t control[100] = "";
    printf("Type (Yes) to start ATM or (No) to cancel or (Transaction) to get previous transaction data: ");
    scanf("%s", &control);
    control[11] = '\0';

    while (strcmp(control, "No") != 0) {
        
        if (strcmp(control, "Yes") != 0 && strcmp(control, "Transaction") != 0) {
            printf("\n\nType (Yes) or (No) or (Transaction) exactly as in the parentheses.\n\n\n\n___________________________________________________________________________________________________\n\n\n\nType (Yes) to start ATM or (No) to cancel or (Transaction) to get previous transaction data: ");
            scanf("%s", &control);
            control[11] = '\0';
            continue;
        }
        if (strcmp(control, "Transaction") == 0) {
            ST_transaction_t transData;
            uint32_t transactionSequenceNumber = 0;

            printf("\n\nPlease enter transaction sequence number: ");
            scanf("%d", &transactionSequenceNumber);
            if (getTransaction(transactionSequenceNumber, &transData) == TRANSACTION_NOT_FOUND) {
                printf("\n\nRequired transaction not found\n\n\n\n___________________________________________________________________________________________________\n\n\n\nType (Yes) to start ATM or (No) to cancel or (Transaction) to get previous transaction data: ");
                scanf("%s", &control);
                control[11] = '\0';
                continue;
            }
            
            printf("\n\nTransaction number %d\nTransaction state: %s\n\nCard holder name: %s\nCard PAN: %s\nCard expiration date: %s\n\nTransfered amount: %f\nTransfer date: %s\n",
                transData.transactionSequenceNumber, (transData.transState == APPROVED ? "APPROVED" : "DECLINED"),
                transData.cardHolderData.cardHolderName, transData.cardHolderData.primaryAccountNumber, transData.cardHolderData.cardExpirationDate,
                transData.terminalData.transAmount, transData.terminalData.transationDate);
            printf("\n\n\n\n___________________________________________________________________________________________________\n\n\n\nType (Yes) to start ATM or (No) to cancel or (Transaction) to get previous transaction data: ");
            scanf("%s", &control);
            control[11] = '\0';
            continue;
        }

        appStart();

        printf("___________________________________________________________________________________________________\n\n\n\nType (Yes) to start ATM or (No) to cancel or (Transaction) to get previous transaction data: ");
        scanf("%s", &control);
        control[3] = '\0';
    }
    
    return 0;
}

void appStart(void) {

    ST_cardData_t cardData;
    ST_terminalData_t termData;
    ST_transaction_t transData;
    ST_accountDB_t* accountRefrence;

    //Operations on card
    if (getCardHolderName(&cardData) == WRONG_NAME) {
        printf("\n\n               ........DECLINED........\n\nCard holder name does not meet specifications.\nTransaction canceled.\n\n\n\n");
        return;
    }
    if (getCardExpiryDate(&cardData) == WRONG_EXP_DATE) {
        printf("\n\n               ........DECLINED........\n\nCard expiration date does not meet specifications.\nTransaction canceled.\n\n\n\n");
        return;
    }
    if (getCardPAN(&cardData) == WRONG_PAN) {
        printf("\n\n               ........DECLINED........\n\nCard PAN does not meet specifications.\nTransaction canceled.\n\n\n\n");
        return;
    }

    
    //Operations on terminal
    if (getTransactionDate(&termData) == WRONG_DATE) {
        printf("\n\n               ........DECLINED........\n\nSystem date error.\nTransaction canceled.\n\n\n\n");
        return;
    }
    if (isCardExpired(cardData, termData) == EXPIRED_CARD) {
        printf("\n\n               ........DECLINED........\n\nCard date is expired.\nTransaction canceled.\n\n\n\n");
        return;
    }
    if (isValidCardPan(&cardData) == INVALID_CARD) {
        printf("\n\n               ........DECLINED........\n\nCard PAN is invalid.\nTransaction canceled.\n\n\n\n");
        return;
    }
    if (setMaxAmount(&termData) == INVALID_MAX_AMOUNT) {
        printf("\n\n               ........DECLINED........\n\nInvalid transaction maximum amount.\nTransaction canceled.\n\n\n\n");
        return;
    }
    if (getTransactionAmount(&termData) == INVALID_AMOUNT) {
        printf("\n\n               ........DECLINED........\n\nInvalid transaction amount.\nTransaction canceled.\n\n\n\n");
        return;
    }
    if (isBelowMaxAmount(&termData) == EXCEED_MAX_AMOUNT) {
        printf("\n\n               ........DECLINED........\n\nTransaction amount exceeds transaction maximum amount.\nTransaction canceled.\n\n\n\n");
        return;
    }


    //Operations on server
    EN_transState_t transState = recieveTransactionData(&transData, cardData, termData, &accountRefrence);
    if (transState == FRAUD_CARD) {
        printf("\n\n               ........DECLINED........\n\nDeclined due to fraud card.\nTransaction canceled.\n\n\n\n");
        return;
    }
    if (transState == DECLINED_STOLEN_CARD) {
        printf("\n\n               ........DECLINED........\n\nDeclined due to stolen card.\nTransaction canceled.\n\n\n\n");
        return;
    }
    if (transState == DECLINED_INSUFFECIENT_FUND) {
        printf("\n\n               ........DECLINED........\n\nDeclined due to insufecient fund.\nTransaction canceled.\n\n\n\n");
        return;
    }
    if (transState == INTERNAL_SERVER_ERROR) {
        printf("\n\n               ........DECLINED........\n\nInternal server error.\nTransaction %d canceled.\n\n\n\n", transData.transactionSequenceNumber);
        return;
    }
    if (transState == APPROVED) {
        printf("\n\n               ........APPROVED........\n\nTransaction %d succsesful and saved.\nYou have %f L.E remaining.\n\nHave a nice day :D\n\n\n\n", transData.transactionSequenceNumber, accountRefrence->balance);
        return;
    }
    
}

void initializeAccountsDatabase(void) {
    
    //accountsDB decleration in server.h
    accountsDB[0] = (ST_accountDB_t){ 5000.0f, RUNNING, "4539368897014596" };
    accountsDB[1] = (ST_accountDB_t){ 6055.63f, RUNNING,"4916458326833493" };
    accountsDB[2] = (ST_accountDB_t){ 69420.0f, BLOCKED, "4539874507216679" };
    accountsDB[3] = (ST_accountDB_t){ 3000.23f, RUNNING,"4539655283752973" };
    accountsDB[4] = (ST_accountDB_t){ 150.0f, BLOCKED, "4916545006607773" };
    accountsDB[5] = (ST_accountDB_t){ 802.88f, RUNNING,"4080264370946276" };
    accountsDB[6] = (ST_accountDB_t){ 3000.0f, RUNNING, "4024007161855704" };
    accountsDB[7] = (ST_accountDB_t){ 10000000.0f, BLOCKED, "4024007103479761" };
    accountsDB[8] = (ST_accountDB_t){ 1234.56f, BLOCKED, "4916299539856412" };
    accountsDB[9] = (ST_accountDB_t){ 2468.10f, RUNNING, "4343143684180819" };
    accountsDB[10] = (ST_accountDB_t){ 0.0f, RUNNING,"4929428282610321" };
}

void initializeTransactionsDatabase(void) {

    //transactionsDB decleration in server.h
    for (int i = 0; i < NELEMS(transactionsDB); i++) {
        transactionsDB[i].transactionSequenceNumber = 0;
    }
}
