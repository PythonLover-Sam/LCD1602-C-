//  ʵ�ֺ��������ļ�

#include<reg52.h>
#include "lcd1602.h"
#include<intrins.h>
void Delay5Us()
	{
	_nop_();
}
void DelayMs(uchar time)  //�ӳ�ms����
	{
		uint j;
		for(time; time>0; time--)
			for(j=110; j>0; j--);
}

void Delay(uint time)  // ָ�������ӳٺ��� um
	{
		while(time--);
}

void ReadBusy()  // ��æ����
{
		uchar state;
		DATA = 0xff;
		RS = 0;
		RW = 1;
		do
		{
			E = 1;
			state = DATA;
			E = 0;
		}	
		while(state & 0x80);
}

void LcdInit()   //LCD1602��ʼ������
	{
		DATA = 0;
		WriteCmd(0x38, 0);
		DelayMs(5);
		WriteCmd(0x38, 0);
		DelayMs(5);
		WriteCmd(0x38, 0);
		DelayMs(5);
		WriteCmd(0x38, 1);
		WriteCmd(0x08, 1);
		WriteCmd(0x01, 1);
		WriteCmd(0x06, 1);
		WriteCmd(0x0c, 1);

}
void WriteData(uchar Data, uchar BF)  // д���ݺ���
	{
		if(BF)
				ReadBusy();
		DATA = Data;
		RS = 1;
		RW = 0;
		E = 1;
		E = 1;
		E = 0;
}
void WriteCmd(uchar cmd,  uchar BF)  // дָ���
	{
		if(BF)
				ReadBusy();
		DATA = cmd;
		RS = 0;
		RW = 0;
		E = 1;
		E = 1;
		E = 0;

}

void DisplayOneChar(uchar X, uchar Y, uchar Data)  // ��ʾ�����ַ�����
	{
		Y &= 0x1;
		X &= 0xF;
		if(Y)
			X |= 0x40;
		X|= 0x80;
		WriteCmd(X, 1);
		WriteData(Data, 1);
}
void DisplayString(uchar X, uchar Y, uchar *Data)  // ��ʾ�ַ�������
	{
		uchar i = 0;
		X &= 0xf;
		Y &= 0x1;
		if(Y)
			X |= 0x40;
		X |= 0x80;
		
		while(Data[i] != '\0')	
		{
			WriteCmd(X++, 1);
			WriteData(Data[i], 1);
			i++;
		}
		
}
uchar ButtonCheck(uchar button)  // ��鰴���Ƿ��º��������������
	{
	if(button == 0)
		{
			DelayMs(80);
			if(button == 0)
				return 1;
			else return 0;
		}	
	else return 0;
}
void IntToString(int Data, uchar *list)
	{
	uchar i, j, temp;
	uchar temp_list[10];
	i = 0;
	j = 0;
	do
	{
		temp = Data % 10;
		temp_list[i] = temp + 0x30;
		Data /= 10;
		i++;
	}
	while(Data != 0);
	while(temp_list[i-1-j])
	{
		list[j] = temp_list[i-1-j];
		j++;
	}
	list[j] = '\0';
}
void WriteCGRAM(uchar addr, uchar *Data)
	{
	uchar i=0;
 	WriteCmd(addr, 1);
	while(Data[i])
	{
		WriteData(Data[i], 1);
		i++;
	}	
}

/************************************ ��DS18B20���в�������غ��� *******************************************************/

uchar DS_Reset() // ���¶ȴ��������и�λ
	{
	uchar i;
 	DS_DATA = 0;
	i = 70;
	while(i--);
	DS_DATA = 1;
	i = 0;
	while(DS_DATA)
	{
		i++;
		if(i > 5000)  return 0;
	}
	return 1;
}

void DS_WriteCmd(uchar cmd)
	{
	uchar i, j;
	for(i=0; i<8; i++)
	{
		
		DS_DATA = 0;
		j++;
		DS_DATA = cmd & 0x01;
		j = 6;
		while(j--);
		DS_DATA = 1;
		cmd >>= 1;
	}
}

void DS_WriteData(uchar Data)
	{
	uchar i, j;
	
	for(i=0; i<8; i++)
	{
		DS_DATA = 0;
		j++;
		DS_DATA = Data & 0x1;
		j = 6;
		while(j--);
		DS_DATA = 1;
		Data >>= 1;
	}
}

uchar DS_ReadData()
	{
	uchar i, temp;
    uchar content=0;

	for(i=0; i<8; i++)
	{
		DS_DATA = 0;
		_nop_();
		DS_DATA = 1;
		Delay5Us();
		Delay5Us();
		temp = DS_DATA;
		content = (content >>=1) | (temp <<= 7);
		Delay(3);
	}
	return content;
}

void DS_Init()
	{
 	DS_Reset();
	DelayMs(1);
	DS_WriteCmd(0xcc);
	DS_WriteData(0x44);
	//DelayMs(1000);
}

double DSDataToTemp(int temp_readh, uchar temp_readl)  // �����������������ȡ
	{
	
 	DelayMs(100);
	DS_Init();
	DS_Reset();
	DelayMs(1);
	DS_WriteCmd(0xcc);
	DS_WriteData(0xbe);
	temp_readl = DS_ReadData();
	temp_readh = DS_ReadData();
	return ((temp_readh <<= 8)| temp_readl) * 0.0625;
}
