/* 2052532 ��04 ���ܷ� */
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <cmath>
#include <windows.h>
#include "../include/cmd_console_tools.h"//�������ߺ���
#include "90-01-b1-hanoi.h"
#include "..\include\common_output.h"//�����Լ��Ĺ��ú���

using namespace std;
static int ji_shu = 0;//���ں�ŵ�����ƶ��Լ�����
static int A_[10] = { 0,0,0,0,0,0,0,0,0,0 };
static int B_[10] = { 0,0,0,0,0,0,0,0,0,0 };
static int C_[10] = { 0,0,0,0,0,0,0,0,0,0 };
static int top_A = 0, top_B = 0, top_C = 0;
static int yan_shi = 0;

/* -----------------------------------------

     ���ļ����ܣ�
	1���ű� hanoi_main.cpp/hanoi_menu.cpp �еĸ��������õĲ˵������Ӧ��ִ�к���

     ���ļ�Ҫ��
	1�����������ⲿȫ�ֱ�����const��#define�������Ʒ�Χ�ڣ�
	2�������徲̬ȫ�ֱ�����������Ҫ��������Ҫ�����ĵ���ʾ��ȫ�ֱ�����ʹ��׼���ǣ����á����á��ܲ��þ������ã�
	3����̬�ֲ����������������ƣ���ʹ��׼��Ҳ�ǣ����á����á��ܲ��þ�������
	4���������ϵͳͷ�ļ����Զ���ͷ�ļ��������ռ��

   ----------------------------------------- */

   /***************************************************************************
	 �������ƣ�menu_out_hnoi
	 ��    �ܣ������Ӧ�Ĳ˵����漰���뺯��
	 ���������void
	 �� �� ֵ��int
	 ˵    ������
   ***************************************************************************/
int menu_out_hanoi(void)
{

	struct menu eli_start[10] = { {'1',"������"},
								  {'2',"������(������¼)"},
								  {'3',"�ڲ�������ʾ(����)"},
								  {'4',"�ڲ�������ʾ(����+����)"},
								  {'5',"ͼ�ν�-Ԥ��-������Բ��"},
								  {'6',"ͼ�ν�-Ԥ��-����ʼ���ϻ�n������"},
								  {'7',"ͼ�ν�-Ԥ��-��һ���ƶ�"},
								  {'8',"ͼ�ν� - �Զ��ƶ��汾"},
								  {'9',"ͼ�ν�-��Ϸ��"},
								  {'0',"�˳�"} };
	char back = menu_out(eli_start, 10);
	switch (back)
	{
		case 'A':
			item_1();
			break;
		case 'B':
			item_2();
			break;
		case 'C':
			item_3();
			break;
		case 'D':
			item_4();
			break;
		case 'E':
			item_5();
			break;
		case 'F':
			item_6();
			break;
		case 'G':
			item_7();
			break;
		case 'H':
			item_8();
			break;
		case 'I':
			item_9();
			break;
		case 'J':
			return 0;
		default:
			cout << "error!" << endl;
			return -1;//��ʱΪ�����������
	}
	switch (back)
	{
		case 'A':
		case 'B':
		case 'C':
			cout << endl;
			cout << "���س�������";
			while (1)
			{
				if (_getch() == '\r')
					break;
			}
			break;
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
		case 'I':
			cct_gotoxy(0, 38);
			cout << "���س�������";
			while (1)
			{
				if (_getch() == '\r')
					break;
			}
			break;
		default:
			cout << "error!" << endl;
			break;
	}
	data_renew();
	return 1;
}

