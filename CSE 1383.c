#include <stdio.h>
#include <string.h>

#define ROWS 6
#define COLS 5

void calculatePriority(float data[ROWS][COLS], float priority[ROWS])
{
    int i;

    for (i = 0; i < ROWS; i++)
    {
        float requested = data[i][0];
        float minimum = data[i][1];
        float loss = data[i][2];
        float waiting = data[i][3];
        float typeWeight = data[i][4];

        priority[i] = typeWeight
                    + (minimum / 100.0)
                    + (waiting * 8)
                    + ((requested - minimum) / requested * 20)
                    + ((100 - loss) * 0.10);
    }
}

void sortByPriority(float data[ROWS][COLS],
                    float priority[ROWS])
{
    int i, j, k;
    float temp;

    for (i = 0; i < ROWS - 1; i++)
    {
        for (j = i + 1; j < ROWS; j++)
        {
            if (priority[j] > priority[i])
            {
                temp = priority[i];
                priority[i] = priority[j];
                priority[j] = temp;

                for (k = 0; k < COLS; k++)
                {
                    temp = data[i][k];
                    data[i][k] = data[j][k];
                    data[j][k] = temp;
                }
            }
        }
    }
}

void allocateWater(float data[ROWS][COLS],
                   float priority[ROWS],
                   float allocated[ROWS],
                   float effective[ROWS],
                   float shortage[ROWS])
{
    float available = 13500;
    float requiredRelease;
    int i;

    for (i = 0; i < ROWS; i++)
    {
        if (available <= 0)
        {
            allocated[i] = 0;
        }
        else
        {
            requiredRelease =
                data[i][0] / (1 - data[i][2] / 100.0);

            if (requiredRelease <= available)
            {
                allocated[i] = requiredRelease;
                available -= requiredRelease;
            }
            else
            {
                allocated[i] = available;
                available = 0;
            }
        }

        effective[i] =
            allocated[i] -
            (allocated[i] * data[i][2] / 100.0);

        shortage[i] = data[i][0] - effective[i];

        if (shortage[i] < 0)
            shortage[i] = 0;
    }
}

void searchZone(float data[ROWS][COLS], int n)
{
    int id, i;
    int found = 0;

    printf("\nEnter Zone Number (1-6): ");
    scanf("%d", &id);

    if (id < 1 || id > 6)
    {
        printf("Zone not found!\n");
        return;
    }

    for (i = 0; i < n; i++)
    {
        if ((int)data[i][4] == id)
        {
            printf("\nZone Found!\n");
            printf("Zone ID: Z%02d\n", id);
            printf("Requested Water: %.2f L\n", data[i][0]);
            printf("Minimum Requirement: %.2f L\n", data[i][1]);
            printf("Loss: %.2f%%\n", data[i][2]);
            printf("Waiting Cycles: %.0f\n", data[i][3]);

            found = 1;
            break;
        }
    }

    if (!found)
        printf("Zone not found!\n");
}

int main()
{
    /*
       Columns:
       [0] Requested Water
       [1] Minimum Requirement
       [2] Loss %
       [3] Waiting Cycles
       [4] Service Weight
    */

    float data[ROWS][COLS] =
    {
        {4000, 3000, 2, 0, 50}, // Z01 Hospital
        {3500, 2500, 4, 1, 40}, // Z02 Emergency Shelter
        {4500, 2000, 8, 2, 25}, // Z03 Residential
        {3800, 1800, 12, 3, 25}, // Z04 Residential
        {2600, 1600, 5, 1, 40}, // Z05 Emergency Shelter
        {2000, 1500, 1, 0, 45} // Z06 Emergency Service
    };

    float priority[ROWS];
    float allocated[ROWS];
    float effective[ROWS];
    float shortage[ROWS];

    int i;

    /* Calculate Priority */
    calculatePriority(data, priority);

    /* Sort according to Priority */
    sortByPriority(data, priority);

    /* Allocate Water */
    allocateWater(data, priority,
                  allocated, effective, shortage);

    /* Display Report */
    printf("\n");
    printf("====================================================\n");
    printf(" EMERGENCY POTABLE WATER ALLOCATION SYSTEM\n");
    printf("====================================================\n");

    printf("\n%-5s %-12s %-12s %-10s %-12s %-12s\n",
           "Zone", "Requested", "Allocated",
           "Effective", "Shortage", "Priority");

    printf("------------------------------------------------------------\n");

    for (i = 0; i < ROWS; i++)
    {
        printf("Z%02d %-12.2f %-12.2f %-10.2f %-12.2f %-12.2f\n",
               i + 1,
               data[i][0],
               allocated[i],
               effective[i],
               shortage[i],
               priority[i]);
    }

    /* Search */
    searchZone(data, ROWS);

    return 0;
}
