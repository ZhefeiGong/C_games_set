/* 2052532 龚哲飞 信04 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "../include/cmd_console_tools.h"        //包含命令行工具函数
#include "../include/cmd_gmw_tools.h"            //包含伪图形工具函数
#include "../include/common_output.h"            //包含自己写的工具函数
#include "90-02-b2-gmw-2048.h"                   //包含本项目的头文件

using namespace std;

/* -----------------------------------------
	 本文件功能：
	存放main函数
   ----------------------------------------- */

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：main主函数
***************************************************************************/
int main()
{
	int score_highest = 0;
	while (1)
	{

		if (!First())//首页按下退出键 直接退出
			return 0;
		/*声明一个CGI变量*/
		CONSOLE_GRAPHICS_INFO M2048_CGI;
		gmw_init(&M2048_CGI);//进行结构体的初始化
		 
        const BLOCK_DISPLAY_INFO bdi[] = {
		    {BDI_VALUE_BLANK, -1, -1, NULL},  //0不显示，用空格填充即可
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
	     	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
		};


		while (1)
		{

			/*定义内部数组*/
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
			/*计算分数*/
			if (score > score_highest)
				score_highest = score;
			if (go == 0)
				break; //退出游戏 返回游戏界面
			else if (go == 1)
				continue;//重新玩游戏
			else if (go == 2)//游戏结束
			{
				
				int go_on=my2048_end(&M2048_CGI);
				if (go_on == 0)
					break;//退出游戏 返回游戏界面
				else if (go_on == 1)
					continue;//重新玩游戏
			}
		}
	}
}