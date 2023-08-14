#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

// Global Declarations
const int acc_no_len = 11;
char AccNumPreFix[8] = "01520521";

struct user usr;
FILE *fPtr;
char filename[50] = "Accounts/", hfilename[50] = "Accounts/";

// User Data Structure
struct user
{
    char acc_no[12];
    char name[50];
    char phone_no[50];
    char password[50];
    char pin[10];
    float balance;
};

// Function Definitions
void welcome_scr();
void displayHeader();
void menu();
int registerUser();
int loginUser();
void handleUser();
void depositMoney();
void withdrawMoney();
void transfer_money(int isClosure);
void account_details();
void transaction_details();
int logout();
void account_closure();
void printDivider(int n);
int verifyPin();
void generateAccountNumber();

// Main function
int main()
{
    welcome_scr();

    return 0;
}

// Welcome screen
void welcome_scr()
{
    int choice;
Label_1:
    system("cls");
    displayHeader();
    printf("\t 1. Register a new account\n");
    printf("\t 2. Login to an existing account\n\n");
    printf("\t    Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        if (registerUser() == 0)
            goto Label_1;
    }
    else if (choice == 2)
    {
        if (loginUser() == 0)
            goto Label_1;
    }
    else
    {
        system("cls");
        printf("\t INVALID CHOICE!\nPress any key to return....");
        getch();
        goto Label_1;
    }
    handleUser();
}

void displayHeader()
{
    printf("\t\t\033[1;36m-----------------------\n");
    printf("\t\t| Welcome to DigiBank |\n");
    printf("\t\t-----------------------\033[0;m\n\n");
}

// Menu for user
void menu()
{
    displayHeader();
    printf("\t\033[1;33m --- Main Menu ---\033[0;m\n");
    printf("\t 1. Deposit Money  \n");
    printf("\t 2. Withdraw Money  \n");
    printf("\t 3. Transfer Money  \n");
    printf("\t 4. Account Status  \n");
    printf("\t 5. Transaction details  \n");
    printf("\t 6. Account Closure  \n");
    printf("\t 7. Logout  \n");
}

// Register new user account
int registerUser()
{
    system("cls");
    printf("\t Enter your name:\t");
    fflush(stdin);
    scanf("%[^\n]", usr.name);
    printf("\t Enter your phone number:\t");
    scanf("%s", usr.phone_no);
    printf("\t Enter your password:\t");
    scanf("%s", usr.password);
    printf("\t Enter your transaction pin:\t");
    scanf("%s", usr.pin);
    usr.balance = 0;
    generateAccountNumber();
    strcat(filename, usr.phone_no);
    fPtr = fopen(strcat(filename, ".dat"), "w");
    fwrite(&usr, sizeof(struct user), 1, fPtr);
    int done = 0; // holds info of whether the acc creation is successfull or not
    fclose(fPtr);
    if (fwrite != 0)
    {
        printf("\n\t \033[1;32mACCOUNT SUCCESSFULLY CREATED.\033[0;m\n");
        printf("\t Your Account Number is: %s\n\n", usr.acc_no);
        strcat(hfilename, usr.phone_no);
        fPtr = fopen(strcat(hfilename, "h.txt"), "a");
        done = 1;
        fclose(fPtr);
    }
    else
    {
        printf("\t \033[1;31m SOMETHING WENT WRONG!!!\033[0;m\n\n");
    }
    printf("\t Press any key to continue....\t");
    getch();
    return done;
}

// Logs in an existing user
int loginUser()
{
    char phone[50], pword[50];
    system("cls");
    printf("\t Phone Number:\t");
    scanf("%s", &phone);
    printf("\t Password:\t");
    scanf("%s", pword);
    strcat(filename, phone);
    fPtr = fopen(strcat(filename, ".dat"), "r");
    if (fPtr == NULL)
    {
        printf("\t \033[1;33mACCOUNT NOT REGISTERED!\033[0;m\n\n");
        strcpy(filename, "Accounts/");
        fclose(fPtr);
    }
    else
    {

        fread(&usr, sizeof(struct user), 1, fPtr);
        fclose(fPtr);
        if (!strcmp(usr.password, pword))
        {
            // Move to home screen
            strcat(hfilename, usr.phone_no);
            strcat(hfilename, "h.txt");
            printf("\t Password Matched!\n\n"); // Password hashing
            return 1;
        }
        else
        {
            printf("\t \033[1;31mINVALID PASSWORD!!!\033[0;m\n\n");
            strcpy(filename, "Accounts/");
        }
    }
    printf("\t Press any key to continue....");
    getch();
    return 0;
}

// Handles the logged in user
void handleUser()
{
    int choice;
    choice = 0;
    goto Label_2;
Label_2:
    while (1)
    {
        system("cls");
        menu();
        printf("\n\t Enter your choice: ");
        scanf("%d", &choice);
        system("cls");
        switch (choice)
        {
        case 1:
            depositMoney();
            break;
        case 2:
            withdrawMoney();
            break;
        case 3:
            transfer_money(0);
            break;
        case 4:
            account_details();
            break;
        case 5:
            transaction_details();
            break;
        case 6:
            account_closure();
            break;
        case 7:
            if (logout())
                welcome_scr();
            break;
        default:
            printf("\t \033[1;31mINVALID CHOICE!!!\033[0;m!");
            getch();
            break;
        }
    }
}

