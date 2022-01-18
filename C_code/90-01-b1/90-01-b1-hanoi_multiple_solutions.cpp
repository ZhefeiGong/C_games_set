/* 2052532 信04 龚哲飞 */
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <cmath>
#include <windows.h>
#include "../include/cmd_console_tools.h"//包含工具函数
#include "90-01-b1-hanoi.h"
#include "..\include\common_output.h"//包含自己的共用函数

using namespace std;
static int ji_shu = 0;//用于汉诺塔的移动以及计数
static int A_[10] = { 0,0,0,0,0,0,0,0,0,0 };
static int B_[10] = { 0,0,0,0,0,0,0,0,0,0 };
static int C_[10] = { 0,0,0,0,0,0,0,0,0,0 };
static int top_A = 0, top_B = 0, top_C = 0;
static int yan_shi = 0;

/* -----------------------------------------

     本文件功能：
	1、放被 hanoi_main.cpp/hanoi_menu.cpp 中的各函数调用的菜单各项对应的执行函数

     本文件要求：
	1、不允许定义外部全局变量（const及#define不在限制范围内）
	2、允许定义静态全局变量（具体需要的数量不要超过文档显示，全局变量的使用准则是：少用、慎用、能不用尽量不用）
	3、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
	4、按需加入系统头文件、自定义头文件、命名空间等

   ----------------------------------------- */

   /***************************************************************************
	 函数名称：menu_out_hnoi
	 功    能：输出相应的菜单界面及输入函数
	 输入参数：void
	 返 回 值：int
	 说    明：无
   ***************************************************************************/
int menu_out_hanoi(void)
{

	struct menu eli_start[10] = { {'1',"基本解"},
								  {'2',"基本解(步数记录)"},
								  {'3',"内部数组显示(横向)"},
								  {'4',"内部数组显示(纵向+横向)"},
								  {'5',"图形解-预备-画三个圆柱"},
								  {'6',"图形解-预备-在起始柱上画n个盘子"},
								  {'7',"图形解-预备-第一次移动"},
								  {'8',"图形解 - 自动移动版本"},
								  {'9',"图形解-游戏版"},
								  {'0',"退出"} };
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
			return -1;//此时为程序出错的情况
	}
	switch (back)
	{
		case 'A':
		case 'B':
		case 'C':
			cout << endl;
			cout << "按回车键继续";
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
			cout << "按回车键继续";
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
  函数名称：hanoi
  功    能：递归函数打印相关的内容
  输入参数：int n(搬运的层数), char src(起始柱), char tmp(中间柱), char dst(终点柱),int mode(输出的模式)
  返 回 值：void
  说    明：无
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
  函数名称：hanoi_out
  功    能：打印汉诺塔的相关内容
  输入参数：int n, char src, char tmp, char dst, int mode
  返 回 值：void
  说    明：无
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
				tower();//打印塔
				start_plate();//打印起始盘
				cct_setcolor(0, 7);//恢复原来的颜色
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
  函数名称：column_out_48
  功    能：输出4\8的列
  输入参数：int n, char src, char dst, int mode, int start(0是起始行:1不是起始行)
  返 回 值：void
  说    明：无
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
  函数名称：row_out_348
  功    能：输出3\4\8的行
  输入参数：int n, char src, char dst,int mode, int start(4\8中的起始行:0是起始行，1不是起始行)
  返 回 值：void
  说    明：无
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
		cout << "第" << setw(4) << setiosflags(ios::right) << ji_shu << " 步" << '(' << setw(2) << setiosflags(ios::right)
			<< n << "#:" << setw(2) << setiosflags(ios::right) << src << "-->" << dst << ") ";
		row_inner();
	}
	return;
}

/***************************************************************************
  函数名称：row_out_1
  功    能：输出1的行
  输入参数：int n, char src, char dst
  返 回 值：void
  说    明：无
***************************************************************************/
void row_out_1(int n, char src, char dst)
{
	cout << setw(2) << setiosflags(ios::right) << n << '#' << setw(2)
		<< setiosflags(ios::right) << src << "---->" << dst << endl;
	return;
}

/***************************************************************************
  函数名称：row_out_2
  功    能：输出2的行
  输入参数：int n, char src, char dst
  返 回 值：void
  说    明：无
***************************************************************************/
void row_out_2(int n, char src, char dst)
{
	cout << "第" << setw(4) << setiosflags(ios::right) << ji_shu << " 步" << '(' << setw(2) << setiosflags(ios::right)
		<< n << "#:" << setw(2) << setiosflags(ios::right) << src << "-->" << dst << ")" << endl;
	return;
}

