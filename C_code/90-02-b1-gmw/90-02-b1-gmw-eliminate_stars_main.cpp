/* 2052532 ���ܷ� ��04 */
#include <iostream>
#include "../include/cmd_console_tools.h"        //���������й��ߺ���
#include "../include/cmd_gmw_tools.h"            //����αͼ�ι��ߺ���
#include "../include/common_output.h"            //�����Լ�д�Ĺ��ߺ���
#include "90-02-b1-gmw-eliminate_stars.h"        //��������Ŀ��ͷ�ļ�

using namespace std;

/* -----------------------------------------
	���ļ����ܣ�
	���main����
----------------------------------------- */

/*****************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����main������
***************************************************************************/
int main()
{
	while (1)
	{
		if (!First())//��ҳ�����˳��� ֱ���˳�
			return 0;

		/*����һ��CGI����*/
		CONSOLE_GRAPHICS_INFO STAR_CGI;

        /*���нṹ��ĳ�ʼ��*/
		/*���ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ��*/
		gmw_init(&STAR_CGI);

		/*��ʼ��һ���������鴢���ڲ���Ϣ*/
		int inbase[ROW_MAX][COL_MAX] = {
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
		};

		/*����1-5�����Ժ�����ʽ�ڽ�������ʾ*/
		const BLOCK_DISPLAY_INFO bdi[] = {
			{BLANK, -1, -1, "  "},//�հ�
			{1, COLOR_HBLUE, COLOR_BLACK, "��"},//��������״̬
		    {2, COLOR_HGREEN, COLOR_BLACK, "��"},
		    {3, COLOR_HCYAN, COLOR_BLACK, "��"},
		    {4, COLOR_HRED, COLOR_BLACK, "��"},
		    {5, COLOR_HPINK, COLOR_BLACK, "��"},
			/*��������Ҫ����״̬֮�����BETWEEN*/
			{BLANK+BETWEEN, -1, -1, "  "},//�հ�
			{11, COLOR_HBLUE, COLOR_HWHITE, "��"},//������Ҫ���Ե�״̬
			{12, COLOR_HGREEN, COLOR_HWHITE, "��"},
			{13, COLOR_HCYAN, COLOR_HWHITE, "��"},
			{14, COLOR_HRED, COLOR_HWHITE, "��"},
			{15, COLOR_HPINK, COLOR_HWHITE, "��"},
			{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
		};

		/*ȡ����Ҫ������ֵ*/
		rowcol(&STAR_CGI);
		int score = 0;


		while (1)
		{
			popstar_start(&STAR_CGI, inbase, bdi);
			popstar_mid(&STAR_CGI, inbase, bdi, score);
			if (popstar_end(&STAR_CGI, inbase, score))
				;
			else
				break;
		}//end of while(С��
	}//end of while(��)

}