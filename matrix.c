#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "matrix.h"

matrix init_matrix(int height, int width, float** arr) {
    matrix m;
    m.height = height;
    m.width = width;
    m.arr = arr;
    return m;
}

matrix null_matrix(int height, int width) {

    float** arr = (float**)malloc(sizeof(float*) * height);
    for (int i = 0; i < height; i++) {
        float* temp = (float*)malloc(sizeof(float) * width);
        for (int j = 0; j < width; j++) {
            temp[j] = 0.0;
        }
        arr[i] = temp;
    }

    return init_matrix(height, width, arr);

}

matrix identity_matrix(int size) {

    float** arr = (float**)malloc(sizeof(float*) * size);
    for (int i = 0; i < size; i++) {
        float* temp = (float*)malloc(sizeof(float) * size);
        for (int j = 0; j < size; j++) {
            temp[j] = i == j ? 1.0 : 0.0;
        }
        arr[i] = temp;
    }

    return init_matrix(size, size, arr);

}

matrix matrix_add(matrix m1, matrix m2) {

    // assert(m1.width == m2.width && m1.height == m2.height);

    matrix m = null_matrix(m1.height, m1.width);

    for (int i = 0; i < m1.height; i++) {
        for (int j = 0; j < m1.width; j++) {
            m.arr[i][j] = m1.arr[i][j] + m2.arr[i][j];
        }
    }

    return m;

}

void matrix_add_self(matrix m, matrix m2) {

    for (int i = 0; i < m.height; i++) {
        for (int j = 0; j < m.width; j++) {
            m.arr[i][j] += m2.arr[i][j];
        }
    }

}

matrix matrix_mult_by_scalar(matrix m, float scalar) {

    float** array = (float**)malloc(sizeof(float*) * m.height);
    
    for (int i = 0; i < m.height; i++) {
        
        float* subarray = (float*)malloc(sizeof(float) * m.width);
        array[i] = subarray;
        
        for (int j = 0; j < m.width; j++) {
            subarray[j] = m.arr[i][j] * scalar;
        }

    }

    return init_matrix(m.height, m.width, array);

}

void matrix_mult_by_scalar_self(matrix m, int scalar) {

    for (int i = 0; i < m.height; i++) {
        for (int j = 0; j < m.width; j++) {
            m.arr[i][j] *= scalar;
        }
    }

}

matrix matrix_mult(matrix m1, matrix m2) {

    // si je crois / je teste au moins / c'est clairement pas bon en fait / après ce qui est sus c'est que ça crash encore avant je crois
    matrix m = null_matrix(m1.height, m2.width); // jaurais dit

    for (int i = 0; i < m1.height; i++) {
        for (int j = 0; j < m2.width; j++) {

            for (int k = 0; k < m2.height; k++) {
                m.arr[i][j] += m1.arr[i][k] * m2.arr[k][j];
            }
            
        }
    }

    return m;

}

matrix matrix_binary_pow(matrix base, int exponent) {

    matrix res = identity_matrix(base.height);

    while (exponent > 0) {

        if (exponent & 1) {
            res = matrix_mult(res, base);
        }

        exponent >>= 1;
        base = matrix_mult(base, base);

    }

    return res;

}

matrix matrix_transpose(matrix m) {

    matrix res = null_matrix(m.width, m.height);

    for (int i = 0; i < m.width; i++) {
        for (int j = 0; j < m.height; j++) {
            res.arr[i][j] = m.arr[j][i];
        }
    }

    return res;

}

void print_matrix(matrix m) {

    printf("[");
    for (int i = 0; i < m.height; i++) {
        
        if (i != 0) {
            printf(" ");
        }
        printf(" [");

        for (int j = 0; j < m.width; j++) {
            printf("%.1f", m.arr[i][j]);
            if (j != m.width - 1) {
                printf(", ");
            }
        }

        printf("]");

        if (i != m.height - 1) {
            printf("\n");
        }

    }
    printf("]\n");

}

void matrix_add_scalar_self(matrix m, float scalar) {

    for (int i = 0; i < m.height; i++) {
        for (int j = 0; j < m.width; j++) {
            m.arr[i][j] += scalar;
        }
    }

}

matrix matrix_add_scalar(matrix m, float scalar) {

    matrix res = null_matrix(m.height, m.width);

    for (int i = 0; i < m.height; i++) {
        for (int j = 0; j < m.width; j++) {
            res.arr[i][j] = m.arr[i][j] + scalar;
        }
    }

    return res;

}

matrix matrix_sigmoid(matrix m) {
    
    matrix res = null_matrix(m.height, m.width);

    for (int i = 0; i < m.height; i++) {
        for (int j = 0; j < m.width; j++) {
            res.arr[i][j] = 1/(1 + exp(m.arr[i][j]));
        }
    }

    return res;

}

void matrix_sigmoid_self(matrix m) {
    
    for (int i = 0; i < m.height; i++) {
        for (int j = 0; j < m.width; j++) {
            m.arr[i][j] = 1/(1 + exp(m.arr[i][j]));
        }
    }

}