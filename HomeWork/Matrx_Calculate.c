//
// Created by lenovo on 2026/3/16.
//
#include "Matrx_Calculate.h"
void mat3x3_mul_vec3(float mat[3][3], float vec[3], float result[3])
{
    for (int i = 0; i < 3; i++) {
        result[i] = 0;
        for (int j = 0; j < 3; j++) {
            result[i] += mat[i][j] * vec[j];
        }
    }
}

void mat3x3_mul_mat3x3(float A[3][3], float B[3][3], float result[3][3])
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            result[i][j] = 0;
            for (int k = 0; k < 3; k++)
                result[i][j] += A[i][k] * B[k][j];
        }
}
