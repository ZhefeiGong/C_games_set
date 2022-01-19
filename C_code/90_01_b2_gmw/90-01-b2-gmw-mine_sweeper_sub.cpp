/* 2052532 ���ܷ� ��04 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "../include/cmd_console_tools.h"//���������й��ߺ���
#include "../include/cmd_gmw_tools.h"//����αͼ�ι��ߺ���
#include "90-01-b2-gmw-mine_sweeper.h"//�����Լ�д�ĺ���

using namespace std;

/* -----------------------------------------
	 ���ļ����ܣ�
	����ڲ�ʵ�ֺ���
   ----------------------------------------- */

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ��������������ѡ��
***************************************************************************/
int level(void)
{
	cct_cls();//����
	cout << "��ѡ���Ѷȣ�" << endl;
	cout << "  1.����( 9*9  - 10����)" << endl;
	cout << "  2.�м�(16*16 - 40����)" << endl;
	cout << "  3.�߼�(16*30 - 99����)" << endl;
	cout << "������[1..3]��";
	char shuru;
	while (1)//����ѡ����׵�����
	{
		shuru = _getch();//���ض�Ӧ��ASC����
		if (shuru >= '1' && shuru <= '3')
		{
			cout << shuru;
			break;
		}
	}
	return (int)(shuru - '0');
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������� 1 ����
			���� 0 �˳�
***************************************************************************/
int First(void)
{
	cct_cls();//����
	cct_setconsoleborder(50, 15, 50, 15);//��ʼ������
	cct_setfontsize("������", 32, 14);
	cct_setcursor(CURSOR_VISIBLE_NORMAL);
	system("color E9");//��ʼ��������ɫ
	cct_setcolor(COLOR_HYELLOW, COLOR_HBLUE);
	cct_gotoxy(12, 5);
	cout << "�X�T�T�T�T�T�T�T�T�[";
	cct_gotoxy(12, 6);
	cout << "�U��ӭ����ɨ����Ϸ�U";
	cct_gotoxy(12, 7);
	cout << "�^�T�T�T�T�T�T�T�T�a";
	cct_gotoxy(30, 10);
	cout << "���»س�����";
	cct_gotoxy(30, 11);
	cout << "����Q/q�˳�";
	cct_setcolor();//��ɫ�ָ�����

	cct_setcursor(CURSOR_INVISIBLE);                                //��겻��ʾ
	
	char shuru;
	while (1)//�������뷵��
	{
		shuru = _getch();
		if (shuru == '\r')
			return 1;
		else if (shuru == 'q' || shuru == 'Q')
			return 0;
	}

	cct_setcolor();//��ɫ�ָ�����
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʼ��ɨ�׵Ľ���
***************************************************************************/
void mine_sweeper_start(CONSOLE_GRAPHICS_INFO* const  MINE_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* const bdi, const int row, const int col, const int num)
{
	cct_cls();//����

	/*����row/col��ֵ������Ϸ���������кͲ��ֳ�ʼ������*/
	gmw_set_rowcol(MINE_CGI, row, col);
	gmw_set_status_line_switch(MINE_CGI, TOP_STATUS_LINE, true);     //������״̬��
	gmw_set_status_line_switch(MINE_CGI, LOWER_STATUS_LINE, true);   //������״̬��
	gmw_set_frame_color(MINE_CGI, COLOR_HWHITE, COLOR_BLACK);        //���ÿ����ɫ
	gmw_set_font(MINE_CGI, "��������", 16, 8);                       //��������
	gmw_set_frame_style(MINE_CGI, BLOCK_WIDTH, BLOCK_HIGH, false);  //����Ҫ�߿�
	gmw_set_block_border_switch(MINE_CGI, true);                     //ɫ������Ҫ�߿�
	gmw_set_rowno_switch(MINE_CGI, true);                            //��Ҫ�к�
	gmw_set_colno_switch(MINE_CGI, true);                            //��Ҫ�б�
	cct_setcursor(CURSOR_INVISIBLE);                                 //��겻��ʾ


	/*��ʾ���*/
	gmw_draw_frame(MINE_CGI);

	/*��ӡ��״̬����ʾ����*/
	char line_top[64] = "��ESC�˳����ո���ʾʱ��";
	gmw_status_line(MINE_CGI, TOP_STATUS_LINE, line_top);//��ӡ��״̬��

	/*��ӡɫ����*/
	for (int j = 0; j < row; j++)
		for (int i = 0; i < col; i++)
			gmw_draw_block(MINE_CGI, j, i, COVER_SIGN, bdi);//���������� ��ӡ��Ӧ������

	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
		  �����ֵ˵����
						   1   ɨ�׳ɹ�����Ϸ����
						  -1   ɨ��ʧ�ܣ���Ϸ����
						   0   ɨ���˳�����Ϸδ��
***************************************************************************/
int mine_sweeper_mid(CONSOLE_GRAPHICS_INFO* const MINE_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* const bdi, int num)
{

	/*ȡ��ʱ�Ļ�׼*/
	clock_t start_time = clock();

	/*�������챬ըЧ��*/
	int BOOM1 = 1, BOOM2 = 2, BOOM3 = 3;
	const BLOCK_DISPLAY_INFO bdi_special[] = {
		{BDI_VALUE_BLANK,-1,-1,"  "},
		{BOOM1,COLOR_RED,COLOR_HRED,"��"},
		{BOOM2,COLOR_RED,COLOR_HRED,"��"},
		{BOOM3,COLOR_RED,COLOR_HRED,"��"},
	};//������������ըЧ��������

	int maction, mrow = -1, mcol = -1;
	int keycode1, keycode2;
	int ret;
	int start = 0;

	while (1) {

		ret = gmw_read_keyboard_and_mouse(MINE_CGI, maction, mrow, mcol, keycode1, keycode2);

		if (ret == CCT_MOUSE_EVENT) {

			/*����ƶ�*/
			if (maction == MOUSE_ONLY_MOVED) {//�Ȳ����з��Ե��ƶ�
				continue;
			}

			/*�����Ҽ�*/
			else if (maction == MOUSE_RIGHT_BUTTON_CLICK) {
				if (mrow == -1 || mcol == -1)//���ڷǷ�λ�� �����Ч
					continue;
				else if (inbase[mrow][mcol] == COVER_SIGN || inbase[mrow][mcol] == MINE_SIGN) {//�����ڿɱ�ǵ�λ��

					if (inbase[mrow][mcol] == COVER_SIGN)
						inbase[mrow][mcol] = TAB_NOT_SIGN;
					else
						inbase[mrow][mcol] = TAB_MINE_SIGN;
					gmw_draw_block(MINE_CGI, mrow, mcol, inbase[mrow][mcol], bdi);//���Դ�ӡ������
					mine_count(MINE_CGI, inbase, num);
				}
				else if (inbase[mrow][mcol] == TAB_NOT_SIGN || inbase[mrow][mcol] == TAB_MINE_SIGN) {
					if (inbase[mrow][mcol] == TAB_NOT_SIGN)
						inbase[mrow][mcol] = COVER_SIGN;
					else
						inbase[mrow][mcol] = MINE_SIGN;
					gmw_draw_block(MINE_CGI, mrow, mcol, COVER_SIGN, bdi);//�������ش�ӡ������
					mine_count(MINE_CGI, inbase, num);
				}
				else if (start == 0) {//��һ��δ��ֵʱ
					inbase[mrow][mcol] = TAB_NOT_SIGN;
					gmw_draw_block(MINE_CGI, mrow, mcol, inbase[mrow][mcol], bdi);//���Դ�ӡ������
					mine_count(MINE_CGI, inbase, num);
				}
				//����λ���޷����б��
			}

			/*�������*/
			else if (maction == MOUSE_LEFT_BUTTON_CLICK) {
				if (mrow == -1 || mcol == -1)//��ʱλ�÷Ƿ� �����Ч������
					continue;
				else if (start == 0 && inbase[mrow][mcol] != TAB_NOT_SIGN) { //��ʱ���г�ʼ�� �����һ�ξ͵㵽��
					make_inbase(MINE_CGI, inbase, num, mrow, mcol);
					spread(MINE_CGI, inbase, mrow, mcol);

					for (int i = 0; i < MINE_CGI->row_num; i++)
						for (int j = 0; j < MINE_CGI->col_num; j++)
							if (inbase[i][j] >= 0 && inbase[i][j] <= 8)//��0-9�����ֲŵ㿪������������㿪
								gmw_draw_block(MINE_CGI, i, j, inbase[i][j], bdi);

					start = 1;//����ʼ��һ��
				}
				else if (inbase[mrow][mcol] == COVER_SIGN) {//COVER_SIGNλ�òſɵ㿪 ���е㿪����
					spread(MINE_CGI, inbase, mrow, mcol);//���������չ��

					for (int i = 0; i < MINE_CGI->row_num; i++)
						for (int j = 0; j < MINE_CGI->col_num; j++)
							if (inbase[i][j] >= 0 && inbase[i][j] <= 8)//��0-9�����ֲŵ㿪������������㿪
								gmw_draw_block(MINE_CGI, i, j, inbase[i][j], bdi);

					if (judge_win(MINE_CGI, inbase))//�ɹ���������
					{
						time_out(MINE_CGI, start_time, LOWER_STATUS_LINE, true);
						return 1;//�ɹ�
					}


				}
				else if (inbase[mrow][mcol] == MINE_SIGN) {
					for (int i = 1; i <= 4; i++)//���ֱ�ըЧ��
					{
						gmw_draw_block(MINE_CGI, mrow, mcol, BOOM1, bdi_special);
						Sleep(40);
						gmw_draw_block(MINE_CGI, mrow, mcol, BOOM2, bdi_special);
						Sleep(40);
						gmw_draw_block(MINE_CGI, mrow, mcol, BOOM3, bdi_special);
						Sleep(50);
					}
					gmw_draw_block(MINE_CGI, mrow, mcol, MINE_SIGN, bdi);//��ʾ��
					time_out(MINE_CGI, start_time, LOWER_STATUS_LINE);//Ĭ��ʧ��
					return -1;//ʧ��

				}
			}

			else
				continue;
		}
		else {
			switch (keycode1) {//ֻ���ǰ���ESC�Ϳո������� ���迼�ǵ�һ��������������
				case 27: //����ESC ֱ���˳�
					return 0;//�˳�
					break;

				case 32: //���¿ո� ��ʾʱ��
					time_out(MINE_CGI, start_time, TOP_STATUS_LINE);
					break;
				default:
					break;
			}//end of switch
		}//end of else
	}//end of while

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �����ж�"���س�������..."�Ĳ���
***************************************************************************/
void mine_sweeper_end(CONSOLE_GRAPHICS_INFO* const MINE_CGI)
{
	int x = MINE_CGI->SLI.lower_start_x;
	int y = MINE_CGI->SLI.lower_start_y + 3;
	cct_gotoxy(x, y);
	cout << "���س�������...";

	char shuru;
	while (1)//�������뷵��
	{
		shuru = _getch();
		if (shuru == '\r')
			return;
	}

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������ݵ�һ�ε㿪��λ�� ��ʼ����
***************************************************************************/
void make_inbase(CONSOLE_GRAPHICS_INFO* const MINE_CGI, int inbase[ROW_MAX][COL_MAX], const int num, const int st_row, const int st_col)
{
	int row_max = MINE_CGI->row_num;
	int col_max = MINE_CGI->col_num;

	/*�Ƚ����������ʾ���ڸǵ����*/
	for (int i = 0; i <= row_max - 1; i++)
		for (int j = 0; j <= col_max - 1; j++)
			if (inbase[i][j] != TAB_NOT_SIGN)
				inbase[i][j] = COVER_SIGN;//������ʾΪCOVER_SIGN


	/*��һ���㿪λ�ñ���ΪBLANK������*/
	int row_m = st_row + 1;
	int row_n = st_row - 1;
	int col_m = st_col + 1;
	int col_n = st_col - 1;


	/*�����׵ĸ�ֵ*/
	srand(unsigned(time(NULL)));//ȡ���µ�ϵͳʱ��Ϊ���ӵ�ֵ
	int i, x, y;
	for (i = 1; i <= num; i++)//���������
	{
		while (1)
		{
			x = random(row_max);
			y = random(col_max);//���ڸǵ����ʱ�Ž��и�ֵΪ��
			if (inbase[x][y] == COVER_SIGN || inbase[x][y] == TAB_NOT_SIGN) {
				if (x >= row_n && x <= row_m)
					if (y >= col_n && y <= col_m)
						;
					else
						break;
				else
					break;
			}
		}
		if (inbase[x][y] == COVER_SIGN)
			inbase[x][y] = MINE_SIGN;
		else
			inbase[x][y] = TAB_MINE_SIGN;
	}

	inbase[st_row][st_col] = BLANK;//��λ��ΪBLANK


	return;

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������ݵ㿪��λ�ý�����ɢ
***************************************************************************/
void spread(CONSOLE_GRAPHICS_INFO* const MINE_CGI, int inbase[ROW_MAX][COL_MAX], int row, int col)
{
	/*�����ģʽ�µ������С*/
	int row_max = MINE_CGI->row_num;
	int col_max = MINE_CGI->col_num;

	/*����˸����򣺶��� �� ���� �� �� ���� �� ���� */
	int row_move[8] = { 1,1,1,0,0,-1,-1,-1 };
	int col_move[8] = { 1,0,-1,1,-1,-1,0,1 };
	int i, mines = 0;
	for (i = 0; i <= 7; i++) {//ͳ��8�������ϵ��׵�����
		int next_row = row + row_move[i];
		int next_col = col + col_move[i];
		if (next_row >= 0 && next_row <= (row_max - 1) && next_col >= 0 && next_col <= (col_max - 1))//���ȺͿ�ȴ���Ĳ�����������ı߽磬��Ҫ��1������
			if (inbase[next_row][next_col] == MINE_SIGN || inbase[next_row][next_col] == TAB_MINE_SIGN)//ע��MINE_SIGN��TAB_MINE_SIGN�����ף�����
				mines++;
	}

	/*����߽�򵽴����з�0�������׵�λ�ý�������*/
	if (mines == 0) {//��������Χ�ƶ���
		inbase[row][col] = BLANK;
		for (i = 0; i <= 7; i++) {
			int next_row = row + row_move[i];
			int next_col = col + col_move[i];
			if (next_row >= 0 && next_row <= (row_max - 1) && next_col >= 0 && next_col <= (col_max - 1))//���ȺͿ�ȴ���Ĳ�����������ı߽磬��Ҫ��1������
				if (inbase[next_row][next_col] == COVER_SIGN)//��������δ��������λ���ұ���Ҫ��δ����ǵĵط��ű�����������ǵĵط�������ɢ
					spread(MINE_CGI, inbase, next_row, next_col);//�������ݵ���λ�����꣨��0��ʼ��������
		}
	}
	else {
		inbase[row][col] = mines;
		return;
	}

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �����ж���Ϸ�Ƿ�ɹ�
***************************************************************************/
int judge_win(CONSOLE_GRAPHICS_INFO* const MINE_CGI, const int inbase[ROW_MAX][COL_MAX])
{

	int row_max = MINE_CGI->row_num;
	int col_max = MINE_CGI->col_num;

	/*�����ж��Ƿ�ɹ�*/
	int row_move[8] = { 1,1,1,0,0,-1,-1,-1 };
	int col_move[8] = { 1,0,-1,1,-1,-1,0,1 };
	for (int i = 0; i <= (row_max - 1); i++)//��δ��ֵ�׵�λ�ø�ֵ��Χ������
	{
		for (int j = 0; j <= (col_max - 1); j++)
		{
			if (inbase[i][j] == MINE_SIGN || inbase[i][j] == TAB_MINE_SIGN)
				;
			else {
				int mines = 0;
				for (int k = 0; k <= 7; k++)//ͳ��8�������ϵ��׵�����
				{
					int next_row = i + row_move[k];
					int next_col = j + col_move[k];
					if (next_row >= 0 && next_row <= (row_max - 1) && next_col >= 0 && next_col <= (col_max - 1))
						if (inbase[next_row][next_col] == MINE_SIGN || inbase[next_row][next_col] == TAB_MINE_SIGN)
							mines++;
				}
				if (inbase[i][j] != mines)
					return 0;//���δ��ȫ�㿪 ��ʾδ�ɹ�������
			}
		}
	}
	return 1;//�Ѿ��ɹ�

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������ݱ�ǵ�ֵ ���м�������
***************************************************************************/
void mine_count(CONSOLE_GRAPHICS_INFO* const MINE_CGI, const int inbase[ROW_MAX][COL_MAX], int mine_num)
{
	char cat_top[64];
	char nor_top[64] = "��ESC�˳����ո���ʾʱ��";
	int num = 0;
	for (int i = 0; i < MINE_CGI->row_num; i++)
		for (int j = 0; j < MINE_CGI->col_num; j++)
			if (inbase[i][j] == TAB_MINE_SIGN ||
				inbase[i][j] == TAB_NOT_SIGN)
				num++;
	sprintf(cat_top, "ʣ�������� %d ", (mine_num - num));

	/*����״̬����ʾʣ������*/
	gmw_status_line(MINE_CGI, TOP_STATUS_LINE, nor_top, cat_top);

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����cosnt bool win_los Ϊ1 ��ʾ�ɹ� ; Ϊ0��ʾʧ��
			(Ĭ��win_losΪʧ��0)
***************************************************************************/
void time_out(CONSOLE_GRAPHICS_INFO* const MINE_CGI, const clock_t start_time, const int top_lower, const bool win_los)
{

	/*������Ҫ��ӡ��ֵ*/
	char cat_temp[64];
	char nor_temp_mid[64] = "��ESC�˳����ո���ʾʱ��";
	char nor_temp_los[64] = "�����ˣ���Ϸ����������";
	char nor_temp_win[64] = "��ϲ�㣬��Ӯ����������������";

	double time = ((double)(clock()) - (double)(start_time)) / CLOCKS_PER_SEC;
	switch (top_lower)
	{
		case TOP_STATUS_LINE:
			sprintf(cat_temp, "��ǰʱ�䣺%.6f�� ", time);
			gmw_status_line(MINE_CGI, TOP_STATUS_LINE, nor_temp_mid, cat_temp);//��ӡ��״̬�����ַ���
			break;

		case LOWER_STATUS_LINE:

			if (win_los) {//�ɹ�������
				sprintf(cat_temp, "����ʱ��%.6f�� ", time);
				gmw_status_line(MINE_CGI, LOWER_STATUS_LINE, nor_temp_win, cat_temp);//��ӡ��״̬�����ַ���
			}
			else {//ʧ�ܣ�����
				sprintf(cat_temp, "����ʱ��%.6f�� ", time);
				gmw_status_line(MINE_CGI, LOWER_STATUS_LINE, nor_temp_los, cat_temp);//��ӡ��״̬�����ַ���
			}
			break;

		default://��ʱ�������������
			break;

	}

	return;

}

