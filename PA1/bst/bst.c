#include <stdio.h> 
#include <ctype.h>
#include <stdlib.h>

struct Node{
    int val; 
    struct Node *left, *right; 
};

struct Node* createNode(int num){
    struct Node* n = (struct Node*)malloc(sizeof(struct Node));
    n->val = num; 
    n->left = NULL; 
    n->right = NULL; 
    return n; 
}

struct Node* findNode(int num, struct Node *root){
    struct Node *ptr = root; 
    while (ptr != NULL){
        if (ptr->val < num){
            ptr = ptr->right;
        }
        else if (ptr->val > num){
            ptr = ptr->left;
        }
        else{
            return ptr;
        }
    }
    return NULL;
}

struct Node* recInsert(int num, struct Node *n){
    if (n == NULL){
        struct Node *newN = createNode(num);
        return newN;
    }
    if (n->val < num){
        n->right = recInsert(num, n->right);
    }
    else if (n->val > num){
        n->left = recInsert(num, n->left);
    }
    return n;
}

void insertNode(int num, struct Node **n){
    if (findNode(num, *n) == NULL){
        *n = recInsert(num, *n);
        printf("inserted\n");
    }
    else{
        printf("not inserted\n");
    }
}

void printBST(struct Node *n){
    if (n == NULL){
        return;
    }
    printf("(");
    printBST(n->left);
    printf("%d", n->val);
    printBST(n->right);
    printf(")");
}

struct Node* findMax(struct Node* n){
    if (n->right == NULL){
        return n;
    }
    else{
        return findMax(n->right);
    }
}

struct Node* recDelete(int num, struct Node* n){
    if (n == NULL){
        return NULL;
    }
    if (n->val < num){
        n->right = recDelete(num, n->right);
    }
    else if (n->val > num){
        n->left = recDelete(num, n->left);
    }
    else{
        if (n->left == NULL){
            struct Node* save = n->right; 
            free(n);
            return save;
        }
        if (n->right == NULL){
            struct Node* save = n->left; 
            free(n);
            return save;
        }
        struct Node* max = findMax(n->left);
        n->val = max->val; 
        n->left = recDelete(max->val, n->left);
    }
    return n; 
}

void deleteNode(int num, struct Node **n){
    if (findNode(num, *n) != NULL){
        *n = recDelete(num, *n);
        printf("deleted\n");
    }
    else{
        printf("absent\n");
    }
}

int main(){
    char method; 
    int val; 
    struct Node *root = NULL;
    while (scanf("%c %d", &method, &val) != -1){
        if (method == 'p'){
            if (root == NULL){
                printf("\n");
            }
            else{
                printBST(root);
                printf("\n");
            }
        }
        else if (method == 's'){
            if (findNode(val, root) == NULL){
                printf("absent\n");
            }
            else{
                printf("present\n");
            }
        }
        else if (method == 'i'){
            insertNode(val, &root); 
        }
        else if (method == 'd'){
            deleteNode(val, &root);
        }
    }
    return 0; 
}