// Deposits money
void depositMoney()
{
    system("cls");
    time_t tm;
    time(&tm);
    printf("\t \033[1;33mDEPOSIT MONEY\033[0;m\n");
    printDivider(50);
    float dip_amt;
    printf("\n\t Enter the amount to be deposited:\t");
    scanf("%f", &dip_amt);
    if (verifyPin())
    {
        usr.balance += dip_amt;
        printf("\n\t \033[1;32mMONEY DEPOSITED.\033[0;m\n");
        printf("\t Current balance: %.2f\n", usr.balance);
        // saving updated data
        fPtr = fopen(filename, "w");
        fwrite(&usr, sizeof(struct user), 1, fPtr);
        fclose(fPtr);
        // saving history
        fPtr = fopen(hfilename, "a");
        fprintf(fPtr, "\t Rs %.2f has been deposited to your account \n", dip_amt);
        fprintf(fPtr, "\t Date /time of transaction %s\n", ctime(&tm));
        fclose(fPtr);
    }
    else
    {
        printf("\n\t \033[1;31mINCORRECT TRANSACTION PIN!\033[0;m\n");
    }

    printf("\n\t Press any key to continue..........\n");
    getch();
}

// Withdraws money
void withdrawMoney()
{
    system("cls");
    time_t tm;
    time(&tm);
    printf("\t \033[1;33mWITHDRAW MONEY\033[0;m\n");
    printDivider(50);
    float with_amt;
    printf("\n\t Enter the amount to withdraw:\t");
    scanf("%f", &with_amt);
    if (verifyPin())
    {
        if (usr.balance < with_amt)
        {
            printf("\n\t \033[1;31mINSUFFICIENT BALANCE!\033[0;m\n");
        }
        else
        {
            usr.balance -= with_amt;
            printf("\n\t \033[1;32mMONEY SUCCESSFULLY WITHDRAWN\033[0;m\n");
            printf("\t Current balance: Rs %.2f\n", usr.balance);
            fPtr = fopen(filename, "w");
            fwrite(&usr, sizeof(struct user), 1, fPtr);
            fclose(fPtr);
            fPtr = fopen(hfilename, "a");
            fprintf(fPtr, "\t Rs %.2f has been withdrawn from your account \n", with_amt);
            fprintf(fPtr, "\t Date /time of transaction %s\n", ctime(&tm));
            fclose(fPtr);
        }
    }
    else
    {
        printf("\n\t \033[1;31mINCORRECT TRANSACTION PIN!\033[0;m\n");
    }
    printf("\n\t Press any key to continue..........\n");
    getch();
}

// Transfers money
void transfer_money(int isClosure)
{
    time_t tm;
    if (!isClosure)
    {
        system("cls");
        time(&tm);
        printf("\t \033[1;33mTRANSFER MONEY\033[0;m\n");
        printDivider(50);
    }
    char rec_phn[50];
    float trans_amt;
    printf("\n\t Enter the phone number of receiver: ");
    scanf("%s", &rec_phn);
    char rec_tdata[50] = "Accounts/";
    char rec_data[50] = "Accounts/";
    strcat(rec_tdata, rec_phn);
    strcat(rec_data, rec_phn);
    strcat(rec_data, ".dat");
    strcat(rec_tdata, "h.txt");
    if (!isClosure)
    {
        printf("\t Enter the amount to be transferred: ");
        scanf("%f", &trans_amt);
    }
    else
    {
        trans_amt = usr.balance;
    }
    if (verifyPin())
    {
        fPtr = fopen(rec_data, "r");
        if (fPtr == NULL)
        {
            printf("\n\t \033[1;31mRECEIVER'S ACCOUNT NUMBER NOT REGISTERED!\033[0;m\n");
            fclose(fPtr);
        }
        else
        {
            struct user rec;
            fread(&rec, sizeof(struct user), 1, fPtr);
            fclose(fPtr);
            printf("\n\t Transferring Rs %.2f to %s\n", trans_amt, rec.name);
            printf("\t press 'y' to confirm and 'n' to cancel:\t");
            char ch = getch();
            if (ch == 'y')
            {
                if (usr.balance < trans_amt)
                {
                    printf("\n\t \033[1;31mINSUFFICIENT BALANCE!\033[0;m\n");
                    fclose(fPtr);
                }
                else
                {
                    usr.balance -= trans_amt;
                    printf("\n\n\t \033[1;32mMONEY TRANSFERRED SUCCESSFULLY\033[0;m\n");
                    if (!isClosure)
                        printf("\t Current balance: Rs %.2f\n", usr.balance);
                    rec.balance += trans_amt;
                    fPtr = fopen(rec_data, "w");
                    fwrite(&rec, sizeof(struct user), 1, fPtr);
                    fclose(fPtr);
                    fPtr = fopen(rec_tdata, "a");
                    fprintf(fPtr, "\t Rs %.2f has been transferred from %s \n", trans_amt, usr.name);
                    fprintf(fPtr, "\t Date /time of transaction %s\n", ctime(&tm));
                    fclose(fPtr);
                    fPtr = fopen(filename, "w");
                    fwrite(&usr, sizeof(struct user), 1, fPtr);
                    fclose(fPtr);
                    fPtr = fopen(hfilename, "a");
                    fprintf(fPtr, "\t Rs %.2f has been transferred to %s \n", trans_amt, rec.name);
                    fprintf(fPtr, "\t Date /time of transaction %s\n", ctime(&tm));
                    fclose(fPtr);
                }
            }
            else
            {
                printf("\n\t \033[1;31mTRANSACTION FAILED\033[0;m\n");
            }
        }
    }
    else
    {
        printf("\n\t \033[1;31mINCORRECT TRANSACTION PIN!\033[0;m\n");
    }
    printf("\n\t Press any key to continue..........\n");
    getch();
}

