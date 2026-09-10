#include <stdio.h>
#include <string.h>

#define MAX_MEDICINES 50
#define COLS 5

/*
2D Array Columns:
[0] Stock
[1] Daily Requirement
[2] Minimum Stock
[3] Expiry Days
[4] Essentiality
*/

float calculateCoverage(float data[MAX_MEDICINES][COLS], int i)
{
    return data[i][0] / data[i][1];
}

void determineCondition(float data[MAX_MEDICINES][COLS],
                        float coverage[],
                        char condition[][30],
                        int i)
{
    int lowStock = data[i][0] < data[i][2];
    int expiryRisk = data[i][3] <= coverage[i];
    int critical = ((int)data[i][4] == 3 &&
                    (lowStock || coverage[i] < 5));

    if (critical && lowStock && expiryRisk)
        strcpy(condition[i], "Critical Condition");

    else if (lowStock && expiryRisk)
        strcpy(condition[i], "Shortage + Expiry");

    else if (critical)
        strcpy(condition[i], "Critical Condition");

    else if (lowStock)
    {
        if ((int)data[i][4] == 3)
            strcpy(condition[i], "Urgent Reorder");
        else
            strcpy(condition[i], "Reorder Required");
    }

    else if (expiryRisk)
        strcpy(condition[i], "Expiry Attention");

    else
        strcpy(condition[i], "Sufficient Stock");
}

void calculatePriority(float data[MAX_MEDICINES][COLS],
                       float coverage[],
                       int priority[],
                       int i)
{
    int score = 0;

    score += (int)data[i][4] * 20;

    if (data[i][0] < data[i][2])
        score += 30;

    if (coverage[i] < 5)
        score += 25;
    else if (coverage[i] < 10)
        score += 15;

    if (data[i][3] <= coverage[i])
        score += 25;
    else if (data[i][3] <= 30)
        score += 10;

    priority[i] = score;
}

void analyseAll(float data[MAX_MEDICINES][COLS],
                float coverage[],
                int priority[],
                char condition[][30],
                int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        coverage[i] = calculateCoverage(data, i);

        determineCondition(data, coverage,
                           condition, i);

        calculatePriority(data, coverage,
                          priority, i);
    }
}

void displayMedicine(float data[MAX_MEDICINES][COLS],
                     float coverage[],
                     int priority[],
                     char condition[][30],
                     char code[][10],
                     char name[][30],
                     int i)
{
    printf("\nMedicine Code : %s", code[i]);
    printf("\nMedicine Name : %s", name[i]);
    printf("\nAvailable Stock : %.0f", data[i][0]);
    printf("\nDaily Requirement : %.0f", data[i][1]);
    printf("\nMinimum Stock : %.0f", data[i][2]);
    printf("\nStock Coverage : %.2f days", coverage[i]);
    printf("\nExpiry Remaining : %.0f days", data[i][3]);
    printf("\nEssentiality Level : %.0f", data[i][4]);
    printf("\nStock Condition : %s", condition[i]);
    printf("\nManagement Priority : %d\n", priority[i]);
}

int searchMedicine(char code[][10],
                   int n,
                   char target[])
{
    int i;

    for (i = 0; i < n; i++)
    {
        if (strcmp(code[i], target) == 0)
            return i;
    }

    return -1;
}

void swapRows(float data[MAX_MEDICINES][COLS],
              char code[][10],
              char name[][30],
              float coverage[],
              int priority[],
              char condition[][30],
              int a,
              int b)
{
    int k;
    float temp;
    char tempCode[10];
    char tempName[30];
    char tempCondition[30];
    int tempPriority;

    /* Swap 2D array row */
    for (k = 0; k < COLS; k++)
    {
        temp = data[a][k];
        data[a][k] = data[b][k];
        data[b][k] = temp;
    }

    /* Swap medicine code */
    strcpy(tempCode, code[a]);
    strcpy(code[a], code[b]);
    strcpy(code[b], tempCode);

    /* Swap medicine name */
    strcpy(tempName, name[a]);
    strcpy(name[a], name[b]);
    strcpy(name[b], tempName);

    /* Swap coverage */
    temp = coverage[a];
    coverage[a] = coverage[b];
    coverage[b] = temp;

    /* Swap priority */
    tempPriority = priority[a];
    priority[a] = priority[b];
    priority[b] = tempPriority;

    /* Swap condition */
    strcpy(tempCondition, condition[a]);
    strcpy(condition[a], condition[b]);
    strcpy(condition[b], tempCondition);
}

