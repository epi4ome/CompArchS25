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
    char upper[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char lower[] = "abcdefghijklmnopqrstuvwxyz";
    int len = strlen(word);
    char *locate; 
    int x;
    for (int i = 0; i < len; i++){
        if (strchr(upper, word[i]) != NULL){
            locate = strchr(upper, word[i]);
            x = locate - upper; 
            putchar(upper[(x + 13) % 26]); 
        }
        else if (strchr(lower, word[i]) != NULL){
            locate = strchr(lower, word[i]);
            x = locate - lower; 
            putchar(lower[(x + 13) % 26]); 
        }
        else {
            putchar(word[i]);
        }
    }
    printf("\n");
    return 0;
}