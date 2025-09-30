#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char *argv[]){
    if (argc < 2){
        return 1;
    }
    char *word = argv[1]; 
    if (strlen(word) == 0){
        printf("\n");
        return 0;
    }
    int len = strlen(word);
    char *front, *end; 
    front = &word[0];
    end = &word[len-1];
    *front = tolower(*front);
    *end = tolower(*end);
    while (front < end){
        while (!isalpha(*front)){
            front++;
            *front = tolower(*front);
        }
        while (!isalpha(*end)){
            end--;
            *end = tolower(*end);
        }
        if (*front == *end){
            front++;
            end--;
            *front = tolower(*front);
            *end = tolower(*end);
        }
        else{
            printf("no\n");
            return 0;
        }
    }
    printf("yes\n");
    return 0;
}