/***************************************************************************
  �������ƣ�hanoi
  ��    �ܣ��ݹ麯����ӡ��ص�����
  ���������int n(���˵Ĳ���), char src(��ʼ��), char tmp(�м���), char dst(�յ���),int mode(�����ģʽ)
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void hanoi(int n, char src, char tmp, char dst, int mode)
{
	if (n == 1) {
		ji_shu = ji_shu + 1;
		hanoi_out(n, src, tmp, dst, mode);
		return;
	}
	else {
		hanoi((n - 1), src, dst, tmp, mode);
		ji_shu = ji_shu + 1;
		hanoi_out(n, src, tmp, dst, mode);
		hanoi((n - 1), tmp, src, dst, mode);
	}
}

/***************************************************************************
  �������ƣ�hanoi_out
  ��    �ܣ���ӡ��ŵ�����������
  ���������int n, char src, char tmp, char dst, int mode
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void hanoi_out(int n, char src, char tmp, char dst, int mode)
{
	switch (mode)
	{
		case 1:
			row_out_1(n, src, dst);
			break;
		case 2:
			row_out_2(n, src, dst);
			break;
		case 3:
			change(src, dst);
			row_out_348(n, src, dst, mode, 1);
			break;
		case 4:
			if (ji_shu == 1)
			{
				row_out_348(n, src, dst, mode, 0);
				column_out_48(n, src, dst, mode, 0);
				stop();
			}
			change(src, dst);
			row_out_348(n, src, dst, mode, 1);
			column_out_48(n, src, dst, mode, 1);
			stop();
			break;
		case 8:
			if (ji_shu == 1)
			{
				row_out_348(n, src, dst, mode, 0);
				column_out_48(n, src, dst, mode, 0);
				tower();//��ӡ��
				start_plate();//��ӡ��ʼ��
				cct_setcolor(0, 7);//�ָ�ԭ������ɫ
				stop();
			}
			change(src, dst);
			row_out_348(n, src, dst, mode, 1);
			column_out_48(n, src, dst, mode, 1);
			plate_move(src, dst);
			stop();
			break;
		default:
			break;

	}
	return;

}

/***************************************************************************
  �������ƣ�column_out_48
  ��    �ܣ����4\8����
  ���������int n, char src, char dst, int mode, int start(0����ʼ��:1������ʼ��)
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void column_out_48(int n, char src, char dst, int mode, int start)
{
	if (start == 0)
	{
		switch (mode)
		{
			case 4:
				cct_gotoxy(9, 12);
				cout << setw(25) << setfill('=') << '=';
				cct_gotoxy(11, 13);
				cout << setfill(' ') << "A         B         C";
				column_inner(mode);
				break;
			case 8:
				cct_gotoxy(9, 27);
				cout << setw(25) << setfill('=') << '=';
				cct_gotoxy(11, 28);
				cout << setfill(' ') << "A         B         C";
				column_inner(mode);
				break;
			default:
				cout << "error" << endl;
				break;
		}

	}
	else
	{
		column_inner(mode);
	}
	return;
}

/***************************************************************************
  �������ƣ�row_out_348
  ��    �ܣ����3\4\8����
  ���������int n, char src, char dst,int mode, int start(4\8�е���ʼ��:0����ʼ�У�1������ʼ��)
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void row_out_348(int n, char src, char dst, int mode, int start)
{
	if (start == 0)
	{
		switch (mode)
		{
			case 4:
				cct_gotoxy(0, 17);
				break;
			case 8:
				cct_gotoxy(0, 32);
				break;
			default:
				cout << "error" << endl;
				break;
		}
		start_row_48();
	}
	else
	{
		switch (mode)
		{
			case 3:
				break;
			case 4:
				cct_gotoxy(0, 17);
				break;
			case 8:
				cct_gotoxy(0, 32);
				break;
			default:
				cout << "error" << endl;
				break;
		}
		cout << "��" << setw(4) << setiosflags(ios::right) << ji_shu << " ��" << '(' << setw(2) << setiosflags(ios::right)
			<< n << "#:" << setw(2) << setiosflags(ios::right) << src << "-->" << dst << ") ";
		row_inner();
	}
	return;
}

/***************************************************************************
  �������ƣ�row_out_1
  ��    �ܣ����1����
  ���������int n, char src, char dst
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void row_out_1(int n, char src, char dst)
{
	cout << setw(2) << setiosflags(ios::right) << n << '#' << setw(2)
		<< setiosflags(ios::right) << src << "---->" << dst << endl;
	return;
}

/***************************************************************************
  �������ƣ�row_out_2
  ��    �ܣ����2����
  ���������int n, char src, char dst
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void row_out_2(int n, char src, char dst)
{
	cout << "��" << setw(4) << setiosflags(ios::right) << ji_shu << " ��" << '(' << setw(2) << setiosflags(ios::right)
		<< n << "#:" << setw(2) << setiosflags(ios::right) << src << "-->" << dst << ")" << endl;
	return;
}

/***************************************************************************
  �������ƣ�start_row_48
  ��    �ܣ����4\8���еĳ�ʼ��
  �����������
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void start_row_48(void)
{
	int i;
	cout << "��ʼ:  A:";
	for (i = 0; i <= 9; i++)
	{
		if (A_[i] == 0)
			cout << setw(2) << setiosflags(ios::right) << ' ';
		else
			cout << setw(2) << setiosflags(ios::right) << A_[i];
	}
	cout << " B:";
	for (i = 0; i <= 9; i++)
	{
		if (B_[i] == 0)
			cout << setw(2) << setiosflags(ios::right) << ' ';
		else
			cout << setw(2) << setiosflags(ios::right) << B_[i];
	}
	cout << " C:";
	for (i = 0; i <= 9; i++)
	{
		if (C_[i] == 0)
			cout << setw(2) << setiosflags(ios::right) << ' ';
		else
			cout << setw(2) << setiosflags(ios::right) << C_[i];
	}
	cout << endl;
	return;
}

/***************************************************************************
  �������ƣ�row_inner
  ��    �ܣ���ӡ��ŵ������ϸ�ƶ�����(����)
  ���������void
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void row_inner(void)
{
	int i;
	cout << " A:";
	for (i = 0; i <= 9; i++)
	{
		if (A_[i] == 0)
			cout << setw(2) << setiosflags(ios::right) << ' ';
		else
			cout << setw(2) << setiosflags(ios::right) << A_[i];
	}
	cout << " B:";
	for (i = 0; i <= 9; i++)
	{
		if (B_[i] == 0)
			cout << setw(2) << setiosflags(ios::right) << ' ';
		else
			cout << setw(2) << setiosflags(ios::right) << B_[i];
	}
	cout << " C:";
	for (i = 0; i <= 9; i++)
	{
		if (C_[i] == 0)
			cout << setw(2) << setiosflags(ios::right) << ' ';
		else
			cout << setw(2) << setiosflags(ios::right) << C_[i];
	}
	cout << endl;
	return;
}

/***************************************************************************
  �������ƣ�stop
  ��    �ܣ�����֮�����ʱ
  ���������void
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void stop(void)
{
	switch (yan_shi)//Ϊ����ͣ�ٵ�ѡ��ṹ
	{
		case 0:
			while (1)
			{
				if (_getch() == '\r')
					break;
			}
			break;
		case 1:
			Sleep(1000);
			break;
		case 2:
			Sleep(250);
			break;
		case 3:
			Sleep(100);
			break;
		case 4:
			Sleep(25);
			break;
		case 5:
			break;
	}
	return;
}

/***************************************************************************
  �������ƣ�change
  ��    �ܣ���������Ķ���
  ���������char start, char goal
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void change(char start, char goal)//��������Ķ���
{
	if (start == 'A' && goal == 'B')//A-B
	{
		B_[top_B++] = A_[top_A - 1];
		A_[--top_A] = 0;
	}
	if (start == 'B' && goal == 'A')//B-A
	{
		A_[top_A++] = B_[top_B - 1];
		B_[--top_B] = 0;
	}
	if (start == 'A' && goal == 'C')//A-C
	{
		C_[top_C++] = A_[top_A - 1];
		A_[--top_A] = 0;
	}
	if (start == 'C' && goal == 'A')//C-A
	{
		A_[top_A++] = C_[top_C - 1];
		C_[--top_C] = 0;
	}
	if (start == 'B' && goal == 'C')//B-C
	{
		C_[top_C++] = B_[top_B - 1];
		B_[--top_B] = 0;
	}
	if (start == 'C' && goal == 'B')//C-B
	{
		B_[top_B++] = C_[top_C - 1];
		C_[--top_C] = 0;
	}
	return;
}

/***************************************************************************
  �������ƣ�column_inner
  ��    �ܣ���ӡ4\8����������
  ���������int mode
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void column_inner(int mode)//��ӡ��������
{
	int i;
	for (i = 0; i <= 9; i++)
	{
		switch (mode)
		{
			case 4:
				cct_gotoxy(10, 11 - i);
				break;
			case 8:
				cct_gotoxy(10, 26 - i);
				break;
			default:
				cout << "error" << endl;
				break;
		}
		if (A_[i] == 0)
			cout << "  ";
		else
			cout << setw(2) << setiosflags(ios::right) << A_[i];
	}
	for (i = 0; i <= 9; i++)
	{
		switch (mode)
		{
			case 4:
				cct_gotoxy(20, 11 - i);
				break;
			case 8:
				cct_gotoxy(20, 26 - i);
				break;
			default:
				cout << "error" << endl;
				break;
		}
		if (B_[i] == 0)
			cout << "  ";
		else
			cout << setw(2) << setiosflags(ios::right) << B_[i];
	}
	for (i = 0; i <= 9; i++)
	{
		switch (mode)
		{
			case 4:
				cct_gotoxy(30, 11 - i);
				break;
			case 8:
				cct_gotoxy(30, 26 - i);
				break;
			default:
				cout << "error" << endl;
				break;
		}
		if (C_[i] == 0)
			cout << "  ";
		else
			cout << setw(2) << setiosflags(ios::right) << C_[i];
	}
	return;
}

/***************************************************************************
  �������ƣ�input
  ��    �ܣ�123456789�Ĳ��������뺯��
  ���������int* floor, char* start, char* mid,char* goal, int* yan_shi,int yby(0����Ҫ��ʱ:1��Ҫ��ʱ)
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void input(int* floor, char* start, char* mid,char* goal, int* yan_shi,int yby)
{
	int mubiao = 0;
	char chongfu;
	while (1)//�жϲ����Ƿ�������ȷ
	{
		cout << "�����뺺ŵ���Ĳ���(1-10):" << endl;
		cin >> *floor;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1024, '\n');
		}
		else
		{
			if (*floor >= 1 && *floor <= 10)
			{
				cin.ignore(1024, '\n');
				break;
			}

			cin.ignore(1024, '\n');
		}
	}
	while (1)//�ж���ʼ���Ƿ�������ȷ
	{
		cout << "��������ʼ��(A-C):" << endl;
		cin >> *start;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1024, '\n');
		}
		else
		{
			if (*start == 'A' || *start == 'a')
			{
				*start = 'A';
				cin.ignore(1024, '\n');
				break;
			}
			else if (*start == 'B' || *start == 'b')
			{
				*start = 'B';
				cin.ignore(1024, '\n');
				break;
			}
			else if (*start == 'C' || *start == 'c')
			{
				*start = 'C';
				cin.ignore(1024, '\n');
				break;
			}
			else
				cin.ignore(1024, '\n');
		}
	}
	chongfu = *start;
	while (1)//�ж�Ŀ�����Ƿ�������ȷ
	{
		cout << "������Ŀ����(A-C):" << endl;;
		cin >> *goal;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1024, '\n');
		}
		else
		{
			switch (*goal)
			{
				case 'A':
				case 'a':
					if (*goal == chongfu || *goal == chongfu + 32)
					{
						cout << "Ŀ����(A)��������ʼ��(A)��ͬ" << endl;
						break;
					}
					else
					{
						*goal = 'A';
						mubiao = 1;
						cin.ignore(1024, '\n');
						break;
					}
				case 'B':
				case 'b':
					if (*goal == chongfu || *goal == chongfu + 32)
					{
						cout << "Ŀ����(B)��������ʼ��(B)��ͬ" << endl;
						break;
					}
					else
					{
						*goal = 'B';
						mubiao = 1;
						cin.ignore(1024, '\n');
						break;
					}
				case 'C':
				case 'c':
					if (*goal == chongfu || *goal == chongfu + 32)
					{
						cout << "Ŀ����(C)��������ʼ��(C)��ͬ" << endl;
						break;
					}
					else
					{
						*goal = 'C';
						mubiao = 1;
						cin.ignore(1024, '\n');
						break;
					}
				default:
					break;
			}
			if (mubiao != 0)
				break;
			cin.ignore(1024, '\n');
		}
	}
	*mid = 198 - *start - *goal;//ȷ���м�����ֵ
	if (yby)
	{
		while (1)//�����ж��ƶ��ٶȵ�ֵ
		{
			cout << "�������ƶ��ٶ�(0-5: 0-���س�������ʾ 1-��ʱ� 5-��ʱ���)" << endl;
			cin >> *yan_shi;
			if (!cin)
			{
				cin.clear();
				cin.ignore(1024, '\n');
			}
			else if (*yan_shi >= 0 && *yan_shi <= 5)
			{
				cin.ignore(1024, '\n');
				break;
			}
			else
				cin.ignore(1024, '\n');
		}
	}
	return;

}

/***************************************************************************
  �������ƣ�item_1
  ��    �ܣ�ִ����Ŀ1�ĺ���
  ���������void
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void item_1(void)
{
	cct_cls();
	char src, tmp, dst;
	int floor;
	input(&floor, &src, &tmp, &dst, &yan_shi, 0);
	hanoi(floor, src, tmp, dst, 1);
	return;
}

/***************************************************************************
  �������ƣ�item_2
  ��    �ܣ�ִ����Ŀ2�ĺ���
  ���������void
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void item_2(void)
{
	cct_cls();
	char src, tmp, dst;
	int floor;
	input(&floor, &src, &tmp, &dst, &yan_shi, 0);
	hanoi(floor, src, tmp, dst, 2);
	return;

}

/***************************************************************************
  �������ƣ�item_3
  ��    �ܣ�ִ����Ŀ3�ĺ���
  ���������void
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void item_3(void)
{
	cct_cls();
	char src, tmp, dst;
	int floor;
	input(&floor, &src, &tmp, &dst, &yan_shi, 0);
	initial_zhu(floor, src);
	hanoi(floor, src, tmp, dst, 3);
	return;

}

/***************************************************************************
  �������ƣ�item_4
  ��    �ܣ�ִ����Ŀ4�ĺ���
  ���������void
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void item_4(void)
{
	cct_cls();
	char src, tmp, dst;
	int floor;
	input(&floor, &src, &tmp, &dst, &yan_shi, 1);
	cct_cls();//������е�����
	cout << "�� " << char(src) << " �ƶ��� " << char(dst) << "���� " << floor << " �㣬��ʱ����Ϊ " << yan_shi <<endl;
	initial_zhu(floor, src);//������������ݸ��������Ӹ�ֵ
	cct_setcursor(3);//ʹ�ù�겻��ʾ
	hanoi(floor, src, tmp, dst, 4);
	cct_setcursor(2);//ʹ�ù����ʾ
	return;

}

/***************************************************************************
  �������ƣ�initial_zhu
  ��    �ܣ���ABC�������ӳ�ʼ��ֵ
  ���������int floor, char start
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void initial_zhu(int floor, char start)
{
	int i;
	if (start == 'A')//�������Ӹ�ֵ
	{
		for (i = 0; i <= floor - 1; i++)
		{
			A_[i] = floor - i;
			top_A++;
		}
	}
	else if (start == 'B')
	{
		for (i = 0; i <= floor - 1; i++)
		{
			B_[i] = floor - i;
			top_B++;
		}
	}
	else
	{
		for (i = 0; i <= floor - 1; i++)
		{
			C_[i] = floor - i;
			top_C++;
		}
	}
	return;
}

/***************************************************************************
  �������ƣ�data_renew
  ��    �ܣ�ÿ����Ŀִ����ɺ󣬽���̬ȫ�ֱ�������
  ���������void
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void data_renew(void)
{
	int i;
	for (i = 0; i <= 9; i++)
	{
		A_[i] = 0;
		B_[i] = 0;
		C_[i] = 0;
	}
	top_A = 0;
	top_B = 0;
	top_C = 0;
	yan_shi = 0;
    ji_shu = 0;//���ں�ŵ�����ƶ��Լ�����
	return;
}

/***************************************************************************
  �������ƣ�item_8
  ��    �ܣ�ִ����Ŀ8�ĺ���
  ���������void
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void item_8(void)
{
	cct_cls();
	char src, tmp, dst;
	int floor;
	input(&floor, &src, &tmp, &dst, &yan_shi, 1);
	cct_cls();//������е�����
	cout << "�� " << char(src) << " �ƶ��� " << char(dst) << "���� " << floor << " �㣬��ʱ����Ϊ " << yan_shi << endl;
	initial_zhu(floor, src);
	cct_setcursor(3);//ʹ�ù�겻��ʾ
	hanoi(floor, src, tmp, dst, 8);
	cct_setcursor(2);//ʹ�ù����ʾ
	return;

}

/***************************************************************************
  �������ƣ�tower
  ��    �ܣ���ӡ56789��αͼ�ν������
  ���������void
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void tower(void)
{
	int i;
	cct_setcolor(14, 14);
	cct_gotoxy(1, 15);
	cout << "                       " << endl;
	cct_gotoxy(33, 15);
	cout << "                       " << endl;
	cct_gotoxy(65, 15);
	cout << "                       " << endl;
	for (i = 1; i <= 12; i++)
	{
		cct_gotoxy(12, (15-i));
		cout << ' ';
		cct_gotoxy(44, (15 - i));
		cout << ' ';
		cct_gotoxy(76, (15 - i));
		cout << ' ';
		Sleep(125);
	}
	return;
}

/***************************************************************************
  �������ƣ�item_5
  ��    �ܣ�ִ����Ŀ5�ĺ���
  ���������void
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void item_5(void)
{
	cct_cls();//������е�����
	cct_setcursor(3);//ʹ�ù�겻��ʾ
	tower();
	cct_setcursor(2);//ʹ�ù����ʾ
	cct_setcolor(0, 7);//ɫ�ʻָ�ԭ״
	return;
}

/***************************************************************************
  �������ƣ�item_6
  ��    �ܣ�ִ����Ŀ6�ĺ���
  ���������void
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void item_6(void)
{
	cct_cls();
	char src, tmp, dst;
	int floor;
	input(&floor, &src, &tmp, &dst, &yan_shi, 0);
	cct_cls();//������е�����
	cout << "�� " << char(src) << " �ƶ��� " << char(dst) << "���� " << floor << " ��"<< endl;
	initial_zhu(floor, src);//��������������������Ӹ�ֵ
	cct_setcursor(3);//ʹ�ù�겻��ʾ
	tower();
	start_plate();//��ӡ��ʼ��
	cct_setcursor(2);//ʹ�ù����ʾ
	cct_setcolor(0, 7);//�ָ�ԭ������ɫ
	return;

}

/***************************************************************************
  �������ƣ�start_plate
  ��    �ܣ���ӡ��ʼ��Բ��
  ���������void
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void start_plate(void)
{
	int i,j;
	for (i = 0; i <= 9; i++)
	{
		cct_gotoxy(12, 14 - i);
		if (A_[i] != 0)
		{
			cct_gotoxy(12-A_[i], 14 - i);
			cct_setcolor(A_[i], A_[i]);
			Sleep(50);
			for (j = 0; j <= (2 * A_[i]); j++)
			{
				cout << ' ';
			}
		}
	}
	for (i = 0; i <= 9; i++)
	{
		cct_gotoxy(44, 14 - i);
		if (B_[i] != 0)
		{
			cct_gotoxy(44 - B_[i], 14 - i);
			cct_setcolor(B_[i], B_[i]);
			Sleep(50);
			for (j = 0; j <= (2 * B_[i]); j++)
			{
				cout << ' ';
			}
		}
	}
	for (i = 0; i <= 9; i++)
	{
		cct_gotoxy(76, 14 - i);
		if (C_[i] != 0)
		{
			cct_gotoxy(76 - C_[i], 14 - i);
			cct_setcolor(C_[i], C_[i]);
			Sleep(50);
			for (j = 0; j <= (2 * C_[i]); j++)
			{

				cout << ' ';
			}
		}
	}
	return;

}

/***************************************************************************
  �������ƣ�plate_move_data
  ��    �ܣ�ȷ���ƶ����ƶ�ɫ����������
  ���������int*start_floor,int* end_floor,int* lump(ȷ�����Ӷ��ɶɫ),char start,char goal
  �� �� ֵ��void
  ˵    ���������������ݴ����ǻ��������Ѿ��ƶ���ɵ�����
***************************************************************************/
void plate_move_data(int*start_floor,int* start_zhu,int* end_floor, int* end_zhu,int* lump,char start,char goal)
{
	int i;
	switch (int(goal))//ȷ���ƶ�������,���յ���ȥ��
	{
		case 'A':
			for (i = 9; i >= 0; i--)
			{
				if (A_[i] != 0)
				{
					*end_zhu = 12;//���ӵ�λ��
					*lump = A_[i];
					*end_floor = i;
					break;
				}	
			}
			break;
		case 'B':
			for (i = 9; i >= 0; i--)
			{
				if (B_[i] != 0)
				{
					*end_zhu = 44;//���ӵ�λ��
					*lump = B_[i];
					*end_floor = i;
					break;
				}	
			}
			break;
		case 'C':
			for (i = 9; i >= 0; i--)
			{
				if (C_[i] != 0)
				{
					*end_zhu = 76;///���ӵ�λ��
					*lump = C_[i];
					*end_floor = i;
					break;
				}
			}
			break;
		default:
			cout << "error" << endl;
			break;
	}
	switch (int(start))
	{
		case 'A':
			for (i = 9; i>=0; i--)
			{
				*start_zhu = 12;//���ӵ�λ��
				if (A_[i] != 0)//ȷ����ʼ���Ĳ���
				{
					*start_floor = i+1;
					break;
				}
				if(i==0)
					*start_floor = 0;
			}
			break;
		case 'B':
			for (i = 9; i >= 0; i--)
			{
				*start_zhu = 44;//���ӵ�λ��
				if (B_[i] != 0)//ȷ����ʼ���Ĳ���
				{
					*start_floor = i + 1;
					break;
				}
				if (i == 0)
					*start_floor = 0;
			}
			break;
		case 'C':
			for (i = 9; i >= 0; i--)
			{
				*start_zhu = 76;//���ӵ�λ��
				if (C_[i] != 0)//ȷ����ʼ���Ĳ���
				{
					*start_floor = i + 1;
					break;
				}
				if (i == 0)
					*start_floor = 0;
			}
			break;
		default:
			cout << "error";
			break;
	}
	return;
}

