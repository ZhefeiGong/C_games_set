/* 2052532 ���ܷ� ��04 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "../include/cmd_console_tools.h"        //���������й��ߺ���
#include "../include/cmd_gmw_tools.h"            //����αͼ�ι��ߺ���
#include "../include/common_output.h"            //�����Լ�д�Ĺ��ߺ���
#include "90-02-b1-gmw-eliminate_stars.h"        //��������Ŀ��ͷ�ļ�

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
  ˵    �������� 1 ����
			���� 0 �˳�
***************************************************************************/
int First(void)
{
	cct_cls();//����
	cct_setconsoleborder(50, 15, 50, 15);//��ʼ������
	cct_setfontsize("������", 32, 14);
	cct_setcursor(CURSOR_VISIBLE_NORMAL);
	system("color B9");//��ʼ��������ɫ
	cct_setcolor(COLOR_HCYAN, COLOR_HRED);
	cct_gotoxy(12, 5);
	cout << "�X�T�T�T�T�T�T�T�T�T�T�[";
	cct_gotoxy(12, 6);
	cout << "�U��ӭ��������������Ϸ�U";
	cct_gotoxy(12, 7);
	cout << "�^�T�T�T�T�T�T�T�T�T�T�a";
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
  ˵    ����������Ҫ��ӡ������
***************************************************************************/
void rowcol(CONSOLE_GRAPHICS_INFO* const  STAR_CGI)
{
	cct_cls();//����

	/*��ʼ����Ҫ�����ֵ*/
	int tem_row, tem_col;

	/*������*/
	cout << "����������(8-10)�� ";
	int x_1, y_1;
	cct_getxy(x_1, y_1);
	while (1)
	{
		cin >> tem_row;
		if (!cin)//����������
		{
			cin.clear();
			cin.ignore(1024, '\n');//��������ǲ���ջ�����(1024�����������з�Ϊֹ)
			cout << "���벻�Ϸ�������������";
			cct_gotoxy(x_1, y_1);//�ص���������λ��
			cout << "                      ";//����ղŵ���������
			cct_gotoxy(x_1, y_1);//�ص���������λ��
		}
		else
		{
			if (tem_row < ROW_MIN || tem_row>ROW_MAX)//��ʱ��������û������Ҳ���벻��ȷ
			{
				cin.ignore(1024, '\n');//��������ǲ���ջ�����(1024�����������з�Ϊֹ)
				cout << "���벻�Ϸ�������������";
				cct_gotoxy(x_1, y_1);//�ص���������λ��
				cout << "                      ";//����ղŵ���������
				cct_gotoxy(x_1, y_1);//�ص���������λ��
			}
			else
			{
				cout << "                      ";//�ڸ�֮ǰ�ġ����벻�Ϸ�����ʾ
				cct_gotoxy(0, (y_1 + 1));//������һ������λ��
				break;//������ȷ����whileѭ��
			}
		}
	}//end of while

	/*������*/
	cout << "����������(8-10)�� ";
	int x_2, y_2;
	cct_getxy(x_2, y_2);
	while (1)
	{
		cin >> tem_col;
		if (!cin)//����������
		{
			cin.clear();
			cin.ignore(1024, '\n');//��������ǲ���ջ�����(1024�����������з�Ϊֹ)
			cout << "���벻�Ϸ�������������";
			cct_gotoxy(x_2, y_2);//�ص���������λ��
			cout << "                       ";//����ղŵ���������
			cct_gotoxy(x_2, y_2);//�ص���������λ��
		}
		else
		{
			if (tem_col < COL_MIN || tem_col>COL_MAX)//��ʱ��������û������Ҳ���벻�Ϸ�
			{
				cin.ignore(1024, '\n');//��������ǲ���ջ�����(1024�����������з�Ϊֹ)
				cout << "���벻�Ϸ�������������";
				cct_gotoxy(x_2, y_2);//�ص���������λ��
				cout << "                      ";//����ղŵ���������
				cct_gotoxy(x_2, y_2);//�ص���������λ��
			}
			else
			{
				cout << "                      ";//�ڸ�֮ǰ�ġ����벻�Ϸ�����ʾ
				cct_gotoxy(0, (y_2 + 1));
				break;
			}
		}
	}//end of while

	/*�����������еĳ�ʼ��*/
	gmw_set_rowcol(STAR_CGI, tem_row, tem_col);

	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �����������ǵĳ�ʼ������
***************************************************************************/
void popstar_start(CONSOLE_GRAPHICS_INFO* const  STAR_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi)
{
	cct_cls();//����

	/*���г�ʼ��*/
	gmw_set_frame_color(STAR_CGI, COLOR_HWHITE, COLOR_BLACK);        //���ÿ����ɫ
	gmw_set_font(STAR_CGI, "��������", 16, 8);                       //��������
	gmw_set_status_line_switch(STAR_CGI, TOP_STATUS_LINE, true);     //������״̬��
	gmw_set_status_line_switch(STAR_CGI, LOWER_STATUS_LINE, true);   //������״̬��
	gmw_set_frame_style(STAR_CGI, BLOCK_WIDTH, BLOCK_HIGH, true);    //��Ҫ�߿�
	gmw_set_block_border_switch(STAR_CGI, true);                     //ɫ������Ҫ�߿�
	gmw_set_rowno_switch(STAR_CGI, true);                            //��Ҫ�к�
	gmw_set_colno_switch(STAR_CGI, true);                            //��Ҫ�б�

	/*���û���ܵ���ʱ*/
	STAR_CGI->delay_of_draw_frame = DELAY_FRAME;
	STAR_CGI->delay_of_block_moved = DELAY_MOVE;

	cct_setcursor(CURSOR_INVISIBLE);                                //��겻��ʾ

	/*��ӡ���*/
	gmw_draw_frame(STAR_CGI);                           

	/*��ӡ��״̬����ʼ��Ϣ*/
	char temp[64];
	sprintf(temp, "��Ļ��ǰ����Ϊ��%d��%d��", STAR_CGI->lines, STAR_CGI->cols);
	gmw_status_line(STAR_CGI, TOP_STATUS_LINE, temp);

	

	/*���ڲ����������ʼ��*/
	srand(unsigned(time(NULL)));//ȡ���µ�ϵͳʱ��Ϊ���ӵ�ֵ
	for (int j = 0; j < STAR_CGI->row_num; j++)
		for (int i = 0; i < STAR_CGI->col_num; i++)
		{
			while (1)
			{
				int star = random(6);//��ʱ�������0-5������
				if (star >= 1 && star <= 5)//ֻ�ܸ�ֵΪ1-5
				{
					inbase[j][i] = star;//��ֵΪ�ַ�1-5������
					break;
				}
			}
		}

	/*��ӡɫ����*/
	for (int j = 0; j < STAR_CGI->row_num; j++)
		for (int i = 0; i < STAR_CGI->col_num; i++)
			gmw_draw_block(STAR_CGI, j, i, inbase[j][i], bdi);

	return;

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �����������ǵ�����ʵ�ֺ���
***************************************************************************/
void popstar_mid(CONSOLE_GRAPHICS_INFO* const  STAR_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi,int &score)
{

	/*��ʼ������*/
	int maction, old_mrow, old_mcol, mrow = -1, mcol = -1;
	int keycode1, keycode2;
	int ret;

	while (1) {
		/*��¼��һ�ε�����λ��*/
		old_mrow = mrow;
		old_mcol = mcol;
		ret = gmw_read_keyboard_and_mouse(STAR_CGI, maction, mrow, mcol, keycode1, keycode2);

		/*����0��λ��Ϊ�Ƿ�λ��*/
		if (mrow >= 0 && mcol >= 0)
			if (inbase[mrow][mcol] == BLANK) {
				mrow = -1;
				mcol = -1;
			}

		/*����¼�*/
		if (ret == CCT_MOUSE_EVENT) {

			/*����ƶ�*/
			if (maction == MOUSE_ONLY_MOVED) {

				/*�Ƿ�λ�õ�����*/
				if (mrow == -1 || mcol == -1) {//��ӡλ�÷Ƿ���־
					char temp[64] = "[��ǰ���] λ�÷Ƿ�";
					gmw_status_line(STAR_CGI, LOWER_STATUS_LINE, temp);

				}

				/*���δ�㿪 ����ɢ�������ԭ*/
				back(STAR_CGI, inbase, bdi);

				/* ԭɫ��������ʾ */
				if (old_mrow >= 0 && old_mcol >= 0)
					gmw_draw_block(STAR_CGI, old_mrow, old_mcol, inbase[old_mrow][old_mcol], bdi);

				/* ��ɫ������ */
				if (mrow >= 0 && mcol >= 0)
					gmw_draw_block(STAR_CGI, mrow, mcol, inbase[mrow][mcol] + BETWEEN, bdi);

			}

			/*�������*/
			else if (maction == MOUSE_LEFT_BUTTON_CLICK) {
				if (mrow >= 0 && mcol >= 0) {
					if (mrow == old_mrow && mcol == old_mcol) {

						if (inbase[mrow][mcol] > BETWEEN) {//���е㿪
							count(STAR_CGI, inbase, score);
							move(STAR_CGI, inbase, bdi);
							if (judge_win(STAR_CGI, inbase))//�ɹ�
								return;
						}
						else {//�㿪ȷ��
							if (same_spread(STAR_CGI, inbase, mrow, mcol)) {//�ɽ�����ɢ
								char temp[64] = "��ͷ��/����ƶ�ȡ����ǰѡ�񣬻س���/��������ϳ�";
								gmw_status_line(STAR_CGI, LOWER_STATUS_LINE, temp);//���·���ӡ��ʾ
								for (int j = 0; j < STAR_CGI->row_num; j++)
									for (int i = 0; i < STAR_CGI->col_num; i++)
										if (inbase[j][i] > BETWEEN)
											gmw_draw_block(STAR_CGI, j, i, inbase[j][i], bdi);//���Դ�ӡ����
							}
							else {//���ɽ�����ɢ
								char nor_temp[64] = "��ͷ��/����ƶ����س���/�������ѡ��";
								char cat_temp[64] = "��Χ����ֵͬ�� ";
								gmw_status_line(STAR_CGI, LOWER_STATUS_LINE, nor_temp, cat_temp);
							}

						}
					}
				}
				else
					continue;
			}

			/*�������������*/
			else
				continue;
		}

		/*�����¼�*/
		else {
			switch (keycode1) {
				case 0x0D://�س���
					if (mrow >= 0 && mcol >= 0) {
							if (inbase[mrow][mcol] > BETWEEN) {//���е㿪
								count(STAR_CGI, inbase, score);
								move(STAR_CGI, inbase, bdi);
								if (judge_win(STAR_CGI, inbase))
									return;
									
							}
							else {//�㿪ȷ��
								if (same_spread(STAR_CGI, inbase, mrow, mcol)) {//�ɽ�����ɢ
									char temp[64] = "��ͷ��/����ƶ�ȡ����ǰѡ�񣬻س���/��������ϳ�";
									gmw_status_line(STAR_CGI, LOWER_STATUS_LINE, temp);//���·���ӡ��ʾ
									for (int j = 0; j < STAR_CGI->row_num; j++)
										for (int i = 0; i < STAR_CGI->col_num; i++)
											if (inbase[j][i] > BETWEEN)
												gmw_draw_block(STAR_CGI, j, i, inbase[j][i], bdi);//���Դ�ӡ����
								}
								else {//���ɽ�����ɢ
									char nor_temp[64] = "��ͷ��/����ƶ����س���/�������ѡ��";
									char cat_temp[64] = "��Χ����ֵͬ�� ";
									gmw_status_line(STAR_CGI, LOWER_STATUS_LINE, nor_temp, cat_temp);
								}

							}
					}
					else
						continue;
					break;
				case 0xE0: //224 ���µ�Ϊ���̼�

					/*���δ�㿪 ����ɢ�������ԭ*/
					back(STAR_CGI, inbase, bdi);
					/*ȷ�� mrow��mcol��ֵ*/
					if (old_mrow < 0 || old_mcol < 0)//�ϴ�����Ƿ� ���δ����´���ʼ
					{
						mrow = STAR_CGI->row_num - 1;
						mcol = 0;
					}
					else
					{
						mrow = old_mrow;
						mcol = old_mcol;
					}
					char temp[64];//��ӡ����λ��
					switch (keycode2) {
						case KB_ARROW_UP:
							for (int i = 0; i < STAR_CGI->row_num; i++)//���ֻ�ܽ���STAR_CGI->row_num�Σ��������������Ϊ����
							{
								mrow--;
								if (mrow == -1)
									mrow = STAR_CGI->row_num - 1;
								if (inbase[mrow][mcol] != BLANK)//BLANK���ɷǷ�����
									break;
							}
							sprintf(temp, "[��ǰ����] %c��%d��", (char)(mrow + 'A'), mcol);
							gmw_status_line(STAR_CGI, LOWER_STATUS_LINE, temp);
							break;
						case KB_ARROW_DOWN:
							for (int i = 0; i < STAR_CGI->row_num; i++)//���ֻ�ܽ���STAR_CGI->row_num�Σ��������������Ϊ����
							{
								mrow++;
								if (mrow == STAR_CGI->row_num)
									mrow = 0;
								if (inbase[mrow][mcol] != BLANK)//BLANK���ɷǷ�����
									break;
							}
							sprintf(temp, "[��ǰ����] %c��%d��", (char)(mrow + 'A'), mcol);
							gmw_status_line(STAR_CGI, LOWER_STATUS_LINE, temp);
							break;
						case KB_ARROW_LEFT:
							for (int i = 0; i < STAR_CGI->col_num; i++)//���ֻ�ܽ���STAR_CGI->col_num�Σ��������������Ϊ����
							{
								mcol--;
								if (mcol == -1)
									mcol = STAR_CGI->col_num - 1;
								if (inbase[mrow][mcol] != BLANK)//BLANK���ɷǷ�����
									break;
							}
							sprintf(temp, "[��ǰ����] %c��%d��", (char)(mrow + 'A'), mcol);
							gmw_status_line(STAR_CGI, LOWER_STATUS_LINE, temp);
							break;
						case KB_ARROW_RIGHT:
							for (int i = 0; i < STAR_CGI->col_num; i++)//���ֻ�ܽ���STAR_CGI->col_num�Σ��������������Ϊ����
							{
								mcol++;
								if (mcol == STAR_CGI->col_num)
									mcol = 0;
								if (inbase[mrow][mcol] != BLANK)//BLANK���ɷǷ�����
									break;
							}
							sprintf(temp, "[��ǰ����] %c��%d��", (char)(mrow + 'A'), mcol);
							gmw_status_line(STAR_CGI, LOWER_STATUS_LINE, temp);
							break;
					}
					/* ԭɫ��������ʾ */
					if (old_mrow >= 0 && old_mcol >= 0)
						gmw_draw_block(STAR_CGI, old_mrow, old_mcol, inbase[old_mrow][old_mcol], bdi);

					/* ��ɫ������ */
					if (mrow >= 0 && mcol >= 0)
						gmw_draw_block(STAR_CGI, mrow, mcol, inbase[mrow][mcol] + BETWEEN, bdi);

					break;

				default://�����������迼��
					break;
			}//end of switch
		}//end of else
	}//end of while

	/*�޷���ֵ*/
	return;

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ���������������ӡ�ĺ���
***************************************************************************/
void count(CONSOLE_GRAPHICS_INFO* const  STAR_CGI, const int inbase[ROW_MAX][COL_MAX], int& score)
{
	int num = 0;
	int score_now = 0;
	for (int row = 0; row < STAR_CGI->row_num; row++)
		for (int col = 0; col < STAR_CGI->col_num; col++)
			if (inbase[row][col] >BETWEEN)
				num++;
	score_now = num * num * 5;
	score += score_now;
	char temp[64];
	sprintf(temp, "���ε÷֣�%d �ܷ֣�%d", score_now, score);
	gmw_status_line(STAR_CGI, TOP_STATUS_LINE, temp);//��ӡ������Ϣ

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������� ���� 1
            �˳� ���� 0
***************************************************************************/
int popstar_end(CONSOLE_GRAPHICS_INFO* const  STAR_CGI, const int inbase[ROW_MAX][COL_MAX],int & score)
{
	/*���㱾�ε÷�*/
	int num = 0;
	int score_now = 0;
	for (int row = 0; row < STAR_CGI->row_num; row++)
		for (int col = 0; col < STAR_CGI->col_num; col++)
			if (inbase[row][col] >= 1 && inbase[row][col] <= 5)
				num++;
	if (num < 10)
		score_now = (10 - num) * 180;
	score += score_now;
	char up_temp[64];
	sprintf(up_temp, "�����÷֣�%d �ܷ֣�%d",score_now, score);
	gmw_status_line(STAR_CGI, TOP_STATUS_LINE, up_temp);//��ӡ������Ϣ



	char cat_temp[64];
	char nor_temp[64] = "�س����� ESC����";
	sprintf(cat_temp, "ʣ��%d�����ǣ��޿���������ؽ�����", num);
	gmw_status_line(STAR_CGI, LOWER_STATUS_LINE, nor_temp, cat_temp);//��ӡ��ʾ��Ϣ

	char shuru;
	while (1)//�������뷵��
	{
		shuru = _getch();
		if (shuru == '\r')//�س���
			return 1;
		else if (shuru == 27)//ESC��
			return 0;
	}


}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ɢ�������ڲ����ĵݹ麯��
***************************************************************************/
void  same_spread_in(CONSOLE_GRAPHICS_INFO* const  STAR_CGI, const int inbase[ROW_MAX][COL_MAX], const int row, const int col, int in[ROW_MAX][COL_MAX])
{
	/*�����ģʽ�µ������С*/
	int row_max = STAR_CGI->row_num;
	int col_max = STAR_CGI->col_num;

	/*�����ĸ����򣺶� �� �� ��  */
	int row_move[DIRECTION] = { 1,0,-1, 0 };
	int col_move[DIRECTION] = { 0,1, 0,-1 };

	int num = 0;

	for (int k = 0; k < DIRECTION; k++) {
		int next_row = row + row_move[k];//����λ�������x
		int next_col = col + col_move[k];//����λ�������y
		if (next_row >= 0 && next_row < row_max && next_col >= 0 && next_col < col_max) {//ֻ�г��Ϳ��ڸ÷�Χ�ڲſɼ���
			if (inbase[next_row][next_col] >= 1 && inbase[next_row][next_col] <= 5 && in[next_row][next_col] == 0) {//ֻ�д���1-5�ſɽ�����ɢ�Ҽ�¼��������λ��Ϊ0

				if (inbase[row][col] == inbase[next_row][next_col])
				{
					num++;
					in[next_row][next_col] = BETWEEN;//����������ֵͬ��in�����¼λ��
					same_spread_in(STAR_CGI, inbase, next_row, next_col, in);//��ʱ����ݹ飬��˵���Χ����һ���ַ�ʱ����λ�����跴��
				}
			}
		}
	}

	if (in[row][col] == 0)
		in[row][col] = BETWEEN;

	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������ݵ㿪λ�õ��ڲ�����ģʽ������ɢ
***************************************************************************/
int same_spread(CONSOLE_GRAPHICS_INFO* const  STAR_CGI, int inbase[ROW_MAX][COL_MAX], const int row, const int col)
{
	/*����һ����¼��ɢ���������*/
	int in[ROW_MAX][COL_MAX] = {
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
	int row_max = STAR_CGI->row_num;
	int col_max = STAR_CGI->col_num;
	int num = 0;

	/*���ݱ������ �ı���ʱ�����ֵ*/
	same_spread_in(STAR_CGI, inbase, row, col, in);

	/*��ѯ�������*/
	for (int i = 0; i < row_max; i++)
		for (int j = 0; j < col_max; j++)
		{
			if (in[i][j] == BETWEEN)
				num++;
		}

	if (num == 1)     //��λ���޷�������ɢ
		return 0;
	else              //��λ�ÿ��Խ�����ɢ
	{
		for (int i = 0; i < row_max; i++)
			for (int j = 0; j < col_max; j++)
			{
				if (in[i][j] == BETWEEN)
					inbase[i][j] += BETWEEN;
			}
	}

	return 1;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������� 1 ��Ϸ����
			���� 0 ��Ϸ����
***************************************************************************/
int judge_win(CONSOLE_GRAPHICS_INFO* const  STAR_CGI, const int inbase[ROW_MAX][COL_MAX])
{
	/*�����ĸ����򣺶� �� �� ��  */
	int row_move[DIRECTION] = { 1,0,-1, 0 };
	int col_move[DIRECTION] = { 0,1, 0,-1 };

	int row_max = STAR_CGI->row_num;
	int col_max = STAR_CGI->col_num;

	/*�������������ڵ�����(����1-5)*/
	for (int row = 0; row < row_max ; row++)
	{
		for (int col = 0; col < col_max; col++)
		{

			if (inbase[row][col] >= 1 && inbase[row][col] <= 5)//����Ϊ����1-5ʱ�ſ�ʼ����
				for (int k = 0; k < DIRECTION; k++)
				{
					int row_next = row + row_move[k];
					int col_next = col + col_move[k];
					if (row_next >= 0 && row_next < row_max && col_next >= 0 && col_next < col_max)
					{
						if (inbase[row][col] == inbase[row_next][col_next])
							return 0;
					}
				}
		}
	}
	return 1;

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ���������Ե��ڲ����黹ԭ
***************************************************************************/
void back(CONSOLE_GRAPHICS_INFO* const  STAR_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi)
{
	int row_max = STAR_CGI->row_num;
	int col_max = STAR_CGI->col_num;

	/*�ָ����� �޸��ڲ�ֵ ��ԭ��ӡ����*/
	for (int row = 0; row < row_max; row++)
		for (int col = 0; col < col_max; col++)
		{
			if (inbase[row][col] > BETWEEN)
				inbase[row][col] -= BETWEEN;
			gmw_draw_block(STAR_CGI, row, col, inbase[row][col], bdi);
		}

	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ��������ͼ�ν����µ�ɫ���ƶ�
***************************************************************************/
void move(CONSOLE_GRAPHICS_INFO* const  STAR_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi)
{
	int row_max = STAR_CGI->row_num;
	int col_max = STAR_CGI->col_num;

	/*ʹ�÷��Ե�λ�ñ�ΪBLANK*/
	for (int row = 0; row < row_max; row++)
		for (int col = 0; col < col_max; col++)
		{
			if (inbase[row][col] > BETWEEN) {
				inbase[row][col] = BLANK;
				gmw_draw_block(STAR_CGI, row, col, inbase[row][col], bdi);
			}
		}

	Sleep(SLEEP_BLANK);

    /*����������� ���ı��ڲ�����*/
	int move_down_base[ROW_MAX][COL_MAX] = {
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

    /*����仯��ɫ����Ҫ�����ƶ��ľ���*/
	for (int col = 0; col < col_max; col++)
	{
		for (int row = 0; row < row_max; row++)
		{
			int num = 0;
			if (inbase[row][col] != BLANK)//�����·��ж���BLANK �����ƶ�
			{
				for (int tem_row = (row+1) ; tem_row <row_max; tem_row++)
					if (inbase[tem_row][col] == BLANK)
						num++;
				
			}
			move_down_base[row][col] = num;
		}
			
	}//end of for col

	/*�����ƶ�*/
	for (int col = 0; col < col_max; col++)
		for (int row = row_max - 1; row >= 0; row--)//����������!!!!!
				gmw_move_block(STAR_CGI, row, col, inbase[row][col], BLANK, bdi, UP_TO_DOWN, move_down_base[row][col]);

	 /*�ı��ڲ�����*/
	for (int col = 0; col < col_max; col++)
	{
		for (int row = 0; row < row_max; row++)
		{
			if (inbase[row][col] == BLANK)
			{
				for (int tem_row = row; tem_row >= 1; tem_row--)//��0����ð��
				{
					int mid;
					mid = inbase[tem_row][col];
					inbase[tem_row][col] = inbase[tem_row - 1][col];
					inbase[tem_row - 1][col] = mid;
				}
			}
		}
	}//end of for col


	/*����������� ���ı��ڲ�����*/
	int blank_inbase[COL_MAX] = { 1,1,1,1,1,1,1,1,1,1 };//����һ���Ƿ�ȫΪ0�����
	int move_left_inbase[COL_MAX] = { 0,0,0,0,0,0,0,0,0,0 };//�����ƶ����

    /*����һ���Ƿ�ȫΪBLANK*/
	for (int col = 0; col < col_max; col++)
	{
		for (int row = 0; row < row_max; row++)
		{
			if (inbase[row][col] != BLANK)
				break;
			if (row == row_max - 1)//��ȫΪBLANKʱ
				blank_inbase[col] = 0;
		}
	}//end of for col

	/*�����ƶ����*/
	for (int col = 0; col < col_max; col++)
	{
		/*����仯��ɫ������Ҫ���Ƶľ���*/
		int num = 0;
		for (int tem_col = (col - 1); tem_col >= 0; tem_col--)//����ж��ٿհ�
			if (blank_inbase[tem_col] == BLANK)
				num++;
		move_left_inbase[col] = num;
	}
	/*���ƶ���ʼ*/
	for (int row = 0; row < row_max; row++)
		for (int col = 0; col < col_max; col++)
			if (inbase[row][col] != BLANK)
				gmw_move_block(STAR_CGI, row, col, inbase[row][col], BLANK, bdi, RIGHT_TO_LEFT, move_left_inbase[col]);

    /*�ı��ڲ�����*/
	for (int col = 0; col < col_max; col++)
	{
		if (blank_inbase[col] == BLANK)
		{
			for (int row = 0; row < row_max; row++)
				for (int tem_col = col; tem_col < (col_max - 1); tem_col++)//��������ð��
				{
					int mid;
					mid = inbase[row][tem_col];
					inbase[row][tem_col] = inbase[row][tem_col + 1];
					inbase[row][tem_col + 1] = mid;
				}
		}
	}
	
	/*�޷���ֵ*/
	return;
}
