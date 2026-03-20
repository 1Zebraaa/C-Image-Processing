//
// Created by ZhangZhejia on 2026/3/16.
//
#include "IMG_Processing.h"
#include <stdio.h>
#include <math.h>

void read_bmpImgData(char *filename, unsigned char imArray_s[HEIGHT][WIDTH])
{
    FILE *fp;
    int dataOffset;
    unsigned char temp;

    dataOffset = 14 + 40 + CLRUSED * 4;

    if ((fp = fopen(filename, "rb")) == NULL) {
        printf("Failure to open file!\n");
        return;
    }

    fseek(fp, dataOffset, SEEK_SET);

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            fread(&temp, 1, 1, fp);
            imArray_s[i][j] = temp;
        }
    }

    fclose(fp);
}

void image_black(unsigned char imArray_s[][WIDTH])
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if ((i <= 49 || i >= 206) && (j <= 49 || j >= 206))
            {
                imArray_s[i][j] = 0;
            }
        }
    }
}

// void image_transform(unsigned char input[HEIGHT][WIDTH],
//                      unsigned char output[HEIGHT][WIDTH],
//                      float dx, float dy,
//                      float angle_deg,
//                      float scale)
// {
//     if (dx >= 256 || dx <= -256 || dy >= 256 || dy <= -256)
//         printf("警告：超出显示范围，可能无法看到变换图像主体\n");
//
//     float angle = angle_deg * 3.14159265f / 180.0f;
//     float cos_a = cosf(angle);
//     float sin_a = sinf(angle);
//     float cx = 127.5f, cy = 127.5f;
//
//     float T_fwd[3][3] = {
//         {1, 0, dx},
//         {0, 1, dy},
//         {0, 0,  1}
//     };
//
//     float Tc_inv[3][3] = {
//         {1, 0, -cx},
//         {0, 1, -cy},
//         {0, 0,   1}
//     };
//
//     float R_fwd[3][3] = {
//         {cos_a, -sin_a, 0},
//         {sin_a,  cos_a, 0},
//         {0,      0,     1}
//     };
//
//     float Tc[3][3] = {
//         {1, 0, cx},
//         {0, 1, cy},
//         {0, 0,  1}
//     };
//
//     float S_fwd[3][3] = {
//         {scale, 0,     0},
//         {0,     scale, 0},
//         {0,     0,     1}
//     };
//
//      float t1[3][3], t2[3][3], t3[3][3], M[3][3];
//     mat3x3_mul_mat3x3(S_fwd,  Tc_inv, t1);
//     mat3x3_mul_mat3x3(R_fwd,  t1,     t2);
//     mat3x3_mul_mat3x3(Tc,     t2,     t3);
//     mat3x3_mul_mat3x3(T_fwd,  t3,     M);
//
//     for (int i = 0; i < HEIGHT; i++)
//         for (int j = 0; j < WIDTH; j++)
//             output[i][j] = 0;
//
//     for (int i = 0; i < HEIGHT; i++) {
//         for (int j = 0; j < WIDTH; j++) {
//             float p[3] = {(float)j, (float)i, 1.0f};
//             float dst[3];
//
//             mat3x3_mul_vec3(M, p, dst);   // 源坐标 → 目标坐标
//
//             int dst_x = (int)(dst[0] + 0.5f);
//             int dst_y = (int)(dst[1] + 0.5f);
//
//             if (dst_x >= 0 && dst_x < WIDTH && dst_y >= 0 && dst_y < HEIGHT)
//                 output[dst_y][dst_x] = input[i][j];  // ④ 方向反过来
//         }
//     }
// }


void image_transform(unsigned char input[HEIGHT][WIDTH],
                     unsigned char output[HEIGHT][WIDTH],
                     float dx, float dy,
                     float angle_deg,
                     float scale)
{
    if (dx >= 256 || dx <= -256 || dy >= 256 || dy <= -256)
        printf("警告：超出显示范围，可能无法看到变换图像主体\n");

    float angle = angle_deg * 3.14159265f / 180.0f;
    float cos_a = cosf(-angle);
    float sin_a = sinf(-angle);
    float cx = 127.5f, cy = 127.5f;

    // 逆平移
    float T_inv[3][3] = {
        {1, 0, -dx},
        {0, 1, -dy},
        {0, 0,   1}
    };

    // 移到中心
    float Tc_inv[3][3] = {
        {1, 0, -cx},
        {0, 1, -cy},
        {0, 0,   1}
    };

    // 逆旋转
    float R_inv[3][3] = {
        {cos_a, -sin_a, 0},
        {sin_a,  cos_a, 0},
        {0,      0,     1}
    };

    // 移回原点
    float Tc[3][3] = {
        {1, 0, cx},
        {0, 1, cy},
        {0, 0,  1}
    };

    // 逆缩放
    float S_inv[3][3] = {
        {1.0f/scale, 0,          0},
        {0,          1.0f/scale, 0},
        {0,          0,          1}
    };

    // 合并：M = S_inv * Tc * R_inv * Tc_inv * T_inv
    float t1[3][3], t2[3][3], t3[3][3], M[3][3];
    mat3x3_mul_mat3x3(Tc_inv, T_inv,  t1);
    mat3x3_mul_mat3x3(R_inv,  t1,     t2);
    mat3x3_mul_mat3x3(Tc,     t2,     t3);
    mat3x3_mul_mat3x3(S_inv,  t3,     M);

    // 初始化输出为全黑
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            output[i][j] = 0;

    // 逆向映射
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            float p[3] = {(float)j, (float)i, 1.0f};
            float src[3];

            mat3x3_mul_vec3(M, p, src);

            int src_x = (int)(src[0] + 0.5f);
            int src_y = (int)(src[1] + 0.5f);

            if (src_x >= 0 && src_x < WIDTH && src_y >= 0 && src_y < HEIGHT)
                output[i][j] = input[src_y][src_x];
        }
    }
}

void image_filter_mean(unsigned char input[HEIGHT][WIDTH],
                       unsigned char output[HEIGHT][WIDTH],
                       int kernel_size)   // 建议传3或5
{
    int half = kernel_size / 2;

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int sum = 0, count = 0;

            for (int ki = -half; ki <= half; ki++) {
                for (int kj = -half; kj <= half; kj++) {
                    int ni = i + ki;
                    int nj = j + kj;
                    // 边界处理：超出范围的像素直接跳过
                    if (ni >= 0 && ni < HEIGHT && nj >= 0 && nj < WIDTH) {
                        sum += input[ni][nj];
                        count++;
                    }
                }
            }
            output[i][j] = (unsigned char)(sum / count);
        }
    }
}

void write_bmpFile(unsigned char *FileHeader_s, unsigned char *InfoHeader_s,unsigned char *rgbQuad_s, unsigned char imArray_s[][WIDTH], char *filename)
{
    FILE *fp = fopen(filename, "wb");

    if (fp == NULL) {
        printf("Failure to create file!\n");
        return;
    }

    fwrite(FileHeader_s, 1, 14, fp);
    fwrite(InfoHeader_s, 1, 40, fp);
    fwrite(rgbQuad_s,    1, CLRUSED * 4, fp);
    fwrite(imArray_s,    1, WIDTH * HEIGHT, fp);

    fclose(fp);
}