/***************************************************************************
  �������ƣ�plate_move
  ��    �ܣ�ִ��789�������ƶ����ܺ���
  ���������char src,char dst
  �� �� ֵ��void
  ˵    ���������ڲ��Ѱ����ָ�ԭ����ɫ������
***************************************************************************/
void plate_move(char src,char dst)
{
	int start_floor, start_zhu, end_floor, end_zhu, lump;
	plate_move_data(&start_floor, &start_zhu, &end_floor, &end_zhu, &lump, src, dst);
	plate_move_vertical_up(start_zhu, start_floor, lump);
	plate_move_level(start_zhu, end_zhu, lump);
	plate_move_vertical_down(end_zhu, end_floor, lump);
	cct_setcolor(0, 7);//�ָ�ԭ������ɫ
	return;
}

/***************************************************************************
  �������ƣ�plate_move_level
  ��    �ܣ���������ˮƽ�ƶ�
  ���������int start, int goal, int lump
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void plate_move_level(int start, int goal, int lump)
{
	int i, j, delt;//i�����ڴ�ӡĳ��ɫ��,������j��������
	delt = goal - start;
	if (delt >= 0)
	{
		for (j =0;j <= delt; j++)
		{
			cct_gotoxy((start - lump + j), 1);
			cct_setcolor(lump, lump);
			for (i = 0; i <= (2 * lump); i++)
				cout << ' ';
			sleep();
			cct_gotoxy((start - lump + j), 1);
			cct_setcolor(0, 7);//�ָ�ԭ����ɫ��
			for (i = 0; i <= (2 * lump); i++)
				cout << ' ';
		}
	}
	else
	{
		for (j = 0; j >= delt; j--)
		{
			cct_gotoxy((start - lump + j), 1);
			cct_setcolor(lump, lump);
			for (i = 0; i <= (2 * lump); i++)
				cout << ' ';
			sleep();
			cct_gotoxy((start - lump + j), 1);
			cct_setcolor(0, 7);//�ָ�ԭ����ɫ��
			for (i = 0; i <= (2 * lump); i++)
				cout << ' ';
		}
	}
	return;
}

/***************************************************************************
  �������ƣ�plate_move_vertical_up
  ��    �ܣ���������ֱ�����ƶ�
  ���������int zhu, int floor, int lump
  �� �� ֵ��void
  ˵    ����ʹ�����������ƶ�
***************************************************************************/
void plate_move_vertical_up(int zhu, int floor, int lump)
{
	int i,j;//j�����ڴ�ӡĳ��ɫ��,������i��������
	cct_gotoxy((zhu - lump), (5 + (9 - floor)));//����ʼ���Ķ��˵�ɫ�����
	cct_setcolor(0, 7);
	for (j = 0; j <= (2 * lump); j++)
		cout << ' ';
	cct_setcolor(14, 14);
	cct_gotoxy(zhu, (5 + (9 - floor)));
	cout << ' ';
	for (i = 1; i <= (2 + (9 - floor)); i++)
	{
		cct_gotoxy((zhu - lump), (5 + (9 - floor) - i));
		cct_setcolor(lump, lump);
		for (j = 0; j <= (2 * lump); j++)//����Ĵ�ӡ
			cout << ' ';
		sleep();
		cct_gotoxy((zhu - lump), (5 + (9 - floor) - i));//���ɫ��
		cct_setcolor(0, 7);
		for (j = 0; j <= (2 * lump); j++)
			cout << ' ';
		cct_setcolor(14, 14);//�ָ�����
		cct_gotoxy(zhu, (5 + (9 - floor) - i));
		cout << ' ';
	}
	cct_gotoxy((zhu - lump), 2);
	cct_setcolor(lump, lump);
	for (j = 0; j <= (2 * lump); j++)//����Ĵ�ӡ
		cout << ' ';
	sleep();
	cct_gotoxy((zhu - lump), 2);//���ɫ��
	cct_setcolor(0, 7);
	for (j = 0; j <= (2 * lump); j++)//���ɫ��
		cout << ' ';
	return;
}

