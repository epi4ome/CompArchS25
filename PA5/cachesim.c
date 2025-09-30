#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int hit = 0;
int miss = 0;
int read = 0;
int write = 0; 

int phit = 0;
int pmiss = 0;
int pread = 0;
int pwrite = 0; 

int powerTwo(int n){
    if ((n > 0) && ((n & (n - 1)) == 0)){
        return 1; 
    }
    return 0; 
}

int logTwo(int n){
    int log = -1;
    while (n > 0){
        log++;
        n >>= 1; 
    }
    return log;
}

void reset(int size, int *age, int x){
    for (int k = 0; k < size; k++){
        age[k] = age[k] + 1;
    }
    age[x] = 0; 
}

void findMax(int size, int *age, long int *cache, long int tag){
    int max = 0;
    for (int r = 1; r < size; r++) {
        if (age[r] > age[max]) {
            max = r;
        }
    }
    cache[max] = tag;
    reset(size, age, max);
}

void directMap(char f, long int *cache, long int setIndex, long int tag){
    if(cache[setIndex] == tag){
        hit++; 
    }
    else{
        miss++;
        read++;
        cache[setIndex] = tag;
    }
    if (f == 'W'){
        write++; 
    }
}

void directPre(char f, long int *cache, long int setIndex, long int tag, long int psetIndex, long int ptag){
    if(cache[setIndex] == tag){
        phit++; 
    }
    else{
        pmiss++;
        pread++;
        cache[setIndex] = tag;
        if (cache[psetIndex] != ptag){
            pread++;
            cache[psetIndex] = ptag; 
        }
    }
    if (f == 'W'){
        pwrite++; 
    }
}

void fullyAsso(char f, char *rp, long int *cache, int *age, long int tag, int setNum){
    int i = 0;
    while (i < setNum && cache[i] != -1){
        if (cache[i] == tag){
            hit++; 
            if (f == 'W'){
                write++; 
            }
            if (strcmp(rp, "lru") == 0){
                reset(setNum, age, i);
            }
            return; 
        }
        i++; 
    }
    miss++;
    read++;
    if (i < setNum){
        cache[i] = tag;
        reset(setNum, age, i);
    }
    else{
        findMax(setNum, age, cache, tag); 
    }
    if (f == 'W'){
        write++; 
    }
}

void fullyPre(char f, char *rp, long int *pcache, int *page, long int tag, long int ptag, int setNum){
    int i = 0;
    while (i < setNum && pcache[i] != -1){
        if (pcache[i] == tag){
            phit++; 
            if (f == 'W'){
                pwrite++; 
            }
            if (strcmp(rp, "lru") == 0){
                reset(setNum, page, i);
            }
            return; 
        }
        i++; 
    }
    pmiss++;
    pread++;
    if (i < setNum){
        pcache[i] = tag;
        reset(setNum, page, i);
    }
    else{
        findMax(setNum, page, pcache, tag); 
    }
    if (f == 'W'){
        pwrite++; 
    }
    int q = 0; 
    while (q < setNum && pcache[q] != -1){
        if (pcache[q] == ptag){
            return; 
        }
        q++; 
    }
    pread++;
    if (q < setNum){
        pcache[q] = ptag;
        reset(setNum, page, q);
    }
    else{
        findMax(setNum, page, pcache, ptag); 
    }
}

void nwayAsso(char f, char *rp, long int **cache2d, int **age2d, long int tag, long int setIndex, int assocN){
    int i = 0;
    while (i < assocN && cache2d[setIndex][i] != -1){
        if (cache2d[setIndex][i] == tag){
            hit++; 
            if (f == 'W'){
                write++; 
            }
            if (strcmp(rp, "lru") == 0){
                reset(assocN, age2d[setIndex], i);
            }
            return; 
        }
        i++; 
    }
    miss++;
    read++;
    if (i < assocN){
        cache2d[setIndex][i] = tag;
        reset(assocN, age2d[setIndex], i);
    }
    else{
        findMax(assocN, age2d[setIndex], cache2d[setIndex], tag); 
    }
    if (f == 'W'){
        write++; 
    }
}

void nwayPre(char f, char *rp, long int **pcache2d, int **page2d, long int tag, long int setIndex, long int ptag, long int psetIndex, int assocN){
    int i = 0;
    while (i < assocN && pcache2d[setIndex][i] != -1){
        if (pcache2d[setIndex][i] == tag){
            phit++; 
            if (f == 'W'){
                pwrite++; 
            }
            if (strcmp(rp, "lru") == 0){
                reset(assocN, page2d[setIndex], i);
            }
            return; 
        }
        i++; 
    }
    pmiss++;
    pread++;
    if (i < assocN){
        pcache2d[setIndex][i] = tag;
        reset(assocN, page2d[setIndex], i);
    }
    else{
        findMax(assocN, page2d[setIndex], pcache2d[setIndex], tag); 
    }
    if (f == 'W'){
        pwrite++; 
    }
    int q = 0; 
    while (q < assocN && pcache2d[psetIndex][q] != -1){
        if (pcache2d[psetIndex][q] == ptag){
            return; 
        }
        q++; 
    }
    pread++;
    if (q < assocN){
        pcache2d[psetIndex][q] = ptag;
        reset(assocN, page2d[psetIndex], q);
    }
    else{
        findMax(assocN, page2d[psetIndex], pcache2d[psetIndex], ptag); 
    }
}

