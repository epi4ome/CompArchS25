#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef enum{
    AND, OR, NAND, NOR, XOR, NOT, PASS, DECODER, MULTIPLEXER
} kind_t;

struct gate{
    kind_t kind;
    int inout[3]; 
    int inout2[2];
    int size; 
    int *inputs; 
    int *outputs;
    int *selectors; 
    int output; 
};

char* strdup(const char *c){
    char *dup = malloc(strlen(c) + 1);

    if (dup != NULL)
       strcpy(dup, c);

    return dup;
}

int NodeSize = 0;

struct Node{
    char* name; 
    int index; 
    struct Node *next; 
}; 

struct Node* createNode(char *n, int i){
    struct Node *new = (struct Node*)malloc(sizeof(struct Node)); 
    new->name = strdup(n);
    new->index = i;
    new->next = NULL;
    NodeSize++; 
    return new;
}

kind_t convert(char *name){
    if (strcmp(name, "AND") == 0) return AND;
    if (strcmp(name, "OR") == 0) return OR;
    if (strcmp(name, "NAND") == 0) return NAND;
    if (strcmp(name, "NOR") == 0) return NOR;
    if (strcmp(name, "XOR") == 0) return XOR;
    if (strcmp(name, "NOT") == 0) return NOT;
    if (strcmp(name, "PASS") == 0) return PASS;
    if (strcmp(name, "DECODER") == 0) return DECODER;
    if (strcmp(name, "MULTIPLEXER") == 0) return MULTIPLEXER;
    else return -1; 
}

int addVal(struct Node **save, char *add){
    if (*save == NULL){
        *save = createNode("0", 0);
        struct Node *newNode = createNode("1", 1);
        (*save)->next = newNode;
    } 
    struct Node *ptr = *save;  
    while (ptr->next != NULL){
        if (strcmp(ptr->name, add) == 0){
            return ptr->index; 
        }
        ptr = ptr->next;
    }
    if (strcmp(ptr->name, add) != 0){
        struct Node *newNode = createNode(add, NodeSize);
        ptr->next = newNode; 
        return newNode->index; 
    }
    else{
        return ptr->index; 
    }
}

struct gate* createGate(FILE *file, kind_t type, int count, struct Node **save){
    struct gate *add = (struct gate*)malloc(sizeof(struct gate));
    char temp[17];
    if (type == DECODER){
        int *inListd = (int*)malloc(count * sizeof(int));
        int *outListd = (int*)malloc((1<<count) * sizeof(int));
        for (int i = 0; i < count; i++){
            fscanf(file, "%16s", temp);
            inListd[i] = addVal(save, temp);
        }
        for (int k = 0; k < (1 << count); k++){
            fscanf(file, "%16s", temp);
            outListd[k] = addVal(save, temp); 
        }
        add->kind = type; 
        add->size = count; 
        add->inputs = inListd; 
        add->outputs = outListd;
        return add; 
    }
    else if (type == MULTIPLEXER){
        int *inListm = (int*)malloc((1<<count) * sizeof(int));
        int outListm;
        int *selectm = (int*)malloc(count * sizeof(int));
        for (int i = 0; i < (1 << count); i++){
            fscanf(file, "%16s", temp);
            inListm[i] = addVal(save, temp); 
        }
        for (int k = 0; k < count; k++){
            fscanf(file, "%16s", temp);
            selectm[k] = addVal(save, temp);
        }
        fscanf(file, "%16s", temp); 
        outListm = addVal(save, temp); 
        add->kind = type; 
        add->size = count; 
        add->inputs = inListm; 
        add->selectors = selectm; 
        add->output = outListm;
        return add; 
    } 
    else if (type == PASS || type == NOT){
        for (int i = 0; i < 2; i++){
            fscanf(file, "%16s", temp);
            add->kind = type; 
            add->inout2[i] = addVal(save, temp);
        }
        return add; 
    }
    else {
        for (int i = 0; i < 3; i++){
            fscanf(file, "%16s", temp);
            add->kind = type; 
            add->inout[i] = addVal(save, temp);
        }
        return add; 
    }
    return NULL;
}

int decode(int var[], int *val, int size) {
    int index = 0;
    for (int i = 0; i < size; i++) {
        index = (index << 1) | val[var[i]];
    }
    return index;
}