/***************************************************************************
  �������ƣ�plate_move_vertical_down
  ��    �ܣ���������ֱ�����ƶ�
  ���������int zhu, int floor, int lump
  �� �� ֵ��void
  ˵    ����ʹ�����������ƶ�
***************************************************************************/
void plate_move_vertical_down(int zhu, int floor, int lump)
{
	int i,j;//j�����ڴ�ӡĳ��ɫ��,������i��������
	cct_gotoxy((zhu - lump), 2);
	cct_setcolor(lump, lump);
	for (j = 0; j <= (2 * lump); j++)//����Ĵ�ӡ
		cout << ' ';
	sleep();
	cct_gotoxy((zhu - lump), 2);//���ɫ��
	cct_setcolor(0, 7);
	for (j = 0; j <= (2 * lump); j++)//���ɫ��
		cout << ' ';
	for (i = 1; i <= (3 + (9 - floor)); i++)
	{
		cct_gotoxy((zhu - lump), (2 + i));
		cct_setcolor(lump, lump);
		for (j = 0; j <= (2 * lump); j++)//����Ĵ�ӡ
			cout << ' ';
		if (i != (3 + (9 - floor)))
		{
			sleep();
			cct_gotoxy((zhu - lump), (2 + i));//���ɫ��
			cct_setcolor(0, 7);
			for (j = 0; j <= (2 * lump); j++)
				cout << ' ';
			cct_setcolor(14, 14);//�ָ�����
			cct_gotoxy(zhu, (2 + i));
			cout << ' ';
		}
	}
	return;
}

