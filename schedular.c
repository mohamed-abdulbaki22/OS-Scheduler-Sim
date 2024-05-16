#include <stdio.h>
#include <string.h>

#define MEMORY_SIZE 60
#define MAX_PROCESSES 10
#define MAX_PRIORITY_LEVELS 4
#define MAX_INSTRUCTIONS 10
#define MAX_BLOCKED 10

typedef struct
{
    char name[20];
    char value[20];
} Word;

typedef struct
{
    int processID;
    char state[10];
    int priority;
    int programCounter;
    int memoryLowerBound;
    int memoryUpperBound;
} PCB;

Word Memory[MEMORY_SIZE];

void intToStr(int value, char *str)
{
    sprintf(str, "%d", value);
}

void createPCB(int startIndex, int processID, const char *state, int priority, int programCounter, int memoryLowerBound, int memoryUpperBound)
{
    if (startIndex + 6 >= MEMORY_SIZE)
    {
        printf("Not enough memory to create PCB.\n");
        return;
    }

    char valueStr[20];

    // Storing processID
    strcpy(Memory[startIndex].name, "processID");
    sprintf(valueStr, "%d", processID);
    strcpy(Memory[startIndex].value, valueStr);

    // Storing state
    strcpy(Memory[startIndex + 1].name, "state");
    strcpy(Memory[startIndex + 1].value, state);

    // Storing priority
    strcpy(Memory[startIndex + 2].name, "priority");
    sprintf(valueStr, "%d", priority);
    strcpy(Memory[startIndex + 2].value, valueStr);

    // Storing programCounter
    strcpy(Memory[startIndex + 3].name, "programCounter");
    sprintf(valueStr, "%d", programCounter);
    strcpy(Memory[startIndex + 3].value, valueStr);

    // Storing memoryLowerBound
    strcpy(Memory[startIndex + 4].name, "memoryLowerBound");
    sprintf(valueStr, "%d", memoryLowerBound);
    strcpy(Memory[startIndex + 4].value, valueStr);

    // Storing memoryUpperBound
    strcpy(Memory[startIndex + 5].name, "memoryUpperBound");
    sprintf(valueStr, "%d", memoryUpperBound);
    strcpy(Memory[startIndex + 5].value, valueStr);
}
int getInstructions(char filename[], int startIndex)
{
    FILE *file;
    char line[256];

    // Open the file in read mode
    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Could not open file %s\n", filename);
        return -1;
    }

    // Read and store each line of the file
    int i = startIndex;
    while (fgets(line, sizeof(line), file) && i < MEMORY_SIZE)
    {
        // printf("%s",line);
        // Assuming each line contains a name and a value separated by space
        sscanf(line, "%19s %19s", Memory[i].name, Memory[i].value);
        i++;
    }

    // Close the file
    fclose(file);
    return i;
}

int main()
{
    int i = getInstructions("Program_1.txt", 0);
    int j = getInstructions("Program_2.txt", i);
    int k = getInstructions("Program_3.txt", j);
    createPCB(30, 1, "Ready", 0, 1, 0, 10);
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        if (strlen(Memory[i].name) > 0)
        {
            printf("Memory[%d]: Name = %s, Value = %s\n", i, Memory[i].name, Memory[i].value);
        }
    }

    return 0;
}
