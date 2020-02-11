#ifndef LCD1602_H  // 头文件标准开头，条件编译
#define LCD1602_H

#include<reg52.h>

#ifndef uint
#define uint unsigned int
#endif
#ifndef uchar
#define uchar unsigned char
#endif
#ifndef uint32
#define uint32 unsigned long int

#endif

#define DATA P0  // 宏定义数据总线
sbit RS = P2^6;  // 1602 数据/命令选择接口
sbit RW = P2^5;   //  1602 读/写选择接口
sbit E =  P2^7;  // 1602 使能端
sbit BUTTON1 = P2^4;  // 按键
sbit BUTTON2 = P2^3;
sbit BUTTON3 = P2^2;
sbit BUTTON4 = P2^1;
sbit HEATER = P2^0;  // 加热器输出引脚

sbit DS_DATA = P3^7;  // 定义温度传感器DS18B20数据总线

void DisplayOneChar(uchar X, uchar Y, uchar Data);
void WriteData(uchar Data, uchar BF);
void ReadBusy();
void Delay(uint  time);
void DelayMs(uchar time);
void Delay5Us();
void LcdInit();
void WriteCmd(uchar cmd, uchar BF);
void DisplayString(uchar X, uchar Y, uchar *Data);
uchar ButtonCheck(uchar button);
void IntToString(int Data, uchar *list);
void WriteCGRAM(uchar addr, uchar *Data);

uchar DS_Reset();
void DS_WriteCmd(uchar cmd);
void DS_WriteData(uchar Data);
uchar DS_ReadData();
void DS_Init();
void DS_ChangeTemp();
double DSDataToTemp(int temp_readh, uchar temp_readl);  // 传感器数据生成与读取




#endif