void computeGate(struct gate *gate, int *val){
    int in1 = gate->inout[0];
    int in2 = gate->inout[1];
    int out = gate->inout[2];
    switch (gate->kind){
        case AND:
            val[out] = val[in1] & val[in2];
            break;
        case OR: 
            val[out] = val[in1] | val[in2];
            break;
        case NAND:
            val[out] = !(val[in1] & val[in2]);
            break;
        case NOR:
            val[out] = !(val[in1] | val[in2]);
            break;
        case XOR:
            val[out] = val[in1] ^ val[in2];
            break;
        case NOT:
            val[gate->inout2[1]] = !(val[gate->inout2[0]]);
            break;
        case PASS:
            val[gate->inout2[1]] = val[gate->inout2[0]];
            break;
        case DECODER:
            int outputNum = 1 << gate->size; 
            int find1 = decode(gate->inputs, val, gate->size);
            for (int i = 0; i < outputNum; i++){
                val[gate->outputs[i]] = 0; 
            }
            val[gate->outputs[find1]] = 1; 
            break;
        case MULTIPLEXER:
            int findVar = decode(gate->selectors, val, gate->size);
            val[gate->output] = val[gate->inputs[findVar]];
            break;
    }
}

int findIndex(char *valName, struct Node *save){
    struct Node *pointer = save; 
    while (pointer != NULL){
        if (strcmp(pointer->name, valName) == 0){
            return pointer->index; 
        }
        pointer = pointer->next; 
    }
    return -1;
}

int main(int argc, char* argv[]){
    if (argc < 2){
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    char si[17], **ivar, so[17], **ovar;
    int inum, onum; 
    
    fscanf(file, "%16s", si);
    fscanf(file, "%d", &inum);
    ivar = (char**)malloc(inum * sizeof(char*));
    for (int i = 0; i < inum; i++){
        ivar[i] = (char*)malloc(17*sizeof(char));
        fscanf(file, "%16s", ivar[i]);
    }

    fscanf(file, "%16s", so);
    fscanf(file, "%d", &onum);
    ovar = (char**)malloc(onum * sizeof(char*));
    for (int i = 0; i < onum; i++){
        ovar[i] = (char*)malloc(17*sizeof(char));
        fscanf(file, "%16s", ovar[i]);
    }

    char current[17]; 
    int circSize = 0; 
    struct gate **circuit = NULL; 
    struct Node *saveNode = NULL; 
    
    while (fscanf(file, "%16s", current) > 0){
        circuit = realloc(circuit, (circSize + 1) * sizeof(struct gate*)); 
        kind_t gateName = convert(current);
        if (gateName == DECODER || gateName == MULTIPLEXER){
            int varCount; 
            fscanf(file, "%d", &varCount);
            circuit[circSize] = createGate(file, gateName, varCount, &saveNode); 
        }
        else{
            circuit[circSize] = createGate(file, gateName, 0, &saveNode); 
        }
        circSize++; 
    }
 
    int *values = (int*)malloc(NodeSize * sizeof(int));
    values[0] = 0; 
    values[1] = 1;
    int tableSize = 1 << inum; 
    for (int i = 0; i < tableSize; i++){
        for (int j = 0; j < inum; j++) {
            int bit = 1 & (i >> (inum - 1 - j)); 
            int idx = findIndex(ivar[j], saveNode);
            values[idx] = bit;
        }

        for (int k = 0; k < circSize; k++) {
            computeGate(circuit[k], values);
        }

        for (int j = 0; j < inum; j++) {
            printf("%d ", values[findIndex(ivar[j], saveNode)]);
        }
        printf("| ");
    
        for (int j = 0; j < onum - 1; j++) {
            printf("%d ", values[findIndex(ovar[j], saveNode)]);
        }
        
        printf("%d\n", values[findIndex(ovar[onum-1], saveNode)]);
    }

    for (int i = 0; i < inum; i++){
        free(ivar[i]);
    }
    for (int i = 0; i < onum; i++){
        free(ovar[i]);
    }
    free(ivar);
    free(ovar);
    
    for (int i = 0; i < circSize; i++) {
        if (circuit[i]->kind == DECODER) {
            free(circuit[i]->inputs);
            free(circuit[i]->outputs);
        }
        if (circuit[i]->kind == MULTIPLEXER) {
            free(circuit[i]->inputs);
            free(circuit[i]->selectors);
        }
        free(circuit[i]);
    }
    free(circuit);
    free(values);

    struct Node *curr = saveNode;
    while (curr != NULL) {
        struct Node *next = curr->next;
        free(curr->name);
        free(curr);
        curr = next;
    }
}   