/***************************************************************************
  �������ƣ�item_7
  ��    �ܣ�ִ����Ŀ7�ĺ���
  ���������void
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void item_7(void)
{
	cct_cls();
	char src, tmp, dst;
	int floor;
	input(&floor, &src, &tmp, &dst, &yan_shi, 0);
	cct_cls();//������е�����
	cout << "�� " << char(src) << " �ƶ��� " << char(dst) << "���� " << floor << " ��" << endl;
	initial_zhu(floor, src);//��������������������Ӹ�ֵ
	cct_setcursor(3);//ʹ�ù�겻��ʾ
	tower();
	start_plate();//��ӡ��ʼ��
	change(src, dst);
	cct_setcolor(0, 7);
	yan_shi = 0;
	plate_move(src, dst);
	cct_setcursor(2);//ʹ�ù����ʾ
	return;

}

/***************************************************************************
  �������ƣ�sleep
  ��    �ܣ����������ƶ�ʱ���ֹͣʱ��
  ���������void
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void sleep(void)
{
	switch (yan_shi)
	{
		case 0:
			Sleep(50);
			break;
		case 1:
			Sleep(150);
			break;
		case 2:
			Sleep(100);
			break;
		case 3:
			Sleep(50);
			break;
		case 4:
			Sleep(25);
			break;
		case 5:
			break;
	}
	return;
}

/***************************************************************************
  �������ƣ�item_9
  ��    �ܣ�ִ����Ŀ9�ĺ���(��ϷӴ������)
  ���������void
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void item_9(void)
{
	cct_cls();
	char src, tmp, dst;
	int floor;
	input(&floor, &src, &tmp, &dst, &yan_shi, 0);
	yan_shi = 4;//���Ʒ�����ƶ��ٶ�
	cct_cls();//������е�����
	cout << "�� " << char(src) << " �ƶ��� " << char(dst) << "���� " << floor << " ��" << endl;
	initial_zhu(floor, src);//��������������������Ӹ�ֵ
	cct_setcursor(3);//ʹ�ù�겻��ʾ
	column_out_48(floor, src, dst, 8, 0);//��ӡ��ʼ����
	row_out_348(floor, src, dst, 8, 0);//��ӡ��ʼ��
	tower();
	start_plate();//��ӡ��ʼ��
	cct_setcursor(2);//ʹ�ù����ʾ
	cct_setcolor(0, 7);//�ָ�ԭ������ɫ
	cct_gotoxy(0, 34);
	cout << "�������ƶ�������(������ʽ��AC��ʾ��A���˵������ƶ���C��Q��ʾ�˳�) ��" << endl;
	while (1)
	{
		char start, goal;
		int cengshu, shuru, zhixing;
		cct_gotoxy(68, 34);
		shuru = item_9_in(&start,&goal);
		if (shuru == 0)
		{
			cct_gotoxy(0, 35);
			cout << "��Ϸ��ֹ!!!!!" << endl;
			break;
		}
		item_9_pdcs(start, &cengshu);
		zhixing = item_9_examine(shuru,cengshu);
		item_9_out(zhixing, start, goal, cengshu, dst, floor);
		if (item_9_judge(dst, floor))
		{
			cct_gotoxy(0, 35);
			cout << "��Ϸ����!!!!!" << endl;
			break;
		}	
	}
	return;
}

/***************************************************************************
  �������ƣ�item_9_in
  ��    �ܣ���Ŀ9��ִ����Ŀ9������
  ���������char *start,char*goal
  �� �� ֵ��0�˳�:1 A-B:2 A-C;3 B-A;4 B-C;5 C-A;6 C-B
  ˵    ������
***************************************************************************/
int item_9_in(char* start, char* goal)
{
	int i;
	while (1)
	{
		char data[20] = { '\0' };
		cct_gotoxy(68, 34);
		for (i = 0; i <= 19; i++)
		{
			while (1)
			{
				int temp = _getch();
				if (temp >= '!' && temp <= '~')//���ӻ�ͼ�βŸ�ֵ�����
				{
					data[i] = temp;
					cout << data[i];
					break;
				}
				else if (temp == '\r')
				{
					data[i] = temp;
					break;
				}	
			}
			if (data[i] == '\r')
				break;
		}
		if ((data[0] == 'A' || data[0] == 'a' || data[0] == 'B' || data[0] == 'b' || data[0] == 'C' || data[0] == 'c') &&
			(data[1] == 'A' || data[1] == 'a' || data[1] == 'B' || data[1] == 'b' || data[1] == 'C' || data[1] == 'c') && (data[2] == '\r'))
		{
			switch (int(data[0]))
			{
				case 'A':
				case 'a':
					*start = 'A';
					switch (int(data[1]))
					{
						case 'A':
						case 'a':
							cct_gotoxy(68, 34);
							cout << "                    ";
							break;
						case 'B':
						case 'b':
							*goal = 'B';
							return 1;
						case 'C':
						case 'c':
							*goal = 'C';
							return 2;
					}
					break;
				case 'B':
				case 'b':
					*start = 'B';
					switch (int(data[1]))
					{
						case 'A':
						case 'a':
							*goal = 'A';
							return 3;
						case 'B':
						case 'b':
							cct_gotoxy(68, 34);
							cout << "                    ";
							break;
						case 'C':
						case 'c':
							*goal = 'C';
							return 4;
					}
					break;
				case 'C':
				case 'c':
					*start = 'C';
					switch (int(data[1]))
					{
						case 'A':
						case 'a':
							*goal = 'A';
							return 5;
						case 'B':
						case 'b':
							*goal = 'B';
							return 6;
						case 'C':
						case 'c':
							cct_gotoxy(68, 34);
							cout << "                    ";
							break;
					}
					break;
			}
		}
		else if ((data[0] == 'Q' || data[0] == 'q') && data[1] == '\r')
			return 0;
		else
		{
			cct_gotoxy(68, 34);
			cout << "                    ";
		}
	}
}

