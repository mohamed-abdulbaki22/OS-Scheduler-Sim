#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>
#include <string.h>

// Define bool type
typedef enum {
    false = 0,
    true = 1
} bool;

#define MAX_SIZE 100

// Define the queue structure
typedef struct {
    int items[MAX_SIZE];
    int front;
    int rear;
    unsigned int size;
} Queue;

// Initialize the queue
void initializeQueue(Queue *queue) {
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
}

// Check if the queue is empty
bool isEmpty(Queue *queue) {
    return (queue->size == 0);
}

// Check if the queue is full
bool isFull(Queue *queue) {
    return (queue->size == MAX_SIZE);
}

// Get the size of the queue
int size(Queue *queue) {
    return queue->size;
}

// Enqueue an element into the queue
void enqueue(Queue *queue, int value) {
    if (isFull(queue)) {
        printf("Queue is full. Cannot enqueue.\n");
        return;
    }
    queue->rear = (queue->rear + 1) % MAX_SIZE;
    queue->items[queue->rear] = value;
    queue->size++;
}

// Dequeue an element from the queue
int dequeue(Queue *queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty. Cannot dequeue.\n");
        return -1;
    }
    int value = queue->items[queue->front];
    queue->front = (queue->front + 1) % MAX_SIZE;
    queue->size--;
    return value;
}

// Display the queue
void display(Queue *queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty.\n");
        return;
    }
    printf("Queue elements: ");
    int i = queue->front;
    do {
        printf("%d ", queue->items[i]);
        i = (i + 1) % MAX_SIZE;
    } while (i != (queue->rear + 1) % MAX_SIZE);
    printf("\n");
}

// Define the mutex structure
typedef struct {
    bool flag;           // Mutex flag
    // int ownerID;         // Owner ID (if needed)
    Queue blockedqueue;  // Queue for blocked threads
} Mutex;

// Initialize the mutex
void initMutex(Mutex *mutex) {
    mutex->flag = false;  // Mutex is initially unlocked
    initializeQueue(&(mutex->blockedqueue));  // Initialize the blocked queue
}

// Wait operation on mutex
void semWait(Mutex *mutex, int processID) {
    if (!(mutex->flag)) {
        mutex->flag = true;
    } else {
        if (size(&(mutex->blockedqueue)) < MAX_SIZE) {
            enqueue(&(mutex->blockedqueue), processID);
        } else {
            printf("Error: Blocked queue for mutex is full.\n");
        }
    }
}

// Signal operation on mutex
void semSignal(Mutex *mutex) {
    if (!isEmpty(&(mutex->blockedqueue))) {
        int unblockedProcess = dequeue(&(mutex->blockedqueue));
        // Unblock the process (implementation dependent)
        printf("Unblocked process with ID %d\n", unblockedProcess);
        // Add unblocked process to the ready queue (implementation dependent)
    } else {
        mutex->flag = false;
    }
}

// Define the global mutex variables
Mutex userInput;
Mutex userOutput;
Mutex file;
Queue Readyqueue1,Readyqueue2,Readyqueue3,Readyqueue4;
// semWait(userInput)
// semSignal(userInput)
// semWait(file)
// semSignal(file)
int main() {
    // Initialize the global mutex
    initMutex(&userOutput);
    initMutex(&userInput);
    initMutex(&file);

    // Initialize the 4 priority queues
    initializeQueue(&(Readyqueue1));
    initializeQueue(&(Readyqueue2));
    initializeQueue(&Readyqueue3);
    initializeQueue(&Readyqueue4);

    char message[] = "Hello, world!\n";
    print(message);

    int a = 5;
    char b[] = "5";
    assign(&a, b);
    printf("a = %d\n", a);

    char str1[20], str2[] = "Hello";
    assign(str1, str2);
    printf("str1 = %s\n", str1);

    printFromTo(1, 5);

    return 0;
}




#define MEMORY_SIZE 60
#define MAX_PROCESSES 10
#define MAX_PRIORITY_LEVELS 4
#define MAX_INSTRUCTIONS 10
#define MAX_BLOCKED 10

typedef struct {
    int processID;
    char state[10];
    int priority;
    int programCounter;
    int memoryLowerBound;
    int memoryUpperBound;
    char instructions[MAX_INSTRUCTIONS][50];
} PCB;

// typedef struct {
//     int value;
//     int blockedQueue[MAX_BLOCKED];
//     int blockedCount;
// } Mutex;

// typedef struct {
//     PCB* queue[MAX_PROCESSES];
//     int count;
// } PriorityQueue;

// char memory[MEMORY_SIZE];
// int memoryIndex = 0;

// Mutex userInput = {1, {0}, 0};
// Mutex userOutput = {1, {0}, 0};
// Mutex fileAccess = {1, {0}, 0};

// PriorityQueue readyQueues[MAX_PRIORITY_LEVELS];
// PriorityQueue blockedQueue;

// void initializeQueues() {
//     for (int i = 0; i < MAX_PRIORITY_LEVELS; i++) {
//         readyQueues[i].count = 0;
//     }
//     blockedQueue.count = 0;
// }

