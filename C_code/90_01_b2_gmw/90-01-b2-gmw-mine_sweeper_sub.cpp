/* 2052532 龚哲飞 信04 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "../include/cmd_console_tools.h"//包含命令行工具函数
#include "../include/cmd_gmw_tools.h"//包含伪图形工具函数
#include "90-01-b2-gmw-mine_sweeper.h"//包含自己写的函数

using namespace std;

/* -----------------------------------------
	 本文件功能：
	存放内部实现函数
   ----------------------------------------- */

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：进行雷数的选择
***************************************************************************/
int level(void)
{
	cct_cls();//清屏
	cout << "请选择难度：" << endl;
	cout << "  1.初级( 9*9  - 10颗雷)" << endl;
	cout << "  2.中级(16*16 - 40颗雷)" << endl;
	cout << "  3.高级(16*30 - 99颗雷)" << endl;
	cout << "请输入[1..3]：";
	char shuru;
	while (1)//输入选择的雷的类型
	{
		shuru = _getch();//返回对应的ASCⅡ码
		if (shuru >= '1' && shuru <= '3')
		{
			cout << shuru;
			break;
		}
	}
	return (int)(shuru - '0');
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：返回 1 继续
			返回 0 退出
***************************************************************************/
int First(void)
{
	cct_cls();//清屏
	cct_setconsoleborder(50, 15, 50, 15);//初始化界面
	cct_setfontsize("新宋体", 32, 14);
	cct_setcursor(CURSOR_VISIBLE_NORMAL);
	system("color E9");//初始化背景颜色
	cct_setcolor(COLOR_HYELLOW, COLOR_HBLUE);
	cct_gotoxy(12, 5);
	cout << "╔════════╗";
	cct_gotoxy(12, 6);
	cout << "║欢迎来到扫雷游戏║";
	cct_gotoxy(12, 7);
	cout << "╚════════╝";
	cct_gotoxy(30, 10);
	cout << "按下回车继续";
	cct_gotoxy(30, 11);
	cout << "按下Q/q退出";
	cct_setcolor();//颜色恢复正常

	cct_setcursor(CURSOR_INVISIBLE);                                //光标不显示
	
	char shuru;
	while (1)//根据输入返回
	{
		shuru = _getch();
		if (shuru == '\r')
			return 1;
		else if (shuru == 'q' || shuru == 'Q')
			return 0;
	}

	cct_setcolor();//颜色恢复正常
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：初始化扫雷的界面
***************************************************************************/
void mine_sweeper_start(CONSOLE_GRAPHICS_INFO* const  MINE_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* const bdi, const int row, const int col, const int num)
{
	cct_cls();//清屏

	/*按照row/col的值重设游戏主区域行列和部分初始化操作*/
	gmw_set_rowcol(MINE_CGI, row, col);
	gmw_set_status_line_switch(MINE_CGI, TOP_STATUS_LINE, true);     //开启上状态栏
	gmw_set_status_line_switch(MINE_CGI, LOWER_STATUS_LINE, true);   //开启下状态栏
	gmw_set_frame_color(MINE_CGI, COLOR_HWHITE, COLOR_BLACK);        //设置框架颜色
	gmw_set_font(MINE_CGI, "点阵字体", 16, 8);                       //设置字体
	gmw_set_frame_style(MINE_CGI, BLOCK_WIDTH, BLOCK_HIGH, false);  //不需要边框
	gmw_set_block_border_switch(MINE_CGI, true);                     //色块内需要边框
	gmw_set_rowno_switch(MINE_CGI, true);                            //需要行号
	gmw_set_colno_switch(MINE_CGI, true);                            //需要列标
	cct_setcursor(CURSOR_INVISIBLE);                                 //光标不显示


	/*显示框架*/
	gmw_draw_frame(MINE_CGI);

	/*打印上状态栏显示内容*/
	char line_top[64] = "按ESC退出，空格显示时间";
	gmw_status_line(MINE_CGI, TOP_STATUS_LINE, line_top);//打印上状态栏

	/*打印色块们*/
	for (int j = 0; j < row; j++)
		for (int i = 0; i < col; i++)
			gmw_draw_block(MINE_CGI, j, i, COVER_SIGN, bdi);//均采用隐藏 打印对应的数组

	return;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
		  各项返回值说明：
						   1   扫雷成功，游戏结束
						  -1   扫雷失败，游戏结束
						   0   扫雷退出，游戏未完
***************************************************************************/
int mine_sweeper_mid(CONSOLE_GRAPHICS_INFO* const MINE_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* const bdi, int num)
{

	/*取计时的基准*/
	clock_t start_time = clock();

	/*用于制造爆炸效果*/
	int BOOM1 = 1, BOOM2 = 2, BOOM3 = 3;
	const BLOCK_DISPLAY_INFO bdi_special[] = {
		{BDI_VALUE_BLANK,-1,-1,"  "},
		{BOOM1,COLOR_RED,COLOR_HRED,"〇"},
		{BOOM2,COLOR_RED,COLOR_HRED,"¤"},
		{BOOM3,COLOR_RED,COLOR_HRED,"●"},
	};//可用于制作爆炸效果！！！

	int maction, mrow = -1, mcol = -1;
	int keycode1, keycode2;
	int ret;
	int start = 0;

	while (1) {

		ret = gmw_read_keyboard_and_mouse(MINE_CGI, maction, mrow, mcol, keycode1, keycode2);

		if (ret == CCT_MOUSE_EVENT) {

			/*鼠标移动*/
			if (maction == MOUSE_ONLY_MOVED) {//先不进行反显的移动
				continue;
			}

			/*按下右键*/
			else if (maction == MOUSE_RIGHT_BUTTON_CLICK) {
				if (mrow == -1 || mcol == -1)//处于非法位置 标记无效
					continue;
				else if (inbase[mrow][mcol] == COVER_SIGN || inbase[mrow][mcol] == MINE_SIGN) {//当处于可标记的位置

					if (inbase[mrow][mcol] == COVER_SIGN)
						inbase[mrow][mcol] = TAB_NOT_SIGN;
					else
						inbase[mrow][mcol] = TAB_MINE_SIGN;
					gmw_draw_block(MINE_CGI, mrow, mcol, inbase[mrow][mcol], bdi);//反显打印标记情况
					mine_count(MINE_CGI, inbase, num);
				}
				else if (inbase[mrow][mcol] == TAB_NOT_SIGN || inbase[mrow][mcol] == TAB_MINE_SIGN) {
					if (inbase[mrow][mcol] == TAB_NOT_SIGN)
						inbase[mrow][mcol] = COVER_SIGN;
					else
						inbase[mrow][mcol] = MINE_SIGN;
					gmw_draw_block(MINE_CGI, mrow, mcol, COVER_SIGN, bdi);//均以隐藏打印标记情况
					mine_count(MINE_CGI, inbase, num);
				}
				else if (start == 0) {//第一次未赋值时
					inbase[mrow][mcol] = TAB_NOT_SIGN;
					gmw_draw_block(MINE_CGI, mrow, mcol, inbase[mrow][mcol], bdi);//反显打印标记情况
					mine_count(MINE_CGI, inbase, num);
				}
				//其余位置无法进行标记
			}

			/*按下左键*/
			else if (maction == MOUSE_LEFT_BUTTON_CLICK) {
				if (mrow == -1 || mcol == -1)//此时位置非法 点击无效！！！
					continue;
				else if (start == 0 && inbase[mrow][mcol] != TAB_NOT_SIGN) { //此时进行初始化 避免第一次就点到雷
					make_inbase(MINE_CGI, inbase, num, mrow, mcol);
					spread(MINE_CGI, inbase, mrow, mcol);

					for (int i = 0; i < MINE_CGI->row_num; i++)
						for (int j = 0; j < MINE_CGI->col_num; j++)
							if (inbase[i][j] >= 0 && inbase[i][j] <= 8)//仅0-9的数字才点开，其余情况不点开
								gmw_draw_block(MINE_CGI, i, j, inbase[i][j], bdi);

					start = 1;//仅初始化一次
				}
				else if (inbase[mrow][mcol] == COVER_SIGN) {//COVER_SIGN位置才可点开 进行点开操作
					spread(MINE_CGI, inbase, mrow, mcol);//进行数组的展开

					for (int i = 0; i < MINE_CGI->row_num; i++)
						for (int j = 0; j < MINE_CGI->col_num; j++)
							if (inbase[i][j] >= 0 && inbase[i][j] <= 8)//仅0-9的数字才点开，其余情况不点开
								gmw_draw_block(MINE_CGI, i, j, inbase[i][j], bdi);

					if (judge_win(MINE_CGI, inbase))//成功啦！！！
					{
						time_out(MINE_CGI, start_time, LOWER_STATUS_LINE, true);
						return 1;//成功
					}


				}
				else if (inbase[mrow][mcol] == MINE_SIGN) {
					for (int i = 1; i <= 4; i++)//出现爆炸效果
					{
						gmw_draw_block(MINE_CGI, mrow, mcol, BOOM1, bdi_special);
						Sleep(40);
						gmw_draw_block(MINE_CGI, mrow, mcol, BOOM2, bdi_special);
						Sleep(40);
						gmw_draw_block(MINE_CGI, mrow, mcol, BOOM3, bdi_special);
						Sleep(50);
					}
					gmw_draw_block(MINE_CGI, mrow, mcol, MINE_SIGN, bdi);//显示雷
					time_out(MINE_CGI, start_time, LOWER_STATUS_LINE);//默认失败
					return -1;//失败

				}
			}

			else
				continue;
		}
		else {
			switch (keycode1) {//只考虑按下ESC和空格键的情况 仅需考虑第一键码的情况！！！
				case 27: //按下ESC 直接退出
					return 0;//退出
					break;

				case 32: //按下空格 显示时间
					time_out(MINE_CGI, start_time, TOP_STATUS_LINE);
					break;
				default:
					break;
			}//end of switch
		}//end of else
	}//end of while

}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：判断"按回车键继续..."的操作
***************************************************************************/
void mine_sweeper_end(CONSOLE_GRAPHICS_INFO* const MINE_CGI)
{
	int x = MINE_CGI->SLI.lower_start_x;
	int y = MINE_CGI->SLI.lower_start_y + 3;
	cct_gotoxy(x, y);
	cout << "按回车键继续...";

	char shuru;
	while (1)//根据输入返回
	{
		shuru = _getch();
		if (shuru == '\r')
			return;
	}

}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：根据第一次点开的位置 初始化雷
***************************************************************************/
void make_inbase(CONSOLE_GRAPHICS_INFO* const MINE_CGI, int inbase[ROW_MAX][COL_MAX], const int num, const int st_row, const int st_col)
{
	int row_max = MINE_CGI->row_num;
	int col_max = MINE_CGI->col_num;

	/*先将整个数组表示被掩盖的情况*/
	for (int i = 0; i <= row_max - 1; i++)
		for (int j = 0; j <= col_max - 1; j++)
			if (inbase[i][j] != TAB_NOT_SIGN)
				inbase[i][j] = COVER_SIGN;//即均显示为COVER_SIGN


	/*第一个点开位置必须为BLANK！！！*/
	int row_m = st_row + 1;
	int row_n = st_row - 1;
	int col_m = st_col + 1;
	int col_n = st_col - 1;


	/*进行雷的赋值*/
	srand(unsigned(time(NULL)));//取当下的系统时间为种子的值
	int i, x, y;
	for (i = 1; i <= num; i++)//随机生成雷
	{
		while (1)
		{
			x = random(row_max);
			y = random(col_max);//被掩盖的情况时才进行赋值为雷
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

	inbase[st_row][st_col] = BLANK;//首位置为BLANK


	return;

}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：根据点开的位置进行扩散
***************************************************************************/
void spread(CONSOLE_GRAPHICS_INFO* const MINE_CGI, int inbase[ROW_MAX][COL_MAX], int row, int col)
{
	/*定义该模式下的数组大小*/
	int row_max = MINE_CGI->row_num;
	int col_max = MINE_CGI->col_num;

	/*定义八个方向：东南 南 西南 东 西 西北 西 东北 */
	int row_move[8] = { 1,1,1,0,0,-1,-1,-1 };
	int col_move[8] = { 1,0,-1,1,-1,-1,0,1 };
	int i, mines = 0;
	for (i = 0; i <= 7; i++) {//统计8个方向上的雷的总数
		int next_row = row + row_move[i];
		int next_col = col + col_move[i];
		if (next_row >= 0 && next_row <= (row_max - 1) && next_col >= 0 && next_col <= (col_max - 1))//长度和宽度代表的不是所处数组的边界，需要减1！！！
			if (inbase[next_row][next_col] == MINE_SIGN || inbase[next_row][next_col] == TAB_MINE_SIGN)//注意MINE_SIGN和TAB_MINE_SIGN都是雷！！！
				mines++;
	}

	/*到达边界或到达所有非0但不是雷的位置结束遍历*/
	if (mines == 0) {//继续向周围移动，
		inbase[row][col] = BLANK;
		for (i = 0; i <= 7; i++) {
			int next_row = row + row_move[i];
			int next_col = col + col_move[i];
			if (next_row >= 0 && next_row <= (row_max - 1) && next_col >= 0 && next_col <= (col_max - 1))//长度和宽度代表的不是所处数组的边界，需要减1！！！
				if (inbase[next_row][next_col] == COVER_SIGN)//继续遍历未被遍历的位置且必须要是未被标记的地方才遍历！！！标记的地方不能扩散
					spread(MINE_CGI, inbase, next_row, next_col);//函数传递的是位置坐标（从0开始！！！）
		}
	}
	else {
		inbase[row][col] = mines;
		return;
	}

}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：判断游戏是否成功
***************************************************************************/
int judge_win(CONSOLE_GRAPHICS_INFO* const MINE_CGI, const int inbase[ROW_MAX][COL_MAX])
{

	int row_max = MINE_CGI->row_num;
	int col_max = MINE_CGI->col_num;

	/*遍历判断是否成功*/
	int row_move[8] = { 1,1,1,0,0,-1,-1,-1 };
	int col_move[8] = { 1,0,-1,1,-1,-1,0,1 };
	for (int i = 0; i <= (row_max - 1); i++)//给未赋值雷的位置赋值周围的雷数
	{
		for (int j = 0; j <= (col_max - 1); j++)
		{
			if (inbase[i][j] == MINE_SIGN || inbase[i][j] == TAB_MINE_SIGN)
				;
			else {
				int mines = 0;
				for (int k = 0; k <= 7; k++)//统计8个方向上的雷的总数
				{
					int next_row = i + row_move[k];
					int next_col = j + col_move[k];
					if (next_row >= 0 && next_row <= (row_max - 1) && next_col >= 0 && next_col <= (col_max - 1))
						if (inbase[next_row][next_col] == MINE_SIGN || inbase[next_row][next_col] == TAB_MINE_SIGN)
							mines++;
				}
				if (inbase[i][j] != mines)
					return 0;//如果未完全点开 表示未成功！！！
			}
		}
	}
	return 1;//已经成功

}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：根据标记的值 进行计算雷数
***************************************************************************/
void mine_count(CONSOLE_GRAPHICS_INFO* const MINE_CGI, const int inbase[ROW_MAX][COL_MAX], int mine_num)
{
	char cat_top[64];
	char nor_top[64] = "按ESC退出，空格显示时间";
	int num = 0;
	for (int i = 0; i < MINE_CGI->row_num; i++)
		for (int j = 0; j < MINE_CGI->col_num; j++)
			if (inbase[i][j] == TAB_MINE_SIGN ||
				inbase[i][j] == TAB_NOT_SIGN)
				num++;
	sprintf(cat_top, "剩余雷数： %d ", (mine_num - num));

	/*在上状态栏显示剩余雷数*/
	gmw_status_line(MINE_CGI, TOP_STATUS_LINE, nor_top, cat_top);

}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：cosnt bool win_los 为1 表示成功 ; 为0表示失败
			(默认win_los为失败0)
***************************************************************************/
void time_out(CONSOLE_GRAPHICS_INFO* const MINE_CGI, const clock_t start_time, const int top_lower, const bool win_los)
{

	/*定义需要打印的值*/
	char cat_temp[64];
	char nor_temp_mid[64] = "按ESC退出，空格显示时间";
	char nor_temp_los[64] = "你输了，游戏结束！！！";
	char nor_temp_win[64] = "恭喜你，你赢啦啦啦啦啦！！！";

	double time = ((double)(clock()) - (double)(start_time)) / CLOCKS_PER_SEC;
	switch (top_lower)
	{
		case TOP_STATUS_LINE:
			sprintf(cat_temp, "当前时间：%.6f秒 ", time);
			gmw_status_line(MINE_CGI, TOP_STATUS_LINE, nor_temp_mid, cat_temp);//打印上状态栏的字符串
			break;

		case LOWER_STATUS_LINE:

			if (win_los) {//成功！！！
				sprintf(cat_temp, "共用时：%.6f秒 ", time);
				gmw_status_line(MINE_CGI, LOWER_STATUS_LINE, nor_temp_win, cat_temp);//打印上状态栏的字符串
			}
			else {//失败！！！
				sprintf(cat_temp, "共用时：%.6f秒 ", time);
				gmw_status_line(MINE_CGI, LOWER_STATUS_LINE, nor_temp_los, cat_temp);//打印上状态栏的字符串
			}
			break;

		default://此时不存在其他情况
			break;

	}

	return;

}

