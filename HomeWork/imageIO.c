

/*********************************************
*
*   功能：读BMP图像文件
*   注意事项：只适用于8位灰度图的操作。
*
************************************************/

#include "cips.h"

/*读BMP图像文件的文件头、信息头和调色板三部分信息*/
void read_bmpHeader(char *filename, unsigned char bmFileHeader[], unsigned char bmInfoHeader[], unsigned char bmClrMap[])
{
	FILE *fp;
	unsigned char temp;
	int i;

	//检查打开文件是否成功，在没有配置 configure_file(lena_in.bmp ${CMAKE_BINARY_DIR}/lena_in.bmp COPYONLY)的时候会触发此代码
	//因为路径不对会找不到照片文件

	//查询到：这是以只读二进制的方式打开文件
    if ((fp = fopen(filename, "rb")) == NULL)
    {
        printf("Failure to open file! \n");
        exit(0);
	}

	//文件头包含的信息：文件类型两字节(就是BM)
	//				 文件大小4字节
	//				 两个保留位分别2字节
	//				 以及四个字节的数据偏移量
	for (i = 0; i < 14; i++)
	{
		fread(&temp, 1, 1, fp);
		bmFileHeader[i] = temp;
	}


	for (i = 0; i < 40; i++)
	{
		fread(&temp, 1, 1, fp);
		bmInfoHeader[i] = temp;
	}

	for (i = 0; i < CLRUSED*4; i++)
	{
		fread(&temp, 1, 1, fp);
		bmClrMap[i] = temp;
	}

	fclose(fp);
}

void print_bmp_info(unsigned char bmFileHeader[], unsigned char bmInfoHeader[])
{
	// 文件类型 (偏移0，2字节)
	printf("文件类型: %c%c\n", bmFileHeader[0], bmFileHeader[1]);

	// 信息头长度 (偏移14，4字节，小端序)
	int header_size = *(int*)&bmInfoHeader[0];
	printf("文件信息头长度: %d bytes\n", header_size);

	// 图像宽度 (偏移18，4字节)
	int width = *(int*)&bmInfoHeader[4];
	printf("图像宽度: %d px\n", width);

	// 图像高度 (偏移22，4字节)
	int height = *(int*)&bmInfoHeader[8];
	printf("图像高度: %d px\n", height);
}
