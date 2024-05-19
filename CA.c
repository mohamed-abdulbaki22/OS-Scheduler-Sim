#include <stdio.h>
#include <stdint.h>

int8_t registerFile [64] ;
unsigned short int instructionMemory[1024] ;
int numberofInstructions = 0;
int8_t dataMemory [2048];
int pc = 0;
int SREG [8];
unsigned short int fetched ;
unsigned short int toBeDecoded ;
uint8_t fCount = 0;
uint8_t opcode = 0;  // bits15:12
uint8_t rs = 0;      // bits11:6
uint8_t rt = 0;      // bits5:0
int8_t imm = 0;
uint8_t address = 0;
int numOfCycles=0;
int registerBeforChange;
int memoryBeforChange;


int chekcarry(uint8_t r1,uint8_t r2){
    if(((uint32_t)(r1+r2) & 0b100000000) > 1){
        return 1;
    }
    return 0;
}

int checkoverflow(int8_t r1 , int8_t r2 , int opp){
    int16_t result = 0;
    
    if(opp == 0){
        result = r1 + r2;
        if(r1 > 0 & r2 > 0 & (int8_t)result < 0){
            return 1;
        }
        else if (r1 < 0 & r2 < 0 & (int8_t)result >= 0)
        {
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        result=r1-r2;
        if (r1 > 0 & r2 < 0 & (int8_t)result < 0)
        {
            return 1;
        }
        else if (r1 < 0 & r2 > 0 & (int8_t)result >= 0)
        {
            return 1;
        }
        else{
            return 0;
        }
        
    }
}


void parseText(char * filename){
    char opcode[5];
    char var1[3];
    char var2[6];
    int opcodeI;
    int var1I;
    int var2I;

    FILE *file = fopen(filename , "r");
    if(!file){
        perror("Failed to open instruction file");
        exit(1);
    }

    char line[32];
    int count = 0;

    // Loop through each line in the file
    while (fgets(line, sizeof(line), file) != NULL && count < 1024) {
        // Trim trailing newline character
        line[strcspn(line, "\n")] = '\0';
    int num_parsed = sscanf(line, "%s %s %s", 
                                opcode, 
                                var1, 
                                var2);
    if (num_parsed != 3) {
            printf("Invalid instruction format on line %d: %s\n", count + 1, line);
            continue;
    }
    if (strcmp(opcode,"ADD")==0){
        opcodeI = 0;
    }
    else if (strcmp(opcode,"SUB")==0)
    {
        opcodeI = 1;
    }
    else if (strcmp(opcode,"MUL")==0)
    {
        opcodeI = 2;
    }
    else if (strcmp(opcode,"MOVI")==0)
    {
        opcodeI = 3;
    }
    else if (strcmp(opcode,"BEQZ")==0)
    {
        opcodeI = 4;
    }
    else if (strcmp(opcode,"ANDI")==0)
    {
        opcodeI = 5;
    }
    else if (strcmp(opcode,"EOR")==0)
    {
       opcodeI = 6;
    }
    else if (strcmp(opcode,"BR")==0)
    {
        opcodeI = 7;
    }
    else if (strcmp(opcode,"SAL")==0)
    {
        opcodeI = 8;
    }
    else if (strcmp(opcode,"SAR")==0)
    {
        opcodeI = 9;
    }
    else if (strcmp(opcode,"LDR")==0)
    {
        opcodeI = 10;
    }
    else if (strcmp(opcode,"STR")==0)
    {
        opcodeI = 11;
    }
    else{
        opcodeI=12;
    }
    opcodeI = opcodeI << 12;
    char * temp1 = var1;
    char * temp2 = var2;
    temp1 +=1 ; 
    var1I = atoi(temp1);
    var1I = var1I << 6;
    if(var2[0]=='R'){
        temp2 +=1 ; 
        var2I = atoi(temp2);
    }
    else{
        var2I = atoi(var2);
    }
    opcodeI = opcodeI + var1I + var2I;
   
    instructionMemory[count] = opcodeI;
    
    
    count++;
    
}
numberofInstructions=count;
fclose(file);    
}


// Binary int format in c is 0b00000000000000000000000000000000 (32 bits)

void fetch() {
    if(fCount!=0){
        toBeDecoded=fetched;
    }
    fetched=instructionMemory[pc++];
    fCount++;
    
}

void decode() {
        int instruction = toBeDecoded;
        opcode = instruction >> 12;
        rs = (instruction & 0b0000111111000000) >> 6;
        rt= (instruction & 0b0000000000111111) ;
        imm=(instruction & 0b0000000000111111) ;
        if(imm >> 5 == 1){
            imm = imm | 0b1111111111000000 ;
        }
        address=(instruction & 0b0000000000111111) ;
        
        // printf("opcode decoded : %i \n",opcode);
}
void execute (){
    // printf("opcode : %i \n",opcode);
    int var1=rs;
    int var2=rt;
    
    switch (opcode)
    {
    case 0:
        registerBeforChange = registerFile[var1];
        SREG[4] = chekcarry(registerFile[var1],registerFile[var2]);
        SREG[3] = checkoverflow(registerFile[var1],registerFile[var2],0);
        registerFile[var1]+=registerFile[var2];
        if(registerFile[var1]==0){
            SREG[0] = 1;
        }
        if (registerFile[var1]<0)
        {
            SREG[2] = 1;
        }
         SREG[1]= SREG[2] ^ SREG[3];
        printf("R%i : %i \n",rt,registerFile[var2]);
        printf("register %i before change : %i \n",rs,registerBeforChange);
        printf("register %i after change : %i \n",rs,registerFile[rs]);
        break;
    case 1:
        registerBeforChange = registerFile[var1];
        SREG[3] = checkoverflow(registerFile[var1],registerFile[var2],1);
        registerFile[var1]-=registerFile[var2];
        if(registerFile[var1]==0){
            SREG[0] = 1;
        }
        if (registerFile[var1]<0)
        {
            SREG[2] = 1;
        }
         SREG[1]= SREG[2] ^ SREG[3];
         printf("R%i : %i \n",rt,registerFile[var2]);
        printf("register %i before change : %i \n",rs,registerBeforChange);
        printf("register %i after change : %i \n",rs,registerFile[rs]);
        break;
    case 2:

        registerBeforChange = registerFile[var1];
        registerFile[var1]*=registerFile[var2];
        if(registerFile[var1]==0){
            SREG[0] = 1;
        }
        if (registerFile[var1]<0)
        {
            SREG[2] = 1;
        }
        printf("R%i : %i \n",rt,registerFile[var2]);
        printf("register %i before change : %i \n",rs,registerBeforChange);
        printf("register %i after change : %i \n",rs,registerFile[rs]);
        break;
    case 3:
        registerBeforChange = registerFile[var1];
        registerFile[var1]=imm;
        printf("immediate : %i\n",imm);
        printf("register %i before change : %i \n",rs,registerBeforChange);
        printf("register %i after change : %i \n",rs,registerFile[rs]);
        break;    
    case 4:
        int temp=pc;
        printf("R%i : %i \n",rs,registerFile[var1]);
        if(registerFile[var1]==0){
            pc=pc+imm-2;
            fCount=0;
            printf("A jump has occured\n");
        }
        else
            printf("The register value is not zero\n");
        break;
    case 5:
        registerBeforChange = registerFile[var1];
        registerFile[var1] = registerFile[var1] & imm;
        if(registerFile[var1]==0){
            SREG[0] = 1;
        }
        if (registerFile[var1]<0)
        {
            SREG[2] = 1;
        }
        printf("immediate : %i\n",imm);
        printf("register %i before change : %i \n",rs,registerBeforChange);
        printf("register %i after change : %i \n",rs,registerFile[rs]);
        break;
    case 6:
        registerBeforChange = registerFile[var1];
        registerFile[var1] = registerFile[var1] ^ registerFile[var2]; 
        if(registerFile[var1]==0){
            SREG[0] = 1;
        }
        if (registerFile[var1]<0)
        {
            SREG[2] = 1;
        }
        printf("R%i : %i \n",rt,registerFile[var2]);
        printf("register %i before change : %i \n",rs,registerBeforChange);
        printf("register %i after change : %i \n",rs,registerFile[rs]);   
        break; 
    case 7:
        int tmp;
        tmp = registerFile[var1] << 6;
        tmp += registerFile[var2];
        pc = tmp;
        fCount=0;
        printf("R%i : %i \n",rs,registerFile[var1]);
        printf("R%i : %i \n",rt,registerFile[var2]);
        printf("A branch has occured\n");
        break;      
    case 8:
        registerBeforChange = registerFile[var1];
        registerFile[var1] = registerFile[var1] << (unsigned)imm;
        if(registerFile[var1]==0){
            SREG[0] = 1;
        }
        if (registerFile[var1]<0)
        {
            SREG[2] = 1;
        }
        printf("immediate : %i\n",imm);
        printf("register %i before change : %i \n",rs,registerBeforChange);
        printf("register %i after change : %i \n",rs,registerFile[rs]);
        break;
    case 9:
        registerBeforChange = registerFile[var1];
        registerFile[var1] = registerFile[var1] >> (unsigned) imm;
        if(registerFile[var1]==0){
            SREG[0] = 1;
        }
        if (registerFile[var1]<0)
        {
            SREG[2] = 1;
        }
        printf("immediate : %i\n",imm);
        printf("register %i before change : %i \n",rs,registerBeforChange);
        printf("register %i after change : %i \n",rs,registerFile[rs]);
        break; 
    case 10:
        registerBeforChange = registerFile[var1];
        registerFile[var1] = dataMemory[address];
        printf("Memory address %i : %i \n",rt,dataMemory[address]);
        printf("register %i before change : %i \n",rs,registerBeforChange);
        printf("register %i after change : %i \n",rs,registerFile[rs]);
        break;
    case 11:
        memoryBeforChange = dataMemory[address];
        dataMemory[address] = registerFile[var1];
        printf("R%i : %i\n",rs,registerFile[var1]);
        printf("memory %i before change : %i \n",rt,memoryBeforChange);
        printf("memory %i after change : %i \n",rt,dataMemory[rt]);
        break;
    default:
        break;
    }
}
void run(){
    int cycle =1;
    printf("cycle %i\n",cycle++);
    printf("PC : %i \n",pc);
    fetch();
    printf("fetch instruction %i : %i \n",pc,instructionMemory[pc-1]);
    printf("There is no instruction neither decoded nor executed in this cycle \n");
    printf("-----------------------------\n");
    while (fetched != 0){
        printf("cycle %i\n",cycle++);
        printf("PC : %i \n",pc);
        if(fCount==0){
            fetch();
            printf("fetch instruction %i : %i \n",pc,instructionMemory[pc-1]);
            printf("There is no instruction neither decoded nor executed in this cycle \n");
        }
        else if (fCount==1){
            fetch();
            if(fetched == 0)
                printf("There is no instruction fetched in this cycle \n");
            else
                printf("fetch instruction %i : %i \n",pc,instructionMemory[pc-1]);
            decode();
            printf("decode instruction %i : %i \n",pc-1,instructionMemory[pc-2]);
            printf("There is no instruction executed in this cycle \n");

        }
        else{
            fetch();
            if(fetched==0)
                printf("There is no instruction fetched in this cycle \n");
            else    
                printf("fetch instruction %i : %i \n",pc,instructionMemory[pc-1]);
            printf("decode instruction %i : %i \n",pc-1,instructionMemory[pc-2]);
            printf("execute instruction %i : %i \n",pc-2,instructionMemory[pc-3]);        
            execute();
            decode();
        }
        printf("SREG : \n");
        printf("Carry iS : %i \n",SREG[4]);
        printf("Overflow iS : %i \n",SREG[3]);
        printf("Negative bit iS : %i \n",SREG[2]);
        printf("Sign Flag iS : %i \n",SREG[1]);
        printf("Zero Flag iS : %i \n",SREG[0]);
        printf("-----------------------------\n");
    }
    printf("cycle %i\n",cycle++);
    printf("There is no instruction neither fetched nor decoded in this cycle \n");
    printf("execute instruction %i : %i \n",pc-1,instructionMemory[pc-2]);
    execute();
    
    printf("-----------------------------------\n");
    printf("There are no more instructions in the instruction memory\n");
    printf("PC : %i \n",pc);
    printf("SREG : \n");
    printf("Carry iS : %i \n",SREG[4]);
    printf("Overflow iS : %i \n",SREG[3]);
    printf("Negative bit iS : %i \n",SREG[2]);
    printf("Sign Flag iS : %i \n",SREG[1]);
    printf("Zero Flag iS : %i \n",SREG[0]);
    printf("-----------------------------------\n");



    // printf("Registers :\n");
    // for(int i =0 ; i<64 ; i++){
    //     printf("R%i = %i ",i,registerFile[i]); 
    // }
    // printf("\n -----------------------------------\n");

    // printf("Instuction memory :\n");
    // for(int i =0 ; i<1024 ; i++){
    //    printf("%i = %i ",i,instructionMemory[i]);    
    // }
    //  printf("\n-----------------------------------\n");
    // printf("Data memory :\n");
    // for(int i =0 ; i<2048 ; i++){
    //     printf("%i = %i ",i,dataMemory[i]);
    // }
    //   printf("\n-----------------------------------\n");
}


void main() {
    parseText("data.txt") ; 
    registerFile[1]=2;
    registerFile[2]=4;
    registerFile[3]=3;
    registerFile[4]=4;
    registerFile[5]=6;
    registerFile[6]=1;
    dataMemory[12]=9;
    run();
}