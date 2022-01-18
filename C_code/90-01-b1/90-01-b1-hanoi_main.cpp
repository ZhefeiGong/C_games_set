/* 2052532 信04 龚哲飞 */
#include <iostream>
#include <conio.h>
#include <windows.h>
#include "../include/cmd_console_tools.h"//包含工具函数
#include "90-01-b1-hanoi.h"

using namespace std;

/* -----------------------------------------

     本文件功能：
	1、放main函数
	2、初始化屏幕
	3、调用菜单函数（hanoi_menu.cpp中）并返回选项
	4、根据选项调用菜单各项对应的执行函数（hanoi_multiple_solutions.cpp中）

     本文件要求：
	1、不允许定义全局变量（含外部全局和静态全集，const及#define不在限制范围内）
	2、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
	3、按需加入系统头文件、自定义头文件、命名空间等

   ----------------------------------------- */

/***************************************************************************
  函数名称：main
  功    能：主函数
  输入参数：void
  返 回 值：0
  说    明：无
***************************************************************************/
int main()
{
	/* demo中首先执行此句，将cmd窗口设置为40行x120列（缓冲区宽度120列，行数9000行，即cmd窗口右侧带有垂直滚动杆）*/
	cct_setconsoleborder(120, 40, 120, 9000);
	while (1)
	{
		cct_cls();
		int result = menu_out_hanoi();
		if (result == 0 || result == -1)//此时自行退出或者程序出错 
		{
			cct_gotoxy(0, 37);
			return 0;
		}
	}
	return 0;
}
