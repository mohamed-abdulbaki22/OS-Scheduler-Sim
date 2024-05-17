#include <stdio.h>
#include <string.h>

#define MEMORY_SIZE 60
#define MAX_PROCESSES 10
#define MAX_PRIORITY_LEVELS 4
#define MAX_INSTRUCTIONS 10
#define MAX_SIZE 100
#define MAX_BLOCKED 10
#define PCB1_START 0
#define PCB2_START 6
#define PCB3_START 12
#define A1_INDEX 50
#define B1_INDEX 51
#define A2_INDEX 52
#define B2_INDEX 53
#define A3_INDEX 54
#define B3_INDEX 55
static int time;
static int arrival1;
static int arrival2;
static int arrival3;
static int nextMemIndex = 18;

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

typedef enum
{
    false = 0,
    true = 1
} bool;

typedef struct
{
    int items[MAX_SIZE];
    int front;
    int rear;
    unsigned int size;
} Queue;

void initializeQueue(Queue *queue)
{
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
}

bool isEmpty(Queue *queue)
{
    return (queue->size == 0);
}

bool isFull(Queue *queue)
{
    return (queue->size == MAX_SIZE);
}

// Get the size of the queue
int size(Queue *queue)
{
    return queue->size;
}

// Enqueue an element into the queue
void enqueue(Queue *queue, int value)
{
    if (isFull(queue))
    {
        printf("Queue is full. Cannot enqueue.\n");
        return;
    }
    queue->rear = (queue->rear + 1) % MAX_SIZE;
    queue->items[queue->rear] = value;
    queue->size++;
}

// Dequeue an element from the queue
int dequeue(Queue *queue)
{
    if (isEmpty(queue))
    {
        printf("Queue is empty. Cannot dequeue.\n");
        return -1;
    }
    int value = queue->items[queue->front];
    queue->front = (queue->front + 1) % MAX_SIZE;
    queue->size--;
    return value;
}

// Display the queue
void display(Queue *queue)
{
    if (isEmpty(queue))
    {
        printf("Queue is empty.\n");
        return;
    }
    printf("Queue elements: ");
    int i = queue->front;
    do
    {
        printf("%d ", queue->items[i]);
        i = (i + 1) % MAX_SIZE;
    } while (i != (queue->rear + 1) % MAX_SIZE);
    printf("\n");
}
// Define the mutex structure
typedef struct
{
    bool flag;          // Mutex flag
    int ownerID;        // Owner ID (if needed)
    Queue blockedqueue; // Queue for blocked threads
} Mutex;

// Initialize the mutex
void initMutex(Mutex *mutex)
{
    mutex->flag = false;                     // Mutex is initially unlocked
    initializeQueue(&(mutex->blockedqueue)); // Initialize the blocked queue
}

// Wait operation on mutex
void semWait(Mutex *mutex, int processID)
{
    if (!(mutex->flag))
    {
        mutex->flag = true;
    }
    else
    {
        if (size(&(mutex->blockedqueue)) < MAX_SIZE)
        {
            enqueue(&(mutex->blockedqueue), processID);
        }
        else
        {
            printf("Error: Blocked queue for mutex is full.\n");
        }
    }
}

// Signal operation on mutex
void semSignal(Mutex *mutex)
{
    if (!isEmpty(&(mutex->blockedqueue)))
    {
        int unblockedProcess = dequeue(&(mutex->blockedqueue));
        // Unblock the process (implementation dependent)
        printf("Unblocked process with ID %d\n", unblockedProcess);
        // Add unblocked process to the ready queue (implementation dependent)
    }
    else
    {
        mutex->flag = false;
    }
}

Mutex userInput;
Mutex userOutput;
Mutex file;
Queue Readyqueue1, Readyqueue2, Readyqueue3, Readyqueue4;
Word Memory[MEMORY_SIZE];

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
        strcpy(Memory[i].name, line);
        i++;
    }

    // Close the file
    fclose(file);
    return i;
}

// printing numbers in a range
void printFromTo(int a, int b)
{
    for (int i = a; i <= b; i++)
    {
        printf(" %d", i);
    }
}

