/* 2052532 龚哲飞 信04 */
#include <iostream>
#include "../include/cmd_console_tools.h"        //包含命令行工具函数
#include "../include/cmd_gmw_tools.h"            //包含伪图形工具函数
#include "../include/common_output.h"            //包含自己写的工具函数
#include "90-02-b1-gmw-eliminate_stars.h"        //包含本项目的头文件

using namespace std;

/* -----------------------------------------
	本文件功能：
	存放main函数
----------------------------------------- */

/*****************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：main主函数
***************************************************************************/
int main()
{
	while (1)
	{
		if (!First())//首页按下退出键 直接退出
			return 0;

		/*声明一个CGI变量*/
		CONSOLE_GRAPHICS_INFO STAR_CGI;

        /*进行结构体的初始化*/
		/*窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略*/
		gmw_init(&STAR_CGI);

		/*初始化一个最大的数组储存内部信息*/
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

		/*定义1-5数字以何种形式在界面上显示*/
		const BLOCK_DISPLAY_INFO bdi[] = {
			{BLANK, -1, -1, "  "},//空白
			{1, COLOR_HBLUE, COLOR_BLACK, "★"},//储存正常状态
		    {2, COLOR_HGREEN, COLOR_BLACK, "★"},
		    {3, COLOR_HCYAN, COLOR_BLACK, "★"},
		    {4, COLOR_HRED, COLOR_BLACK, "★"},
		    {5, COLOR_HPINK, COLOR_BLACK, "★"},
			/*正常和需要反显状态之间相差BETWEEN*/
			{BLANK+BETWEEN, -1, -1, "  "},//空白
			{11, COLOR_HBLUE, COLOR_HWHITE, "★"},//储存需要反显的状态
			{12, COLOR_HGREEN, COLOR_HWHITE, "★"},
			{13, COLOR_HCYAN, COLOR_HWHITE, "★"},
			{14, COLOR_HRED, COLOR_HWHITE, "★"},
			{15, COLOR_HPINK, COLOR_HWHITE, "★"},
			{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
		};

		/*取所需要的行列值*/
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
		}//end of while(小）
	}//end of while(大)

}