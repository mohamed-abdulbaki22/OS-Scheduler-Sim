// change the queue to be queue of processes
// simwait changed it so the blocked queue store the blocked process through checking ID( id=1:process1, id=2:process2, id=3:process3)
// simSignal
// programExecute
// in programInstruction simsignal and simwait is inserted but how will I know the process that is currently executing (ownerID)

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
// #define B1_INDEX 51
// #define A2_INDEX 52
// #define B2_INDEX 53
// #define A3_INDEX 54
// #define B3_INDEX 55
static int Quantum;
static int cyclesLeft;
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

typedef struct
{
    int processID;
    char state[10];
    int priority;
    int programCounter;
    int instructionsLeft;
    int memoryLowerBound;
    int memoryUpperBound;
} Process;

typedef enum
{
    false = 0,
    true = 1
} bool;

typedef struct
{
    Process items[MAX_SIZE];
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
void enqueue(Queue *queue, Process value)
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
Process dequeue(Queue *queue)
{
    Process emptyProcess = {0, "", 0, 0, 0}; // Default empty process

    if (isEmpty(queue))
    {
        printf("Queue is empty. Cannot dequeue.\n");
        return emptyProcess;
    }
    Process value = queue->items[queue->front];
    queue->front = (queue->front + 1) % MAX_SIZE;
    queue->size--;
    return value;
}

// Peek at the front element of the queue without removing it
Process peek(Queue *queue)
{
    Process emptyProcess = {0, "", 0, 0, 0}; // Default empty process

    if (isEmpty(queue))
    {
        printf("Queue is empty. Cannot peek.\n");
        return emptyProcess;
    }
    return queue->items[queue->front];
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
        printf("%d ", queue->items[i].processID); // Display process IDs
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
    mutex->flag = true;                      // Mutex is initially unlocked
    initializeQueue(&(mutex->blockedqueue)); // Initialize the blocked queue
    mutex->ownerID = -1;                     // till a process owns the Mutex
}

int MutexOwner(Mutex *mutex)
{
    return mutex->ownerID;
}

// Wait operation on mutex
// Wait operation on mutex
void semWait(Mutex *mutex, Process process)
{
    extern Process process1, process2, process3; // Declare external variables
    extern Queue Readyqueue1;
    sleep(2);
    printf("\n \n I am inside the SimWait w 7yat omak \n \n");
    if (mutex->flag)
    {
        mutex->flag = false;
        mutex->ownerID = process.processID;
    }
    else
    {
        if (size(&(mutex->blockedqueue)) < MAX_SIZE)
        {
            cyclesLeft = Quantum;
            if (process.processID == 1)
            {
                // Process has been blocked
                updateState(process1.processID, "Blocked");
                enqueue(&(mutex->blockedqueue), process1);
                dequeue(&Readyqueue1);
                printf("process1 is blocked");
            }
            if (process.processID == 2)
            {
                // Process has been blocked
                updateState(process2.processID, "Blocked");
                enqueue(&(mutex->blockedqueue), process2);
                dequeue(&Readyqueue1);
                printf("process2 is blocked");
            }
            if (process.processID == 3)
            {
                // Process has been blocked
                updateState(process3.processID, "Blocked");
                enqueue(&(mutex->blockedqueue), process3);
                dequeue(&Readyqueue1);
                printf("process3 is blocked");
            }
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
    extern Queue Readyqueue1; // Declare external variable
    printf("\n \n I am inside the SimSignal Abos reglak \n \n");
    if (!isEmpty(&(mutex->blockedqueue)))
    {
        Process unblockedProcess = dequeue(&(mutex->blockedqueue));
        // Process has is ready
        updateState(unblockedProcess.processID, "Ready");
        enqueue(&Readyqueue1, unblockedProcess);
        // Unblock the process (implementation dependent)
        printf("Unblocked process with ID %d\n", unblockedProcess.processID);
        // Add unblocked process to the ready queue (implementation dependent)
    }
    mutex->flag = true;
    mutex->ownerID = -1;
}
Mutex userInput;
Mutex userOutput;
Mutex file;
Queue Readyqueue1;
Word Memory[MEMORY_SIZE];
Process process1, process2, process3;
Process *current = &process1;

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
    printf("printed range\n");
    for (int i = a; i <= b; i++)
    {
        printf(" %d", i);
    }
    printf("\n\n");
}

// assign a value to an index in memory
void assign(int index, char method[])
{
    int tmp;
    char valueStr[20];
    if (strcmp(method, "input") == 0)
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
        process1.processID = 1;
        strcpy(process1.state, "Ready");
        process1.priority = 1;
        process1.programCounter = 0;
        process1.instructionsLeft = i - lower;
        process1.memoryLowerBound = lower;
        process1.memoryUpperBound = upper;
        nextMemIndex = j;
        break;
    case 2:
        i = getInstructions("Program_2.txt", lower);
        setVariables(i);
        j = i + 3;
        upper = j - 1;
        createPCB(PCB2_START, 2, "Ready", 1, 0, lower, upper);
        process2.processID = 2;
        strcpy(process2.state, "Ready");
        process2.priority = 1;
        process2.programCounter = 0;
        printf("\n\n no. of instr. is %d\n\n", (i - lower));
        process2.instructionsLeft = i - lower;
        process2.memoryLowerBound = lower;
        process2.memoryUpperBound = upper;
        nextMemIndex = j;
        break;
    case 3:
        i = getInstructions("Program_3.txt", lower);
        setVariables(i);
        j = i + 3;
        upper = j - 1;
        createPCB(PCB3_START, 3, "Ready", 1, 0, lower, upper);
        process3.processID = 3;
        strcpy(process3.state, "Ready");
        process3.priority = 1;
        process3.programCounter = 0;
        // a = i - lower;
        printf("no. of instr. is %d", (i - lower));
        process3.instructionsLeft = i - lower;
        process3.memoryLowerBound = lower;
        process3.memoryUpperBound = upper;
        nextMemIndex = j;
        break;
    default:
        printf("there is no such process");
        break;
    }
}
void print(char toBePrinted[])
{
    printf("%s", toBePrinted);
}

int getId(Process p)
{
    return p.processID;
}

void updateState(int processId, char state[])
{
    switch (processId)
    {
    case 1:
        strcpy(process1.state, state);
        strcpy(Memory[PCB1_START + 1].value, state);
        break;
    case 2:
        strcpy(process2.state, state);
        strcpy(Memory[PCB2_START + 1].value, state);
        break;
    case 3:
        strcpy(process3.state, state);
        strcpy(Memory[PCB3_START + 1].value, state);
        break;

    default:
        break;
    }
}

void updateInstructionsLeft(int processId)
{
    switch (processId)
    {
    case 1:
        process1.instructionsLeft--;
        break;
    case 2:
        process2.instructionsLeft--;
        break;
    case 3:
        process3.instructionsLeft--;
        break;

    default:
        break;
    }
}

void updateProgramCounter(int processId)
{
    char valueStr[20];
    switch (processId)
    {
    case 1:
        process1.programCounter++;
        sprintf(valueStr, "%d", process1.programCounter);
        strcpy(Memory[PCB1_START + 3].value, valueStr);
        break;
    case 2:
        process2.programCounter++;
        sprintf(valueStr, "%d", process2.programCounter);
        strcpy(Memory[PCB2_START + 3].value, valueStr);
        break;
    case 3:
        process3.programCounter++;
        sprintf(valueStr, "%d", process3.programCounter);
        strcpy(Memory[PCB3_START + 3].value, valueStr);
        break;

    default:
        break;
    }
}
void init()
{
    // Initialize the global mutex
    initMutex(&userOutput);
    initMutex(&userInput);
    initMutex(&file);

    // Initialize the 4 priority queues
    initializeQueue(&Readyqueue1);

    // // initialize variable space
}

char *getCurrInstruction(int index)
{
    // Allocate memory for the string
    char *str = (char *)malloc(20 * sizeof(char));
    if (str == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }
    switch (index)
    {
    case 1:
        printf("The current instruction %d\n", process1.programCounter);
        strcpy(str, Memory[process1.memoryLowerBound + process1.programCounter].name);
        printf("%s a7a\n", str);
        break;
    case 2:
        printf("The current instruction %d\n", process2.programCounter);
        strcpy(str, Memory[process2.memoryLowerBound + process2.programCounter].name);
        printf("%s a7a\n", str);
        break;
    case 3:
        printf("The current instruction %d\n", process3.programCounter);
        strcpy(str, Memory[process3.memoryLowerBound + process3.programCounter].name);
        printf("%s a7a\n", str);
        break;    
    default:
        break;
    }
    // Copy a string into the allocated memory

    return str;
}
void executeProgram()
{
    cyclesLeft = Quantum;
    for (time = 0; time < 20; time++)
    {
        printf("---------------------------Instruction Cycle %d------------------------------------\n", time);
        if (time == arrival1)
        {
            initProcess(1);
            enqueue(&Readyqueue1, process1);
            printf("Process1 arrived for execution\n");
        }
        if (time == arrival2)
        {
            initProcess(2);
            enqueue(&Readyqueue1, process2);
            printf("Process2 arrived for execution\n");
        }
        // if (time == arrival3)
        // {
        //     initProcess(3);
        //     enqueue(&Readyqueue1, process3);
        //     printf("Process3 arrived for execution\n");
        // }
        if (!isEmpty(&Readyqueue1))
        {
            Process currProcess = peek(&Readyqueue1);
            char *instruction = getCurrInstruction(getId(currProcess));
            updateInstructionsLeft(getId(currProcess));
            updateProgramCounter(getId(currProcess));
            cyclesLeft--;
            printf("(process %d)", currProcess.processID);
            executeInstruction(currProcess, instruction);
            free(instruction);
            if (currProcess.instructionsLeft <= 0)
            {
                updateState(currProcess.processID, "Terminated");
                printf("\nprocess Terminated \n\n");
                dequeue(&Readyqueue1);
                display(&Readyqueue1);
            }
            if (cyclesLeft == 0 && currProcess.state != "Terminated")
            {
                printf("-----------process preampted----------\n");
                Process p = dequeue(&Readyqueue1);
                enqueue(&Readyqueue1, p);
                cyclesLeft = Quantum;
            }
        }
        // if (!(isEmpty(&Readyqueue1)))
        // {
        //     Process currProcess = dequeue(&Readyqueue1);
        //     if (currProcess.programCounter < Quantum - 1)
        //     {
        //         int test[] = {1, 4};
        //         executeInstruction("printFromTo 4 9");
        //         currProcess.instructionsLeft--;
        //         currProcess.programCounter++;
        //     }
        //     else if (currProcess.instructionsLeft <= 0)
        //     {
        //         // Process has finished execution
        //         // strcpy(currProcess.state, "Terminated");
        //         updateState(getId(currProcess), "Terminated");
        //     }
        //     else
        //     {
        //         enqueue(&Readyqueue1, currProcess);
        //     }
        // }
    }

    printf("program ended\n");
}
void executeInstruction(Process p, char instruction[])
{
    char command[20];
    sscanf(instruction, "%s", command);

    if (strcmp(command, "printFromTo") == 0)
    {
        printf("\n \n test \n \n %d", p.processID);
        char a[2], b[2];
        int start;
        int end;
        printf("ID \n \n ");
        sscanf(instruction, "printFromTo %s %s", a, b);
        if (strcmp(a, "a") == 0)
            start = p.memoryUpperBound - 2;
        // printf("Mashy el7al %d", start);
        if (strcmp(b, "b") == 0)
            end = p.memoryUpperBound - 1;
        start = atoi(Memory[start].value);
        end = atoi(Memory[end].value);
        printFromTo(start, end);
        // printf("\n \n end test \n \n");
    }

    else if (strcmp(command, "assign") == 0)
    {
        char var[1];
        char method[20];
        int index = 0;
        int matched = sscanf(instruction, "assign %s %s", var, method);
        if (strcmp(var, "a") == 0)
            index = p.memoryUpperBound - 2;
        else
            index = p.memoryUpperBound - 1;
        printf("\n\n\ndata assigned to address %d for process %d\n\n\n", index, p.processID);
        assign(index, method);
    }

    else if (strcmp(command, "writeFile") == 0)
    {
        char filename[20];
        int index;
        sscanf(instruction, "%*s %s %d", filename, &index);
        writeFile(filename, index);
    }

    else if (strcmp(command, "readFile") == 0)
    {
        char filename[20];
        int index;
        sscanf(instruction, "%*s %s %d", filename, &index);
        readFile(filename, index);
    }

    else if (strcmp(command, "semWait") == 0)
    {
        char mutexName[20];
        sscanf(instruction, "%*s %s", mutexName);
        if (strcmp(mutexName, "userInput") == 0)
        {
            semWait(&userInput, p);
        }
        else if (strcmp(mutexName, "file") == 0)
        {
            semWait(&file, p);
        }
        else if (strcmp(mutexName, "userOutput") == 0)
        {
            semWait(&userOutput, p);
        }
        else
        {
            printf("Unknown mutex: %s\n", mutexName);
        }
    }

    else if (strcmp(command, "semSignal") == 0)
    {
        char mutexName[20];
        sscanf(instruction, "%*s %s", mutexName);
        if (strcmp(mutexName, "userInput") == 0)
        {
            semSignal(&userInput);
        }
        else if (strcmp(mutexName, "file") == 0)
        {
            semSignal(&file);
        }
        else if (strcmp(mutexName, "userOutput") == 0)
        {
            semSignal(&userOutput);
        }
        else
        {
            printf("Unknown mutex: %s\n", mutexName);
        }
    }

    else
    {
        printf("Unknown instruction: %s\n", instruction);
    }
}
int main()
{
    init();

    for (int i = 1; i <= 4; i++)
    {
        if (i < 4)
        {
            printf("arrival time of process %d\n", i);
            if (i == 1)
                scanf("%d", &arrival1);
            else if (i == 2)
                scanf("%d", &arrival2);
            else if (i == 3)
                scanf("%d", &arrival3);
        }
        else
        {
            printf("Enter The RR Quantum\n");
            scanf("%d", &Quantum);
        }
    }

    executeProgram();

    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        if (strlen(Memory[i].name) > 0)
        {
            printf("Memory[%d]: Name = %s, Value = %s\n", i, Memory[i].name, Memory[i].value);
        }
    }
    // updateProgramCounter(2);
    // updateState(2, "Blocked");
    // updateInstructionsLeft(2);
    // display(&Readyqueue1);
    // printf("Process ID: %d\n", process1.processID);
    // printf("State: %s\n", process1.state);
    // printf("Priority: %d\n", process1.priority);
    // printf("Program Counter: %d\n", process1.programCounter);
    // printf("Instructions Left: %d\n", process1.instructionsLeft);
    // printf("lower bound %d\n", process1.memoryLowerBound);
    // printf("upper bound %d\n", process1.memoryUpperBound);
    // char *t = getCurrInstruction(process1);
    // printf("%s", t);
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
    // return 0;
}