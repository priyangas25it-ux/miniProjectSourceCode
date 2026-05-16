#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structure for bank account
struct BankAccount {
    int accNo;
    char name[30];
    char mobile[15];
    float balance;
};

// function declarations
void createAccount(FILE *fp);
void displayAccounts(FILE *fp);
void depositMoney(FILE *fp);
void withdrawMoney(FILE *fp);
void searchAccount(FILE *fp);
void deleteAccount(FILE *fp);

int main() {

    FILE *fp;
    int choice;

    // open file
    fp = fopen("bank.dat", "rb+");

    // create file if not exists
    if (fp == NULL) {

        fp = fopen("bank.dat", "wb+");

        if (fp == NULL) {
            printf("File cannot be opened.\n");
            exit(1);
        }
    }

    while (1) {

        printf("\n========== BANK MANAGEMENT SYSTEM ==========\n");

        printf("1. Create New Account\n");
        printf("2. Display All Accounts\n");
        printf("3. Deposit Money\n");
        printf("4. Withdraw Money\n");
        printf("5. Search Account\n");
        printf("6. Delete Account\n");
        printf("7. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {

            case 1:
                createAccount(fp);
                break;

            case 2:
                displayAccounts(fp);
                break;

            case 3:
                depositMoney(fp);
                break;

            case 4:
                withdrawMoney(fp);
                break;

            case 5:
                searchAccount(fp);
                break;

            case 6:
                deleteAccount(fp);
                break;

            case 7:
                fclose(fp);
                printf("Thank You for using Bank Management System.\n");
                exit(0);

            default:
                printf("Invalid Choice.\n");
        }
    }

    return 0;
}

// create account
void createAccount(FILE *fp) {

    struct BankAccount acc;
    struct BankAccount temp;

    printf("\nEnter Account Number: ");
    scanf("%d", &acc.accNo);

    // check duplicate account
    rewind(fp);

    while (fread(&temp, sizeof(temp), 1, fp) == 1) {

        if (temp.accNo == acc.accNo) {
            printf("Account already exists.\n");
            return;
        }
    }

    printf("Enter Customer Name: ");
    scanf(" %[^\n]", acc.name);

    printf("Enter Mobile Number: ");
    scanf("%s", acc.mobile);

    printf("Enter Initial Balance: ");
    scanf("%f", &acc.balance);

    fwrite(&acc, sizeof(acc), 1, fp);

    printf("\nAccount Created Successfully!\n");
}

// display all accounts
void displayAccounts(FILE *fp) {

    struct BankAccount acc;

    rewind(fp);

    printf("\n=============== ACCOUNT DETAILS ===============\n");

    printf("%-10s %-25s %-15s %-10s\n",
           "AccNo", "Name", "Mobile", "Balance");

    while (fread(&acc, sizeof(acc), 1, fp) == 1) {

        if (acc.accNo != 0) {

            printf("%-10d %-25s %-15s %.2f\n",
                   acc.accNo,
                   acc.name,
                   acc.mobile,
                   acc.balance);
        }
    }
}

// deposit money
void depositMoney(FILE *fp) {

    struct BankAccount acc;
    int accNo, found = 0;
    float amount;

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);

    rewind(fp);

    while (fread(&acc, sizeof(acc), 1, fp) == 1) {

        if (acc.accNo == accNo) {

            found = 1;

            printf("Current Balance: %.2f\n", acc.balance);

            printf("Enter Deposit Amount: ");
            scanf("%f", &amount);

            acc.balance += amount;

            // corrected line
            fseek(fp, -(long)sizeof(acc), SEEK_CUR);

            fwrite(&acc, sizeof(acc), 1, fp);

            printf("Amount Deposited Successfully.\n");
            printf("Updated Balance: %.2f\n", acc.balance);

            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }
}

// withdraw money
void withdrawMoney(FILE *fp) {

    struct BankAccount acc;
    int accNo, found = 0;
    float amount;

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);

    rewind(fp);

    while (fread(&acc, sizeof(acc), 1, fp) == 1) {

        if (acc.accNo == accNo) {

            found = 1;

            printf("Current Balance: %.2f\n", acc.balance);

            printf("Enter Withdraw Amount: ");
            scanf("%f", &amount);

            if (amount > acc.balance) {

                printf("Insufficient Balance.\n");

            } else {

                acc.balance -= amount;

                // corrected line
                fseek(fp, -(long)sizeof(acc), SEEK_CUR);

                fwrite(&acc, sizeof(acc), 1, fp);

                printf("Withdrawal Successful.\n");
                printf("Remaining Balance: %.2f\n", acc.balance);
            }

            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }
}

// search account
void searchAccount(FILE *fp) {

    struct BankAccount acc;
    int accNo, found = 0;

    printf("\nEnter Account Number to Search: ");
    scanf("%d", &accNo);

    rewind(fp);

    while (fread(&acc, sizeof(acc), 1, fp) == 1) {

        if (acc.accNo == accNo) {

            found = 1;

            printf("\n===== ACCOUNT FOUND =====\n");

            printf("Account Number : %d\n", acc.accNo);
            printf("Customer Name  : %s\n", acc.name);
            printf("Mobile Number  : %s\n", acc.mobile);
            printf("Balance        : %.2f\n", acc.balance);

            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }
}

// delete account
void deleteAccount(FILE *fp) {

    struct BankAccount acc;
    struct BankAccount blank = {0, "", "", 0};

    int accNo, found = 0;

    printf("\nEnter Account Number to Delete: ");
    scanf("%d", &accNo);

    rewind(fp);

    while (fread(&acc, sizeof(acc), 1, fp) == 1) {

        if (acc.accNo == accNo) {

            found = 1;

            // corrected line
            fseek(fp, -(long)sizeof(acc), SEEK_CUR);

            fwrite(&blank, sizeof(blank), 1, fp);

            printf("Account Deleted Successfully.\n");

            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }
}