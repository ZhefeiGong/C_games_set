/* 2052532 龚哲飞 信04 */
#include <iostream>
#include <conio.h>
#include <windows.h>
#include "../include/cmd_console_tools.h"//包含命令行工具函数
#include "../include/cmd_gmw_tools.h"//包含伪图形工具函数
#include "90-01-b2-gmw-mine_sweeper.h"//包含自己写的函数


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
	while (1)
	{

		if (!First())//首页按下退出键 直接退出
			return 0;
		/*声明一个CGI变量*/
		CONSOLE_GRAPHICS_INFO MINE_CGI;
		gmw_init(&MINE_CGI);//进行结构体的初始化
		/*定义初始化内部数组*/
		/*用可能存在的最大数组去初始化*/
		 int inbase[ROW_MAX][COL_MAX] = {
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		};
		/*定义扫雷界面的情况*/
		const BLOCK_DISPLAY_INFO bdi[] = {
		{BLANK, COLOR_WHITE, -1, "  "},  //BLANK表示将处于空白的情况
		{1, COLOR_WHITE, COLOR_BLUE, NULL},
		{2, COLOR_WHITE,COLOR_GREEN,  NULL},
		{3, COLOR_WHITE,COLOR_CYAN	,  NULL},
		{4,COLOR_WHITE, COLOR_RED,  NULL},
		{5,COLOR_WHITE, COLOR_PINK, NULL},
		{6,COLOR_WHITE, COLOR_HBLACK,  NULL},
		{7, COLOR_WHITE, COLOR_HGREEN, NULL},
		{8,COLOR_WHITE,COLOR_HRED,NULL},
		{MINE_SIGN,COLOR_WHITE,COLOR_HRED,"●"},           //此时为雷
		{TAB_MINE_SIGN,COLOR_RED,COLOR_WHITE,"☆"},         //雷被标记的情况
		{TAB_NOT_SIGN,COLOR_RED,COLOR_WHITE,"☆"},          //非雷位置被标记的情况
		{COVER_SIGN,COLOR_YELLOW, -1,  "  "},               //此时为掩盖情况
		{BDI_VALUE_END, -1, -1, NULL}                       //判断结束条件为-999
		};
		/*定义行列*/
		int col = 0, row = 0, mine = 0;
		switch (level())
		{
			case LEVEL_1:
				col = COL_1;
				row = ROW_1;
				mine = MINE_1;
				break;
			case LEVEL_2:
				col = COL_2;
				row = ROW_2;
				mine = MINE_2;
				break;
			case LEVEL_MAX:
				col = COL_MAX;
				row = ROW_MAX;
				mine = MINE_MAX;
				break;
			default:
				return -1;//此时出错！！！
		}
		/*执行函数部分*/
		mine_sweeper_start(&MINE_CGI, inbase, bdi, row, col, mine);
		mine_sweeper_mid(&MINE_CGI, inbase, bdi, mine);
		mine_sweeper_end(&MINE_CGI);


	}
}


