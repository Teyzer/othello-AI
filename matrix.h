#ifndef MATRIX
#define MATRIX

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct matrix {
    float** arr;
    int height;
    int width;
} matrix;

matrix init_matrix(int height, int width, float** arr);
matrix null_matrix(int height, int width);
matrix identity_matrix(int size);
matrix matrix_add(matrix m1, matrix m2);
matrix matrix_mult_by_scalar(matrix m, float scalar);
matrix matrix_mult(matrix m1, matrix m2);
matrix matrix_binary_pow(matrix base, int exponent);
matrix matrix_transpose(matrix m);
void print_matrix(matrix m);
void matrix_add_scalar_self(matrix m, float scalar);
matrix matrix_add_scalar(matrix m, float scalar);
matrix matrix_sigmoid(matrix m);
void matrix_sigmoid_self(matrix m);

#endif
