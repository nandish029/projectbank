#include <stdio.h>

int MAX_ADHAAR = 10;
int MAX_ACCOUNTS = 5;
int a, b, naccno, accno;
long *ppass, *dep;
long temp;
long adh[10] = {0};
long acc[2][5][10] = {0}; // [0]: Passwords, [1]: Balances

// Function to find Aadhaar index by Aadhaar number
int findAadhaarIndex(long aadhaar) {
    for (int i = 0; i < MAX_ADHAAR; i++) {
        if (adh[i] == aadhaar) {
            return i; // Return index of Aadhaar
        }
    }
    return -1; // Aadhaar not found
}

// Save data to file
void saveDataToFile() {
    FILE *file = fopen("banking_data.txt", "w");
    if (file == NULL) {
        printf("Error: Unable to save data.\n");
        return;
    }
    // Save Aadhaar numbers
    for (int i = 0; i < MAX_ADHAAR; i++) {
        fprintf(file, "%ld ", adh[i]);
    }
    fprintf(file, "\n");

    // Save accounts data
    for (int i = 0; i < MAX_ACCOUNTS; i++) {
        for (int j = 0; j < MAX_ADHAAR; j++) {
            fprintf(file, "%ld  %ld |", acc[0][i][j], acc[1][i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    printf("Data saved successfully.\n");
}

// Load data from file
void loadDataFromFile() {
    FILE *file = fopen("banking_data.txt", "r");
    if (file == NULL) {
        printf("No previous data found. Starting fresh.\n");
        return;
    }
    // Load Aadhaar numbers
    for (int i = 0; i < MAX_ADHAAR; i++) {
        fscanf(file, "%ld", &adh[i]);
    }

    // Load accounts data
    for (int i = 0; i < MAX_ACCOUNTS; i++) {
        for (int j = 0; j < MAX_ADHAAR; j++) {
            fscanf(file, "| %ld | %ld |", &acc[0][i][j], &acc[1][i][j]);
        }
    }
    fclose(file);
    printf("Data loaded successfully.\n");
}

// Other functions remain the same
long mine(int i, int j, int k) {
    /*
    j = operation type (1: create account, 2: deposit, 3: check balance, 4: withdraw)
    k = password for account (only for create account, check balance, and withdraw)
    i = Aadhaar index
    */
    if (j == 1) { // Creating a new account
        for (a = 0; a < MAX_ACCOUNTS; a++) {
            if (acc[0][a][i] == 0) { // Find empty account slot
                naccno = a + 1; // New account number
                acc[0][a][i] = k; // Set password
                acc[1][a][i] = 0; // Initialize balance to 0
                printf("Account created successfully. Account number: %d\n", naccno);
                return naccno;
            }
        }
        printf("No available account slots.\n");
    } else if (j == 2) { // Deposit operation
        printf("Enter deposit amount: ");
        long deposit;
        scanf("%ld", &deposit);
        dep = &acc[1][accno - 1][i];
        *dep += deposit; // Add deposit amount to the balance
        printf("Deposit successful. New balance: %ld\n", *dep);
    } else if (j == 3) { // Check balance
        if (acc[0][accno - 1][i] == k) { // Verify password
            printf("Current balance: %ld\n", acc[1][accno - 1][i]);
        } else {
            printf("Incorrect password.\n");
        }
    } else if (j == 4) { // Withdraw operation
        if (acc[0][accno - 1][i] == k) { // Verify password
            printf("Enter withdrawal amount: ");
            long withdraw;
            scanf("%ld", &withdraw);
            dep = &acc[1][accno - 1][i];
            if (*dep >= withdraw) {
                *dep -= withdraw; // Deduct withdrawal amount
                printf("Withdrawal successful. New balance: %ld\n", *dep);
            } else {
                printf("Insufficient balance.\n");
            }
        } else {
            printf("Incorrect password.\n");
        }
    }
    return 0;
}



char create(void) {
    for (a = 0; a < MAX_ADHAAR; a++) {
        if (adh[a] == 0) { // Find an empty Aadhaar slot
            printf("Enter Aadhaar number: ");
            scanf("%ld", &temp);

            // Check for duplicate Aadhaar
            int is_duplicate = 0;
            for (b = 0; b < MAX_ADHAAR; b++) {
                if (temp == adh[b]) {
                    printf("Aadhaar number already exists.\n");
                    is_duplicate = 1;
                    break;
                }
            }

            if (!is_duplicate) {
                adh[a] = temp; // Store Aadhaar number
                printf("Aadhaar number added successfully.\n");
            }
            return 0;
        }
    }
    printf("No available Aadhaar slots.\n");
    return 0;
}

int main() {
    loadDataFromFile(); // Load data at startup

    int choice;
    while (1) {
        printf("\nSimple Banking System\n");
        printf("1. Add Aadhaar\n");
        printf("2. Create Account\n");
        printf("3. Deposit\n");
        printf("4. Check Balance\n");
        printf("5. Withdraw Money\n");
        printf("6. Save Data\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            create();
            break;
        case 2: {
            long aadhaarNumber;
            int password;

            printf("Enter Aadhaar number: ");
            scanf("%ld", &aadhaarNumber);

            int index = findAadhaarIndex(aadhaarNumber);
            if (index != -1) {
                printf("Enter a password for the account: ");
                scanf("%d", &password);
                mine(index, 1, password); // Use detected index
            } else {
                printf("Aadhaar number not registered.\n");
            }
            break;
        }
        case 3: { // Deposit money
            long aadhaarNumber;
            printf("Enter Aadhaar number: ");
            scanf("%ld", &aadhaarNumber);

            int index = findAadhaarIndex(aadhaarNumber);
            if (index != -1) {
                printf("Enter account number (1-%d): ", MAX_ACCOUNTS);
                scanf("%d", &accno);
                if (accno > 0 && accno <= MAX_ACCOUNTS && acc[0][accno - 1][index] != 0) {
                    mine(index, 2, 0); // Use detected index
                } else {
                    printf("Invalid account number or account not found.\n");
                }
            } else {
                printf("Aadhaar number not registered.\n");
            }
            break;
        }
        case 4: { // Check balance
            long aadhaarNumber;
            int password;

            printf("Enter Aadhaar number: ");
            scanf("%ld", &aadhaarNumber);

            int index = findAadhaarIndex(aadhaarNumber);
            if (index != -1) {
                printf("Enter account number (1-%d): ", MAX_ACCOUNTS);
                scanf("%d", &accno);
                if (accno > 0 && accno <= MAX_ACCOUNTS && acc[0][accno - 1][index] != 0) {
                    printf("Enter account password: ");
                    scanf("%d", &password);
                    mine(index, 3, password); // Use detected index
                } else {
                    printf("Invalid account number or account not found.\n");
                }
            } else {
                printf("Aadhaar number not registered.\n");
            }
            break;
        }
        case 5: { // Withdraw money
            long aadhaarNumber;
            int password;
            printf("Enter Aadhaar number: ");
            scanf("%ld", &aadhaarNumber);
            int index = findAadhaarIndex(aadhaarNumber);
            if (index != -1) {
                printf("Enter account number (1-%d): ", MAX_ACCOUNTS);
                scanf("%d", &accno);
                if (accno > 0 && accno <= MAX_ACCOUNTS && acc[0][accno - 1][index] != 0) {
                    printf("Enter account password: ");
                    scanf("%d", &password);
                    mine(index, 4, password); // Use detected index
                } else {
                    printf("Invalid account number or account not found.\n");
                }
            } else {
                printf("Aadhaar number not registered.\n");
            }
            break;
        }
        case 6:
            saveDataToFile();
            break;
        case 7:
            printf("Exiting...\n");
            saveDataToFile(); // Save data before exit
            return 0;
        default:
            printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}
