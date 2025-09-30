#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

double** createMatrix(int r, int c){
    double** matrix = (double**)malloc(r * sizeof(double*));
    double* data = (double *)malloc(r * c * sizeof(double));
    for (int i = 0; i < r; i++){
        matrix[i] = &data[i*c]; 
    }
    return matrix; 
}

double** transposeMatrix(double** result, double** input, int r, int c){
    for (int a = 0; a < r; a++){
        for (int b = 0; b < c; b++){
            result[b][a] = input[a][b];
        }
    }
    return result; 
}

double** identityMatrix(double **input, int dim){
    for (int a = 0; a < dim; a++){
        for (int b = 0; b < dim; b++){
            if (a == b){
                input[a][b] = 1;
            }
            else{
                input[a][b] = 0; 
            }
        }
    }
    return input; 
}

double** mutiplyMatrix(double** result, double** x, double** y, int xr, int xc, int yc){
    for (int a = 0; a < xr; a++){
        for (int b = 0; b < yc; b++){
            double sum = 0; 
            for (int c = 0; c < xc; c++){
                sum += x[a][c] * y[c][b];
            }
            result[a][b] = sum;
        }
    }
    return result; 
}

double** inverseMatrix(double** inverse, double** original, int dim){
    double save; 
    for (int x = 0; x < dim; x++){
        save = original[x][x];
        for (int a = 0; a < dim; a++){
            original[x][a] = original[x][a]/save; 
            inverse[x][a] = inverse[x][a]/save;
        }
        for (int y = x + 1; y < dim; y++){
            save = original[y][x]; 
            for (int b = 0; b < dim; b++){
                original[y][b] = original[y][b] - original[x][b] * save; 
                inverse[y][b] = inverse[y][b] - inverse[x][b] * save;  
            }
        }
    } 
    for (int x = dim - 1; x >= 0; x--){
        for (int y = x - 1; y >= 0; y--){
            save = original[y][x];
            for (int z = 0; z < dim; z++){
                original[y][z] = original[y][z] - original[x][z] * save; 
                inverse[y][z] = inverse[y][z] - inverse[x][z] * save; 
            }
        }
    }
    return inverse; 
}

void printM(double** matrix, int r, int c){
    for (int i = 0; i < r; i++){
        for (int j = 0; j < c; j++){
            printf("%lf ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]){
    if (argc < 2){
        return 1;
    }
    FILE *trainFile = fopen(argv[1], "r");
    char t[6];
    fscanf(trainFile, "%5s", t);
    
    int attr, house;
    fscanf(trainFile, "%d", &attr);
    fscanf(trainFile, "%d", &house);
    double** X = createMatrix(house, attr + 1); 
    double** Y = createMatrix(house, 1);
    for (int a = 0; a < house; a++){
        X[a][0] = 1; 
        for (int b = 1; b < attr+1; b++){
            fscanf(trainFile, "%lf", &X[a][b]);
        }
        fscanf(trainFile, "%lf", &Y[a][0]);
    }

    double** Xt = createMatrix(attr + 1, house);
    Xt = transposeMatrix(Xt, X, house, attr + 1);
    double** XtX = createMatrix(house, house);
    XtX = mutiplyMatrix(XtX, Xt, X, attr + 1, house, attr + 1);
    double** XtXi = createMatrix(attr + 1, attr + 1);
    XtXi = identityMatrix(XtXi, attr + 1);
    XtXi = inverseMatrix(XtXi, XtX, attr + 1);
    double** Xi = createMatrix(attr + 1, house);
    Xi = mutiplyMatrix(Xi, XtXi, Xt, attr + 1, attr + 1, house);
    double** W = createMatrix(attr + 1, 1);
    W = mutiplyMatrix(W, Xi, Y, attr + 1, house, 1); 

    FILE *dataFile = fopen(argv[2], "r");
    char d[5];
    fscanf(dataFile, "%4s", d);

    int dAttr, dHouse;
    fscanf(dataFile, "%d", &dAttr);
    fscanf(dataFile, "%d", &dHouse);

    double** Z = createMatrix(dHouse, dAttr + 1); 
    for (int a = 0; a < dHouse; a++){
        Z[a][0] = 1; 
        for (int b = 1; b < dAttr + 1; b++){
            fscanf(dataFile, "%lf", &Z[a][b]);
        }
    }
    double** price = createMatrix(dHouse, 1);
    price = mutiplyMatrix(price, Z, W, dHouse, dAttr + 1, 1);
    for (int x = 0; x < dHouse; x++){
        printf("%.0f\n", price[x][0]);
    }

    free(X[0]);
    free(X);
    free(Y[0]);
    free(Y);
    free(Xt[0]);
    free(Xt);
    free(XtX[0]);
    free(XtX);
    free(XtXi[0]);
    free(XtXi);
    free(Xi[0]);
    free(Xi);
    free(W[0]);
    free(W);
    free(Z[0]);
    free(Z);
    free(price[0]);
    free(price);
    return 0;
}
