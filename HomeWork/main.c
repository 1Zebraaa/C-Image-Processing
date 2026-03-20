#include "cips.h"
#include "IMG_Processing.h"
int main()
{
	//这里添加一行刷新缓冲区的代码方便调试
	setvbuf(stdout, NULL, _IONBF, 0);
	//这里相当于声明一个FILE实例，用于处理文件
	FILE *fp;  // FILE类型，在stdio.h头文件中，FILE类是一个结构体，代表任何一个任何类型的文件，用于文件的读写操作。

	int i, j;
	unsigned char FileHeader[14];
	unsigned char InfoHeader[40];
	unsigned char rgbQuad[CLRUSED*4];
	unsigned char imArray[HEIGHT][WIDTH];
	unsigned char imArray_temp_for_tsf[HEIGHT][WIDTH];

	char fileName_IN[]	= "lena_in.bmp";

	unsigned char temp;
	//为了方便我们处理数据，这里使用了直接定位到像素数据的偏置
	int dataOffset;  //从文件头到实际位图数据的偏移字节数=位图文件头长度+位图信息头长度+调色板长度

	read_bmpHeader(fileName_IN, FileHeader, InfoHeader, rgbQuad);

	//读图像数据
	dataOffset = 14 + 40 + CLRUSED*4;

	if ((fp = fopen(fileName_IN, "rb")) == NULL)
    {
        printf("Failure to open file! \n");
        exit(0);
	}
//-------------------------------------------默认文件部分-------------------------------------//
	// fseek(fp, dataOffset, SEEK_SET);  //跳过图像文件头部分，开始读图像数据
	//
	// for (i = 0; i < HEIGHT; i++)
	// {
	// 	for (j = 0; j < WIDTH; j++)
	// 	{
	// 		fread(&temp, 1, 1, fp);
	// 		imArray[i][j] = temp;
	// 	}
	// }

	// fclose(fp);
 //
	// /*输出显示BMP图像的文件头、信息头和调色板三部分信息*/
	// //输出文件头，14字节
	// printf("\n  BMP File Header\n");
	// for (i = 0; i < 14; i++){
	// 	printf("%d: %d\n", i+1, FileHeader[i]);
	// }
 //
	// //输出信息头，40字节
	// printf("\n  BMP Information Header\n");
	// for (i = 0; i < 40; i++){
	// 	printf("%d: %d\n", i+1, InfoHeader[i]);
	// }
 //
	// //输出调色板，4*256字节
 //    printf("\n  Color Map\n");
	// for (i = 0; i < 256; i++){
	// 	printf("%d: \t", i);
	// 	for (j = 0; j < 4; j++){
	// 	printf("%d\t", rgbQuad[i*4+j]);
	// 	}
	// 	printf("\n");
	// }
//-------------------------------------------默认文件部分-------------------------------------//

//----------------------------------------输出显示重要图像信息-------------------------------------//

	print_bmp_info(	FileHeader,InfoHeader);

//----------------------------------------输出显示重要图像信息-------------------------------------//

//--------------------------------------------------------在固定文件目录内写入函数的操作---------------------------------------------//

//固定操作：
	//-----------------------------------第一步，读图片，一个函数就够-----------------------------------------//
	read_bmpImgData(fileName_IN, imArray);
	//--------------------第二步，数据处理，根据不同的要求选择不同的图片处理函数，一次只能用一个---------------------//

	// image_black(imArray);//需要的时候请取消注释并对其它数据处理函数进行注释

	image_transform(imArray,imArray_temp_for_tsf,62.0f,128.0f,45,1);//需要的时候请取消注释并对其它数据处理函数进行注释

	// image_filter_mean(imArray,imArray_temp_for_tsf,5);
	memcpy(imArray, imArray_temp_for_tsf, HEIGHT * WIDTH);
	//--------------------------------------第三步，输出图片到想要的路径下-----------------------------------//
	write_bmpFile(FileHeader, InfoHeader, rgbQuad, imArray,
		"E:/Four_years_of_Electronic_Information_Engineering/Second_semester_of _the_F_Y/C_Language_Short_Course/Picture_processing_HomeWork/HomeWork/picture-result/holl.bmp");

	return 0;
}