// assign a value to an index in memory
void assign(int index, char method[])
{
    int tmp;
    char valueStr[20];
    if (method == "input")
    {
        printf("Please enter a value: ");
        scanf("%d", &tmp);
        sprintf(valueStr, "%d", tmp);
        strcpy(Memory[index].value, valueStr);
    }
    printf("The value u inserted is %s", Memory[index].value);
}
void writeFile(char filename[], int index)
{
    FILE *fptr;

    // Open a file in writing mode
    fptr = fopen(filename, "w");

    // Write some text to the file
    fprintf(fptr, "%s", Memory[index].value);

    // Close the file
    fclose(fptr);
}
void readFile(char filename[], int index)
{
    FILE *fptr;

    // Open a file in read mode
    fptr = fopen(filename, "r");

    // Read the content and store it inside myString
    fgets(Memory[index].value, 20, fptr);

    // Print the file content
    printf("The data in the file is %s", Memory[index].value);

    // Close the file
    fclose(fptr);
}
void setVariables(int start)
{
    for (int i = 0; i < 3; i++)
    {
        // printf("%s",line);
        // Assuming each line contains a name and a value separated by space
        switch (i)
        {
        case 0:
            strcpy(Memory[start + i].name, "a");
            break;
        case 1:
            strcpy(Memory[start + i].name, "b");
            break;
        case 2:
            strcpy(Memory[start + i].name, "c");
            break;
        default:
            break;
        }
    }
}
void initProcess(int processNum)
{
    int lower = nextMemIndex;
    int upper = nextMemIndex;
    int i;
    int j;
    switch (processNum)
    {
    case 1:
        i = getInstructions("Program_1.txt", lower);
        setVariables(i);
        j = i + 3;
        upper = j - 1;
        createPCB(PCB1_START, 1, "Ready", 1, 0, lower, upper);
        nextMemIndex = j;
        break;
    case 2:
        i = getInstructions("Program_2.txt", lower);
        setVariables(i);
        j = i + 3;
        upper = j - 1;
        createPCB(PCB2_START, 2, "Ready", 1, 0, lower, upper);
        nextMemIndex = j;
        break;
    case 3:
        i = getInstructions("Program_3.txt", lower);
        setVariables(i);
        j = i + 3;
        upper = j - 1;
        createPCB(PCB3_START, 3, "Ready", 1, 0, lower, upper);
        nextMemIndex = j;
        break;
    default:
        printf("there is no such process");
        break;
    }
    // int i = getInstructions("Program_1.txt", 0);
    // int j = getInstructions("Program_2.txt", i);
    // int k = getInstructions("Program_3.txt", j);
}
void init()
{
    // int i = getInstructions("Program_1.txt", 0);
    // int j = getInstructions("Program_2.txt", i);
    // int k = getInstructions("Program_3.txt", j);

    // Initialize the global mutex
    initMutex(&userOutput);
    initMutex(&userInput);
    initMutex(&file);

    // Initialize the 4 priority queues
    initializeQueue(&(Readyqueue1));
    initializeQueue(&(Readyqueue2));
    initializeQueue(&Readyqueue3);
    initializeQueue(&Readyqueue4);

    // // initialize variable space
    // strcpy(Memory[A1_INDEX].name, "a1");
    // strcpy(Memory[B1_INDEX].name, "b1");
    // strcpy(Memory[A2_INDEX].name, "a2");
    // strcpy(Memory[B2_INDEX].name, "b2");
    // strcpy(Memory[A_INDEX].value, "a3");
}
void executeProgram()
{
    for (time = 0; time < 100; time++)
    {
        if (time == arrival1)
        {
            initProcess(1);
            enqueue(&Readyqueue1,1);
        }
        if (time == arrival2)
        {
            initProcess(2);
            enqueue(&Readyqueue1,2);
        }
        if (time == arrival3)
        {
            initProcess(3);
            enqueue(&Readyqueue1,3);
        }
    }
    printf("program ended");
}
int main()
{
    init();

    for (int i = 1; i <= 3; i++)
    {
        printf("arrival time of: %d\n", i);
        if (i == 1)
            scanf("%d", &arrival1);
        else if (i == 2)
            scanf("%d", &arrival2);
        else if (i == 3)
            scanf("%d", &arrival3);
    }

    executeProgram();

    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        if (strlen(Memory[i].name) > 0)
        {
            printf("Memory[%d]: Name = %s, Value = %s\n", i, Memory[i].name, Memory[i].value);
        }
    }
    // assign(A1_INDEX, "input");
    // printFromTo(2, 6);
    // writeFile("meow.txt", B1_INDEX);
    // readFile("meow.txt", B1_INDEX);
    // for (int i = 0; i < MEMORY_SIZE; i++)
    // {
    //     if (strlen(Memory[i].name) > 0)
    //     {
    //         printf("Memory[%d]: Name = %s, Value = %s\n", i, Memory[i].name, Memory[i].value);
    //     }
    // }
    return 0;
}