/***************************************************************************
  �������ƣ�item_9_pdcs
  ��    �ܣ��ж�Ҫ�ƶ����ǵڼ��������
  ���������char start, int* cengshu
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void item_9_pdcs(char start, int* cengshu)
{
	int i;
	switch (int(start))//��ʼ����ߵ�һ�����Ӽ���ҪŲ��������
	{
		case 'A':
			for (i = 9; i >= 0; i--)
			{
				if (A_[i] != 0)
				{
					*cengshu = A_[i];
					return;
				}
			}
			break;
		case 'B':
			for (i = 9; i >= 0; i--)
			{
				if (B_[i] != 0)
				{
					*cengshu = B_[i];
					return;
				}
			}
			break;
		case 'C':
			for (i = 9; i >= 0; i--)
			{
				if (C_[i] != 0)
				{
					*cengshu = C_[i];
					return;
				}
			}
			break;
		default:
			cout << "error";
			break;
	}
	*cengshu = 0;//ʹ�ñ���cengshu������ֵ�����ܴ�������Խ����Ӱ��
	return;
}

/***************************************************************************
  �������ƣ�item_9_examine
  ��    �ܣ��ж������Ƿ����ִ��
  ���������int shuru(��ֵ������Ϊ123456),int cengshu
  �� �� ֵ��1 Դ��Ϊ��:2 ����ѹС��;0 ��ȷ
  ˵    ������
***************************************************************************/
int item_9_examine(int shuru,int cengshu)
{
	int end_top = 0, i;
	switch (shuru)//����Ƿ�Դ��Ϊ��
	{
		case 1:
		case 2:
			if (A_[0] == 0)
				return 1;
			break;
		case 3:
		case 4:
			if (B_[0] == 0)
				return 1;
			break;
		case 5:
		case 6:
			if (C_[0] == 0)
				return 1;
			break;
		default:
			cout << "error";
			break;	
	}
	switch (shuru)//�ж��Ƿ����ѹС��
	{
		case 3:
		case 5:
			for (i = 9; i >= 0; i--)
			{
				if (A_[i] != 0)
				{
					end_top = A_[i];
					break;
				}		
			}
			break;
		case 1:
		case 6:
			for (i = 9; i >= 0; i--)
			{
				if (B_[i] != 0)
				{
					end_top = B_[i];
					break;
				}
			}
			break;
		case 2:
		case 4:
			for (i = 9; i >= 0; i--)
			{
				if (C_[i] != 0)
				{
					end_top = C_[i];
					break;
				}
			}
			break;
		default:
			cout << "error";
			break;
	}
	if (cengshu > end_top && end_top != 0)
		return 2;
	return 0;

}

