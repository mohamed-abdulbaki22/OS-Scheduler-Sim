#include <stdio.h>
#include <string.h>

#define MEMORY_SIZE 60
#define MAX_PROCESSES 10
#define MAX_PRIORITY_LEVELS 4
#define MAX_INSTRUCTIONS 10
#define MAX_SIZE 100
#define MAX_BLOCKED 10
#define PCB1_START 30
#define PCB2_START 36
#define PCB3_START 42
#define A1_INDEX 50
#define B1_INDEX 51
#define A2_INDEX 52
#define B2_INDEX 53
#define A3_INDEX 54
#define B3_INDEX 55
static int time;

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
void init()
{
    int i = getInstructions("Program_1.txt", 0);
    int j = getInstructions("Program_2.txt", i);
    int k = getInstructions("Program_3.txt", j);
    createPCB(PCB1_START, 1, "Ready", 0, 0, 0, 6);
    createPCB(PCB2_START, 2, "Ready", 0, i, 7, 13);
    createPCB(PCB3_START, 3, "Ready", 0, j, 14, 22);
    // Initialize the global mutex
    initMutex(&userOutput);
    initMutex(&userInput);
    initMutex(&file);

    // Initialize the 4 priority queues
    initializeQueue(&(Readyqueue1));
    initializeQueue(&(Readyqueue2));
    initializeQueue(&Readyqueue3);
    initializeQueue(&Readyqueue4);

    // initialize variable space
    strcpy(Memory[A1_INDEX].name, "a1");
    strcpy(Memory[B1_INDEX].name, "b1");
    strcpy(Memory[A2_INDEX].name, "a2");
    strcpy(Memory[B2_INDEX].name, "b2");
    // strcpy(Memory[A_INDEX].value, "a3");
}

int main()
{
    init();
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        if (strlen(Memory[i].name) > 0)
        {
            printf("Memory[%d]: Name = %s, Value = %s\n", i, Memory[i].name, Memory[i].value);
        }
    }
    assign(A1_INDEX, "input");
    // printFromTo(2, 6);
    // writeFile("meow.txt", B1_INDEX);
    readFile("meow.txt",B1_INDEX);
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        if (strlen(Memory[i].name) > 0)
        {
            printf("Memory[%d]: Name = %s, Value = %s\n", i, Memory[i].name, Memory[i].value);
        }
    }
    return 0;
}
