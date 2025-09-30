#include <stdio.h> 
#include <stdlib.h>

struct Node{
    int val; 
    struct Node *next; 
};

struct Node* createNode(int num){
    struct Node* n = (struct Node*)malloc(sizeof(struct Node));
    n->val = num; 
    n->next = NULL; 
    return n; 
}

void insert(struct Node **list, int num){
    struct Node* add = createNode(num);
    if (*list == NULL){
        *list = add; 
        return;
    }
    if (add->val < (*list)->val){
        add->next = *list;
        *list = add; 
        return;
    }
    struct Node *ptr = *list;
    while (ptr->next != NULL){
        if (ptr->val == add->val){
            free(add);
            return;
        }
        if (ptr->next->val > add->val){
            add->next = ptr->next;
            ptr->next = add; 
            return;
        }
        ptr = ptr->next; 
    }
    if (ptr->val != add->val){
        ptr->next = add; 
    }
    else {
        free(add);
    }
}

void delete(struct Node **list, int num){
    struct Node *ptr = *list; 
    if (ptr == NULL){
        return; 
    }
    if (ptr->val == num){
        *list = ptr->next;
        free(ptr);
        return; 
    }
    while (ptr->next != NULL){
        if (ptr->next->val == num){
            struct Node *save = ptr->next; 
            ptr->next = ptr->next->next; 
            free(save);
            return;
        }
        ptr = ptr->next;
    }
}

void printList(struct Node **list){
    if (*list == NULL){
        printf("0 :\n");
        return;
    }
    int count = 0; 
    struct Node *ptr = *list;
    while (ptr != NULL){
        count++;
        ptr = ptr->next; 
    }
    printf("%d :", count);
    ptr = *list; 
    while (ptr != NULL){
        printf(" %d", ptr->val);
        ptr = ptr->next;
    }
    printf("\n");
}

int main(){
    int value; 
    char method; 
    struct Node *list = NULL; 
    while (scanf("%c %d", &method, &value) != -1){
        if (method == 'i'){
            insert(&list, value); 
            printList(&list);
        }
        else if (method == 'd'){
            delete(&list, value);
            printList(&list);
        }
    }
    return 0; 
}