#include <reg52.h>
#include <stdio.h>
#include"lcd1602.h"


void main(void){
	uchar sign_temp[] = {0x10,0x06,0x09,0x08,0x08,0x09,0x06,0x00};
	int temp_readh = 0x00;
	uchar temp_readl = 0x00;
	uchar temp_data[10];			   
	uchar temp_goal_data[10];		   // �¶��趨ֵ��ʾ����
	double temp_goal = 27.00;		   // �¶��趨ֵ����
	uchar UI_text_Now[] = "Now";	   // ��ǰ�¶�UI
	uchar UI_text_Goal[] = "Goal";	   // Ŀ���¶�UI
	uchar UI_text_Set[] = "Set Temp";
	uchar set_temp[] = {2, 7, 0, 0};   // �趨�¶���ֵ����
	uchar set_temp_pos = 0; 		   // �趨�¶���ֵλ������
	double temperature = 0;
	uchar is_heating = 'N';
	uchar UI_state = 0;  // ����UI����״̬
	HEATER = 1;  // ������״̬����
	

	WriteCGRAM(0x40, sign_temp);	  // ��CGRAM��д����������
	DelayMs(400);
	LcdInit();
	
	
	while(1)
	{
		/////////////////////////////// ʵ���¶Ȳɼ�����ʾ ////////////////////////////////////////
		if(is_heating == 'Y') HEATER = 0;
		if(UI_state == 0)
		{
			DisplayOneChar(5, 1, 0x00);
			DisplayOneChar(12, 1, 0x00);
			DisplayString(0, 0, UI_text_Now);
			DisplayString(8, 0, UI_text_Goal);		
			sprintf(temp_goal_data, "%.2f", temp_goal);
			DisplayString(7, 1, temp_goal_data);
			temperature = DSDataToTemp(temp_readh, temp_readl);
			sprintf(temp_data, "%.2f", temperature);
			DisplayString(0, 1, temp_data);
			DisplayOneChar(14, 0, is_heating);

		}
		if(UI_state == 1)
		{
		    sprintf(temp_goal_data, "%.2f", temp_goal);
			DisplayString(0, 0, UI_text_Set);
			DisplayString(0, 1, temp_goal_data);
			DisplayOneChar(5, 1, 0x00);
		}

		//����Ƿ񵽴��趨�¶�
		if(temperature >= temp_goal && HEATER==0&& is_heating == 'Y') {HEATER = 1; is_heating = 'N';}


		if(ButtonCheck(BUTTON1) && UI_state == 0)  // ��������/�¶��趨λ����
		{
			if(HEATER == 1) {HEATER = 0; is_heating = 'Y';}
			else {HEATER = 1; is_heating = 'N';}
		}
		if(ButtonCheck(BUTTON2))  // ���ܼ�
		{
			if(UI_state == 0) {UI_state = 1; WriteCmd(0x01, 1);}
			else if (UI_state == 1) {UI_state = 0; WriteCmd(0x0c, 1);}
		}

		if(UI_state == 1)
		{
			WriteCmd(0x0f, 1);
			if(ButtonCheck(BUTTON1))  // ��������/�¶��趨λ����
			{
				set_temp_pos++;
				if(set_temp_pos > 3)  set_temp_pos = 0;
			}
			if(ButtonCheck(BUTTON3))  	// ����
			{
				set_temp[set_temp_pos]++;
				if(set_temp[set_temp_pos]>9) set_temp[set_temp_pos] = 0;
			}
			if(ButtonCheck(BUTTON4))	// -��
			{
				set_temp[set_temp_pos]--;
				if(set_temp[set_temp_pos]<0) set_temp[set_temp_pos] = 9;
			}

			temp_goal = set_temp[0]*10 + set_temp[1] + set_temp[2]*0.1 + set_temp[3]*0.01;
		}		
	}

}

