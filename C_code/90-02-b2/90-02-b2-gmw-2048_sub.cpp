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
	����ڲ�ʵ�ֺ���
   ----------------------------------------- */


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ͼ�ν���
***************************************************************************/
int First(void)
{
	cct_cls();//����
	cct_setconsoleborder(50, 15, 50, 15);//��ʼ������
	cct_setfontsize("������", 32, 14);
	cct_setcursor(CURSOR_VISIBLE_NORMAL);
	system("color D9");//��ʼ��������ɫ
	cct_setcolor(COLOR_HPINK, COLOR_HGREEN);
	cct_gotoxy(14, 5);
	cout << "�X�T�T�T�T�T�T�T�T�[";
	cct_gotoxy(14, 6);
	cout << "�U��ӭ����2048��Ϸ�U";
	cct_gotoxy(14, 7);
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
  ˵    ����������Ϸ����
***************************************************************************/
void level(CONSOLE_GRAPHICS_INFO* const  M2048_CGI,int &score)
{
	cct_cls();//����
	cct_setconsoleborder(50, 15, 50, 15);//��ʼ������
	system("color 07");//��ʼ��������ɫ
	cct_setcolor();//Ĭ����ɫ����
	cct_setfontsize("������", 38, 16);//��������

	int tem_row, tem_col;
	int delay;
	int time = 0;

	cout << "��Ϸ2048�������ã�" << endl;

	/*��������*/
	cout << "����������[4-8]";
	int x_1, y_1;
	cct_getxy(x_1, y_1);
	while (1){
		cin >> tem_row;
		if (!cin) {//����������
			cin.clear();
			cin.ignore(1024, '\n');//��������ǲ���ջ�����(1024�����������з�Ϊֹ)
			cct_gotoxy(x_1, y_1);
			cout << "                    ";//����ղŵ���������
			cct_gotoxy(x_1, y_1);
		}
		else {//���������ȷ
			if (tem_row<ROW_MIN || tem_row>ROW_MAX) {
				cin.ignore(1024, '\n');//��������ǲ���ջ�����(1024�����������з�Ϊֹ)
				cct_gotoxy(x_1, y_1);
				cout << "                    ";//����ղŵ���������
				cct_gotoxy(x_1, y_1);
			}
			else
				break;//������ȷ����ѭ��
		}
	}

	/*��������*/
	cout << "����������[4-10]";
	int x_2, y_2;
	cct_getxy(x_2, y_2);
	while (1) {
		cin >> tem_col;
		if (!cin) {//����������
			cin.clear();
			cin.ignore(1024, '\n');//��������ǲ���ջ�����(1024�����������з�Ϊֹ)
			cct_gotoxy(x_2, y_2);
			cout << "                    ";//����ղŵ���������
			cct_gotoxy(x_2, y_2);
		}
		else {//���������ȷ
			if (tem_col<COL_MIN || tem_col>COL_MAX) {
				cin.ignore(1024, '\n');//��������ǲ���ջ�����(1024�����������з�Ϊֹ)
				cct_gotoxy(x_2, y_2);
				cout << "                    ";//����ղŵ���������
				cct_gotoxy(x_2, y_2);
			}
			else
				break;//������ȷ����ѭ��
		}
	}

	/*����Ŀ�����*/
	int score_1;
	switch (tem_row + tem_col)
	{
		case 8:
			score_1 = 512;
			break;
		case 9:
		case 10:
			score_1 = 1024;
			break;
		case 11:
		case 12:
			score_1 = 2048;
			break;
		case 13:
		case 14:
			score_1 = 4096;
			break;
		default:
			score_1 = 8192;
			break;
	}
	int score_2 = score_1 * 2;
	int score_3 = score_2 * 2;
	int score_4 = score_3 * 2;
	cout << "������Ŀ�����[" << score_1 << '/' << score_2 << '/' << score_3 << '/' << score_4 << ']';
	int x_3, y_3;
	cct_getxy(x_3, y_3);
	while (1) {
		cin >> score;
		if (!cin) {//����������
			cin.clear();
			cin.ignore(1024, '\n');//��������ǲ���ջ�����(1024�����������з�Ϊֹ)
			cct_gotoxy(x_3, y_3);
			cout << "                    ";//����ղŵ���������
			cct_gotoxy(x_3, y_3);
		}
		else {//���������ȷ
			if (score != score_1 && score != score_2 && score != score_3 && score != score_4) {
				cin.ignore(1024, '\n');//��������ǲ���ջ�����(1024�����������з�Ϊֹ)
				cct_gotoxy(x_3, y_3);
				cout << "                    ";//����ղŵ���������
				cct_gotoxy(x_3, y_3);
			}
			else
				break;//������ȷ����ѭ��
		}
	}

	/*���붯����ʱ*/
	cout << "�����붯����ʱ[0-5]";
	int x_4, y_4;
	cct_getxy(x_4, y_4);
	while (1) {
		cin >> delay;
		if (!cin) {//����������
			cin.clear();
			cin.ignore(1024, '\n');//��������ǲ���ջ�����(1024�����������з�Ϊֹ)
			cct_gotoxy(x_4, y_4);
			cout << "                    ";//����ղŵ���������
			cct_gotoxy(x_4, y_4);
		}
		else {//���������ȷ
			if (delay < 0 || delay>5) {
				cin.ignore(1024, '\n');//��������ǲ���ջ�����(1024�����������з�Ϊֹ)
				cct_gotoxy(x_4, y_4);
				cout << "                    ";//����ղŵ���������
				cct_gotoxy(x_4, y_4);
			}
			else
				break;//������ȷ����ѭ��
		}
	}

	/*��������*/
	gmw_set_rowcol(M2048_CGI, tem_row, tem_col);//�������еĳ�ʼ��

	/*������ʱ*/
	switch (delay)
	{
		case 0:
			time = DELAY_0;
			break;
		case 1:
			time = DELAY_1;
			break;
		case 2:
			time = DELAY_2;
			break;
		case 3:
			time = DELAY_3;
			break;
		case 4:
			time = DELAY_4;
			break;
		case 5:
			time = DELAY_5;
			break;
		default:
			cout << "error!!!";
			break;

	}
	M2048_CGI->delay_of_block_moved = time;
	M2048_CGI->delay_of_draw_block = 0;


}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʼ��2048
***************************************************************************/
void my2048_start(CONSOLE_GRAPHICS_INFO* const  M2048_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi,const int score_goal,const int score_highest)
{
	cct_cls();//����

	gmw_set_frame_default_linetype(M2048_CGI, 1);                      //ȫ˫
	gmw_set_frame_color(M2048_CGI, COLOR_HWHITE, COLOR_BLACK);         //���ÿ����ɫ
	gmw_set_status_line_switch(M2048_CGI, TOP_STATUS_LINE, true);      //������״̬��
	gmw_set_status_line_switch(M2048_CGI, LOWER_STATUS_LINE, true);   //�ر���״̬��
	gmw_set_font(M2048_CGI, "��������", 16, 8);                        //��������
	gmw_set_frame_style(M2048_CGI, BLOCK_WIDTH, BLOCK_HIGH, true);     //��Ҫ�߿�
	gmw_set_block_border_switch(M2048_CGI, true);                      //ɫ������Ҫ�߿�
	gmw_set_rowno_switch(M2048_CGI, false);                            //�����к�
	gmw_set_colno_switch(M2048_CGI, false);                            //�����б�

	cct_setcursor(CURSOR_INVISIBLE);                                   //��겻��ʾ

	/*��ӡ���*/
	gmw_draw_frame(M2048_CGI);

	/*��ӡ����ʾ����Ϣ*/
	char temp[128];
	sprintf(temp, "Ŀ��:%d ����:0 ���:%d ʱ��:0 (R:���� Q:�˳�)", score_goal, score_highest);
	gmw_status_line(M2048_CGI, TOP_STATUS_LINE, temp);
	
	/*��ʼ�� �ڽ����������������2*/
	srand(unsigned(time(NULL)));//ȡ���µ�ϵͳʱ��Ϊ���ӵ�ֵ
	for (int i = 1; i <= 2; i++) {
		while (1)
		{
			int row_mid = random(M2048_CGI->row_num);//�������0-��M2048_CGI->row_num-1��������
			int col_mid = random(M2048_CGI->col_num);//�������0-��M2048_CGI->col_num-1��������
			if (inbase[row_mid][col_mid] == BLANK) {
				inbase[row_mid][col_mid] = 2;
				break;
			}
			else
				continue;
		}
	}

	/*��ӡɫ����*/
	for (int j = 0; j < M2048_CGI->row_num; j++)
		for (int i = 0; i < M2048_CGI->col_num; i++)
			gmw_draw_block(M2048_CGI, j, i, inbase[j][i], bdi);

	return;

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������� 0 �˳���Ϸ ������Ϸ����
			���� 1 ��������Ϸ
			���� 2 ��Ϸ����
***************************************************************************/
int my2048_mid(CONSOLE_GRAPHICS_INFO* const  M2048_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi, const int score_goal, const int score_highest, int& score)
{

	/*��ʼ������*/
	int maction, mrow = -1, mcol = -1;
	int keycode1, keycode2;
	int ret;

	int start_time = (int)time(0);

	while (1) {
		ret = gmw_read_keyboard_and_mouse(M2048_CGI, maction, mrow, mcol, keycode1, keycode2);//�����Ǽ����¼�

		/*�����¼�*/
		if(ret==CCT_KEYBOARD_EVENT){
			if (keycode1 == 0xE0) { //224 ���µ�Ϊ���̼�
				switch (keycode2) {
					case KB_ARROW_UP:
						score += move_out(M2048_CGI, inbase, bdi, D_U);
						break;
					case KB_ARROW_DOWN:
						score += move_out(M2048_CGI, inbase, bdi,U_D);
						break;
					case KB_ARROW_LEFT:
						score += move_out(M2048_CGI, inbase, bdi,R_L);
						break;
					case KB_ARROW_RIGHT:
						score += move_out(M2048_CGI, inbase, bdi, L_R);
						break;
				}
				status_out(M2048_CGI, score, score_goal, score_highest, start_time);
				if (judge_win(M2048_CGI, inbase))//��ʱ��Ϸ����
					return 2;
				else
					creat(M2048_CGI, inbase, bdi);//��Ϸ����
			}
			else if (keycode1 == 'R' || keycode1 == 'r')
				return 1;
			else if (keycode1 == 'Q' || keycode1 == 'q')
				return 0;
		}//end of if
	}//end of while



}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������� 1 ��Ϸ����
            ���� 0 ��Ϸ�ص���ʼ����
***************************************************************************/
int my2048_end(CONSOLE_GRAPHICS_INFO* const  M2048_CGI)
{
	int x = M2048_CGI->start_x + (M2048_CGI->col_num / 2) * M2048_CGI->CFI.block_width - M2048_CGI->CFI.block_width / 2;
	int y = M2048_CGI->start_y + (M2048_CGI->row_num / 2) * M2048_CGI->CFI.block_high - M2048_CGI->CFI.block_high / 2;
	cct_setcolor(COLOR_HRED, COLOR_HWHITE);
	cct_gotoxy(x, y);
	cout << "�X�T�T�T�T�[";
	cct_gotoxy(x, y+1);
	cout << "�U��Ϸʧ�ܨU";
	cct_gotoxy(x, y + 2);
	cout << "�UR������ �U";
	cct_gotoxy(x, y + 3);
	cout << "�UQ���˳� �U";
	cct_gotoxy(x, y + 4);
	cout << "�^�T�T�T�T�a";

	cct_setcursor(CURSOR_INVISIBLE);//ʹ�����ʧ

	char shuru;
	while (1)
	{
		shuru = _getche();
		if (shuru == 'Q' || shuru == 'q')
			return 0;
		else if (shuru == 'R' || shuru == 'r')
			return 1;
	}


}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ӡ״̬����Ϣ
***************************************************************************/
void status_out(CONSOLE_GRAPHICS_INFO* const  M2048_CGI, int score_now,int score_goal, int score_highest, int start_time)
{
	int time_gap = (int)time(0) - start_time;
	char temp[128];
	sprintf(temp, "Ŀ��:%d ����:%d ���:%d ʱ��:%d (R:���� Q:�˳�)", score_goal, score_now, score_highest, time_gap);
	gmw_status_line(M2048_CGI, TOP_STATUS_LINE, temp);

	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �����������һ��2/4
***************************************************************************/
void creat(CONSOLE_GRAPHICS_INFO* const  M2048_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi)
{
	/*ȷ���������ֵ*/
	int row_max = M2048_CGI->row_num;
	int col_max = M2048_CGI->col_num;

	int go_on = 0;

	for (int row = 0; row < row_max; row++)
		for (int col = 0; col < col_max; col++)
			if (inbase[row][col] == BLANK)
				go_on = 1;


	if (go_on)//���ڰ׿�Ų���
	{
		/*�������ɶ*/
		srand(unsigned(time(NULL)));//ȡ���µ�ϵͳʱ��Ϊ���ӵ�ֵ
		int creat_block;
		int ran = random(100);
		if (ran >= 0 && ran <= 10)//1:9�Ĳ�������
			creat_block = 4;
		else
			creat_block = 2;

		while (1)
		{
			int row = random(row_max);
			int col = random(col_max);
			if (inbase[row][col] == BLANK){
				inbase[row][col] = creat_block;
				gmw_draw_block(M2048_CGI, row, col, inbase[row][col], bdi);
				break;
			}
		}

	}
	
	return;

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������� 1 ��Ϸ����
            ���� 0 ��Ϸ����
***************************************************************************/
int judge_win(CONSOLE_GRAPHICS_INFO* const  M2048_CGI, int inbase[ROW_MAX][COL_MAX])
{
	/*�����ĸ����򣺶� �� �� ��  */
	int row_move[DIRECTION] = { 1,0,-1, 0 };
	int col_move[DIRECTION] = { 0,1, 0,-1 };

	int row_max = M2048_CGI->row_num;
	int col_max = M2048_CGI->col_num;

	int go = 0;

	/*�������������ڵ�����(������Χ����ͬ����)*/
	for (int row = 0; row < row_max; row++)
	{
		for (int col = 0; col < col_max; col++)
		{

			if (inbase[row][col] > 0)//����Ϊ����ʱ�ſ�ʼ���� ΪBLANK(0)ʱ������
				for (int k = 0; k < DIRECTION; k++)
				{
					int row_next = row + row_move[k];
					int col_next = col + col_move[k];
					if (row_next >= 0 && row_next < row_max && col_next >= 0 && col_next < col_max)//�ڷ�Χ��
					{
						if (inbase[row][col] == inbase[row_next][col_next])//������Χ��ͬ��������
							return 0;
					}
				}
		}
	}

	/*�������Ƿ��пհ�λ��*/
	for (int j = 0; j < row_max; j++)
		for (int i = 0; i < col_max; i++)
			if (inbase[j][i] == BLANK)//���ڿհ�λ��
				return 0;


	return 1;//��ʱ��Ϸ����
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����������������н���ɫ����ƶ��Լ��ڲ�����ı仯
            �˺���Ϊ����Ϸ�ĺ��Ĵ��룡
***************************************************************************/
int move_out(CONSOLE_GRAPHICS_INFO* const  M2048_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi, int type)
{
	
	/*ȷ���������ֵ*/
	int row_max = M2048_CGI->row_num;
	int col_max = M2048_CGI->col_num;

	/*����ð�ݴ���*/
	int move_blank_base_1[ROW_MAX][COL_MAX] = {
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
    		{0,0,0,0,0,0,0,0,0,0},
	};

    switch (type)
	{
		case R_L:
			 /*����仯��ɫ����Ҫ�����ƶ��ľ���*/
			for (int row = 0; row < row_max; row++)
			{
				for (int col = (col_max-1); col >=0; col--)
				{
					int num = 0;
					if (inbase[row][col] != BLANK)//���������ж���BLANK �����ƶ�
					{
						for (int tem_col = (col - 1); tem_col >=0; tem_col--)
							if (inbase[row][tem_col] == BLANK)
								num++;

					}
					move_blank_base_1[row][col] = num;
				}

			}//end of for col

			/*�����ƶ�*/
			for (int row = 0; row < row_max; row++)
				for (int col = 0; col < col_max; col++)//����������!!!!!
					if(move_blank_base_1[row][col]!=0)
					gmw_move_block(M2048_CGI, row, col, inbase[row][col], BLANK, bdi, RIGHT_TO_LEFT, move_blank_base_1[row][col]);

			/*�ı��ڲ�����*/
			for (int row = 0; row < row_max; row++)//�հ׿������ұ�ð��
			{
				for (int col = (col_max - 1); col >= 0; col--)
				{
					if (inbase[row][col] == BLANK)
					{
						for (int tem_col = col; tem_col <(col_max-1); tem_col++)//��0����ð��
						{
							int mid;
							mid = inbase[row][tem_col];
							inbase[row][tem_col] = inbase[row][tem_col+1];
							inbase[row][tem_col + 1] = mid;

						}
					}
				}
			}//end of for col
			break;

		case L_R:
			/*����仯��ɫ����Ҫ�����ƶ��ľ���*/
			for (int row = 0; row < row_max; row++)
			{
				for (int col = 0; col < col_max; col++)
				{
					int num = 0;
					if (inbase[row][col] != BLANK)//�����ҷ����ж���BLANK �����ƶ�
					{
						for (int tem_col = (col + 1); tem_col <col_max; tem_col++)
							if (inbase[row][tem_col] == BLANK)
								num++;

					}
					move_blank_base_1[row][col] = num;
				}

			}//end of for col

			/*�����ƶ�*/
			for (int row = 0; row < row_max; row++)
				for (int col = (col_max-1); col >=0; col--)//����������!!!!!
					if (move_blank_base_1[row][col] != 0)
					gmw_move_block(M2048_CGI, row, col, inbase[row][col], BLANK, bdi, LEFT_TO_RIGHT, move_blank_base_1[row][col]);

			/*�ı��ڲ�����*/
			for (int row = 0; row < row_max; row++)//�հ׿��������ð��
			{
				for (int col = 0; col < col_max; col++)
				{
					if (inbase[row][col] == BLANK)
					{
						for (int tem_col = col; tem_col > 0; tem_col--)//��0����ð��
						{
							int mid;
							mid = inbase[row][tem_col];
							inbase[row][tem_col] = inbase[row][tem_col - 1];
							inbase[row][tem_col - 1] = mid;
						}
					}
				}
			}//end of for col
			break;

		case U_D:
			/*����仯��ɫ����Ҫ�����ƶ��ľ���*/
			for (int col = 0; col < col_max; col++)
			{
				for (int row = 0; row < row_max; row++)
				{
					int num = 0;
					if (inbase[row][col] != BLANK)//�����·��ж���BLANK �����ƶ�
					{
						for (int tem_row = (row + 1); tem_row < row_max; tem_row++)
							if (inbase[tem_row][col] == BLANK)
								num++;

					}
					move_blank_base_1[row][col] = num;
				}

			}//end of for col

			/*�����ƶ�*/
			for (int col = 0; col < col_max; col++)
				for (int row = row_max - 1; row >= 0; row--)//����������!!!!!
					if (move_blank_base_1[row][col] != 0)
					gmw_move_block(M2048_CGI, row, col, inbase[row][col], BLANK, bdi, UP_TO_DOWN, move_blank_base_1[row][col]);

			/*�ı��ڲ�����*/
			for (int col = 0; col < col_max; col++)//�հ׿�����ð��
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
			break;

		case D_U:
			/*����仯��ɫ����Ҫ�����ƶ��ľ���*/
			for (int col = 0; col < col_max; col++)
			{
				for (int row = (row_max - 1); row >= 0; row--)
				{
					int num = 0;
					if (inbase[row][col] != BLANK)//�����Ϸ��ж���BLANK �����ƶ�
					{
						for (int tem_row = (row - 1); tem_row >= 0; tem_row--)
							if (inbase[tem_row][col] == BLANK)
								num++;

					}
					move_blank_base_1[row][col] = num;
				}

			}//end of for col

			/*�����ƶ�*/
			for (int col = 0; col < col_max; col++)
				for (int row =0; row <row_max; row++)//����������!!!!!
					if (move_blank_base_1[row][col] != 0)
					gmw_move_block(M2048_CGI, row, col, inbase[row][col], BLANK, bdi, DOWN_TO_UP, move_blank_base_1[row][col]);

			/*�ı��ڲ�����*/
			for (int col = 0; col < col_max; col++)//�հ׿�����ð��
			{
				for (int row =(row_max-1); row >=0; row--)
				{
					if (inbase[row][col] == BLANK)
					{
						for (int tem_row = row; tem_row <(row_max-1); tem_row++)//��0����ð��
						{
							int mid;
							mid = inbase[tem_row][col];
							inbase[tem_row][col] = inbase[tem_row + 1][col];
							inbase[tem_row + 1][col] = mid;
						}
					}
				}
			}//end of for col
			break;

		default:
			break;
	}



    /*���и÷���ĵ���*/
	int move_base[ROW_MAX][COL_MAX] = {
		{0,0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,0,}
	};
	int change_base[ROW_MAX][COL_MAX] = {
		{0,0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,0,}
	};

	switch (type)
	{
		case R_L://���ҵ���
			/*�ڲ�����ı仯*/
			for (int row = 0; row < row_max; row++)
				for (int col = col_max - 1; col > 0; col--)
				{
					if ((inbase[row][col] == inbase[row][col - 1]) && (inbase[row][col] != BLANK)) {//������
						change_base[row][col] = inbase[row][col];
						inbase[row][col - 1] = inbase[row][col - 1] + inbase[row][col];
						inbase[row][col] = BLANK;
						move_base[row][col] = 1;
						change_base[row][col - 1] = inbase[row][col - 1];
						col--;//������һ��
					}
				}
			/*ͼ�ν���������ƶ�*/
			for (int row = 0; row < row_max; row++)//�������
				for (int col = 0; col < row_max; col++)
					if (move_base[row][col] != BLANK) {
						gmw_move_block(M2048_CGI, row, col, change_base[row][col], BLANK, bdi, RIGHT_TO_LEFT, move_base[row][col]);
						gmw_draw_block(M2048_CGI, row, (col - 1), change_base[row][col - 1], bdi);//��һ��ɫ��
					}
			break;

		case L_R://������
			/*�ڲ�����ı仯*/
			for (int row = 0; row < row_max; row++)
				for (int col = 0; col < (col_max - 1); col++)
				{
					if ((inbase[row][col] == inbase[row][col + 1]) && (inbase[row][col] != BLANK)) {//������ ���ƶ�һ���λ��
						change_base[row][col] = inbase[row][col];
						inbase[row][col + 1] = inbase[row][col + 1] + inbase[row][col];
						inbase[row][col] = BLANK;
						move_base[row][col] = 1;
						change_base[row][col + 1] = inbase[row][col + 1];
						col++;//������һ��
					}
				}
			/*ͼ�ν���������ƶ�*/
			for (int row = 0; row < row_max; row++)
				for (int col =(col_max-1); col >=0; col--)//�ұ�����
					if (move_base[row][col] != BLANK) {
						gmw_move_block(M2048_CGI, row, col, change_base[row][col], BLANK, bdi, LEFT_TO_RIGHT, move_base[row][col]);
						gmw_draw_block(M2048_CGI, row, (col + 1), change_base[row][col + 1], bdi);//��һ��ɫ��
					}
			break;

		case U_D://���ϵ���
			/*�ڲ�����ı仯*/
			for (int col = 0; col < col_max; col++)
				for (int row = 0; row < (row_max - 1); row++)
				{
					if ((inbase[row][col] == inbase[row + 1][col]) && (inbase[row][col] != BLANK)) {//������ ���ƶ�һ���λ��
						change_base[row][col] = inbase[row][col];
						inbase[row + 1][col] = inbase[row + 1][col] + inbase[row][col];
						inbase[row][col] = BLANK;
						move_base[row][col] = 1;
						change_base[row + 1][col] = inbase[row + 1][col];
						row++;//������һ��
					}
				}
			/*ͼ�ν���������ƶ�*/
			for (int col = 0; col < col_max; col++)
				for (int row = (row_max - 1); row >= 0; row--)//�·�����
					if (move_base[row][col] != BLANK) {
						gmw_move_block(M2048_CGI, row, col, change_base[row][col], BLANK, bdi, UP_TO_DOWN, move_base[row][col]);
						gmw_draw_block(M2048_CGI, (row + 1), col, change_base[row + 1][col], bdi);//��һ��ɫ��
					}
			break;

		case D_U://���µ���
			/*�ڲ�����ı仯*/
			for (int col = 0; col < col_max; col++)
				for (int row = (row_max - 1); row > 0; row--)
				{
					if ((inbase[row][col] == inbase[row- 1][col]) && (inbase[row][col] != BLANK)) {//������ ���ƶ�һ���λ��
						change_base[row][col] = inbase[row][col];
						inbase[row - 1][col] = inbase[row - 1][col] + inbase[row][col];
						inbase[row][col] = BLANK;
						move_base[row][col] = 1;
						change_base[row - 1][col] = inbase[row - 1][col];
						row--;//������һ��
					}
				}
			/*ͼ�ν���������ƶ�*/
			for (int col = 0; col < col_max; col++)
				for (int row = 0; row <row_max; row++)//�Ϸ�����
					if (move_base[row][col] != BLANK) {
						gmw_move_block(M2048_CGI, row, col, change_base[row][col], BLANK, bdi, DOWN_TO_UP, move_base[row][col]);
						gmw_draw_block(M2048_CGI, (row - 1), col, change_base[row - 1][col], bdi);//��һ��ɫ��
					}
			break;

		default:
			break;
	}


	/*����ð�ݴ���*/
	int move_blank_base_2[ROW_MAX][COL_MAX] = {
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
	};

	switch (type)
	{
		case R_L:
			/*����仯��ɫ����Ҫ�����ƶ��ľ���*/
			for (int row = 0; row < row_max; row++)
			{
				for (int col = (col_max - 1); col >= 0; col--)
				{
					int num = 0;
					if (inbase[row][col] != BLANK )//���������ж���BLANK �����ƶ�
					{
						for (int tem_col = (col - 1); tem_col >= 0; tem_col--)
							if (inbase[row][tem_col] == BLANK)
								num++;

					}
					move_blank_base_2[row][col] = num;
				}

			}//end of for col

			/*�����ƶ�*/
			for (int row = 0; row < row_max; row++)
				for (int col = 0; col < col_max; col++)//����������!!!!!
					if (move_blank_base_2[row][col] != 0)
						gmw_move_block(M2048_CGI, row, col, inbase[row][col], BLANK, bdi, RIGHT_TO_LEFT, move_blank_base_2[row][col]);

			/*�ı��ڲ�����*/
			for (int row = 0; row < row_max; row++)//�հ׿������ұ�ð��
			{
				for (int col = (col_max - 1); col >= 0; col--)
				{
					if (inbase[row][col] == BLANK)
					{
						for (int tem_col = col; tem_col < (col_max - 1); tem_col++)//��0����ð��
						{
							int mid;
							mid = inbase[row][tem_col];
							inbase[row][tem_col] = inbase[row][tem_col + 1];
							inbase[row][tem_col + 1] = mid;
						}
					}
				}
			}//end of for col
			break;

		case L_R:
			/*����仯��ɫ����Ҫ�����ƶ��ľ���*/
			for (int row = 0; row < row_max; row++)
			{
				for (int col = 0; col < col_max; col++)
				{
					int num = 0;
					if (inbase[row][col] != BLANK)//�����ҷ����ж���BLANK �����ƶ�
					{
						for (int tem_col = (col + 1); tem_col < col_max; tem_col++)
							if (inbase[row][tem_col] == BLANK)
								num++;

					}
					move_blank_base_2[row][col] = num;
				}

			}//end of for col

			/*�����ƶ�*/
			for (int row = 0; row < row_max; row++)
				for (int col = (col_max - 1); col >= 0; col--)//����������!!!!!
					if (move_blank_base_2[row][col] != 0)
						gmw_move_block(M2048_CGI, row, col, inbase[row][col], BLANK, bdi, LEFT_TO_RIGHT, move_blank_base_2[row][col]);

			/*�ı��ڲ�����*/
			for (int row = 0; row < row_max; row++)//�հ׿��������ð��
			{
				for (int col = 0; col < col_max; col++)
				{
					if (inbase[row][col] == BLANK)
					{
						for (int tem_col = col; tem_col > 0; tem_col--)//��0����ð��
						{
							int mid;
							mid = inbase[row][tem_col];
							inbase[row][tem_col] = inbase[row][tem_col - 1];
							inbase[row][tem_col - 1] = mid;
						}
					}
				}
			}//end of for col
			break;

		case U_D:
			/*����仯��ɫ����Ҫ�����ƶ��ľ���*/
			for (int col = 0; col < col_max; col++)
			{
				for (int row = 0; row < row_max; row++)
				{
					int num = 0;
					if (inbase[row][col] != BLANK)//�����·��ж���BLANK �����ƶ�
					{
						for (int tem_row = (row + 1); tem_row < row_max; tem_row++)
							if (inbase[tem_row][col] == BLANK)
								num++;

					}
					move_blank_base_2[row][col] = num;
				}

			}//end of for col

			/*�����ƶ�*/
			for (int col = 0; col < col_max; col++)
				for (int row = row_max - 1; row >= 0; row--)//����������!!!!!
					if (move_blank_base_2[row][col] != 0)
						gmw_move_block(M2048_CGI, row, col, inbase[row][col], BLANK, bdi, UP_TO_DOWN, move_blank_base_2[row][col]);

			/*�ı��ڲ�����*/
			for (int col = 0; col < col_max; col++)//�հ׿�����ð��
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
			break;

		case D_U:
			/*����仯��ɫ����Ҫ�����ƶ��ľ���*/
			for (int col = 0; col < col_max; col++)
			{
				for (int row = (row_max - 1); row >= 0; row--)
				{
					int num = 0;
					if (inbase[row][col] != BLANK)//�����Ϸ��ж���BLANK �����ƶ�
					{
						for (int tem_row = (row - 1); tem_row >= 0; tem_row--)
							if (inbase[tem_row][col] == BLANK)
								num++;

					}
					move_blank_base_2[row][col] = num;
				}

			}//end of for col

			/*�����ƶ�*/
			for (int col = 0; col < col_max; col++)
				for (int row = 0; row < row_max; row++)//����������!!!!!
					if (move_blank_base_2[row][col] != 0)
						gmw_move_block(M2048_CGI, row, col, inbase[row][col], BLANK, bdi, DOWN_TO_UP, move_blank_base_2[row][col]);

			/*�ı��ڲ�����*/
			for (int col = 0; col < col_max; col++)//�հ׿�����ð��
			{
				for (int row = (row_max - 1); row >= 0; row--)
				{
					if (inbase[row][col] == BLANK)
					{
						for (int tem_row = row; tem_row < (row_max - 1); tem_row++)//��0����ð��
						{
							int mid;
							mid = inbase[tem_row][col];
							inbase[tem_row][col] = inbase[tem_row + 1][col];
							inbase[tem_row + 1][col] = mid;
						}
					}
				}
			}//end of for col
			break;

		default:
			break;
	}



	int score = 0;
	for (int row = 0; row < row_max; row++)
		for (int col = 0; col < col_max; col++)
			score += change_base[row][col];

	return score;//���ش˴εĵ÷�


}




