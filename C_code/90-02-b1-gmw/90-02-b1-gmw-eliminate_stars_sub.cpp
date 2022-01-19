/* 2052532 龚哲飞 信04 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "../include/cmd_console_tools.h"        //包含命令行工具函数
#include "../include/cmd_gmw_tools.h"            //包含伪图形工具函数
#include "../include/common_output.h"            //包含自己写的工具函数
#include "90-02-b1-gmw-eliminate_stars.h"        //包含本项目的头文件

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
  说    明：返回 1 继续
			返回 0 退出
***************************************************************************/
int First(void)
{
	cct_cls();//清屏
	cct_setconsoleborder(50, 15, 50, 15);//初始化界面
	cct_setfontsize("新宋体", 32, 14);
	cct_setcursor(CURSOR_VISIBLE_NORMAL);
	system("color B9");//初始化背景颜色
	cct_setcolor(COLOR_HCYAN, COLOR_HRED);
	cct_gotoxy(12, 5);
	cout << "╔══════════╗";
	cct_gotoxy(12, 6);
	cout << "║欢迎来到消灭星星游戏║";
	cct_gotoxy(12, 7);
	cout << "╚══════════╝";
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
  说    明：输入需要打印的行列
***************************************************************************/
void rowcol(CONSOLE_GRAPHICS_INFO* const  STAR_CGI)
{
	cct_cls();//清屏

	/*初始化需要输入的值*/
	int tem_row, tem_col;

	/*输入行*/
	cout << "请输入行数(8-10)： ";
	int x_1, y_1;
	cct_getxy(x_1, y_1);
	while (1)
	{
		cin >> tem_row;
		if (!cin)//如果输入错误
		{
			cin.clear();
			cin.ignore(1024, '\n');//清除错误标记并清空缓冲区(1024或者遇到换行符为止)
			cout << "输入不合法，请重新输入";
			cct_gotoxy(x_1, y_1);//回到重新输入位置
			cout << "                      ";//清除刚才的输入数据
			cct_gotoxy(x_1, y_1);//回到重新输入位置
		}
		else
		{
			if (tem_row < ROW_MIN || tem_row>ROW_MAX)//此时尽管输入没报错，但也输入不正确
			{
				cin.ignore(1024, '\n');//清除错误标记并清空缓冲区(1024或者遇到换行符为止)
				cout << "输入不合法，请重新输入";
				cct_gotoxy(x_1, y_1);//回到重新输入位置
				cout << "                      ";//清除刚才的输入数据
				cct_gotoxy(x_1, y_1);//回到重新输入位置
			}
			else
			{
				cout << "                      ";//遮盖之前的“输入不合法”提示
				cct_gotoxy(0, (y_1 + 1));//进入下一个输入位置
				break;//输入正确跳出while循环
			}
		}
	}//end of while

	/*输入列*/
	cout << "请输入列数(8-10)： ";
	int x_2, y_2;
	cct_getxy(x_2, y_2);
	while (1)
	{
		cin >> tem_col;
		if (!cin)//如果输入错误
		{
			cin.clear();
			cin.ignore(1024, '\n');//清除错误标记并清空缓冲区(1024或者遇到换行符为止)
			cout << "输入不合法，请重新输入";
			cct_gotoxy(x_2, y_2);//回到重新输入位置
			cout << "                       ";//清除刚才的输入数据
			cct_gotoxy(x_2, y_2);//回到重新输入位置
		}
		else
		{
			if (tem_col < COL_MIN || tem_col>COL_MAX)//此时尽管输入没报错，但也输入不合法
			{
				cin.ignore(1024, '\n');//清除错误标记并清空缓冲区(1024或者遇到换行符为止)
				cout << "输入不合法，请重新输入";
				cct_gotoxy(x_2, y_2);//回到重新输入位置
				cout << "                      ";//清除刚才的输入数据
				cct_gotoxy(x_2, y_2);//回到重新输入位置
			}
			else
			{
				cout << "                      ";//遮盖之前的“输入不合法”提示
				cct_gotoxy(0, (y_2 + 1));
				break;
			}
		}
	}//end of while

	/*进行所需行列的初始化*/
	gmw_set_rowcol(STAR_CGI, tem_row, tem_col);

	return;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：消灭星星的初始化函数
***************************************************************************/
void popstar_start(CONSOLE_GRAPHICS_INFO* const  STAR_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi)
{
	cct_cls();//清屏

	/*进行初始化*/
	gmw_set_frame_color(STAR_CGI, COLOR_HWHITE, COLOR_BLACK);        //设置框架颜色
	gmw_set_font(STAR_CGI, "点阵字体", 16, 8);                       //设置字体
	gmw_set_status_line_switch(STAR_CGI, TOP_STATUS_LINE, true);     //开启上状态栏
	gmw_set_status_line_switch(STAR_CGI, LOWER_STATUS_LINE, true);   //开启下状态栏
	gmw_set_frame_style(STAR_CGI, BLOCK_WIDTH, BLOCK_HIGH, true);    //需要边框
	gmw_set_block_border_switch(STAR_CGI, true);                     //色块内需要边框
	gmw_set_rowno_switch(STAR_CGI, true);                            //需要行号
	gmw_set_colno_switch(STAR_CGI, true);                            //需要列标

	/*设置画框架的延时*/
	STAR_CGI->delay_of_draw_frame = DELAY_FRAME;
	STAR_CGI->delay_of_block_moved = DELAY_MOVE;

	cct_setcursor(CURSOR_INVISIBLE);                                //光标不显示

	/*打印框架*/
	gmw_draw_frame(STAR_CGI);                           

	/*打印上状态栏初始信息*/
	char temp[64];
	sprintf(temp, "屏幕当前设置为：%d行%d列", STAR_CGI->lines, STAR_CGI->cols);
	gmw_status_line(STAR_CGI, TOP_STATUS_LINE, temp);

	

	/*将内部数组随机初始化*/
	srand(unsigned(time(NULL)));//取当下的系统时间为种子的值
	for (int j = 0; j < STAR_CGI->row_num; j++)
		for (int i = 0; i < STAR_CGI->col_num; i++)
		{
			while (1)
			{
				int star = random(6);//此时随机生成0-5的数字
				if (star >= 1 && star <= 5)//只能赋值为1-5
				{
					inbase[j][i] = star;//赋值为字符1-5！！！
					break;
				}
			}
		}

	/*打印色块们*/
	for (int j = 0; j < STAR_CGI->row_num; j++)
		for (int i = 0; i < STAR_CGI->col_num; i++)
			gmw_draw_block(STAR_CGI, j, i, inbase[j][i], bdi);

	return;

}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：消灭星星的主体实现函数
***************************************************************************/
void popstar_mid(CONSOLE_GRAPHICS_INFO* const  STAR_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi,int &score)
{

	/*初始化参数*/
	int maction, old_mrow, old_mcol, mrow = -1, mcol = -1;
	int keycode1, keycode2;
	int ret;

	while (1) {
		/*记录上一次的所处位置*/
		old_mrow = mrow;
		old_mcol = mcol;
		ret = gmw_read_keyboard_and_mouse(STAR_CGI, maction, mrow, mcol, keycode1, keycode2);

		/*处于0的位置为非法位置*/
		if (mrow >= 0 && mcol >= 0)
			if (inbase[mrow][mcol] == BLANK) {
				mrow = -1;
				mcol = -1;
			}

		/*鼠标事件*/
		if (ret == CCT_MOUSE_EVENT) {

			/*鼠标移动*/
			if (maction == MOUSE_ONLY_MOVED) {

				/*非法位置的扩充*/
				if (mrow == -1 || mcol == -1) {//打印位置非法标志
					char temp[64] = "[当前光标] 位置非法";
					gmw_status_line(STAR_CGI, LOWER_STATUS_LINE, temp);

				}

				/*如果未点开 将扩散的情况还原*/
				back(STAR_CGI, inbase, bdi);

				/* 原色块正常显示 */
				if (old_mrow >= 0 && old_mcol >= 0)
					gmw_draw_block(STAR_CGI, old_mrow, old_mcol, inbase[old_mrow][old_mcol], bdi);

				/* 新色块亮显 */
				if (mrow >= 0 && mcol >= 0)
					gmw_draw_block(STAR_CGI, mrow, mcol, inbase[mrow][mcol] + BETWEEN, bdi);

			}

			/*按下左键*/
			else if (maction == MOUSE_LEFT_BUTTON_CLICK) {
				if (mrow >= 0 && mcol >= 0) {
					if (mrow == old_mrow && mcol == old_mcol) {

						if (inbase[mrow][mcol] > BETWEEN) {//进行点开
							count(STAR_CGI, inbase, score);
							move(STAR_CGI, inbase, bdi);
							if (judge_win(STAR_CGI, inbase))//成功
								return;
						}
						else {//点开确认
							if (same_spread(STAR_CGI, inbase, mrow, mcol)) {//可进行扩散
								char temp[64] = "箭头键/鼠标移动取消当前选择，回车键/单击左键合成";
								gmw_status_line(STAR_CGI, LOWER_STATUS_LINE, temp);//在下方打印提示
								for (int j = 0; j < STAR_CGI->row_num; j++)
									for (int i = 0; i < STAR_CGI->col_num; i++)
										if (inbase[j][i] > BETWEEN)
											gmw_draw_block(STAR_CGI, j, i, inbase[j][i], bdi);//反显打印数组
							}
							else {//不可进行扩散
								char nor_temp[64] = "箭头键/鼠标移动，回车键/单击左键选择";
								char cat_temp[64] = "周围无相同值！ ";
								gmw_status_line(STAR_CGI, LOWER_STATUS_LINE, nor_temp, cat_temp);
							}

						}
					}
				}
				else
					continue;
			}

			/*其他情况不考虑*/
			else
				continue;
		}

		/*键盘事件*/
		else {
			switch (keycode1) {
				case 0x0D://回车键
					if (mrow >= 0 && mcol >= 0) {
							if (inbase[mrow][mcol] > BETWEEN) {//进行点开
								count(STAR_CGI, inbase, score);
								move(STAR_CGI, inbase, bdi);
								if (judge_win(STAR_CGI, inbase))
									return;
									
							}
							else {//点开确认
								if (same_spread(STAR_CGI, inbase, mrow, mcol)) {//可进行扩散
									char temp[64] = "箭头键/鼠标移动取消当前选择，回车键/单击左键合成";
									gmw_status_line(STAR_CGI, LOWER_STATUS_LINE, temp);//在下方打印提示
									for (int j = 0; j < STAR_CGI->row_num; j++)
										for (int i = 0; i < STAR_CGI->col_num; i++)
											if (inbase[j][i] > BETWEEN)
												gmw_draw_block(STAR_CGI, j, i, inbase[j][i], bdi);//反显打印数组
								}
								else {//不可进行扩散
									char nor_temp[64] = "箭头键/鼠标移动，回车键/单击左键选择";
									char cat_temp[64] = "周围无相同值！ ";
									gmw_status_line(STAR_CGI, LOWER_STATUS_LINE, nor_temp, cat_temp);
								}

							}
					}
					else
						continue;
					break;
				case 0xE0: //224 按下的为键盘键

					/*如果未点开 将扩散的情况还原*/
					back(STAR_CGI, inbase, bdi);
					/*确定 mrow和mcol的值*/
					if (old_mrow < 0 || old_mcol < 0)//上次区域非法 本次从左下处开始
					{
						mrow = STAR_CGI->row_num - 1;
						mcol = 0;
					}
					else
					{
						mrow = old_mrow;
						mcol = old_mcol;
					}
					char temp[64];//打印所处位置
					switch (keycode2) {
						case KB_ARROW_UP:
							for (int i = 0; i < STAR_CGI->row_num; i++)//最多只能进行STAR_CGI->row_num次，即如果不存在则为自身
							{
								mrow--;
								if (mrow == -1)
									mrow = STAR_CGI->row_num - 1;
								if (inbase[mrow][mcol] != BLANK)//BLANK当成非法区域
									break;
							}
							sprintf(temp, "[当前键盘] %c行%d列", (char)(mrow + 'A'), mcol);
							gmw_status_line(STAR_CGI, LOWER_STATUS_LINE, temp);
							break;
						case KB_ARROW_DOWN:
							for (int i = 0; i < STAR_CGI->row_num; i++)//最多只能进行STAR_CGI->row_num次，即如果不存在则为自身
							{
								mrow++;
								if (mrow == STAR_CGI->row_num)
									mrow = 0;
								if (inbase[mrow][mcol] != BLANK)//BLANK当成非法区域
									break;
							}
							sprintf(temp, "[当前键盘] %c行%d列", (char)(mrow + 'A'), mcol);
							gmw_status_line(STAR_CGI, LOWER_STATUS_LINE, temp);
							break;
						case KB_ARROW_LEFT:
							for (int i = 0; i < STAR_CGI->col_num; i++)//最多只能进行STAR_CGI->col_num次，即如果不存在则为自身
							{
								mcol--;
								if (mcol == -1)
									mcol = STAR_CGI->col_num - 1;
								if (inbase[mrow][mcol] != BLANK)//BLANK当成非法区域
									break;
							}
							sprintf(temp, "[当前键盘] %c行%d列", (char)(mrow + 'A'), mcol);
							gmw_status_line(STAR_CGI, LOWER_STATUS_LINE, temp);
							break;
						case KB_ARROW_RIGHT:
							for (int i = 0; i < STAR_CGI->col_num; i++)//最多只能进行STAR_CGI->col_num次，即如果不存在则为自身
							{
								mcol++;
								if (mcol == STAR_CGI->col_num)
									mcol = 0;
								if (inbase[mrow][mcol] != BLANK)//BLANK当成非法区域
									break;
							}
							sprintf(temp, "[当前键盘] %c行%d列", (char)(mrow + 'A'), mcol);
							gmw_status_line(STAR_CGI, LOWER_STATUS_LINE, temp);
							break;
					}
					/* 原色块正常显示 */
					if (old_mrow >= 0 && old_mcol >= 0)
						gmw_draw_block(STAR_CGI, old_mrow, old_mcol, inbase[old_mrow][old_mcol], bdi);

					/* 新色块亮显 */
					if (mrow >= 0 && mcol >= 0)
						gmw_draw_block(STAR_CGI, mrow, mcol, inbase[mrow][mcol] + BETWEEN, bdi);

					break;

				default://其他按键不予考虑
					break;
			}//end of switch
		}//end of else
	}//end of while

	/*无返回值*/
	return;

}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：计算分数并打印的函数
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
	sprintf(temp, "本次得分：%d 总分：%d", score_now, score);
	gmw_status_line(STAR_CGI, TOP_STATUS_LINE, temp);//打印分数信息

}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：继续 返回 1
            退出 返回 0
***************************************************************************/
int popstar_end(CONSOLE_GRAPHICS_INFO* const  STAR_CGI, const int inbase[ROW_MAX][COL_MAX],int & score)
{
	/*计算本次得分*/
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
	sprintf(up_temp, "奖励得分：%d 总分：%d",score_now, score);
	gmw_status_line(STAR_CGI, TOP_STATUS_LINE, up_temp);//打印分数信息



	char cat_temp[64];
	char nor_temp[64] = "回车继续 ESC返回";
	sprintf(cat_temp, "剩余%d个星星，无可消除项，本关结束！", num);
	gmw_status_line(STAR_CGI, LOWER_STATUS_LINE, nor_temp, cat_temp);//打印提示信息

	char shuru;
	while (1)//根据输入返回
	{
		shuru = _getch();
		if (shuru == '\r')//回车键
			return 1;
		else if (shuru == 27)//ESC键
			return 0;
	}


}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：扩散函数的内部核心递归函数
***************************************************************************/
void  same_spread_in(CONSOLE_GRAPHICS_INFO* const  STAR_CGI, const int inbase[ROW_MAX][COL_MAX], const int row, const int col, int in[ROW_MAX][COL_MAX])
{
	/*定义该模式下的数组大小*/
	int row_max = STAR_CGI->row_num;
	int col_max = STAR_CGI->col_num;

	/*定义四个方向：东 南 西 北  */
	int row_move[DIRECTION] = { 1,0,-1, 0 };
	int col_move[DIRECTION] = { 0,1, 0,-1 };

	int num = 0;

	for (int k = 0; k < DIRECTION; k++) {
		int next_row = row + row_move[k];//代表位置坐标的x
		int next_col = col + col_move[k];//代表位置坐标的y
		if (next_row >= 0 && next_row < row_max && next_col >= 0 && next_col < col_max) {//只有长和宽在该范围内才可计算
			if (inbase[next_row][next_col] >= 1 && inbase[next_row][next_col] <= 5 && in[next_row][next_col] == 0) {//只有处于1-5才可进行扩散且记录数组所处位置为0

				if (inbase[row][col] == inbase[next_row][next_col])
				{
					num++;
					in[next_row][next_col] = BETWEEN;//将遍历的相同值用in数组记录位置
					same_spread_in(STAR_CGI, inbase, next_row, next_col, in);//此时进入递归，因此当周围均无一样字符时，该位置仍需反显
				}
			}
		}
	}

	if (in[row][col] == 0)
		in[row][col] = BETWEEN;

	return;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：根据点开位置的内部数组模式进行扩散
***************************************************************************/
int same_spread(CONSOLE_GRAPHICS_INFO* const  STAR_CGI, int inbase[ROW_MAX][COL_MAX], const int row, const int col)
{
	/*定义一个记录扩散情况的数组*/
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

	/*根据遍历情况 改变临时数组的值*/
	same_spread_in(STAR_CGI, inbase, row, col, in);

	/*查询遍历情况*/
	for (int i = 0; i < row_max; i++)
		for (int j = 0; j < col_max; j++)
		{
			if (in[i][j] == BETWEEN)
				num++;
		}

	if (num == 1)     //此位置无法进行扩散
		return 0;
	else              //此位置可以进行扩散
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
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：返回 1 游戏结束
			返回 0 游戏继续
***************************************************************************/
int judge_win(CONSOLE_GRAPHICS_INFO* const  STAR_CGI, const int inbase[ROW_MAX][COL_MAX])
{
	/*定义四个方向：东 南 西 北  */
	int row_move[DIRECTION] = { 1,0,-1, 0 };
	int col_move[DIRECTION] = { 0,1, 0,-1 };

	int row_max = STAR_CGI->row_num;
	int col_max = STAR_CGI->col_num;

	/*遍历整个数组内的数据(数字1-5)*/
	for (int row = 0; row < row_max ; row++)
	{
		for (int col = 0; col < col_max; col++)
		{

			if (inbase[row][col] >= 1 && inbase[row][col] <= 5)//仅当为数字1-5时才开始遍历
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
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：将反显的内部数组还原
***************************************************************************/
void back(CONSOLE_GRAPHICS_INFO* const  STAR_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi)
{
	int row_max = STAR_CGI->row_num;
	int col_max = STAR_CGI->col_num;

	/*恢复反显 修改内部值 还原打印界面*/
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
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：进行图形界面下的色块移动
***************************************************************************/
void move(CONSOLE_GRAPHICS_INFO* const  STAR_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi)
{
	int row_max = STAR_CGI->row_num;
	int col_max = STAR_CGI->col_num;

	/*使得反显的位置变为BLANK*/
	for (int row = 0; row < row_max; row++)
		for (int col = 0; col < col_max; col++)
		{
			if (inbase[row][col] > BETWEEN) {
				inbase[row][col] = BLANK;
				gmw_draw_block(STAR_CGI, row, col, inbase[row][col], bdi);
			}
		}

	Sleep(SLEEP_BLANK);

    /*计算下落情况 并改变内部数组*/
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

    /*计算变化后色块需要向下移动的距离*/
	for (int col = 0; col < col_max; col++)
	{
		for (int row = 0; row < row_max; row++)
		{
			int num = 0;
			if (inbase[row][col] != BLANK)//计算下方有多少BLANK 便于移动
			{
				for (int tem_row = (row+1) ; tem_row <row_max; tem_row++)
					if (inbase[tem_row][col] == BLANK)
						num++;
				
			}
			move_down_base[row][col] = num;
		}
			
	}//end of for col

	/*下落移动*/
	for (int col = 0; col < col_max; col++)
		for (int row = row_max - 1; row >= 0; row--)//从上往下落!!!!!
				gmw_move_block(STAR_CGI, row, col, inbase[row][col], BLANK, bdi, UP_TO_DOWN, move_down_base[row][col]);

	 /*改变内部数组*/
	for (int col = 0; col < col_max; col++)
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


	/*计算右移情况 并改变内部数组*/
	int blank_inbase[COL_MAX] = { 1,1,1,1,1,1,1,1,1,1 };//储存一列是否全为0的情况
	int move_left_inbase[COL_MAX] = { 0,0,0,0,0,0,0,0,0,0 };//储存移动情况

    /*计算一列是否全为BLANK*/
	for (int col = 0; col < col_max; col++)
	{
		for (int row = 0; row < row_max; row++)
		{
			if (inbase[row][col] != BLANK)
				break;
			if (row == row_max - 1)//当全为BLANK时
				blank_inbase[col] = 0;
		}
	}//end of for col

	/*计算移动情况*/
	for (int col = 0; col < col_max; col++)
	{
		/*计算变化后色块们需要左移的距离*/
		int num = 0;
		for (int tem_col = (col - 1); tem_col >= 0; tem_col--)//左边有多少空白
			if (blank_inbase[tem_col] == BLANK)
				num++;
		move_left_inbase[col] = num;
	}
	/*左移动开始*/
	for (int row = 0; row < row_max; row++)
		for (int col = 0; col < col_max; col++)
			if (inbase[row][col] != BLANK)
				gmw_move_block(STAR_CGI, row, col, inbase[row][col], BLANK, bdi, RIGHT_TO_LEFT, move_left_inbase[col]);

    /*改变内部数组*/
	for (int col = 0; col < col_max; col++)
	{
		if (blank_inbase[col] == BLANK)
		{
			for (int row = 0; row < row_max; row++)
				for (int tem_col = col; tem_col < (col_max - 1); tem_col++)//该列向右冒泡
				{
					int mid;
					mid = inbase[row][tem_col];
					inbase[row][tem_col] = inbase[row][tem_col + 1];
					inbase[row][tem_col + 1] = mid;
				}
		}
	}
	
	/*无返回值*/
	return;
}
