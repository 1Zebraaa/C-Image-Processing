//
// Created by lenovo on 2026/3/16.
//
#include <math.h>

#ifndef CODEFORSTUDENT_NEWVERSION_MATRX_CALCULATE_H
#define CODEFORSTUDENT_NEWVERSION_MATRX_CALCULATE_H

/* @brief：定义一个3*3的矩阵相乘的函数，用于将所有的线性变换的作用效果集于一体
 * @para：A: 在乘号左边的矩阵，对向量进行线性变换处理
 * @para：B：在乘号右边的向量。
 * @result：需要赋值结果的矩阵的名称
 */
void mat3x3_mul_vec3(float mat[3][3], float vec[3], float result[3]);

/* @brief：定义一个3*3的矩阵相乘的函数，用于将所有的线性变换的作用效果集于一体
 * @para：A: 在乘号左边的矩阵，实际传入数组首元素指针地址
 * @para：B：在乘号右边的矩阵，同上
 * @result：需要赋值结果的矩阵的名称
 */
void mat3x3_mul_mat3x3(float A[3][3], float B[3][3], float result[3][3]);

#endif //CODEFORSTUDENT_NEWVERSION_MATRX_CALCULATE_H