void printResult(){
    printf("Prefetch 0\n");
    printf("Memory reads: %d\n", read);
    printf("Memory writes: %d\n", write);
    printf("Cache hits: %d\n", hit);
    printf("Cache misses: %d\n", miss);
    printf("Prefetch 1\n");
    printf("Memory reads: %d\n", pread);
    printf("Memory writes: %d\n", pwrite);
    printf("Cache hits: %d\n", phit);
    printf("Cache misses: %d\n", pmiss);
}

int main(int argc, char *argv[]){
    if (argc < 2){
        return 1;
    }
    int cacheSize = atoi(argv[1]);
    char *assoc = argv[2];
    char *rp = argv[3];
    int blockSize = atoi(argv[4]);
    FILE *file = fopen(argv[5], "r");
    int setNum = cacheSize/blockSize;
    int method = 0; 
    int assocN; 

    if (powerTwo(cacheSize) == 0 || powerTwo(blockSize) == 0){
        return 1; 
    }
    if (strncmp(assoc, "assoc:", 6) == 0){
        assocN = atoi(assoc + 6); 
        if (powerTwo(assocN) == 0){
            return 1; 
        }
        method = 1; 
        setNum /= assocN;
    }
    else if (strcmp(assoc, "direct") == 0){
        method = 2;
    }

    int bShift = logTwo(blockSize);
    int sBits = logTwo(setNum);

    long int *cache, *pcache, **cache2d, **pcache2d;
    int *age, *page, **age2d, **page2d; 

    if (method == 0 || method == 2){
        cache = malloc(sizeof(long int) * setNum);
        pcache = malloc(sizeof(long int) * setNum);
        age = malloc(sizeof(int) * setNum); 
        page = malloc(sizeof(int) * setNum); 
    }
    else{
        cache2d = malloc(setNum * sizeof(long int*));
        age2d = malloc(setNum * sizeof(int*));
        pcache2d = malloc(setNum * sizeof(long int*));
        page2d = malloc(setNum * sizeof(int*));
    }

    for (int i = 0; i < setNum; i++){
        if (method == 1){
            cache2d[i] = malloc(assocN * sizeof(long int));
            age2d[i] = malloc(assocN * sizeof(int));
            pcache2d[i] = malloc(assocN * sizeof(long int));
            page2d[i] = malloc(assocN * sizeof(int));
            for (int j = 0; j < assocN; j++){
                cache2d[i][j] = -1; 
                age2d[i][j] = 0;
                pcache2d[i][j] = -1; 
                page2d[i][j] = 0;
            }
        }
        else{
            cache[i] = -1;
            pcache[i] = -1; 
            age[i] = 0;
            page[i] = 0;
        }
    }

    char function; 
    long int address, blockId, setIndex, tag, paddress, pblockId, psetIndex, ptag; 
    while (fscanf(file, "%*x: %c %lx", &function, &address) == 2) {
        if (method == 2){
            blockId = address >> bShift;
            
            setIndex = blockId & ((1 << sBits) - 1);
            tag = blockId >> sBits; 

            paddress = address + blockSize;  
            pblockId = paddress >> bShift;

            psetIndex = pblockId & ((1 << sBits) - 1);
            ptag = pblockId >> sBits; 
            directMap(function, cache, setIndex, tag);
            directPre(function, pcache, setIndex, tag, psetIndex, ptag);
        } 
        if (method == 1){
            blockId = address >> bShift;

            setIndex = blockId & ((1 << sBits) - 1);
            tag = blockId >> sBits; 

            paddress = address + blockSize;  
            pblockId = paddress >> bShift;

            psetIndex = pblockId & ((1 << sBits) - 1);
            ptag = pblockId >> sBits; 
            nwayAsso(function, rp, cache2d, age2d, tag, setIndex, assocN);
            nwayPre(function, rp, pcache2d, page2d, tag, setIndex, ptag, psetIndex, assocN);

        }
        if (method == 0){
            tag = address >> bShift; 
            paddress = address + blockSize; 
            ptag = paddress >> bShift; 
            fullyAsso(function, rp, cache, age, tag, setNum);
            fullyPre(function, rp, pcache, page, tag, ptag, setNum);
        }
    }
    printResult();
    if (method == 1){
        for (int i = 0; i < setNum; i++) {
            free(cache2d[i]);
            free(age2d[i]);
            free(pcache2d[i]);
            free(page2d[i]);
        }
        free(cache2d);
        free(age2d);
        free(pcache2d);
        free(page2d);
    }
    else{
        free(cache);
        free(pcache);
        free(age);
        free(page);
    }
}