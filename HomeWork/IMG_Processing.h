//
// Created by ZhangZhejia on 2026/3/16.
//
#include "cips.h"
#include "Matrx_Calculate.h"
#ifndef IMG_PROCESSING_H
#define IMG_PROCESSING_H

/* read_bmpFile_homework
 * @brief: 读取一个图片文件并将像素信息储存在imArray数组内
 *
 * @para：imArray_s：要写入文件内的像素值(需要处理的就是这里的数据)
 * @para：filename：要写入的具体的文件名称
 */
void read_bmpImgData(char *filename, unsigned char imArray_s[HEIGHT][WIDTH]);

/* read_bmpFile_homework
 * @brief: 读取一个图片文件并将像素信息储存在imArray数组内
 *
 * @para：imArray_s：要进行四个角黑色像素块处理的二位数组
 */
void image_black(unsigned char imArray_s[][WIDTH]);

/* image_transform
 * @brief: 对一个图像进行：先平移、再以图片中心为轴旋转(逆时针方向为正方向)和缩放
 *         此函数使用齐次变换矩阵相乘的方法集齐三个图像变换功能于一体，能更整洁和泛化地实现目标任务。
 *
 * @para：input：需要输入的储存图片像素点信息的二维数组名
 * @para：output：需要输出的储存图片像素点信息的二维数组名
 * @para：dx：需要进行平移的x方向偏置像素点数
 * @para：dy：需要进行平移的y方向偏置像素点数
 * @para：scale：需要进行缩放的倍率(精度范围0.01倍到100倍之间)
 * @Warning：禁止将输入和输出使用同一个数组！！，若确有需要，请使用临时变量最后使用memcpy进行赋值。
 */
void image_transform(unsigned char input[HEIGHT][WIDTH],
                     unsigned char output[HEIGHT][WIDTH],
                     float dx, float dy,
                     float angle_deg,
                     float scale);

/* image_filter_mean
 * @brief: 对一个图像实现平滑滤波后进行输出
 *
 * @para：input：需要输入的储存图片像素点信息的二维数组名
 * @para：output：需要输出的储存图片像素点信息的二维数组名
 * @para：kernel_size：需要模糊处理的程度
 */
void image_filter_mean(unsigned char input[HEIGHT][WIDTH],
                       unsigned char output[HEIGHT][WIDTH],
                       int kernel_size);

/* write_bmpFile
 * @brief: 创建并写入一个bmp文件
 *
 * @para：FileHeader_s：要写入文件的文件头信息储存数组名（自动退化为首地址指针）
 * @para：InfoHeader_s：要写入文件的信息头，同上
 * @para：rgbQuad_s：要写入文件的调色板
 * @para：imArray_s：要写入文件内的像素值(需要处理的就是这里的数据)
 * @para：filename：要写入的具体的文件名称
 */
void write_bmpFile(unsigned char *FileHeader_s, unsigned char *InfoHeader_s,unsigned char *rgbQuad_s, unsigned char imArray_s[][WIDTH], char *filename);

#endif //IMG_PROCESSING_H