/***************************************************************************
  �������ƣ�item_9_out
  ��    �ܣ����ݼ���Ľ�����д�ӡ
  ���������int zhixing,char start,char goal,int cengshu,char dst,int floor
  �� �� ֵ��void
  ˵    ������
***************************************************************************/
void item_9_out(int zhixing,char start,char goal,int cengshu,char dst,int floor)
{
	switch (zhixing)
	{
		case 1:
			cct_gotoxy(0, 35);
			cout << "Դ��Ϊ��!";
			Sleep(1000);
			cct_gotoxy(68, 34);
			cout << "                    ";
			cct_gotoxy(0, 35);
			cout << "          ";
			break;
		case 2:
			cct_gotoxy(0, 35);
			cout << "����ѹС�̣��Ƿ��ƶ�!";
			Sleep(1000);
			cct_gotoxy(68, 34);
			cout << "                    ";
			cct_gotoxy(0, 35);
			cout << "                      ";
			break;
		case 0:
			change(start, goal);
			if (item_9_judge(dst, floor))
				;
			else
			{
				cct_gotoxy(68, 34);
				cout << "                    ";
			}
			ji_shu = ji_shu + 1;
			cct_setcursor(3);//ʹ�ù�겻��ʾ
			row_out_348(cengshu, start, goal, 8, 1);
			column_out_48(cengshu, start, goal, 8, 1);
			plate_move(start, goal);
			cct_setcursor(2);//ʹ�ù����ʾ
			break;
	}
	return;
}

