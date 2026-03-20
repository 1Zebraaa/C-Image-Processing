markdown# C Language Digital Image Processing

> 基于 C 语言的 8 位灰度 BMP 图像处理项目，支持图像读写、四角涂黑、仿射变换（平移 / 旋转 / 缩放）及均值滤波。

---

## 项目结构
```
.
├── main.c                  # 主函数入口
├── cips.h                  # 公共头文件（宏定义、BMP 结构体）
├── imageIO.c               # BMP 文件头 / 信息头 / 调色板读取
├── IMG_Processing.c/.h     # 核心图像处理函数
├── Matrx_Calculate.c/.h    # 3×3 矩阵运算工具
├── CMakeLists.txt          # CMake 构建配置
└── lena_in.bmp             # 测试输入图像（256×256 灰度图）
```

---

## 环境要求

- C99 标准编译器（MSVC / GCC / Clang）
- CMake ≥ 3.10
- 推荐 IDE：CLion

---

## 构建与运行
```bash
mkdir build && cd build
cmake ..
cmake --build .
./CodeForStudent_NewVersion
```

> CMakeLists.txt 中已配置 `configure_file`，会自动将 `lena_in.bmp` 复制到构建目录，无需手动拷贝。

---

## 功能说明

### 1. BMP 文件读写

| 函数 | 说明 |
|---|---|
| `read_bmpHeader()` | 读取文件头、信息头、调色板 |
| `read_bmpImgData()` | 读取像素数据至二维数组 |
| `write_bmpFile()` | 将处理后的像素数据写出为新 BMP 文件 |
| `print_bmp_info()` | 打印文件类型、信息头长度、图像宽高 |

### 2. `image_black()` — 四角涂黑

将 256×256 图像的四个角各 50×50 像素区域置为纯黑（像素值 = 0）。
```c
image_black(imArray);
```

### 3. `image_transform()` — 仿射变换（平移 + 旋转 + 缩放）

使用**逆映射 + 齐次变换矩阵**将三种变换合并为一次操作，避免正向映射产生的空洞缺陷。
```c
// 函数签名
void image_transform(
    unsigned char input[HEIGHT][WIDTH],
    unsigned char output[HEIGHT][WIDTH],
    float dx,          // X 轴平移（像素）
    float dy,          // Y 轴平移（像素）
    float angle_deg,   // 旋转角度（逆时针为正，单位：度）
    float scale        // 缩放倍率（推荐范围：0.01 ~ 100）
);
```

**⚠️ 注意：`input` 与 `output` 不能传同一个数组。** 若需原地更新，请使用临时数组并在调用后用 `memcpy` 赋值：
```c
unsigned char imArray_temp[HEIGHT][WIDTH];

image_transform(imArray, imArray_temp, 20.0f, 20.0f, 45.0f, 0.5f);
memcpy(imArray, imArray_temp, HEIGHT * WIDTH);
```

变换矩阵合并顺序（逆映射）：
```
M = S_inv * Tc * R_inv * Tc_inv * T_inv
```

### 4. `image_filter_mean()` — 均值滤波

用邻域平均值替换每个像素，实现图像平滑（降噪 / 模糊）。
```c
void image_filter_mean(
    unsigned char input[HEIGHT][WIDTH],
    unsigned char output[HEIGHT][WIDTH],
    int kernel_size    // 滤波核边长，建议传 3 或 5
);
```

---

## 使用流程

`main.c` 中的标准三步流程：
```c
// Step 1：读图像头信息 + 像素数据
read_bmpHeader(fileName_IN, FileHeader, InfoHeader, rgbQuad);
read_bmpImgData(fileName_IN, imArray);

// Step 2：选择一种处理函数（同时只启用一个）
// image_black(imArray);
image_transform(imArray, imArray_temp_for_tsf, 0.0f, 0.0f, 90.0f, 1.0f);
// image_filter_mean(imArray, imArray_temp_for_tsf, 5);
memcpy(imArray, imArray_temp_for_tsf, HEIGHT * WIDTH);

// Step 3：输出结果图像（⚠️ 请将路径修改为你本机的实际路径）
write_bmpFile(FileHeader, InfoHeader, rgbQuad, imArray,
    "你的输出路径/result.bmp");
```

> **⚠️ 注意**：`write_bmpFile` 的最后一个参数为输出文件的**绝对路径**，
> 项目中默认填写的是作者本机路径，克隆后**必须修改为你自己机器上实际存在的目录**，
> 否则会打印 `Failure to create file!` 并输出失败。
>
> 也可以直接传文件名（如 `"result.bmp"`），文件会生成在 `cmake-build-debug/` 目录下，
> 无需关心路径问题。

---

## 已知问题与解决方案

### CLion 找不到图像文件

**现象**：运行时打印 `Failure to open file!`

**原因**：CLion 默认工作目录为 `cmake-build-debug/`，而非项目根目录。

**解决**：`CMakeLists.txt` 末尾已添加：
```cmake
configure_file(lena_in.bmp ${CMAKE_BINARY_DIR}/lena_in.bmp COPYONLY)
```

### 断点调试时 printf 不实时输出

**现象**：所有打印内容在程序结束后才一次性输出。

**解决**：`main.c` 开头添加：
```c
setvbuf(stdout, NULL, _IONBF, 0);
```

### 图像变换出现黑色空洞

**现象**：正向映射时，旋转 / 缩放后的图像出现大量黑色空洞。

**原因**：正向映射中，浮点坐标强制取整导致部分目标像素未被填充。

**解决**：改用**逆映射**方案——遍历输出图像每个像素，反算其在原图中的对应坐标后取值，确保全像素覆盖。

---

## 许可证

MIT License