void sortByPriority(float data[MAX_MEDICINES][COLS],
                    char code[][10],
                    char name[][30],
                    float coverage[],
                    int priority[],
                    char condition[][30],
                    int n)
{
    int i, j;

    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (priority[j] < priority[j + 1])
            {
                swapRows(data, code, name,
                         coverage, priority,
                         condition, j, j + 1);
            }
        }
    }
}

void updateStock(float data[MAX_MEDICINES][COLS],
                 float coverage[],
                 int priority[],
                 char condition[][30],
                 char code[][10],
                 char name[][30],
                 int n)
{
    char target[10];
    int index;
    int choice;
    int quantity;

    printf("\nEnter Medicine Code: ");
    scanf("%9s", target);

    index = searchMedicine(code, n, target);

    if (index == -1)
    {
        printf("Medicine not found!\n");
        return;
    }

    printf("\n1. Add New Stock\n");
    printf("2. Issue Medicine\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    printf("Enter quantity: ");
    scanf("%d", &quantity);

    if (quantity <= 0)
    {
        printf("Invalid quantity!\n");
        return;
    }

    if (choice == 1)
    {
        data[index][0] += quantity;

        printf("Stock added successfully.\n");
    }

    else if (choice == 2)
    {
        if (quantity > data[index][0])
        {
            printf("Error: Stock cannot become negative.\n");
            return;
        }

        data[index][0] -= quantity;

        printf("Medicine issued successfully.\n");
    }

    else
    {
        printf("Invalid choice!\n");
        return;
    }

    coverage[index] =
        calculateCoverage(data, index);

    determineCondition(data, coverage,
                       condition, index);

    calculatePriority(data, coverage,
                      priority, index);

    displayMedicine(data, coverage,
                    priority, condition,
                    code, name, index);
}

void displayReport(float data[MAX_MEDICINES][COLS],
                   float coverage[],
                   int priority[],
                   char condition[][30],
                   char code[][10],
                   char name[][30],
                   int n)
{
    int i;

    printf("\n==========================================================================\n");
    printf(" MEDICINE STOCK REPORT\n");
    printf("==========================================================================\n");

    printf("%-8s %-15s %-7s %-10s %-10s %-20s %-8s\n",
           "Code",
           "Name",
           "Stock",
           "Coverage",
           "Expiry",
           "Condition",
           "Priority");

    for (i = 0; i < n; i++)
    {
        printf("%-8s %-15s %-7.0f %-10.2f %-10.0f %-20s %-8d\n",
               code[i],
               name[i],
               data[i][0],
               coverage[i],
               data[i][3],
               condition[i],
               priority[i]);
    }
}

void displaySummary(float data[MAX_MEDICINES][COLS],
                    int priority[],
                    char condition[][30],
                    char code[][10],
                    char name[][30],
                    int n)
{
    int i;

    int reorder = 0;
    int urgent = 0;
    int expiry = 0;
    int highest = 0;

    for (i = 0; i < n; i++)
    {
        if (strstr(condition[i], "Reorder") != NULL ||
            strstr(condition[i], "Shortage") != NULL ||
            strstr(condition[i], "Critical") != NULL)
        {
            reorder++;
        }

        if (strcmp(condition[i], "Urgent Reorder") == 0 ||
            strcmp(condition[i], "Critical Condition") == 0)
        {
            urgent++;
        }

        if (strstr(condition[i], "Expiry") != NULL)
        {
            expiry++;
        }

        if (priority[i] > highest)
        {
            highest = priority[i];
        }
    }

    printf("\nTotal medicines analysed : %d\n", n);
    printf("Number requiring reorder : %d\n", reorder);
    printf("Urgent attention : %d\n", urgent);
    printf("Expiry concern : %d\n", expiry);

    printf("\nHighest Management Priority:\n");

    for (i = 0; i < n; i++)
    {
        if (priority[i] == highest)
        {
            printf("%s - %s (Priority %d)\n",
                   code[i],
                   name[i],
                   priority[i]);
        }
    }
}

void saveReport(float data[MAX_MEDICINES][COLS],
                float coverage[],
                int priority[],
                char condition[][30],
                char code[][10],
                char name[][30],
                int n)
{
    FILE *fp;
    int i;

    fp = fopen("medicine_stock_report.txt", "w");

    if (fp == NULL)
    {
        printf("Error creating report file!\n");
        return;
    }

    fprintf(fp,
            "MEDICINE STOCK AND EXPIRY REPORT\n\n");

    for (i = 0; i < n; i++)
    {
        fprintf(fp,
                "%s | %s | Stock=%.0f | Coverage=%.2f | "
                "Expiry=%.0f | Essentiality=%.0f | "
                "%s | Priority=%d\n",
                code[i],
                name[i],
                data[i][0],
                coverage[i],
                data[i][3],
                data[i][4],
                condition[i],
                priority[i]);
    }

    fclose(fp);

    printf("Report saved to medicine_stock_report.txt\n");
}

int main()
{
    /*
    2D Array:
    Stock | Daily Requirement | Minimum Stock |
    Expiry Days | Essentiality
    */

    float data[MAX_MEDICINES][COLS] =
    {
        {120, 35, 80, 45, 3},
        {300, 40, 100, 120, 2},
        {60, 12, 50, 30, 3},
        {200, 25, 80, 20, 3},
        {250, 18, 60, 15, 1},
        {180, 15, 50, 90, 1}
    };

    char code[MAX_MEDICINES][10] =
    {
        "MED01",
        "MED02",
        "MED03",
        "MED04",
        "MED05",
        "MED06"
    };

    char name[MAX_MEDICINES][30] =
    {
        "Oral Saline",
        "Paracetamol",
        "Insulin",
        "Amoxicillin",
        "Antacid",
        "Cetirizine"
    };

    float coverage[MAX_MEDICINES];

    int priority[MAX_MEDICINES];

    char condition[MAX_MEDICINES][30];

    int n = 6;

    int choice;
    int index;

    char target[10];

    /* Initial analysis */
    analyseAll(data,
               coverage,
               priority,
               condition,
               n);

    do
    {
        printf("\n\n====================================\n");
        printf(" MEDICINE MANAGEMENT SYSTEM\n");
        printf("====================================\n");

        printf("1. Display Report\n");
        printf("2. Search Medicine\n");
        printf("3. Update Stock\n");
        printf("4. Sort by Priority\n");
        printf("5. Display Summary\n");
        printf("6. Save Report\n");
        printf("0. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:

                analyseAll(data,
                           coverage,
                           priority,
                           condition,
                           n);

                displayReport(data,
                              coverage,
                              priority,
                              condition,
                              code,
                              name,
                              n);

                break;

            case 2:

                printf("Enter Medicine Code: ");
                scanf("%9s", target);

                index = searchMedicine(code,
                                        n,
                                        target);

                if (index == -1)
                {
                    printf("Medicine not found!\n");
                }
                else
                {
                    displayMedicine(data,
                                    coverage,
                                    priority,
                                    condition,
                                    code,
                                    name,
                                    index);
                }

                break;

            case 3:

                updateStock(data,
                            coverage,
                            priority,
                            condition,
                            code,
                            name,
                            n);

                break;

            case 4:

                sortByPriority(data,
                               code,
                               name,
                               coverage,
                               priority,
                               condition,
                               n);

                displayReport(data,
                              coverage,
                              priority,
                              condition,
                              code,
                              name,
                              n);

                break;

            case 5:

                displaySummary(data,
                               priority,
                               condition,
                               code,
                               name,
                               n);

                break;

            case 6:

                saveReport(data,
                           coverage,
                           priority,
                           condition,
                           code,
                           name,
                           n);

                break;

            case 0:

                printf("Program terminated.\n");

                break;

            default:

                printf("Invalid choice!\n");
        }

    } while (choice != 0);

    return 0;
}