/***************************************************************************
  �������ƣ�item_9_judge
  ��    �ܣ��ж��Ƿ�ɹ�
  ���������char dst,int floor
  �� �� ֵ��1Ϊ�ɹ�;0Ϊʧ��
  ˵    ������
***************************************************************************/
int item_9_judge(char dst,int floor)
{
	int i;
	switch (int(dst))
	{
		case'A':
			for (i = 0; i <= (floor -1); i++)
			{
				if (A_[i] != floor - i)
					return 0;
			}
			break;
		case'B':
			for (i = 0; i <= (floor - 1); i++)
			{
				if (B_[i] != floor - i)
					return 0;
			}
			break;
		case'C':
			for (i = 0; i <= (floor - 1); i++)
			{
				if (C_[i] != floor - i)
					return 0;
			}
			break;
	}
	return 1;
}

/***************************************************************************
  �������ƣ�menu_ebd
  ��    �ܣ�ÿ����Ŀ��ɺ�Ľ������غ���
  ���������int mode
  �� �� ֵ��voiv
  ˵    ������
***************************************************************************/
void menu_end(int mode)
{
	switch (mode)
	{
		case 1:
		case 2:
		case 3:
			cout << endl;
			cout << "���س�������";
			while (1)
			{
				if (_getch() == '\r')
					break;
			}
			break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			cct_gotoxy(0, 38);
			cout << "���س�������";
			while (1)
			{
				if (_getch() == '\r')
					break;
			}
			break;
		default:
			cout << "error" << endl;
			break;
	}
	data_renew();
	return;

}







