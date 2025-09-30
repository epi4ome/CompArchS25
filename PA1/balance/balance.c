#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    if (argc < 2){
        return 1;
    }
    char *word = argv[1];
    int len = strlen(word);
    char *stack = (char*)malloc(len);
    int size = 0;
    for (int i = 0; i < len; i++){
        if (word[i] == '{'){
            stack[size] = '}';
            size++; 
        }
        else if (word[i] == '['){
            stack[size] = ']';
            size++; 
        }
        else if (word[i] == '('){
            stack[size] = ')';
            size++; 
        }
        else if (size != 0 && word[i] == stack[size-1]){
            stack[size] = '\0';
            size--; 
        }
        else if (word[i] == ')' || word[i] == '}' || word[i] == ']'){
            printf("%d: %c\n", i, word[i]);
            free(stack);
            return EXIT_FAILURE;
        }
    }
    if (size == 0){
        free(stack);
        return EXIT_SUCCESS;
    }
    else{
        char *reverse = (char*)malloc(size + 1);
        for (int x = 0; x < size; x++){
            reverse[x] = stack[size-x-1];
        }
        reverse[size] = '\0';
        printf("open: %s\n", reverse);
        free(stack);
        free(reverse);
        return EXIT_FAILURE; 
    }
}