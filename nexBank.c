#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>

// Function Prototypes

void loginDisplay();

void signupDisplay();

int generateUserID(int min, int max);

void bankMenu(char *user, long int userID);

void checkBalance(float balance);

float deposit(long int userID, float balance);

float withdraw(long int userID, float balance);

float loadBalance(long int userID);

void saveBalance(long int userID, float balance);

bool doesUsernameExist(char username[]);

void taxCalculator();

float transferMoney(long int userID, float balance);

bool doesUserIDExist(long int id);

// Main Program

int main(){

    // Variables

    int choice = 0;

    // Title

    printf("\n############################################");
    printf("\n#                                          #");
    printf("\n#                                          #");
    printf("\n#                  nexBank                 #");             
    printf("\n#                                          #");
    printf("\n#                                          #");
    printf("\n############################################");

    printf("\n\n         *** Welcome to nexBank! ***\n");

    do{

        printf("\nPlease choose from one of the options below!\n\n");
        printf("1. Login\n2. Signup\n\n");

        // Accepting user input

        printf("Please enter your choice (1 - 2): ");

        if (scanf("%d", &choice) != 1) {
            printf("\nInvalid Choice! Please enter '1' if you want to login or '2' if you want to signup!\n");

            while (getchar() != '\n');

            choice = 0;

            continue;
        }

        while (getchar() != '\n');

        if(choice < 1 || choice > 2){
            printf("\nInvalid Choice! Please enter '1' if you want to login or '2' if you want to signup!\n");
        }

        else if(choice == 1){
            loginDisplay();
        }

        else{
            signupDisplay();
        }

    } while(choice < 1 || choice > 2);


    return 0;

}


void loginDisplay(){

    char username[50];
    char password[50];
    char storedUsername[50];
    char storedPassword[50];
    long int storedUserID;
    long int loggedInID = -1;

    bool loginSuccess = false;

    printf("\n*** LOGIN ***\n\n");

    // Reading data from the CSV file

    FILE *pFile = fopen("nexBank.csv", "r");

    if(pFile == NULL){
        printf("\nError opening file!\n\n");
        return;
    }

    // 5 attempts

    int i;

    for(i = 1; i <= 5 && !loginSuccess; i++){
    
        // Username

        printf("Username: ");
        fgets(username, sizeof(username), stdin);
        username[strlen(username) - 1] = '\0';

        // Password

        printf("\nPassword: ");
        fgets(password, sizeof(password), stdin);
        password[strlen(password) - 1] = '\0';

        printf("\nConfirming credentials for '%s'...\n", username);
        sleep(1);

        rewind(pFile);

        // Skipping the header

        char header[100];
        fgets(header, sizeof(header), pFile);

        while(fscanf(pFile, "%49[^,],%49[^,],%ld\n", storedUsername, storedPassword, &storedUserID) == 3){

            if(strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0){

                loginSuccess = true;
                loggedInID = storedUserID;
                printf("\nLogin Successful!\n");
                break;

            }

        }

        if(!loginSuccess && i < 5){
            printf("\nInvalid username or password!\n\n");

        }
    }

    fclose(pFile);

    if(loginSuccess){
        bankMenu(username, loggedInID);
    }
    
    else{
        printf("\nToo many failed attempts! Access Denied!\n\n");
    }

}


