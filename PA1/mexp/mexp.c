#include <stdio.h> 
#include <ctype.h>
#include <stdlib.h>

int** createMatrix(int d){
    int** matrix = (int**)malloc(d * sizeof(int*));
    int* data = (int *)malloc(d * d * sizeof(int));
    for (int i = 0; i < d; i++){
        matrix[i] = &data[i*d]; 
    }
    return matrix; 
}

int** powerMatrix(int** matrix, int pow, int dim){
    if (pow == 0){
        for (int r = 0; r < dim; r++){
            for (int c = 0; c < dim; c++){
                if (r == c){
                    matrix[r][c] = 1; 
                }
                else{
                    matrix[r][c] = 0;
                }
            }
        }
        return matrix;
    }
    if (pow == 1){
        return matrix;  
    }
    int** newM = createMatrix(dim);
    for (int p = 0; p < dim; p++){
        for (int q = 0; q < dim; q++){
            newM[p][q] = matrix[p][q];
        }
    }
    for (int i = 0; i < pow - 1; i++){
        int** save = createMatrix(dim);
        for (int x = 0; x < dim; x++){
            for (int y = 0; y < dim; y++){
                int sum = 0;
                for (int z = 0; z < dim; z++){
                    sum += newM[x][z] * matrix[z][y];
                }
                save[x][y] = sum;
            }
        }
        for (int e = 0; e < dim; e++){
            for (int f = 0; f < dim; f++){
                newM[e][f] = save[e][f];
            }
        }
        free(save[0]);
        free(save);
    }
    free(matrix[0]);
    free(matrix);
    return newM; 
}

void printMatrix(int** matrix, int dim){
    for (int i = 0; i < dim; i++){
        for (int j = 0; j < dim; j++){
            if (j == dim - 1){
                printf("%d", matrix[i][j]);
            }
            else{
                printf("%d ", matrix[i][j]);
            }
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2){
        return 1; 
    }
    FILE *file = fopen(argv[1], "r");
    int dim, pow; 
    fscanf(file, "%d", &dim);
    int** matrix = createMatrix(dim); 
    for (int a = 0; a < dim; a++){
        for (int b = 0; b < dim; b++){
            fscanf(file, "%d", &matrix[a][b]);
        }
    }
    fscanf(file, "%d", &pow); 
    matrix = powerMatrix(matrix, pow, dim);
    printMatrix(matrix, dim);
    free(matrix[0]);
    free(matrix);
    return 0;
}