// Logged in user's details
void account_details()
{
    system("cls");
    if (verifyPin())
    {
        system("cls");
        printf("\n\t \033[1;33mACCOUNT DETAILS\033[0;m\n");
        printDivider(50);
        printf("\t Account Holder: %s\n", usr.name);
        printf("\t Account Number: %s\n", usr.acc_no);
        printf("\t Phone Number: %s\n", usr.phone_no);
        printf("\t Current Balance: Rs %.2f\n", usr.balance);
        printDivider(50);
    }
    else
    {
        printf("\n\t \033[1;31mINCORRECT TRANSACTION PIN!\033[0;m\n");
    }
    printf("\n\t Press any key to continue..........\n");
    getch();
}

// Logged in user's history
void transaction_details()
{
    system("cls");
    if (verifyPin())
    {
        system("cls");
        fPtr = fopen(hfilename, "r");
        char c = fgetc(fPtr);
        if (c == EOF)
            printf("\n\t \033[1;36mNO RECENT TRANSACTIONS!\033[0;m\n");
        else
        {
            printf("\n\t \033[1;33mTRANSACTION DETAILS\033[0;m\n");
            printDivider(55);
            while (c != EOF)
            {
                printf("%c", c);
                c = fgetc(fPtr);
            }
            printDivider(55);
        }
        fclose(fPtr);
    }
    else
    {
        printf("\n\t \033[1;31mINCORRECT TRANSACTION PIN!\033[0;m\n");
    }
    printf("\n\t Press any key to continue..........\n");
    getch();
}

// Logout current user
int logout()
{
    system("cls");
    printf("\n\t Are you sure you want to log out?\n");
    char cf;
    printf("\t Press 'y' to confirm and 'n' to cancel:\t");
    cf = getch();
    if (cf == 'y')
    {
        printf("\n\n\t \033[1;36mTHANK YOU FOR USING OUR BANK MANAGEMENT SYSTEM\033[0;m\n\n");
        printf("\t Press any key to continue...\t");
        getch();
        strcpy(filename, "Accounts/");
        strcpy(hfilename, "Accounts/");
        return 1;
    }
    return 0;
}

// Closes The Account Permanently
void account_closure()
{
    char ch;
    system("cls");
    printf("\n\t Are you sure you want to close this account?");
    printf("\n\t Press 'y' to confirm:\n");
    ch = getch();
    if (ch == 'y')
    {
        if (verifyPin())
        {
            if (usr.balance > 0)
            {
                printf("\n\t You have Rs %.2f in your account\n", usr.balance);
                printf("\n\t Click '1' to withdraw and '2' to transfer:\t");
                ch = getch();
                if (ch == '1')
                {
                    printf("\n\t \033[1;32mRs %.2f has been withdrawn from your account\033[0;m\n", usr.balance);
                    usr.balance = 0;
                    printf("\n\t Press any key to continue...\t");
                    getch();
                }
                else if (ch == '2')
                {
                    transfer_money(1);
                }
            }
            remove(filename);
            remove(hfilename);
            strcpy(filename, "Accounts/");
            strcpy(hfilename, "Accounts/");
            welcome_scr();
        }
        else
        {
            printf("\n\t \033[1;31mINCORRECT TRANSACTION PIN!\033[0;m\n");
            getch();
        }
    }
}

// Provides a horizontal line of n width
void printDivider(int n)
{
    printf("\t ");
    while (n != 0)
    {
        printf("-");
        n--;
    }
    printf("\n");
}

// Verifies Transaction Pin before each transaction
int verifyPin()
{
    char gotPin[10];
    printf("\t Enter Your Transaction Pin:\t");
    scanf("%s", gotPin);
    if (!strcmp(gotPin, usr.pin))
        return 1;
    else
        return 0;
}

// Generates a random account number with some constant prefix
void generateAccountNumber()
{
    for (int i = 0; i < 8; i++)
        usr.acc_no[i] = AccNumPreFix[i];
    for (int i = 8; i < acc_no_len; i++)
    {
        usr.acc_no[i] = '0' + rand() % 10;
    }
    usr.acc_no[acc_no_len] = '\0'; // Null-terminate the string
}