void signupDisplay(){

    srand(time(NULL));

    char username[50];
    char password[50];
    long int userID;

    // For the userID - it will be a six digit number

    int min = 100000;
    int max = 999999;

    printf("\n*** SIGNUP ***\n\n");

    // Username

    do{
        printf("Please enter a username: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = '\0';

        if(strlen(username) < 4){
            printf("\nUsername must be at least 4 characters long!\n\n");
        }

        else if(strchr(username, ',') != NULL){
            printf("\nUsername cannot contain commas!\n\n");
        }

        else if(doesUsernameExist(username)){
            printf("\nUsername already exists! Please choose another username.\n\n");
        }

    } while(strlen(username) < 4 || strchr(username, ',') != NULL || doesUsernameExist(username));

    // Password

    do{
        printf("\nPlease enter a password: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = '\0';

        if(strlen(password) < 8){
            printf("\nPassword must be at least 8 characters long!\n");
        }

        else if(strchr(password, ',') != NULL){
            printf("\nPassword cannot contain commas!\n");
        }
    
    } while(strlen(password) < 8 || strchr(password, ',') != NULL);

    // Random userID

    printf("\nGenerating a userID for '%s'...", username);

    do{

        userID = generateUserID(min, max);

    } while(doesUserIDExist(userID));

    fflush(stdout);
    sleep(2);
    printf("\n\nuserID: %ld", userID);

    // Storing details in a CSV file

    bool isNewFile = false;

    FILE *pFile = fopen("nexBank.csv", "r");

    if(pFile == NULL){
        isNewFile = true;
    }

    else{
        fclose(pFile);
    }

    pFile = fopen("nexBank.csv", "a");    // "a" = append mode. This ensures old data is not deleted

    if(pFile == NULL){
        printf("\nError opening file!\n\n");
        return;
    }

    if(isNewFile){
        fprintf(pFile, "username,password,userID\n");
    }

    fprintf(pFile, "%s,%s,%ld\n", username, password, userID);
    fclose(pFile);

    saveBalance(userID, 0.0f);

    printf("\n\nAccount created successfully! You can now login!\n");

    sleep(2);

    loginDisplay();
    
}


bool doesUsernameExist(char username[]){

    char storedUsername[50];

    FILE *pFile = fopen("nexBank.csv", "r");

    if(pFile == NULL){
        return false;
    }

    // Skipping the header

    char header[100];
    fgets(header, sizeof(header), pFile);

    while(fscanf(pFile, "%49[^,],%*[^,],%*ld\n", storedUsername) == 1){

        if(strcmp(username, storedUsername) == 0){

            fclose(pFile);
            return true;

        }

    }

    fclose(pFile);

    return false;

}


bool doesUserIDExist(long int id){

    char storedUsername[50];
    char storedPassword[50];
    long int storedUserID;

    FILE *pFile = fopen("nexBank.csv", "r");

    if(pFile == NULL){
        return false;
    }

    char header[100];
    fgets(header, sizeof(header), pFile);

    while(fscanf(pFile, "%49[^,],%49[^,],%ld\n", storedUsername, storedPassword, &storedUserID) == 3){

        if(storedUserID == id){
            fclose(pFile);
            return true;
        }
    }

    fclose(pFile);
    return false;

}


int generateUserID(int min, int max){

    int number = (rand() % (max - min + 1)) + min;
    return number;

}


void bankMenu(char *user, long int userID){

    int userChoice = 0;
    float balance = loadBalance(userID);

    do{

        printf("\n*** Bank Menu ***\n\n");
        printf("Welcome back %s! Please select from one of the options below:\n\n", user);
        printf("1. Check Balance\n2. Deposit Money\n3. Withdraw Money\n4. Pay & Transfer\n5. Tax Calculator\n6. Exit\n\n");

        printf("Your choice (1 - 6): ");

        if(scanf("%d", &userChoice) != 1){
            printf("\nInvalid input! Please enter a number.\n");
            while(getchar() != '\n');
            userChoice = 0;
            continue;
        }

        while(getchar() != '\n');

        switch(userChoice){

            case 1:
                checkBalance(balance);
                break;

            case 2:
                balance = deposit(userID, balance);
                break;

            case 3:
                balance = withdraw(userID, balance);
                break;

            case 4:
                balance = transferMoney(userID, balance);
                break;

            case 5:
                taxCalculator();
                break;

            case 6:
                printf("\n*** Thank you for using nexBank! ***\n\n");
                exit(0);

            default:
                printf("\nInvalid choice! Please select (1 - 6)!\n");
            
        }

    } while(userChoice != 6);

}


void checkBalance(float balance){

    printf("\nYour current balance is: £%.2f\n", balance);

}


float deposit(long int userID, float balance){

    float amount = 0.0f;

    printf("\nEnter amount to deposit: £");

    if(scanf("%f", &amount) != 1){
        printf("\nInvalid input! Please enter a number.\n");
        while(getchar() != '\n');
        return balance;
    }

    while(getchar() != '\n');

    if(!isfinite(amount) || amount <= 0){
        printf("\nInvalid amount!\n");
        return balance;
    }

    balance += amount;
    saveBalance(userID, balance);
    printf("\nSuccessfully deposited £%.2f\n", amount);

    return balance;

}


float withdraw(long int userID, float balance){

    float amount = 0.0f;

    printf("\nEnter amount to withdraw: £");

    if (scanf("%f", &amount) != 1){
        printf("\nInvalid input! Please enter a number.\n");
        while (getchar() != '\n');
        return balance;
    }

    while (getchar() != '\n');

    if(!isfinite(amount) || amount <= 0){
        printf("\nInvalid amount! Please enter a positive amount.\n");
        return balance;
    }

    else if(amount > balance){
        printf("\nInsufficient funds! Your balance is £%.2f\n", balance);
        return balance;
    }

    balance -= amount;
    saveBalance(userID, balance);

    printf("\nSuccessfully withdrew: £%.2f\n", amount);

    return balance;

}


float loadBalance(long int userID){

    long int storedUserID;
    float storedBalance;

    FILE *pFile = fopen("nexBalance.csv", "r");

    if(pFile == NULL){
        return 0.0f;        // Balance starts at 0
    }

    // Skipping header

    char header[100];
    fgets(header, sizeof(header), pFile);

    while(fscanf(pFile, "%ld,%f\n", &storedUserID, &storedBalance) == 2){

        if(storedUserID == userID){

            fclose(pFile);
            return storedBalance;

        }
    }

    fclose(pFile);
    return 0.0f;        // If user isn't found, default to 0

}


void saveBalance(long int userID, float balance){

    int capacity = 100;
    int count = 0;
    bool found = false;

    long int *ids = malloc(capacity * sizeof(long int));
    float *balances = malloc(capacity * sizeof(float));

    if(ids == NULL || balances == NULL){
        printf("\nMemory error!\n");
        free(ids);
        free(balances);
        return;
    }

    FILE *pFile = fopen("nexBalance.csv", "r");

    if(pFile != NULL){

        char header[100];
        fgets(header, sizeof(header), pFile);

        while(1){

            if(count >= capacity){
                capacity *= 2;
                long int *newIds = realloc(ids, capacity * sizeof(long int));
                float *newBalances = realloc(balances, capacity * sizeof(float));

                if(newIds == NULL || newBalances == NULL){
                    printf("\nMemory error!\n");
                    free(newIds ? newIds : ids);
                    free(newBalances ? newBalances : balances);
                    fclose(pFile);
                    return;
                }

                ids = newIds;
                balances = newBalances;
            }

            if(fscanf(pFile, "%ld,%f\n", &ids[count], &balances[count]) != 2){
                break;
            }

            if(ids[count] == userID){
                balances[count] = balance;
                found = true;
            }

            count++;

        }

        fclose(pFile);

    }

    if(!found){

        if(count >= capacity){
            capacity += 1;
            long int *newIds = realloc(ids, capacity * sizeof(long int));
            float *newBalances = realloc(balances, capacity * sizeof(float));

            if(newIds == NULL || newBalances == NULL){
                printf("\nMemory error!\n");
                free(newIds ? newIds : ids);
                free(newBalances ? newBalances : balances);
                return;
            }

            ids = newIds;
            balances = newBalances;
        }

        ids[count] = userID;
        balances[count] = balance;
        count++;
    }

    pFile = fopen("nexBalance.csv", "w");

    if(pFile == NULL){
        printf("\nError saving balance!\n");
        free(ids);
        free(balances);
        return;
    }

    fprintf(pFile, "userID,balance\n");

    int i;
    for(i = 0; i < count; i++){
        fprintf(pFile, "%ld,%.2f\n", ids[i], balances[i]);
    }

    fclose(pFile);
    free(ids);
    free(balances);

}


void taxCalculator(){

    double income;
    double allowance = 12570.0;
    double taxableIncome;
    double tax = 0.0;
    double amount;

    printf("\nTax year: 2026/27\n\n");
    printf("Enter your annual income: £");

    if (scanf("%lf", &income) != 1 || !isfinite(income) || income < 0) {
        printf("\nInvalid income! Please enter a valid amount.\n");
        while (getchar() != '\n');
        return;
    }

    while (getchar() != '\n');

    if (income > 100000) {
        allowance -= (income - 100000) / 2;

        if (allowance < 0) {
            allowance = 0;
        }
    }

    taxableIncome = income - allowance;

    if (taxableIncome > 0) {
        amount = taxableIncome < 37700 ? taxableIncome : 37700;
        tax += amount * 0.20;
        taxableIncome -= amount;
    }

    if (taxableIncome > 0) {
        amount = taxableIncome < (125140 - allowance - 37700) ? taxableIncome : (125140 - allowance - 37700);
        tax += amount * 0.40;
        taxableIncome -= amount;
    }

    if (taxableIncome > 0) {
        tax += taxableIncome * 0.45;
    }

    printf("########## TAX RESULTS ##########\n\n");
    printf("Annual income:       £%.2f\n", income);
    printf("Personal allowance:  £%.2f\n", allowance);
    printf("Estimated tax:       £%.2f\n", tax);
    printf("Income after tax:    £%.2f\n", income - tax);

    printf("\nThis is an estimate for 2026/27.\n");

    printf("\nPlease note this is after income tax ONLY, before National Insurance, pension contributions or student loan repayments!\n");
    

}

float transferMoney(long int userID, float balance){

    long int recipientID;
    float amount;
    float recipientBalance;

    printf("\nEnter recipient's userID: ");

    if(scanf("%ld", &recipientID) != 1){
        printf("\nInvalid userID!\n");
        while(getchar() != '\n');
        return balance;
    }

    while(getchar() != '\n');

    if(recipientID == userID){
        printf("\nYou cannot transfer money to yourself!\n");
        return balance;
    }

    if(!doesUserIDExist(recipientID)){
        printf("\nNo account found with that userID!\n");
        return balance;
    }

    recipientBalance = loadBalance(recipientID);

    printf("\nEnter amount to transfer: £");

    if(scanf("%f", &amount) != 1){
        printf("\nInvalid amount!\n");
        while(getchar() != '\n');
        return balance;
    }

    while(getchar() != '\n');

    if(!isfinite(amount) || amount <= 0){
        printf("\nPlease enter a valid amount!\n");
        return balance;
    }

    if(amount > balance){
        printf("\nInsufficient funds! Your balance is £%.2f\n", balance);
        return balance;
    }

    balance -= amount;
    recipientBalance += amount;

    saveBalance(userID, balance);
    saveBalance(recipientID, recipientBalance);

    printf("\nSuccessfully transferred £%.2f to userID %ld!\n", amount, recipientID);

    return balance;

}

