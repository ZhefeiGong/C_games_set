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
	存放内部实现函数
   ----------------------------------------- */


/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：首图形界面
***************************************************************************/
int First(void)
{
	cct_cls();//清屏
	cct_setconsoleborder(50, 15, 50, 15);//初始化界面
	cct_setfontsize("新宋体", 32, 14);
	cct_setcursor(CURSOR_VISIBLE_NORMAL);
	system("color D9");//初始化背景颜色
	cct_setcolor(COLOR_HPINK, COLOR_HGREEN);
	cct_gotoxy(14, 5);
	cout << "╔════════╗";
	cct_gotoxy(14, 6);
	cout << "║欢迎来到2048游戏║";
	cct_gotoxy(14, 7);
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
  说    明：输入游戏参数
***************************************************************************/
void level(CONSOLE_GRAPHICS_INFO* const  M2048_CGI,int &score)
{
	cct_cls();//清屏
	cct_setconsoleborder(50, 15, 50, 15);//初始化界面
	system("color 07");//初始化背景颜色
	cct_setcolor();//默认颜色设置
	cct_setfontsize("新宋体", 38, 16);//设置字体

	int tem_row, tem_col;
	int delay;
	int time = 0;

	cout << "游戏2048参数设置：" << endl;

	/*输入行数*/
	cout << "请输入行数[4-8]";
	int x_1, y_1;
	cct_getxy(x_1, y_1);
	while (1){
		cin >> tem_row;
		if (!cin) {//如果输入错误
			cin.clear();
			cin.ignore(1024, '\n');//清除错误标记并清空缓冲区(1024或者遇到换行符为止)
			cct_gotoxy(x_1, y_1);
			cout << "                    ";//清除刚才的输入数据
			cct_gotoxy(x_1, y_1);
		}
		else {//如果输入正确
			if (tem_row<ROW_MIN || tem_row>ROW_MAX) {
				cin.ignore(1024, '\n');//清除错误标记并清空缓冲区(1024或者遇到换行符为止)
				cct_gotoxy(x_1, y_1);
				cout << "                    ";//清除刚才的输入数据
				cct_gotoxy(x_1, y_1);
			}
			else
				break;//输入正确跳出循环
		}
	}

	/*输入列数*/
	cout << "请输入列数[4-10]";
	int x_2, y_2;
	cct_getxy(x_2, y_2);
	while (1) {
		cin >> tem_col;
		if (!cin) {//如果输入错误
			cin.clear();
			cin.ignore(1024, '\n');//清除错误标记并清空缓冲区(1024或者遇到换行符为止)
			cct_gotoxy(x_2, y_2);
			cout << "                    ";//清除刚才的输入数据
			cct_gotoxy(x_2, y_2);
		}
		else {//如果输入正确
			if (tem_col<COL_MIN || tem_col>COL_MAX) {
				cin.ignore(1024, '\n');//清除错误标记并清空缓冲区(1024或者遇到换行符为止)
				cct_gotoxy(x_2, y_2);
				cout << "                    ";//清除刚才的输入数据
				cct_gotoxy(x_2, y_2);
			}
			else
				break;//输入正确跳出循环
		}
	}

	/*输入目标分数*/
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
	cout << "请输入目标分数[" << score_1 << '/' << score_2 << '/' << score_3 << '/' << score_4 << ']';
	int x_3, y_3;
	cct_getxy(x_3, y_3);
	while (1) {
		cin >> score;
		if (!cin) {//如果输入错误
			cin.clear();
			cin.ignore(1024, '\n');//清除错误标记并清空缓冲区(1024或者遇到换行符为止)
			cct_gotoxy(x_3, y_3);
			cout << "                    ";//清除刚才的输入数据
			cct_gotoxy(x_3, y_3);
		}
		else {//如果输入正确
			if (score != score_1 && score != score_2 && score != score_3 && score != score_4) {
				cin.ignore(1024, '\n');//清除错误标记并清空缓冲区(1024或者遇到换行符为止)
				cct_gotoxy(x_3, y_3);
				cout << "                    ";//清除刚才的输入数据
				cct_gotoxy(x_3, y_3);
			}
			else
				break;//输入正确跳出循环
		}
	}

	/*输入动画延时*/
	cout << "请输入动画延时[0-5]";
	int x_4, y_4;
	cct_getxy(x_4, y_4);
	while (1) {
		cin >> delay;
		if (!cin) {//如果输入错误
			cin.clear();
			cin.ignore(1024, '\n');//清除错误标记并清空缓冲区(1024或者遇到换行符为止)
			cct_gotoxy(x_4, y_4);
			cout << "                    ";//清除刚才的输入数据
			cct_gotoxy(x_4, y_4);
		}
		else {//如果输入正确
			if (delay < 0 || delay>5) {
				cin.ignore(1024, '\n');//清除错误标记并清空缓冲区(1024或者遇到换行符为止)
				cct_gotoxy(x_4, y_4);
				cout << "                    ";//清除刚才的输入数据
				cct_gotoxy(x_4, y_4);
			}
			else
				break;//输入正确跳出循环
		}
	}

	/*设置行列*/
	gmw_set_rowcol(M2048_CGI, tem_row, tem_col);//进行行列的初始化

	/*设置延时*/
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
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：初始化2048
***************************************************************************/
void my2048_start(CONSOLE_GRAPHICS_INFO* const  M2048_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi,const int score_goal,const int score_highest)
{
	cct_cls();//清屏

	gmw_set_frame_default_linetype(M2048_CGI, 1);                      //全双
	gmw_set_frame_color(M2048_CGI, COLOR_HWHITE, COLOR_BLACK);         //设置框架颜色
	gmw_set_status_line_switch(M2048_CGI, TOP_STATUS_LINE, true);      //开启上状态栏
	gmw_set_status_line_switch(M2048_CGI, LOWER_STATUS_LINE, true);   //关闭下状态栏
	gmw_set_font(M2048_CGI, "点阵字体", 16, 8);                        //设置字体
	gmw_set_frame_style(M2048_CGI, BLOCK_WIDTH, BLOCK_HIGH, true);     //需要边框
	gmw_set_block_border_switch(M2048_CGI, true);                      //色块内需要边框
	gmw_set_rowno_switch(M2048_CGI, false);                            //无需行号
	gmw_set_colno_switch(M2048_CGI, false);                            //无需列标

	cct_setcursor(CURSOR_INVISIBLE);                                   //光标不显示

	/*打印框架*/
	gmw_draw_frame(M2048_CGI);

	/*打印上显示栏信息*/
	char temp[128];
	sprintf(temp, "目标:%d 分数:0 最高:%d 时间:0 (R:重玩 Q:退出)", score_goal, score_highest);
	gmw_status_line(M2048_CGI, TOP_STATUS_LINE, temp);
	
	/*初始化 在界面上随机生成两个2*/
	srand(unsigned(time(NULL)));//取当下的系统时间为种子的值
	for (int i = 1; i <= 2; i++) {
		while (1)
		{
			int row_mid = random(M2048_CGI->row_num);//随机生成0-（M2048_CGI->row_num-1）的数字
			int col_mid = random(M2048_CGI->col_num);//随机生成0-（M2048_CGI->col_num-1）的数字
			if (inbase[row_mid][col_mid] == BLANK) {
				inbase[row_mid][col_mid] = 2;
				break;
			}
			else
				continue;
		}
	}

	/*打印色块们*/
	for (int j = 0; j < M2048_CGI->row_num; j++)
		for (int i = 0; i < M2048_CGI->col_num; i++)
			gmw_draw_block(M2048_CGI, j, i, inbase[j][i], bdi);

	return;

}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：返回 0 退出游戏 返回游戏界面
			返回 1 重新玩游戏
			返回 2 游戏结束
***************************************************************************/
int my2048_mid(CONSOLE_GRAPHICS_INFO* const  M2048_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi, const int score_goal, const int score_highest, int& score)
{

	/*初始化参数*/
	int maction, mrow = -1, mcol = -1;
	int keycode1, keycode2;
	int ret;

	int start_time = (int)time(0);

	while (1) {
		ret = gmw_read_keyboard_and_mouse(M2048_CGI, maction, mrow, mcol, keycode1, keycode2);//仅考虑键盘事件

		/*键盘事件*/
		if(ret==CCT_KEYBOARD_EVENT){
			if (keycode1 == 0xE0) { //224 按下的为键盘键
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
				if (judge_win(M2048_CGI, inbase))//此时游戏结束
					return 2;
				else
					creat(M2048_CGI, inbase, bdi);//游戏继续
			}
			else if (keycode1 == 'R' || keycode1 == 'r')
				return 1;
			else if (keycode1 == 'Q' || keycode1 == 'q')
				return 0;
		}//end of if
	}//end of while



}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：返回 1 游戏重玩
            返回 0 游戏回到开始界面
***************************************************************************/
int my2048_end(CONSOLE_GRAPHICS_INFO* const  M2048_CGI)
{
	int x = M2048_CGI->start_x + (M2048_CGI->col_num / 2) * M2048_CGI->CFI.block_width - M2048_CGI->CFI.block_width / 2;
	int y = M2048_CGI->start_y + (M2048_CGI->row_num / 2) * M2048_CGI->CFI.block_high - M2048_CGI->CFI.block_high / 2;
	cct_setcolor(COLOR_HRED, COLOR_HWHITE);
	cct_gotoxy(x, y);
	cout << "╔════╗";
	cct_gotoxy(x, y+1);
	cout << "║游戏失败║";
	cct_gotoxy(x, y + 2);
	cout << "║R：再玩 ║";
	cct_gotoxy(x, y + 3);
	cout << "║Q：退出 ║";
	cct_gotoxy(x, y + 4);
	cout << "╚════╝";

	cct_setcursor(CURSOR_INVISIBLE);//使光标消失

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
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：打印状态栏信息
***************************************************************************/
void status_out(CONSOLE_GRAPHICS_INFO* const  M2048_CGI, int score_now,int score_goal, int score_highest, int start_time)
{
	int time_gap = (int)time(0) - start_time;
	char temp[128];
	sprintf(temp, "目标:%d 分数:%d 最高:%d 时间:%d (R:重玩 Q:退出)", score_goal, score_now, score_highest, time_gap);
	gmw_status_line(M2048_CGI, TOP_STATUS_LINE, temp);

	return;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：随机出现一个2/4
***************************************************************************/
void creat(CONSOLE_GRAPHICS_INFO* const  M2048_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi)
{
	/*确立横纵最大值*/
	int row_max = M2048_CGI->row_num;
	int col_max = M2048_CGI->col_num;

	int go_on = 0;

	for (int row = 0; row < row_max; row++)
		for (int col = 0; col < col_max; col++)
			if (inbase[row][col] == BLANK)
				go_on = 1;


	if (go_on)//存在白块才产生
	{
		/*计算产生啥*/
		srand(unsigned(time(NULL)));//取当下的系统时间为种子的值
		int creat_block;
		int ran = random(100);
		if (ran >= 0 && ran <= 10)//1:9的产生概率
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
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：返回 1 游戏结束
            返回 0 游戏继续
***************************************************************************/
int judge_win(CONSOLE_GRAPHICS_INFO* const  M2048_CGI, int inbase[ROW_MAX][COL_MAX])
{
	/*定义四个方向：东 南 西 北  */
	int row_move[DIRECTION] = { 1,0,-1, 0 };
	int col_move[DIRECTION] = { 0,1, 0,-1 };

	int row_max = M2048_CGI->row_num;
	int col_max = M2048_CGI->col_num;

	int go = 0;

	/*遍历整个数组内的数据(数字周围无相同数字)*/
	for (int row = 0; row < row_max; row++)
	{
		for (int col = 0; col < col_max; col++)
		{

			if (inbase[row][col] > 0)//仅当为数字时才开始遍历 为BLANK(0)时不遍历
				for (int k = 0; k < DIRECTION; k++)
				{
					int row_next = row + row_move[k];
					int col_next = col + col_move[k];
					if (row_next >= 0 && row_next < row_max && col_next >= 0 && col_next < col_max)//在范围内
					{
						if (inbase[row][col] == inbase[row_next][col_next])//存在周围相同可消除项
							return 0;
					}
				}
		}
	}

	/*数组中是否有空白位置*/
	for (int j = 0; j < row_max; j++)
		for (int i = 0; i < col_max; i++)
			if (inbase[j][i] == BLANK)//存在空白位置
				return 0;


	return 1;//此时游戏结束
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：根据输入键进行界面色块的移动以及内部数组的变化
            此函数为本游戏的核心代码！
***************************************************************************/
int move_out(CONSOLE_GRAPHICS_INFO* const  M2048_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi, int type)
{
	
	/*确立横纵最大值*/
	int row_max = M2048_CGI->row_num;
	int col_max = M2048_CGI->col_num;

	/*进行冒泡处理*/
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
			 /*计算变化后色块需要向左移动的距离*/
			for (int row = 0; row < row_max; row++)
			{
				for (int col = (col_max-1); col >=0; col--)
				{
					int num = 0;
					if (inbase[row][col] != BLANK)//计算左方向有多少BLANK 便于移动
					{
						for (int tem_col = (col - 1); tem_col >=0; tem_col--)
							if (inbase[row][tem_col] == BLANK)
								num++;

					}
					move_blank_base_1[row][col] = num;
				}

			}//end of for col

			/*向左移动*/
			for (int row = 0; row < row_max; row++)
				for (int col = 0; col < col_max; col++)//从左往右落!!!!!
					if(move_blank_base_1[row][col]!=0)
					gmw_move_block(M2048_CGI, row, col, inbase[row][col], BLANK, bdi, RIGHT_TO_LEFT, move_blank_base_1[row][col]);

			/*改变内部数组*/
			for (int row = 0; row < row_max; row++)//空白块是向右边冒泡
			{
				for (int col = (col_max - 1); col >= 0; col--)
				{
					if (inbase[row][col] == BLANK)
					{
						for (int tem_col = col; tem_col <(col_max-1); tem_col++)//将0向右冒泡
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
			/*计算变化后色块需要向右移动的距离*/
			for (int row = 0; row < row_max; row++)
			{
				for (int col = 0; col < col_max; col++)
				{
					int num = 0;
					if (inbase[row][col] != BLANK)//计算右方向有多少BLANK 便于移动
					{
						for (int tem_col = (col + 1); tem_col <col_max; tem_col++)
							if (inbase[row][tem_col] == BLANK)
								num++;

					}
					move_blank_base_1[row][col] = num;
				}

			}//end of for col

			/*向右移动*/
			for (int row = 0; row < row_max; row++)
				for (int col = (col_max-1); col >=0; col--)//从右往左落!!!!!
					if (move_blank_base_1[row][col] != 0)
					gmw_move_block(M2048_CGI, row, col, inbase[row][col], BLANK, bdi, LEFT_TO_RIGHT, move_blank_base_1[row][col]);

			/*改变内部数组*/
			for (int row = 0; row < row_max; row++)//空白块是向左边冒泡
			{
				for (int col = 0; col < col_max; col++)
				{
					if (inbase[row][col] == BLANK)
					{
						for (int tem_col = col; tem_col > 0; tem_col--)//将0向左冒泡
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
			/*计算变化后色块需要向下移动的距离*/
			for (int col = 0; col < col_max; col++)
			{
				for (int row = 0; row < row_max; row++)
				{
					int num = 0;
					if (inbase[row][col] != BLANK)//计算下方有多少BLANK 便于移动
					{
						for (int tem_row = (row + 1); tem_row < row_max; tem_row++)
							if (inbase[tem_row][col] == BLANK)
								num++;

					}
					move_blank_base_1[row][col] = num;
				}

			}//end of for col

			/*下落移动*/
			for (int col = 0; col < col_max; col++)
				for (int row = row_max - 1; row >= 0; row--)//从下往上落!!!!!
					if (move_blank_base_1[row][col] != 0)
					gmw_move_block(M2048_CGI, row, col, inbase[row][col], BLANK, bdi, UP_TO_DOWN, move_blank_base_1[row][col]);

			/*改变内部数组*/
			for (int col = 0; col < col_max; col++)//空白块向上冒泡
			{
				for (int row = 0; row < row_max; row++)
				{
					if (inbase[row][col] == BLANK)
					{
						for (int tem_row = row; tem_row >= 1; tem_row--)//将0向上冒泡
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
			/*计算变化后色块需要向上移动的距离*/
			for (int col = 0; col < col_max; col++)
			{
				for (int row = (row_max - 1); row >= 0; row--)
				{
					int num = 0;
					if (inbase[row][col] != BLANK)//计算上方有多少BLANK 便于移动
					{
						for (int tem_row = (row - 1); tem_row >= 0; tem_row--)
							if (inbase[tem_row][col] == BLANK)
								num++;

					}
					move_blank_base_1[row][col] = num;
				}

			}//end of for col

			/*上落移动*/
			for (int col = 0; col < col_max; col++)
				for (int row =0; row <row_max; row++)//从下往上落!!!!!
					if (move_blank_base_1[row][col] != 0)
					gmw_move_block(M2048_CGI, row, col, inbase[row][col], BLANK, bdi, DOWN_TO_UP, move_blank_base_1[row][col]);

			/*改变内部数组*/
			for (int col = 0; col < col_max; col++)//空白块向下冒泡
			{
				for (int row =(row_max-1); row >=0; row--)
				{
					if (inbase[row][col] == BLANK)
					{
						for (int tem_row = row; tem_row <(row_max-1); tem_row++)//将0向下冒泡
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



    /*进行该方向的抵消*/
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
		case R_L://从右到左
			/*内部数组的变化*/
			for (int row = 0; row < row_max; row++)
				for (int col = col_max - 1; col > 0; col--)
				{
					if ((inbase[row][col] == inbase[row][col - 1]) && (inbase[row][col] != BLANK)) {//如果相等
						change_base[row][col] = inbase[row][col];
						inbase[row][col - 1] = inbase[row][col - 1] + inbase[row][col];
						inbase[row][col] = BLANK;
						move_base[row][col] = 1;
						change_base[row][col - 1] = inbase[row][col - 1];
						col--;//跳过下一个
					}
				}
			/*图形界面的向左移动*/
			for (int row = 0; row < row_max; row++)//左边先落
				for (int col = 0; col < row_max; col++)
					if (move_base[row][col] != BLANK) {
						gmw_move_block(M2048_CGI, row, col, change_base[row][col], BLANK, bdi, RIGHT_TO_LEFT, move_base[row][col]);
						gmw_draw_block(M2048_CGI, row, (col - 1), change_base[row][col - 1], bdi);//画一个色块
					}
			break;

		case L_R://从左到右
			/*内部数组的变化*/
			for (int row = 0; row < row_max; row++)
				for (int col = 0; col < (col_max - 1); col++)
				{
					if ((inbase[row][col] == inbase[row][col + 1]) && (inbase[row][col] != BLANK)) {//如果相等 仅移动一格的位置
						change_base[row][col] = inbase[row][col];
						inbase[row][col + 1] = inbase[row][col + 1] + inbase[row][col];
						inbase[row][col] = BLANK;
						move_base[row][col] = 1;
						change_base[row][col + 1] = inbase[row][col + 1];
						col++;//跳过下一个
					}
				}
			/*图形界面的向右移动*/
			for (int row = 0; row < row_max; row++)
				for (int col =(col_max-1); col >=0; col--)//右边先落
					if (move_base[row][col] != BLANK) {
						gmw_move_block(M2048_CGI, row, col, change_base[row][col], BLANK, bdi, LEFT_TO_RIGHT, move_base[row][col]);
						gmw_draw_block(M2048_CGI, row, (col + 1), change_base[row][col + 1], bdi);//画一个色块
					}
			break;

		case U_D://从上到下
			/*内部数组的变化*/
			for (int col = 0; col < col_max; col++)
				for (int row = 0; row < (row_max - 1); row++)
				{
					if ((inbase[row][col] == inbase[row + 1][col]) && (inbase[row][col] != BLANK)) {//如果相等 仅移动一格的位置
						change_base[row][col] = inbase[row][col];
						inbase[row + 1][col] = inbase[row + 1][col] + inbase[row][col];
						inbase[row][col] = BLANK;
						move_base[row][col] = 1;
						change_base[row + 1][col] = inbase[row + 1][col];
						row++;//跳过下一个
					}
				}
			/*图形界面的向下移动*/
			for (int col = 0; col < col_max; col++)
				for (int row = (row_max - 1); row >= 0; row--)//下方先落
					if (move_base[row][col] != BLANK) {
						gmw_move_block(M2048_CGI, row, col, change_base[row][col], BLANK, bdi, UP_TO_DOWN, move_base[row][col]);
						gmw_draw_block(M2048_CGI, (row + 1), col, change_base[row + 1][col], bdi);//画一个色块
					}
			break;

		case D_U://从下到上
			/*内部数组的变化*/
			for (int col = 0; col < col_max; col++)
				for (int row = (row_max - 1); row > 0; row--)
				{
					if ((inbase[row][col] == inbase[row- 1][col]) && (inbase[row][col] != BLANK)) {//如果相等 仅移动一格的位置
						change_base[row][col] = inbase[row][col];
						inbase[row - 1][col] = inbase[row - 1][col] + inbase[row][col];
						inbase[row][col] = BLANK;
						move_base[row][col] = 1;
						change_base[row - 1][col] = inbase[row - 1][col];
						row--;//跳过下一个
					}
				}
			/*图形界面的向下移动*/
			for (int col = 0; col < col_max; col++)
				for (int row = 0; row <row_max; row++)//上方先落
					if (move_base[row][col] != BLANK) {
						gmw_move_block(M2048_CGI, row, col, change_base[row][col], BLANK, bdi, DOWN_TO_UP, move_base[row][col]);
						gmw_draw_block(M2048_CGI, (row - 1), col, change_base[row - 1][col], bdi);//画一个色块
					}
			break;

		default:
			break;
	}


	/*进行冒泡处理*/
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
			/*计算变化后色块需要向左移动的距离*/
			for (int row = 0; row < row_max; row++)
			{
				for (int col = (col_max - 1); col >= 0; col--)
				{
					int num = 0;
					if (inbase[row][col] != BLANK )//计算左方向有多少BLANK 便于移动
					{
						for (int tem_col = (col - 1); tem_col >= 0; tem_col--)
							if (inbase[row][tem_col] == BLANK)
								num++;

					}
					move_blank_base_2[row][col] = num;
				}

			}//end of for col

			/*向左移动*/
			for (int row = 0; row < row_max; row++)
				for (int col = 0; col < col_max; col++)//从左往右落!!!!!
					if (move_blank_base_2[row][col] != 0)
						gmw_move_block(M2048_CGI, row, col, inbase[row][col], BLANK, bdi, RIGHT_TO_LEFT, move_blank_base_2[row][col]);

			/*改变内部数组*/
			for (int row = 0; row < row_max; row++)//空白块是向右边冒泡
			{
				for (int col = (col_max - 1); col >= 0; col--)
				{
					if (inbase[row][col] == BLANK)
					{
						for (int tem_col = col; tem_col < (col_max - 1); tem_col++)//将0向右冒泡
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
			/*计算变化后色块需要向右移动的距离*/
			for (int row = 0; row < row_max; row++)
			{
				for (int col = 0; col < col_max; col++)
				{
					int num = 0;
					if (inbase[row][col] != BLANK)//计算右方向有多少BLANK 便于移动
					{
						for (int tem_col = (col + 1); tem_col < col_max; tem_col++)
							if (inbase[row][tem_col] == BLANK)
								num++;

					}
					move_blank_base_2[row][col] = num;
				}

			}//end of for col

			/*向右移动*/
			for (int row = 0; row < row_max; row++)
				for (int col = (col_max - 1); col >= 0; col--)//从右往左落!!!!!
					if (move_blank_base_2[row][col] != 0)
						gmw_move_block(M2048_CGI, row, col, inbase[row][col], BLANK, bdi, LEFT_TO_RIGHT, move_blank_base_2[row][col]);

			/*改变内部数组*/
			for (int row = 0; row < row_max; row++)//空白块是向左边冒泡
			{
				for (int col = 0; col < col_max; col++)
				{
					if (inbase[row][col] == BLANK)
					{
						for (int tem_col = col; tem_col > 0; tem_col--)//将0向左冒泡
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
			/*计算变化后色块需要向下移动的距离*/
			for (int col = 0; col < col_max; col++)
			{
				for (int row = 0; row < row_max; row++)
				{
					int num = 0;
					if (inbase[row][col] != BLANK)//计算下方有多少BLANK 便于移动
					{
						for (int tem_row = (row + 1); tem_row < row_max; tem_row++)
							if (inbase[tem_row][col] == BLANK)
								num++;

					}
					move_blank_base_2[row][col] = num;
				}

			}//end of for col

			/*下落移动*/
			for (int col = 0; col < col_max; col++)
				for (int row = row_max - 1; row >= 0; row--)//从下往上落!!!!!
					if (move_blank_base_2[row][col] != 0)
						gmw_move_block(M2048_CGI, row, col, inbase[row][col], BLANK, bdi, UP_TO_DOWN, move_blank_base_2[row][col]);

			/*改变内部数组*/
			for (int col = 0; col < col_max; col++)//空白块向上冒泡
			{
				for (int row = 0; row < row_max; row++)
				{
					if (inbase[row][col] == BLANK)
					{
						for (int tem_row = row; tem_row >= 1; tem_row--)//将0向上冒泡
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
			/*计算变化后色块需要向上移动的距离*/
			for (int col = 0; col < col_max; col++)
			{
				for (int row = (row_max - 1); row >= 0; row--)
				{
					int num = 0;
					if (inbase[row][col] != BLANK)//计算上方有多少BLANK 便于移动
					{
						for (int tem_row = (row - 1); tem_row >= 0; tem_row--)
							if (inbase[tem_row][col] == BLANK)
								num++;

					}
					move_blank_base_2[row][col] = num;
				}

			}//end of for col

			/*上落移动*/
			for (int col = 0; col < col_max; col++)
				for (int row = 0; row < row_max; row++)//从下往上落!!!!!
					if (move_blank_base_2[row][col] != 0)
						gmw_move_block(M2048_CGI, row, col, inbase[row][col], BLANK, bdi, DOWN_TO_UP, move_blank_base_2[row][col]);

			/*改变内部数组*/
			for (int col = 0; col < col_max; col++)//空白块向下冒泡
			{
				for (int row = (row_max - 1); row >= 0; row--)
				{
					if (inbase[row][col] == BLANK)
					{
						for (int tem_row = row; tem_row < (row_max - 1); tem_row++)//将0向下冒泡
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

	return score;//返回此次的得分


}