// void addProcessToQueue(PCB* process, int priority) {
//     if (readyQueues[priority].count < MAX_PROCESSES) {
//         readyQueues[priority].queue[readyQueues[priority].count++] = process;
//     } else {
//         printf("Error: Priority queue %d is full.\n", priority);
//     }
// }

// PCB* getNextProcess() {
//     for (int i = 0; i < MAX_PRIORITY_LEVELS; i++) {
//         if (readyQueues[i].count > 0) {
//             PCB* nextProcess = readyQueues[i].queue[0];
//             for (int j = 1; j < readyQueues[i].count; j++) {
//                 readyQueues[i].queue[j - 1] = readyQueues[i].queue[j];
//             }
//             readyQueues[i].count--;
//             return nextProcess;
//         }
//     }
//     return NULL;
// }

// void semWait(Mutex* mutex, int processID) {
//     if (mutex->value > 0) {
//         mutex->value--;
//     } else {
//         if (mutex->blockedCount < MAX_BLOCKED) {
//             mutex->blockedQueue[mutex->blockedCount++] = processID;
//         } else {
//             printf("Error: Blocked queue for mutex is full.\n");
//         }
//     }
// }

// void semSignal(Mutex* mutex) {
//     if (mutex->blockedCount > 0) {   free
//         int unblockedProcess = mutex->blockedQueue[0];
//         // Unblock the process (implementation dependent)
//         for (int i = 1; i < mutex->blockedCount; i++) {
//             mutex->blockedQueue[i - 1] = mutex->blockedQueue[i];
//         }
//         mutex->blockedCount--;
//         // Add unblocked process to the ready queue (implementation dependent)
//     } else {
//         mutex->value++;
//     }
// }


////////////////////////////////////////////////////////////////////////////////
// void executeInstruction(PCB* process, char* instruction) {
//     if (strncmp(instruction, "print ", 6) == 0) {
//         semWait(&userOutput, process->processID);
//         printf("%s\n", instruction + 6);
//         semSignal(&userOutput);
//     } else if (strncmp(instruction, "assign ", 7) == 0) {
//         // Handle variable assignment
//     } else if (strncmp(instruction, "writeFile ", 10) == 0) {
//         semWait(&fileAccess, process->processID);
//         // Handle file writing
//         semSignal(&fileAccess);
//     } else if (strncmp(instruction, "readFile ", 9) == 0) {
//         semWait(&fileAccess, process->processID);
//         // Handle file reading
//         semSignal(&fileAccess);
//     } else if (strncmp(instruction, "printFromTo ", 12) == 0) {
//         int x, y;
//         sscanf(instruction + 12, "%d %d", &x, &y);
//         for (int i = x; i <= y; i++) {
//             printf("%d ", i);
//         }
//         printf("\n");
//     } else if (strncmp(instruction, "semWait ", 8) == 0) {
//         // Handle semaphore wait
//     } else if (strncmp(instruction, "semSignal ", 10) == 0) {
//         // Handle semaphore signal
//     }
// }

// void executeProcess(PCB* process) {
//     while (process->programCounter < MAX_INSTRUCTIONS && strlen(process->instructions[process->programCounter]) > 0) {
//         executeInstruction(process, process->instructions[process->programCounter]);
//         process->programCounter++;
//     }
//     strcpy(process->state, "Finished");
// }

// void schedule() {
//     PCB* currentProcess = getNextProcess();
//     while (currentProcess != NULL) {
//         executeProcess(currentProcess);
//         printf("Process %d executed.\n", currentProcess->processID);
//         currentProcess = getNextProcess();
//     }
// }

// void loadProcess(PCB* process, int id, int priority, char instructions[MAX_INSTRUCTIONS][50]) {
//     process->processID = id;
//     strcpy(process->state, "Ready");
//     process->priority = priority;
//     process->programCounter = 0;
//     process->memoryLowerBound = memoryIndex;
//     process->memoryUpperBound = memoryIndex + 10;
//     for (int i = 0; i < MAX_INSTRUCTIONS; i++) {
//         strcpy(process->instructions[i], instructions[i]);
//     }
//     memoryIndex += 10;
//     addProcessToQueue(process, priority);
// }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// int main() {
//     initializeQueues();

//     // Example processes
//     PCB process1;
//     char instructions1[MAX_INSTRUCTIONS][50] = {
//         "print Hello, World!",
//         "printFromTo 1 5",
//         "assign x 10",
//         "assign y 20",
//         "writeFile example.txt Hello, World!",
//         "readFile example.txt"
//     };
//     loadProcess(&process1, 1, 0, instructions1);

//     PCB process2;
//     char instructions2[MAX_INSTRUCTIONS][50] = {
//         "print Starting process 2",
//         "assign a 5",
//         "assign b 15",
//         "printFromTo 5 15",
//         "writeFile test.txt Sample data",
//         "readFile test.txt"
//     };
//     loadProcess(&process2, 2, 1, instructions2);

//     schedule();

//     return 0;
// }