/***************************************************************************
  函数名称：start_row_48
  功    能：输出4\8行中的初始行
  输入参数：无
  返 回 值：void
  说    明：无
***************************************************************************/
void start_row_48(void)
{
	int i;
	cout << "初始:  A:";
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
  函数名称：row_inner
  功    能：打印汉诺塔的详细移动步骤(横向)
  输入参数：void
  返 回 值：void
  说    明：无
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
  函数名称：stop
  功    能：函数之间的延时
  输入参数：void
  返 回 值：void
  说    明：无
***************************************************************************/
void stop(void)
{
	switch (yan_shi)//为用于停顿的选择结构
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
  函数名称：change
  功    能：交换数组的顶针
  输入参数：char start, char goal
  返 回 值：void
  说    明：无
***************************************************************************/
void change(char start, char goal)//交换数组的顶针
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
  函数名称：column_inner
  功    能：打印4\8的竖的数组
  输入参数：int mode
  返 回 值：void
  说    明：无
***************************************************************************/
void column_inner(int mode)//打印竖的数组
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
  函数名称：input
  功    能：123456789的参数的输入函数
  输入参数：int* floor, char* start, char* mid,char* goal, int* yan_shi,int yby(0不需要延时:1需要延时)
  返 回 值：void
  说    明：无
***************************************************************************/
void input(int* floor, char* start, char* mid,char* goal, int* yan_shi,int yby)
{
	int mubiao = 0;
	char chongfu;
	while (1)//判断层数是否输入正确
	{
		cout << "请输入汉诺塔的层数(1-10):" << endl;
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
	while (1)//判断起始柱是否输入正确
	{
		cout << "请输入起始柱(A-C):" << endl;
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
	while (1)//判断目标柱是否输入正确
	{
		cout << "请输入目标柱(A-C):" << endl;;
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
						cout << "目标柱(A)不能与起始柱(A)相同" << endl;
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
						cout << "目标柱(B)不能与起始柱(B)相同" << endl;
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
						cout << "目标柱(C)不能与起始柱(C)相同" << endl;
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
	*mid = 198 - *start - *goal;//确定中间柱的值
	if (yby)
	{
		while (1)//输入判断移动速度的值
		{
			cout << "请输入移动速度(0-5: 0-按回车单步演示 1-延时最长 5-延时最短)" << endl;
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
  函数名称：item_1
  功    能：执行项目1的函数
  输入参数：void
  返 回 值：void
  说    明：无
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
  函数名称：item_2
  功    能：执行项目2的函数
  输入参数：void
  返 回 值：void
  说    明：无
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
  函数名称：item_3
  功    能：执行项目3的函数
  输入参数：void
  返 回 值：void
  说    明：无
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
  函数名称：item_4
  功    能：执行项目4的函数
  输入参数：void
  返 回 值：void
  说    明：无
***************************************************************************/
void item_4(void)
{
	cct_cls();
	char src, tmp, dst;
	int floor;
	input(&floor, &src, &tmp, &dst, &yan_shi, 1);
	cct_cls();//清除所有的内容
	cout << "从 " << char(src) << " 移动到 " << char(dst) << "，共 " << floor << " 层，延时设置为 " << yan_shi <<endl;
	initial_zhu(floor, src);//根据输入的数据给三个柱子赋值
	cct_setcursor(3);//使得光标不显示
	hanoi(floor, src, tmp, dst, 4);
	cct_setcursor(2);//使得光标显示
	return;

}

/***************************************************************************
  函数名称：initial_zhu
  功    能：给ABC三个柱子初始赋值
  输入参数：int floor, char start
  返 回 值：void
  说    明：无
***************************************************************************/
void initial_zhu(int floor, char start)
{
	int i;
	if (start == 'A')//各个柱子赋值
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
  函数名称：data_renew
  功    能：每次项目执行完成后，将静态全局变量重置
  输入参数：void
  返 回 值：void
  说    明：无
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
    ji_shu = 0;//用于汉诺塔的移动以及计数
	return;
}

/***************************************************************************
  函数名称：item_8
  功    能：执行项目8的函数
  输入参数：void
  返 回 值：void
  说    明：无
***************************************************************************/
void item_8(void)
{
	cct_cls();
	char src, tmp, dst;
	int floor;
	input(&floor, &src, &tmp, &dst, &yan_shi, 1);
	cct_cls();//清除所有的内容
	cout << "从 " << char(src) << " 移动到 " << char(dst) << "，共 " << floor << " 层，延时设置为 " << yan_shi << endl;
	initial_zhu(floor, src);
	cct_setcursor(3);//使得光标不显示
	hanoi(floor, src, tmp, dst, 8);
	cct_setcursor(2);//使得光标显示
	return;

}

/***************************************************************************
  函数名称：tower
  功    能：打印56789中伪图形界面的塔
  输入参数：void
  返 回 值：void
  说    明：无
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
  函数名称：item_5
  功    能：执行项目5的函数
  输入参数：void
  返 回 值：void
  说    明：无
***************************************************************************/
void item_5(void)
{
	cct_cls();//清除所有的内容
	cct_setcursor(3);//使得光标不显示
	tower();
	cct_setcursor(2);//使得光标显示
	cct_setcolor(0, 7);//色彩恢复原状
	return;
}

/***************************************************************************
  函数名称：item_6
  功    能：执行项目6的函数
  输入参数：void
  返 回 值：void
  说    明：无
***************************************************************************/
void item_6(void)
{
	cct_cls();
	char src, tmp, dst;
	int floor;
	input(&floor, &src, &tmp, &dst, &yan_shi, 0);
	cct_cls();//清除所有的内容
	cout << "从 " << char(src) << " 移动到 " << char(dst) << "，共 " << floor << " 层"<< endl;
	initial_zhu(floor, src);//根据输入的数据三个柱子赋值
	cct_setcursor(3);//使得光标不显示
	tower();
	start_plate();//打印起始盘
	cct_setcursor(2);//使得光标显示
	cct_setcolor(0, 7);//恢复原来的颜色
	return;

}

/***************************************************************************
  函数名称：start_plate
  功    能：打印起始的圆盘
  输入参数：void
  返 回 值：void
  说    明：无
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
  函数名称：plate_move_data
  功    能：确定移动的移动色块的相关数据
  输入参数：int*start_floor,int* end_floor,int* lump(确定盘子多大，啥色),char start,char goal
  返 回 值：void
  说    明：本函数的数据处理是基于盘子已经移动完成的逆推
***************************************************************************/
void plate_move_data(int*start_floor,int* start_zhu,int* end_floor, int* end_zhu,int* lump,char start,char goal)
{
	int i;
	switch (int(goal))//确定移动的盘子,到终点柱去找
	{
		case 'A':
			for (i = 9; i >= 0; i--)
			{
				if (A_[i] != 0)
				{
					*end_zhu = 12;//柱子的位置
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
					*end_zhu = 44;//柱子的位置
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
					*end_zhu = 76;///柱子的位置
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
				*start_zhu = 12;//柱子的位置
				if (A_[i] != 0)//确定起始柱的层数
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
				*start_zhu = 44;//柱子的位置
				if (B_[i] != 0)//确定起始柱的层数
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
				*start_zhu = 76;//柱子的位置
				if (C_[i] != 0)//确定起始柱的层数
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
  函数名称：plate_move
  功    能：执行789中盘子移动的总函数
  输入参数：char src,char dst
  返 回 值：void
  说    明：函数内部已包括恢复原来颜色的命令
***************************************************************************/
void plate_move(char src,char dst)
{
	int start_floor, start_zhu, end_floor, end_zhu, lump;
	plate_move_data(&start_floor, &start_zhu, &end_floor, &end_zhu, &lump, src, dst);
	plate_move_vertical_up(start_zhu, start_floor, lump);
	plate_move_level(start_zhu, end_zhu, lump);
	plate_move_vertical_down(end_zhu, end_floor, lump);
	cct_setcolor(0, 7);//恢复原来的颜色
	return;
}

/***************************************************************************
  函数名称：plate_move_level
  功    能：控制盘子水平移动
  输入参数：int start, int goal, int lump
  返 回 值：void
  说    明：无
***************************************************************************/
void plate_move_level(int start, int goal, int lump)
{
	int i, j, delt;//i仅用于打印某种色块,避免与j产生混淆
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
			cct_setcolor(0, 7);//恢复原来的色彩
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
			cct_setcolor(0, 7);//恢复原来的色彩
			for (i = 0; i <= (2 * lump); i++)
				cout << ' ';
		}
	}
	return;
}

/***************************************************************************
  函数名称：plate_move_vertical_up
  功    能：将盘子竖直向上移动
  输入参数：int zhu, int floor, int lump
  返 回 值：void
  说    明：使得盘子向上移动
***************************************************************************/
void plate_move_vertical_up(int zhu, int floor, int lump)
{
	int i,j;//j仅用于打印某种色块,避免与i产生混淆
	cct_gotoxy((zhu - lump), (5 + (9 - floor)));//将起始柱的顶端的色块清除
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
		for (j = 0; j <= (2 * lump); j++)//方块的打印
			cout << ' ';
		sleep();
		cct_gotoxy((zhu - lump), (5 + (9 - floor) - i));//清除色块
		cct_setcolor(0, 7);
		for (j = 0; j <= (2 * lump); j++)
			cout << ' ';
		cct_setcolor(14, 14);//恢复柱子
		cct_gotoxy(zhu, (5 + (9 - floor) - i));
		cout << ' ';
	}
	cct_gotoxy((zhu - lump), 2);
	cct_setcolor(lump, lump);
	for (j = 0; j <= (2 * lump); j++)//方块的打印
		cout << ' ';
	sleep();
	cct_gotoxy((zhu - lump), 2);//清除色块
	cct_setcolor(0, 7);
	for (j = 0; j <= (2 * lump); j++)//清除色块
		cout << ' ';
	return;
}

/***************************************************************************
  函数名称：plate_move_vertical_down
  功    能：将盘子竖直向下移动
  输入参数：int zhu, int floor, int lump
  返 回 值：void
  说    明：使得盘子向下移动
***************************************************************************/
void plate_move_vertical_down(int zhu, int floor, int lump)
{
	int i,j;//j仅用于打印某种色块,避免与i产生混淆
	cct_gotoxy((zhu - lump), 2);
	cct_setcolor(lump, lump);
	for (j = 0; j <= (2 * lump); j++)//方块的打印
		cout << ' ';
	sleep();
	cct_gotoxy((zhu - lump), 2);//清除色块
	cct_setcolor(0, 7);
	for (j = 0; j <= (2 * lump); j++)//清除色块
		cout << ' ';
	for (i = 1; i <= (3 + (9 - floor)); i++)
	{
		cct_gotoxy((zhu - lump), (2 + i));
		cct_setcolor(lump, lump);
		for (j = 0; j <= (2 * lump); j++)//方块的打印
			cout << ' ';
		if (i != (3 + (9 - floor)))
		{
			sleep();
			cct_gotoxy((zhu - lump), (2 + i));//清除色块
			cct_setcolor(0, 7);
			for (j = 0; j <= (2 * lump); j++)
				cout << ' ';
			cct_setcolor(14, 14);//恢复柱子
			cct_gotoxy(zhu, (2 + i));
			cout << ' ';
		}
	}
	return;
}

/***************************************************************************
  函数名称：item_7
  功    能：执行项目7的函数
  输入参数：void
  返 回 值：void
  说    明：无
***************************************************************************/
void item_7(void)
{
	cct_cls();
	char src, tmp, dst;
	int floor;
	input(&floor, &src, &tmp, &dst, &yan_shi, 0);
	cct_cls();//清除所有的内容
	cout << "从 " << char(src) << " 移动到 " << char(dst) << "，共 " << floor << " 层" << endl;
	initial_zhu(floor, src);//根据输入的数据三个柱子赋值
	cct_setcursor(3);//使得光标不显示
	tower();
	start_plate();//打印起始盘
	change(src, dst);
	cct_setcolor(0, 7);
	yan_shi = 0;
	plate_move(src, dst);
	cct_setcursor(2);//使得光标显示
	return;

}

/***************************************************************************
  函数名称：sleep
  功    能：调整方块移动时候的停止时间
  输入参数：void
  返 回 值：void
  说    明：无
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
  函数名称：item_9
  功    能：执行项目9的函数(游戏哟！！！)
  输入参数：void
  返 回 值：void
  说    明：无
***************************************************************************/
void item_9(void)
{
	cct_cls();
	char src, tmp, dst;
	int floor;
	input(&floor, &src, &tmp, &dst, &yan_shi, 0);
	yan_shi = 4;//控制方块的移动速度
	cct_cls();//清除所有的内容
	cout << "从 " << char(src) << " 移动到 " << char(dst) << "，共 " << floor << " 层" << endl;
	initial_zhu(floor, src);//根据输入的数据三个柱子赋值
	cct_setcursor(3);//使得光标不显示
	column_out_48(floor, src, dst, 8, 0);//打印起始柱子
	row_out_348(floor, src, dst, 8, 0);//打印起始行
	tower();
	start_plate();//打印起始盘
	cct_setcursor(2);//使得光标显示
	cct_setcolor(0, 7);//恢复原来的颜色
	cct_gotoxy(0, 34);
	cout << "请输入移动的柱号(命令形式：AC表示把A顶端的盘子移动到C，Q表示退出) ：" << endl;
	while (1)
	{
		char start, goal;
		int cengshu, shuru, zhixing;
		cct_gotoxy(68, 34);
		shuru = item_9_in(&start,&goal);
		if (shuru == 0)
		{
			cct_gotoxy(0, 35);
			cout << "游戏中止!!!!!" << endl;
			break;
		}
		item_9_pdcs(start, &cengshu);
		zhixing = item_9_examine(shuru,cengshu);
		item_9_out(zhixing, start, goal, cengshu, dst, floor);
		if (item_9_judge(dst, floor))
		{
			cct_gotoxy(0, 35);
			cout << "游戏结束!!!!!" << endl;
			break;
		}	
	}
	return;
}

/***************************************************************************
  函数名称：item_9_in
  功    能：项目9中执行项目9的输入
  输入参数：char *start,char*goal
  返 回 值：0退出:1 A-B:2 A-C;3 B-A;4 B-C;5 C-A;6 C-B
  说    明：无
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
				if (temp >= '!' && temp <= '~')//可视化图形才赋值并输出
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
  函数名称：item_9_pdcs
  功    能：判断要移动的是第几层的盘子
  输入参数：char start, int* cengshu
  返 回 值：void
  说    明：无
***************************************************************************/
void item_9_pdcs(char start, int* cengshu)
{
	int i;
	switch (int(start))//起始塔最高的一个盘子即是要挪动的盘子
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
	*cengshu = 0;//使得变量cengshu均被赋值，尽管此项操作对结果无影响
	return;
}

/***************************************************************************
  函数名称：item_9_examine
  功    能：判断输入是否可以执行
  输入参数：int shuru(数值的数字为123456),int cengshu
  返 回 值：1 源柱为空:2 大盘压小盘;0 正确
  说    明：无
***************************************************************************/
int item_9_examine(int shuru,int cengshu)
{
	int end_top = 0, i;
	switch (shuru)//检查是否源柱为空
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
	switch (shuru)//判断是否大盘压小盘
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
  函数名称：item_9_out
  功    能：根据检查后的结果进行打印
  输入参数：int zhixing,char start,char goal,int cengshu,char dst,int floor
  返 回 值：void
  说    明：无
***************************************************************************/
void item_9_out(int zhixing,char start,char goal,int cengshu,char dst,int floor)
{
	switch (zhixing)
	{
		case 1:
			cct_gotoxy(0, 35);
			cout << "源柱为空!";
			Sleep(1000);
			cct_gotoxy(68, 34);
			cout << "                    ";
			cct_gotoxy(0, 35);
			cout << "          ";
			break;
		case 2:
			cct_gotoxy(0, 35);
			cout << "大盘压小盘，非法移动!";
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
			cct_setcursor(3);//使得光标不显示
			row_out_348(cengshu, start, goal, 8, 1);
			column_out_48(cengshu, start, goal, 8, 1);
			plate_move(start, goal);
			cct_setcursor(2);//使得光标显示
			break;
	}
	return;
}

/***************************************************************************
  函数名称：item_9_judge
  功    能：判断是否成功
  输入参数：char dst,int floor
  返 回 值：1为成功;0为失败
  说    明：无
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
  函数名称：menu_ebd
  功    能：每个项目完成后的结束返回函数
  输入参数：int mode
  返 回 值：voiv
  说    明：无
***************************************************************************/
void menu_end(int mode)
{
	switch (mode)
	{
		case 1:
		case 2:
		case 3:
			cout << endl;
			cout << "按回车键继续";
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
			cout << "按回车键继续";
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







