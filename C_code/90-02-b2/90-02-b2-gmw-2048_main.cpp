/* 2052532 ���ܷ� ��04 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "../include/cmd_console_tools.h"        //���������й��ߺ���
#include "../include/cmd_gmw_tools.h"            //����αͼ�ι��ߺ���
#include "../include/common_output.h"            //�����Լ�д�Ĺ��ߺ���
#include "90-02-b2-gmw-2048.h"                   //��������Ŀ��ͷ�ļ�

using namespace std;

/* -----------------------------------------
	 ���ļ����ܣ�
	���main����
   ----------------------------------------- */

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����main������
***************************************************************************/
int main()
{
	int score_highest = 0;
	while (1)
	{

		if (!First())//��ҳ�����˳��� ֱ���˳�
			return 0;
		/*����һ��CGI����*/
		CONSOLE_GRAPHICS_INFO M2048_CGI;
		gmw_init(&M2048_CGI);//���нṹ��ĳ�ʼ��
		 
        const BLOCK_DISPLAY_INFO bdi[] = {
		    {BDI_VALUE_BLANK, -1, -1, NULL},  //0����ʾ���ÿո���伴��
		    {2, COLOR_WHITE, COLOR_BLACK, NULL},
		    {4, COLOR_CYAN, COLOR_BLACK, NULL},
		    {8, COLOR_GREEN, COLOR_BLACK, NULL},
		    {16, COLOR_RED, COLOR_BLACK, NULL},
		    {32, COLOR_PINK, COLOR_BLACK, NULL},
		    {64, COLOR_YELLOW, COLOR_BLACK, NULL},
	    	{128, COLOR_BLUE, COLOR_BLACK, NULL},
	     	{256, COLOR_HBLUE, COLOR_BLACK, NULL},
		    {512, COLOR_HGREEN, COLOR_BLACK, NULL},
	    	{1024, COLOR_HCYAN, COLOR_BLACK, NULL},
	    	{2048, COLOR_HRED, COLOR_BLACK, NULL},
	    	{4096, COLOR_HPINK, COLOR_BLACK, NULL},
	    	{8192, COLOR_HYELLOW, COLOR_BLACK, NULL},
	    	{16384, COLOR_HWHITE, COLOR_BLACK, NULL},
	    	{32768, COLOR_HCYAN, COLOR_BLACK, NULL},
	    	{65536, COLOR_HGREEN, COLOR_BLACK, NULL},
	    	{131072, COLOR_HPINK, COLOR_BLACK, NULL},
	    	{262144,COLOR_HYELLOW,COLOR_BLACK,NULL},
	    	{524288,COLOR_HWHITE,COLOR_BLACK,NULL},
	     	{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
		};


		while (1)
		{

			/*�����ڲ�����*/
			int inbase[ROW_MAX][COL_MAX] = {
				{0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0},
			};
			int score_goal = 0;
			int score = 0;
			level(&M2048_CGI, score_goal);
			my2048_start(&M2048_CGI, inbase, bdi, score_goal, score_highest);
			int go = my2048_mid(&M2048_CGI, inbase, bdi, score_goal, score_highest, score);
			/*�������*/
			if (score > score_highest)
				score_highest = score;
			if (go == 0)
				break; //�˳���Ϸ ������Ϸ����
			else if (go == 1)
				continue;//��������Ϸ
			else if (go == 2)//��Ϸ����
			{
				
				int go_on=my2048_end(&M2048_CGI);
				if (go_on == 0)
					break;//�˳���Ϸ ������Ϸ����
				else if (go_on == 1)
					continue;//��������Ϸ
			}
		}
	}
}