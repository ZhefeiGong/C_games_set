/* 2052532 龚哲飞 信04 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <time.h>
#include <climits>
#include <conio.h>
#include "../include/cmd_console_tools.h"//包含命令行底下的工具函数
#include "../include/cmd_gmw_tools.h"//包含伪图形界面下的工具函数
using namespace std;

/* --------------------------------------------------
		此处可以给出需要的静态全局变量（尽可能少，最好没有）、静态全局只读变量/宏定义（个数不限）等
   -------------------------------------------------- */


   /* --------------------------------------------------
		   此处可以给出需要的内部辅助工具函数
		   1、函数名不限，建议为 gmw_inner_*
		   2、个数不限
		   3、必须是static函数，确保只在本源文件中使用
	  -------------------------------------------------- */


   /* -----------------------------------------------
		   实现下面给出的函数（函数声明不准动）
   ----------------------------------------------- */
/***************************************************************************
  函数名称：
  功    能：设置游戏主框架的行列数
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		    const int row						：行数(错误则为0，不设上限，人为保证正确性)
		    const int col						：列数(错误则为0，不设上限，人为保证正确性)
  返 回 值：
  说    明：1、指消除类游戏的矩形区域的行列值
		    2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
 ***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO* const pCGI, const int row, const int col)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*设置行列*/
	pCGI->row_num = row;//设置游戏的行数
	pCGI->col_num = col;//设置游戏的列数

	/*修改cmd窗口的大小*/
	int row_sign = 0, col_sign = 0, top_status = 0, lower_status = 0, special_row = 0, special_col = 0;
	/*确定上下状态栏和行列标的显示情况
	  注意：上状态栏（0/1行）/下状态栏（0/1行）/行号显示（0/2列）/列标显示区域（0/1行）*/
	if (pCGI->draw_frame_with_row_no)//需要行标显示
		row_sign = 2;
	if (pCGI->draw_frame_with_col_no)//需要列标显示
		col_sign = 1;
	if (pCGI->top_status_line)//需要上状态栏
		top_status = 1;
	if (pCGI->lower_status_line)//需要下状态栏
		lower_status = 1;
	if (!pCGI->CFI.separator)//此时无分割线
	{
		special_row = 1;
		special_col = 2;
	}

	/*行（上/下额外空间+上状态栏+列标显示+主区域+下状态栏）+ 4*/
	pCGI->lines = special_row + (pCGI->CFI.bhigh * row + 1) + (pCGI->extern_down_lines + pCGI->extern_up_lines) + (col_sign + top_status + lower_status) + 4;

	/*列（左/右额外空间+行标显示+主区域+3*/
	pCGI->cols = special_col + (pCGI->CFI.bwidth * col) + (pCGI->extern_left_cols + pCGI->extern_right_cols) + (row_sign)+3;

	/*各个上下状态栏的起始坐标*/
	pCGI->SLI.top_start_x = pCGI->extern_left_cols;
	pCGI->SLI.top_start_y = pCGI->extern_up_lines;
	pCGI->SLI.lower_start_x = pCGI->extern_left_cols;
	pCGI->SLI.lower_start_y = pCGI->extern_up_lines + special_row + (pCGI->CFI.bhigh * pCGI->row_num + 1) + col_sign + top_status;

	/*状态栏的所占宽度*/
	pCGI->SLI.width = special_col + pCGI->CFI.bwidth * pCGI->col_num + row_sign + 3;

	/*游戏主框架的起始位置(表示色块图形区的起始位置)*/
	/*注：游戏主框架，除了有包含m行n列的色块的外框外，还有上状态栏（0/1行）/下状态栏（0/1行）/行号显示（0/2列）/列标显示区域（0/1行）
	   -     在上述值各不相同的情况下，start_x/start_y的值是不同的*/
	pCGI->start_x = pCGI->extern_left_cols + row_sign;
	pCGI->start_y = pCGI->extern_up_lines + top_status + col_sign;

	return 0; //返回0表示设置成功
}

/***************************************************************************
  函数名称：
  功    能：设置整个窗口（含游戏区、附加区在内的整个cmd窗口）的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int bg_color					：前景色（缺省COLOR_BLACK）
		   const int fg_color					：背景色（缺省COLOR_WHITE）
		   const bool cascade					：是否级联（缺省为true-级联）
  返 回 值：
  说    明：1、cascade = true时
				同步修改游戏主区域的颜色
				同步修改上下状态栏的正常文本的背景色和前景色，醒目文本的背景色（前景色不变）
			2、不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15
					前景色背景色的值一致导致无法看到内容
					前景色正好是状态栏醒目前景色，导致无法看到醒目提示
					...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO* const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*设置结构体pCGI中整个图形界面的颜色设置*/
	pCGI->area_bgcolor = bgcolor;
	pCGI->area_fgcolor = fgcolor;

	/*需要级联时*/
	if (cascade)
	{
		/*设置游戏主区域的颜色*/
		pCGI->CFI.bgcolor = bgcolor;
		pCGI->CFI.fgcolor = fgcolor;
		/*设置上下状态栏的信息颜色*/
		pCGI->SLI.top_normal_bgcolor = bgcolor;
		pCGI->SLI.top_normal_fgcolor = fgcolor;
		pCGI->SLI.lower_normal_bgcolor = bgcolor;
		pCGI->SLI.lower_normal_fgcolor = fgcolor;
		pCGI->SLI.top_catchy_bgcolor = bgcolor;    //仅改背景色哟
		pCGI->SLI.lower_catchy_bgcolor = bgcolor;  //仅改背景色哟

	}

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置窗口的字体
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *fontname					：字体名称（只能是"Terminal"和"新宋体"两种，错误则返回-1，不改变字体）
		   const int fs_high					：字体高度（缺省及错误为16，不设其它限制，人为保证）
		   const int fs_width					：字体高度（缺省及错误为8，不设其它限制，人为保证）
  返 回 值：
  说    明：1、与cmd_console_tools中的setfontsize相似，目前只支持“点阵字体”和“新宋体”
			2、若设置其它字体则直接返回，保持原字体设置不变
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO* const pCGI, const char* fontname, const int fs_high, const int fs_width)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	/*记录字体更改信息*/
	char font1[] = "点阵字体";
	char font2[] = "新宋体";
	if (strcmp(fontname, font1) == 0 || strcmp(fontname, font2) == 0)//判断字体是否需要改变
		strcpy(pCGI->CFT.font_type, fontname);//记录信息到结构体中
	pCGI->CFT.font_size_high = fs_high;
	pCGI->CFT.font_size_width = fs_width;
	/*更改字体设置*/
	cct_setfontsize(fontname, fs_high, fs_width);

	return 0;
}

/***************************************************************************
  函数名称：
  功    能：设置延时
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type						：延时的类型（目前为3种）
		   const int delay_ms					：以ms为单位的延时
			   画边框的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   画色块的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   色块移动的延时：BLOCK_MOVED_DELAY_MS ~ 不设上限，人为保证正确（ <BLOCK_MOVED_DELAY_MS 则置 BLOCK_MOVED_DELAY_MS）
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO* const pCGI, const int type, const int delay_ms)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	switch (type)
	{
		case DELAY_OF_DRAW_FRAME://画边框的延时
			if (delay_ms < 0)
				pCGI->delay_of_draw_frame = 0;
			else
				pCGI->delay_of_draw_frame = delay_ms;
			break;
		case DELAY_OF_DRAW_BLOCK://画色块的延时
			if (delay_ms < 0)
				pCGI->delay_of_draw_block = 0;
			else
				pCGI->delay_of_draw_block = delay_ms;
			break;
		case DELAY_OF_BLOCK_MOVED://色块移动的延时
			if (delay_ms < BLOCK_MOVED_DELAY_MS)
				pCGI->delay_of_block_moved = BLOCK_MOVED_DELAY_MS;
			else
				pCGI->delay_of_block_moved = delay_ms;
			break;
		default:
			break;
	}

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  输入参数：设置游戏主框架结构之外需要保留的额外区域
  功    能：CONSOLE_GRAPHICS_INFO *const pCGI	：
		   const int up_lines					：上部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int down_lines				：下部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int left_cols					：左边额外的列（缺省及错误为0，不设上限，人为保证）
		   const int right_cols				：右边额外的列（缺省及错误为0，不设上限，人为保证）
  返 回 值：
  说    明：额外行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
			即cmd窗口的行列值会发生改变！！！
***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO* const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*进行相关赋值*/
	pCGI->extern_right_cols = right_cols;
	pCGI->extern_left_cols = left_cols;
	pCGI->extern_up_lines = up_lines;
	pCGI->extern_down_lines = down_lines;

	/*cmd窗口的大小将发生改变*/
	int row_sign = 0, col_sign = 0, top_status = 0, lower_status = 0, special_row = 0, special_col = 0;
	/*确定上下状态栏和行列标的显示情况
	  注意：上状态栏（0/1行）/下状态栏（0/1行）/行号显示（0/2列）/列标显示区域（0/1行）*/
	if (pCGI->draw_frame_with_row_no)//需要行标显示
		row_sign = 2;
	if (pCGI->draw_frame_with_col_no)//需要列标显示
		col_sign = 1;
	if (pCGI->top_status_line)//需要上状态栏
		top_status = 1;
	if (pCGI->lower_status_line)//需要下状态栏
		lower_status = 1;
	if (!pCGI->CFI.separator)//此时无分割线
	{
		special_row = 1;
		special_col = 2;
	}

	/*行（上/下额外空间+上状态栏+列标显示+主区域+下状态栏）+ 4*/
	pCGI->lines = special_row + (pCGI->CFI.bhigh * pCGI->row_num + 1) + (pCGI->extern_down_lines + pCGI->extern_up_lines) + (col_sign + top_status + lower_status) + 4;

	/*列（左/右额外空间+行标显示+主区域+3*/
	pCGI->cols = special_col + (pCGI->CFI.bwidth * pCGI->col_num) + (pCGI->extern_left_cols + pCGI->extern_right_cols) + (row_sign)+3;

	/*各个上下状态栏的起始坐标*/
	pCGI->SLI.top_start_x = pCGI->extern_left_cols;
	pCGI->SLI.top_start_y = pCGI->extern_up_lines;
	pCGI->SLI.lower_start_x = pCGI->extern_left_cols;
	pCGI->SLI.lower_start_y = special_row + pCGI->extern_up_lines + (pCGI->CFI.bhigh * pCGI->row_num + 1) + col_sign + top_status;

	/*状态栏的所占宽度*/
	pCGI->SLI.width = special_col + pCGI->CFI.bwidth * pCGI->col_num + row_sign + 3;

	/*游戏主框架的起始位置(表示色块图形区的起始位置)*/
	/*注：游戏主框架，除了有包含m行n列的色块的外框外，还有上状态栏（0/1行）/下状态栏（0/1行）/行号显示（0/2列）/列标显示区域（0/1行）
	   -     在上述值各不相同的情况下，start_x/start_y的值是不同的*/
	pCGI->start_x = pCGI->extern_left_cols + row_sign;
	pCGI->start_y = pCGI->extern_up_lines + top_status + col_sign;

	return 0;
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const int type)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	switch (type)//根据缺省给相关的边框赋值
	{
		case 1://全线
			strcpy(pCGI->CFI.top_left, "╔");
			strcpy(pCGI->CFI.lower_left, "╚");
			strcpy(pCGI->CFI.top_right, "╗");
			strcpy(pCGI->CFI.lower_right, "╝");
			strcpy(pCGI->CFI.h_normal, "═");
			strcpy(pCGI->CFI.v_normal, "║");
			strcpy(pCGI->CFI.h_top_separator, "╦");
			strcpy(pCGI->CFI.h_lower_separator, "╩");
			strcpy(pCGI->CFI.v_left_separator, "╠");
			strcpy(pCGI->CFI.v_right_separator, "╣");
			strcpy(pCGI->CFI.mid_separator, "╬");
			break;
		case 2://全单线
			strcpy(pCGI->CFI.top_left, "┏");
			strcpy(pCGI->CFI.lower_left, "┗");
			strcpy(pCGI->CFI.top_right, "┓");
			strcpy(pCGI->CFI.lower_right, "┛");
			strcpy(pCGI->CFI.h_normal, "━");
			strcpy(pCGI->CFI.v_normal, "┃");
			strcpy(pCGI->CFI.h_top_separator, "┳");
			strcpy(pCGI->CFI.h_lower_separator, "┻");
			strcpy(pCGI->CFI.v_left_separator, "┣");
			strcpy(pCGI->CFI.v_right_separator, "┫");
			strcpy(pCGI->CFI.mid_separator, "╋");
			break;
		case 3://横双竖单
			strcpy(pCGI->CFI.top_left, "╒");
			strcpy(pCGI->CFI.lower_left, "╘");
			strcpy(pCGI->CFI.top_right, "╕");
			strcpy(pCGI->CFI.lower_right, "╛");
			strcpy(pCGI->CFI.h_normal, "═");
			strcpy(pCGI->CFI.v_normal, "│");
			strcpy(pCGI->CFI.h_top_separator, "╤");
			strcpy(pCGI->CFI.h_lower_separator, "╧");
			strcpy(pCGI->CFI.v_left_separator, "╞");
			strcpy(pCGI->CFI.v_right_separator, "╡");
			strcpy(pCGI->CFI.mid_separator, "╪");
			break;
		case 4://横单竖双
			strcpy(pCGI->CFI.top_left, "╓");
			strcpy(pCGI->CFI.lower_left, "╙");
			strcpy(pCGI->CFI.top_right, "╖");
			strcpy(pCGI->CFI.lower_right, "╜");
			strcpy(pCGI->CFI.h_normal, "─");
			strcpy(pCGI->CFI.v_normal, "║");
			strcpy(pCGI->CFI.h_top_separator, "╥");
			strcpy(pCGI->CFI.h_lower_separator, "╨");
			strcpy(pCGI->CFI.v_left_separator, "╟");
			strcpy(pCGI->CFI.v_right_separator, "╢");
			strcpy(pCGI->CFI.mid_separator, "╫");
			break;
		default:
			cout << "error！！！" << endl;
			return -1;
	}
	return 0;
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const char *...						：共11种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
***************************************************************************/
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const char* top_left, const char* lower_left, const char* top_right,
	const char* lower_right, const char* h_normal, const char* v_normal, const char* h_top_separator,
	const char* h_lower_separator, const char* v_left_separator, const char* v_right_separator, const char* mid_separator)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	/*修改结构中的各种线型*/

	/*top_left*/
	if (strlen(top_left) == NULL)
		strcpy(pCGI->CFI.top_left, "  ");//补两个空格
	else if (strlen(top_left) == 1)
	{
		pCGI->CFI.top_left[0] = top_left[0];
		pCGI->CFI.top_left[1] = ' ';//补一个空格
	}
	else
		strncpy(pCGI->CFI.top_left, top_left, 2);

	/*lower_left*/
	if (strlen(lower_left) == NULL)
		strcpy(pCGI->CFI.lower_left, "  ");//补两个空格
	else if (strlen(lower_left) == 1)
	{
		pCGI->CFI.lower_left[0] = lower_left[0];
		pCGI->CFI.lower_left[1] = ' ';//补一个空格
	}
	else
		strncpy(pCGI->CFI.lower_left, lower_left, 2);

	/*top_right*/
	if (strlen(top_right) == NULL)
		strcpy(pCGI->CFI.top_right, "  ");//补两个空格
	else if (strlen(top_right) == 1)
	{
		pCGI->CFI.top_right[0] = top_right[0];
		pCGI->CFI.top_right[1] = ' ';//补一个空格
	}
	else
		strncpy(pCGI->CFI.top_right, top_right, 2);

	/*lower_right*/
	if (strlen(lower_right) == NULL)
		strcpy(pCGI->CFI.lower_right, "  ");//补两个空格
	else if (strlen(lower_right) == 1)
	{
		pCGI->CFI.lower_right[0] = lower_right[0];
		pCGI->CFI.lower_right[1] = ' ';//补一个空格
	}
	else
		strncpy(pCGI->CFI.lower_right, lower_right, 2);

	/*h_normal*/
	if (strlen(h_normal) == NULL)
		strcpy(pCGI->CFI.h_normal, "  ");//补两个空格
	else if (strlen(h_normal) == 1)
	{
		pCGI->CFI.h_normal[0] = h_normal[0];
		pCGI->CFI.h_normal[1] = ' ';//补一个空格
	}
	else
		strncpy(pCGI->CFI.h_normal, h_normal, 2);

	/*v_normal*/
	if (strlen(v_normal) == NULL)
		strcpy(pCGI->CFI.v_normal, "  ");//补两个空格
	else if (strlen(v_normal) == 1)
	{
		pCGI->CFI.v_normal[0] = v_normal[0];
		pCGI->CFI.v_normal[1] = ' ';//补一个空格
	}
	else
		strncpy(pCGI->CFI.v_normal, v_normal, 2);

	/*h_top_separator*/
	if (strlen(h_top_separator) == NULL)
		strcpy(pCGI->CFI.h_top_separator, "  ");//补两个空格
	else if (strlen(h_top_separator) == 1)
	{
		pCGI->CFI.h_top_separator[0] = h_top_separator[0];
		pCGI->CFI.h_top_separator[1] = ' ';//补一个空格
	}
	else
		strncpy(pCGI->CFI.h_top_separator, h_top_separator, 2);

	/*h_lower_separator*/
	if (strlen(h_lower_separator) == NULL)
		strcpy(pCGI->CFI.h_lower_separator, "  ");//补两个空格
	else if (strlen(h_lower_separator) == 1)
	{
		pCGI->CFI.h_lower_separator[0] = h_lower_separator[0];
		pCGI->CFI.h_lower_separator[1] = ' ';//补一个空格
	}
	else
		strncpy(pCGI->CFI.h_lower_separator, h_lower_separator, 2);

	/*v_left_separator*/
	if (strlen(v_left_separator) == NULL)
		strcpy(pCGI->CFI.v_left_separator, "  ");//补两个空格
	else if (strlen(v_left_separator) == 1)
	{
		pCGI->CFI.v_left_separator[0] = v_left_separator[0];
		pCGI->CFI.v_left_separator[1] = ' ';//补一个空格
	}
	else
		strncpy(pCGI->CFI.v_left_separator, v_left_separator, 2);

	/*v_right_separator*/
	if (strlen(v_right_separator) == NULL)
		strcpy(pCGI->CFI.v_right_separator, "  ");//补两个空格
	else if (strlen(v_right_separator) == 1)
	{
		pCGI->CFI.v_right_separator[0] = v_right_separator[0];
		pCGI->CFI.v_right_separator[1] = ' ';//补一个空格
	}
	else
		strncpy(pCGI->CFI.v_right_separator, v_right_separator, 2);

	/*mid_separator*/
	if (strlen(mid_separator) == NULL)
		strcpy(pCGI->CFI.mid_separator, "  ");//补两个空格
	else if (strlen(mid_separator) == 1)
	{
		pCGI->CFI.mid_separator[0] = mid_separator[0];
		pCGI->CFI.mid_separator[1] = ' ';//补一个空格
	}
	else
		strncpy(pCGI->CFI.mid_separator, mid_separator, 2);

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPEO 结构中的色块数量大小、是否需要分隔线等
  输入参数：输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int block_width						：宽度（错误及缺省2，因为约定表格线为中文制表符，如果给出奇数，要+1）
			const int block_high						：高度（错误及缺省1）
			const bool separator						：是否需要分隔线（缺省为true，需要分隔线）
  返 回 值：
  说    明：框架大小/是否需要分隔线等的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO* const pCGI, const int block_width, const int block_high, const bool separator)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	/*进行相关赋值操作*/
	if (block_width % 2 == 1)//如果为奇数要转化为偶数
		pCGI->CFI.block_width = block_width + 1;
	else
		pCGI->CFI.block_width = block_width;

	pCGI->CFI.block_high = block_high;

	pCGI->CFI.separator = separator;//修改是否需要分割线

	/*修改相关的值*/
	int high_, width_;
	if (pCGI->CFI.separator) {
		width_ = 2;
		high_ = 1;
	}
	else {
		width_ = 0;
		high_ = 0;
	}//注：暂时已使用block_width_ext/block_high_ext
	pCGI->CFI.block_high_ext = 1;
	pCGI->CFI.block_width_ext = 2;
	pCGI->CFI.bhigh = pCGI->CFI.block_high + high_;
	pCGI->CFI.bwidth = pCGI->CFI.block_width + width_;

	/*修改cmd窗口的大小*/
	int row_sign = 0, col_sign = 0, top_status = 0, lower_status = 0, special_row = 0, special_col = 0;
	/*确定上下状态栏和行列标的显示情况
	  注意：上状态栏（0/1行）/下状态栏（0/1行）/行号显示（0/2列）/列标显示区域（0/1行）*/
	if (pCGI->draw_frame_with_row_no)//需要行标显示
		row_sign = 2;
	if (pCGI->draw_frame_with_col_no)//需要列标显示
		col_sign = 1;
	if (pCGI->top_status_line)//需要上状态栏
		top_status = 1;
	if (pCGI->lower_status_line)//需要下状态栏
		lower_status = 1;
	if (!pCGI->CFI.separator)//此时无分割线
	{
		special_row = 1;
		special_col = 2;
	}


	/*行（上/下额外空间+上状态栏+列标显示+主区域+下状态栏）+ 4*/
	pCGI->lines = special_row + (pCGI->CFI.bhigh * pCGI->row_num + 1) + (pCGI->extern_down_lines + pCGI->extern_up_lines) + (col_sign + top_status + lower_status) + 4;

	/*列（左/右额外空间+行标显示+主区域+3*/
	pCGI->cols = special_col + (pCGI->CFI.bwidth * pCGI->col_num) + (pCGI->extern_left_cols + pCGI->extern_right_cols) + (row_sign)+3;

	/*各个上下状态栏的起始坐标*/
	pCGI->SLI.top_start_x = pCGI->extern_left_cols;
	pCGI->SLI.top_start_y = pCGI->extern_up_lines;
	pCGI->SLI.lower_start_x = pCGI->extern_left_cols;
	pCGI->SLI.lower_start_y = special_row + pCGI->extern_up_lines + (pCGI->CFI.bhigh * pCGI->row_num + 1) + col_sign + top_status;

	/*状态栏的所占宽度*/
	pCGI->SLI.width = special_col + pCGI->CFI.bwidth * pCGI->col_num + row_sign + 3;

	/*游戏主框架的起始位置(表示色块图形区的起始位置)*/
	/*注：游戏主框架，除了有包含m行n列的色块的外框外，还有上状态栏（0/1行）/下状态栏（0/1行）/行号显示（0/2列）/列标显示区域（0/1行）
	   -     在上述值各不相同的情况下，start_x/start_y的值是不同的*/
	pCGI->start_x = pCGI->extern_left_cols + row_sign;
	pCGI->start_y = pCGI->extern_up_lines + top_status + col_sign;

	return 0;
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BORDER_TYPE 结构中的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int bg_color					：背景色（缺省 -1表示用窗口背景色）
			const int fg_color					：前景色（缺省 -1表示用窗口前景色）
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO* const pCGI, const int bgcolor, const int fgcolor)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*修改CONSOLE_FRAME_INFO中主框架中颜色*/
	pCGI->CFI.bgcolor = bgcolor;
	pCGI->CFI.fgcolor = fgcolor;

	return 0;
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全双线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const int type)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	switch (type)
	{
		case 1://全双线
			strcpy(pCGI->CBI.top_left, "╔");
			strcpy(pCGI->CBI.lower_left, "╚");
			strcpy(pCGI->CBI.top_right, "╗");
			strcpy(pCGI->CBI.lower_right, "╝");
			strcpy(pCGI->CBI.h_normal, "═");
			strcpy(pCGI->CBI.v_normal, "║");
			break;
		case 2://全单线
			strcpy(pCGI->CBI.top_left, "┏");
			strcpy(pCGI->CBI.lower_left, "┗");
			strcpy(pCGI->CBI.top_right, "┓");
			strcpy(pCGI->CBI.lower_right, "┛");
			strcpy(pCGI->CBI.h_normal, "━");
			strcpy(pCGI->CBI.v_normal, "┃");
			break;
		case 3://横双竖单
			strcpy(pCGI->CBI.top_left, "╒");
			strcpy(pCGI->CBI.lower_left, "╘");
			strcpy(pCGI->CBI.top_right, "╕");
			strcpy(pCGI->CBI.lower_right, "╛");
			strcpy(pCGI->CBI.h_normal, "═");
			strcpy(pCGI->CBI.v_normal, "│");
			break;
		case 4://横单竖双
			strcpy(pCGI->CBI.top_left, "╓");
			strcpy(pCGI->CBI.lower_left, "╙");
			strcpy(pCGI->CBI.top_right, "╖");
			strcpy(pCGI->CBI.lower_right, "╜");
			strcpy(pCGI->CBI.h_normal, "─");
			strcpy(pCGI->CBI.v_normal, "║");
			break;
		default:
			cout << "error！！！" << endl;
			return -1;
	}

	return 0;
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *...					：共6种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
***************************************************************************/
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const char* top_left, const char* lower_left, const char* top_right, const char* lower_right, const char* h_normal, const char* v_normal)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*top_left*/
	if (strlen(top_left) == NULL)
		strcpy(pCGI->CBI.top_left, "  ");//补两个空格
	else if (strlen(top_left) == 1)
	{
		pCGI->CBI.top_left[0] = top_left[0];
		pCGI->CBI.top_left[1] = ' ';//补一个空格
	}
	else
		strncpy(pCGI->CBI.top_left, top_left, 2);

	/*lower_left*/
	if (strlen(lower_left) == NULL)
		strcpy(pCGI->CBI.lower_left, "  ");//补两个空格
	else if (strlen(lower_left) == 1)
	{
		pCGI->CBI.lower_left[0] = lower_left[0];
		pCGI->CBI.lower_left[1] = ' ';//补一个空格
	}
	else
		strncpy(pCGI->CBI.lower_left, lower_left, 2);

	/*top_right*/
	if (strlen(top_right) == NULL)
		strcpy(pCGI->CBI.top_right, "  ");//补两个空格
	else if (strlen(top_right) == 1)
	{
		pCGI->CBI.top_right[0] = top_right[0];
		pCGI->CBI.top_right[1] = ' ';//补一个空格
	}
	else
		strncpy(pCGI->CBI.top_right, top_right, 2);

	/*lower_right*/
	if (strlen(lower_right) == NULL)
		strcpy(pCGI->CBI.lower_right, "  ");//补两个空格
	else if (strlen(lower_right) == 1)
	{
		pCGI->CBI.lower_right[0] = lower_right[0];
		pCGI->CBI.lower_right[1] = ' ';//补一个空格
	}
	else
		strncpy(pCGI->CBI.lower_right, lower_right, 2);

	/*h_normal*/
	if (strlen(h_normal) == NULL)
		strcpy(pCGI->CBI.h_normal, "  ");//补两个空格
	else if (strlen(h_normal) == 1)
	{
		pCGI->CBI.h_normal[0] = h_normal[0];
		pCGI->CBI.h_normal[1] = ' ';//补一个空格
	}
	else
		strncpy(pCGI->CBI.h_normal, h_normal, 2);

	/*v_normal*/
	if (strlen(v_normal) == NULL)
		strcpy(pCGI->CBI.v_normal, "  ");//补两个空格
	else if (strlen(v_normal) == 1)
	{
		pCGI->CBI.v_normal[0] = v_normal[0];
		pCGI->CBI.v_normal[1] = ' ';//补一个空格
	}
	else
		strncpy(pCGI->CBI.v_normal, v_normal, 2);

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置每个游戏色块(彩球)是否需要小边框
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const bool on_off					：true - 需要 flase - 不需要（缺省false）
  返 回 值：
  说    明：边框约定为中文制表符，双线
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO* const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->CBI.block_border = on_off;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示上下状态栏
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）(0上/1下)
			const bool on_off					：true - 需要 flase - 不需要（缺省true）
  返 回 值：
  说    明：1、状态栏的相关约定如下：
			   1.1、上状态栏只能一行，在主区域最上方框线/列标的上面，为主区域的最开始一行（主区域的左上角坐标就是上状态栏的坐标）
			   1.2、下状态栏只能一行，在主区域最下方框线的下面
			   1.3、状态栏的宽度为主区域宽度，如果信息过长则截断
		   2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO* const pCGI, const int type, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*修改是否需要状态栏的参数*/
	switch (type)
	{
		case TOP_STATUS_LINE://上状态栏
			pCGI->top_status_line = on_off;
			break;
		case LOWER_STATUS_LINE://下状态栏
			pCGI->lower_status_line = on_off;
			break;
		default:
			return -1;
	}

	/*修改cmd窗口的大小*/
	int row_sign = 0, col_sign = 0, top_status = 0, lower_status = 0, special_row = 0, special_col = 0;
	/*确定上下状态栏和行列标的显示情况
	  注意：上状态栏（0/1行）/下状态栏（0/1行）/行号显示（0/2列）/列标显示区域（0/1行）*/
	if (pCGI->draw_frame_with_row_no)//需要行标显示
		row_sign = 2;
	if (pCGI->draw_frame_with_col_no)//需要列标显示
		col_sign = 1;
	if (pCGI->top_status_line)//需要上状态栏
		top_status = 1;
	if (pCGI->lower_status_line)//需要下状态栏
		lower_status = 1;
	if (!pCGI->CFI.separator)//此时无分割线
	{
		special_row = 1;
		special_col = 2;
	}

	/*行（上/下额外空间+上状态栏+列标显示+主区域+下状态栏）+ 4*/
	pCGI->lines = special_row + (pCGI->CFI.bhigh * pCGI->row_num + 1) + (pCGI->extern_down_lines + pCGI->extern_up_lines) + (col_sign + top_status + lower_status) + 4;

	/*列（左/右额外空间+行标显示+主区域+3*/
	pCGI->cols = special_col + (pCGI->CFI.bwidth * pCGI->col_num) + (pCGI->extern_left_cols + pCGI->extern_right_cols) + (row_sign)+3;

	/*各个上下状态栏的起始坐标*/
	pCGI->SLI.top_start_x = pCGI->extern_left_cols;
	pCGI->SLI.top_start_y = pCGI->extern_up_lines;
	pCGI->SLI.lower_start_x = pCGI->extern_left_cols;
	pCGI->SLI.lower_start_y = special_row + pCGI->extern_up_lines + (pCGI->CFI.bhigh * pCGI->row_num + 1) + col_sign + top_status;

	/*状态栏的所占宽度*/
	pCGI->SLI.width = special_col + pCGI->CFI.bwidth * pCGI->col_num + row_sign + 3;

	/*游戏主框架的起始位置(表示色块图形区的起始位置)*/
	/*注：游戏主框架，除了有包含m行n列的色块的外框外，还有上状态栏（0/1行）/下状态栏（0/1行）/行号显示（0/2列）/列标显示区域（0/1行）
	   -     在上述值各不相同的情况下，start_x/start_y的值是不同的*/
	pCGI->start_x = pCGI->extern_left_cols + row_sign;
	pCGI->start_y = pCGI->extern_up_lines + top_status + col_sign;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置上下状态栏的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）(0上/1下)
			const int normal_bgcolor			：正常文本背景色（缺省 -1表示使用窗口背景色）
			const int normal_fgcolor			：正常文本前景色（缺省 -1表示使用窗口前景色）
			const int catchy_bgcolor			：醒目文本背景色（缺省 -1表示使用窗口背景色）
			const int catchy_fgcolor			：醒目文本前景色（缺省 -1表示使用亮黄色）
  输入参数：
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO* const pCGI, const int type, const int normal_bgcolor, const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*进行颜色的缺省设置*/
	int nor_bgcolor = 0;
	int nor_fgcolor = 0;
	int cat_bgcolor = 0;
	int cat_fgcolor = 0;

	if (normal_bgcolor == -1)
		nor_bgcolor = pCGI->area_bgcolor;
	else
		nor_bgcolor = normal_bgcolor;
	if (normal_fgcolor == -1)
		nor_fgcolor = pCGI->area_fgcolor;
	else
		nor_fgcolor = normal_fgcolor;
	if (catchy_bgcolor == -1)
		cat_bgcolor = pCGI->area_bgcolor;
	else
		cat_bgcolor = catchy_bgcolor;
	if (catchy_fgcolor == -1)//醒目文字缺省前景色为亮黄色
		cat_fgcolor = COLOR_HYELLOW;
	else
		cat_fgcolor = catchy_fgcolor;


	switch (type)
	{
		case TOP_STATUS_LINE://上状态栏
			pCGI->SLI.top_normal_bgcolor = nor_bgcolor;
			pCGI->SLI.top_normal_fgcolor = nor_fgcolor;
			pCGI->SLI.top_catchy_bgcolor = cat_bgcolor;
			pCGI->SLI.top_catchy_fgcolor = cat_fgcolor;
			break;
		case  LOWER_STATUS_LINE://下状态栏
			pCGI->SLI.lower_normal_bgcolor = nor_bgcolor;
			pCGI->SLI.lower_normal_fgcolor = nor_fgcolor;
			pCGI->SLI.lower_catchy_bgcolor = cat_bgcolor;
			pCGI->SLI.lower_catchy_fgcolor = cat_fgcolor;
		default:
			return -1;
	}

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示行号
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、行号约定为字母A开始连续排列（如果超过26，则从a开始，超过52的统一为*，实际应用不可能）
			2、是否显示行号的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO* const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	/*设置是否需要行号*/
	pCGI->draw_frame_with_row_no = on_off;

	/*修改cmd窗口列的大小*/
	int row_sign = 0, special_row = 0, special_col = 0;
	/*确定上下状态栏和行列标的显示情况
	  注意：上状态栏（0/1行）/下状态栏（0/1行）/行号显示（0/2列）/列标显示区域（0/1行）*/
	if (pCGI->draw_frame_with_row_no)//需要行标显示
		row_sign = 2;
	if (!pCGI->CFI.separator)//此时无分割线
	{
		special_row = 1;
		special_col = 2;
	}


	/*列（左/右额外空间+行标显示+主区域+3*/
	pCGI->cols = special_col + (pCGI->CFI.bwidth * pCGI->col_num) + (pCGI->extern_left_cols + pCGI->extern_right_cols) + (row_sign)+3;


	/*状态栏的所占宽度*/
	pCGI->SLI.width = special_col + pCGI->CFI.bwidth * pCGI->col_num + row_sign + 3;//宽度含行号的位置

	/*游戏主框架的起始位置(表示色块图形区的起始位置)*/
	/*注：游戏主框架，除了有包含m行n列的色块的外框外，还有上状态栏（0/1行）/下状态栏（0/1行）/行号显示（0/2列）/列标显示区域（0/1行）
	   -     在上述值各不相同的情况下，start_x/start_y的值是不同的*/
	pCGI->start_x = pCGI->extern_left_cols + row_sign;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示列标
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、列标约定为数字0开始连续排列（数字0-99，超过99统一为**，实际应用不可能）
			2、是否显示列标的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO* const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*设置是否需要列标*/
	pCGI->draw_frame_with_col_no = on_off;

	/*改变cmd窗口行的大小*/
	int  col_sign = 0, top_status = 0, lower_status = 0, special_row = 0, special_col = 0;
	/*确定上下状态栏和行列标的显示情况
	  注意：上状态栏（0/1行）/下状态栏（0/1行）/行号显示（0/2列）/列标显示区域（0/1行）*/
	if (pCGI->draw_frame_with_col_no)//需要列标显示
		col_sign = 1;
	if (pCGI->top_status_line)//需要上状态栏
		top_status = 1;
	if (pCGI->lower_status_line)//需要下状态栏
		lower_status = 1;
	if (!pCGI->CFI.separator)//此时无分割线
	{
		special_row = 1;
		special_col = 2;
	}


	/*行（上/下额外空间+上状态栏+列标显示+主区域+下状态栏）+ 4*/
	pCGI->lines = special_row + (pCGI->CFI.bhigh * pCGI->row_num + 1) + (pCGI->extern_down_lines + pCGI->extern_up_lines) + (col_sign + top_status + lower_status) + 4;

	/*各个上下状态栏的起始坐标*/
	pCGI->SLI.top_start_x = pCGI->extern_left_cols;
	pCGI->SLI.top_start_y = pCGI->extern_up_lines;
	pCGI->SLI.lower_start_x = pCGI->extern_left_cols;
	pCGI->SLI.lower_start_y = special_row + pCGI->extern_up_lines + (pCGI->CFI.bhigh * pCGI->row_num + 1) + col_sign + top_status;

	/*游戏主框架的起始位置(表示色块图形区的起始位置)*/
	/*注：游戏主框架，除了有包含m行n列的色块的外框外，还有上状态栏（0/1行）/下状态栏（0/1行）/行号显示（0/2列）/列标显示区域（0/1行）
	   -     在上述值各不相同的情况下，start_x/start_y的值是不同的*/
	pCGI->start_y = pCGI->extern_up_lines + top_status + col_sign;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：打印 CONSOLE_GRAPHICS_INFO 结构体中的各成员值
  输入参数：
  返 回 值：
  说    明：1、仅供调试用，打印格式自定义
			2、本函数测试用例中未调用过，可以不实现
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO* const pCGI)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*暂时不实现*/

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：将 CONSOLE_GRAPHICS_INFO 结构体用缺省值进行初始化
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI：整体结构指针
		   const int row					：游戏区域色块行数（缺省10）
		   const int col					：游戏区域色块列数（缺省10）
		   const int bgcolor				：整个窗口背景色（缺省 COLOR_BLACK）
		   const int fgcolor				：整个窗口背景色（缺省 COLOR_WHITE）
  返 回 值：
  说    明：窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO* const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	/* 首先置标记 */
	pCGI->inited = CGI_INITED;//执行此函数即已经进行过初始化

	/*进行初始化*/

	/*进行全部信息结构体中单独部分的初始化*/
	pCGI->extern_up_lines = 0;//无额外行列
	pCGI->extern_down_lines = 0;//无额外行列
	pCGI->extern_left_cols = 0;//无额外行列
	pCGI->extern_right_cols = 0;//无额外行列
	pCGI->row_num = row;
	pCGI->col_num = col;
	pCGI->area_bgcolor = bgcolor;
	pCGI->area_fgcolor = fgcolor;
	pCGI->top_status_line = true;//上下状态栏均有
	pCGI->lower_status_line = true;//上下状态栏均有
	pCGI->draw_frame_with_row_no = false;//无需行号
	pCGI->draw_frame_with_col_no = false;//无需列标
	pCGI->delay_of_block_moved = BLOCK_MOVED_DELAY_MS;
	pCGI->delay_of_draw_block = 0;
	pCGI->delay_of_draw_frame = 0;
	pCGI->start_x = 0;//此时无额外空间，无行号列标，有上下状态栏
	pCGI->start_y = 1;

	/*进行游戏主框架区域的结构体(CFI)的初始化*/
	strcpy(pCGI->CFI.top_left, "╔");
	strcpy(pCGI->CFI.lower_left, "╚");
	strcpy(pCGI->CFI.top_right, "╗");
	strcpy(pCGI->CFI.lower_right, "╝");
	strcpy(pCGI->CFI.h_normal, "═");
	strcpy(pCGI->CFI.v_normal, "║");
	strcpy(pCGI->CFI.h_top_separator, "╦");
	strcpy(pCGI->CFI.h_lower_separator, "╩");
	strcpy(pCGI->CFI.v_left_separator, "╠");
	strcpy(pCGI->CFI.v_right_separator, "╣");
	strcpy(pCGI->CFI.mid_separator, "╬");
	pCGI->CFI.bgcolor = bgcolor;
	pCGI->CFI.fgcolor = fgcolor;
	pCGI->CFI.block_width = 2;
	pCGI->CFI.block_high = 1;
	pCGI->CFI.separator = true;   //缺省色块之间带分隔线
	pCGI->CFI.block_width_ext = 2;
	pCGI->CFI.block_high_ext = 1;
	pCGI->CFI.bwidth = pCGI->CFI.block_width + pCGI->CFI.block_width_ext;
	pCGI->CFI.bhigh = pCGI->CFI.block_high + pCGI->CFI.block_high_ext;

	/*进行色块信息结构体(CBI)的初始化*/
	strcpy(pCGI->CBI.top_left, "╔");
	strcpy(pCGI->CBI.lower_left, "╚");
	strcpy(pCGI->CBI.top_right, "╗");
	strcpy(pCGI->CBI.lower_right, "╝");
	strcpy(pCGI->CBI.h_normal, "═");
	strcpy(pCGI->CBI.v_normal, "║");
	pCGI->CBI.block_border = false;//色块内部无小边框

	/*进行状态栏信息结构体(SLI)的初始化*/
	pCGI->SLI.is_lower_status_line = true;//有下状态栏
	pCGI->SLI.is_top_status_line = true;//有上状态栏
	pCGI->SLI.top_normal_bgcolor = bgcolor;
	pCGI->SLI.top_catchy_bgcolor = bgcolor;
	pCGI->SLI.lower_normal_bgcolor = bgcolor;
	pCGI->SLI.lower_catchy_bgcolor = bgcolor;
	pCGI->SLI.top_normal_fgcolor = fgcolor;
	pCGI->SLI.top_catchy_fgcolor = COLOR_HYELLOW;     //缺省为亮黄色
	pCGI->SLI.lower_catchy_fgcolor = COLOR_HYELLOW;   //缺省为亮黄色
	pCGI->SLI.lower_normal_fgcolor = fgcolor;
	pCGI->SLI.top_start_x = 0;
	pCGI->SLI.top_start_y = 0;
	pCGI->SLI.lower_start_x = 0;
	pCGI->SLI.lower_start_y = (pCGI->CFI.bhigh * pCGI->row_num + 1) + (pCGI->extern_down_lines + pCGI->extern_up_lines) + 1;
	pCGI->SLI.width = pCGI->CFI.bwidth * pCGI->col_num + 3;//width是游戏区域的宽度，不包括扩展区域，信息超过宽度则截断

	/*进行字体信息结构体(CFT)的初始化*/
	pCGI->CFT.font_size_high = 16;
	pCGI->CFT.font_size_width = 8;
	strcpy(pCGI->CFT.font_type, "点阵字体");

	/*最后进行 行列赋值*/
	/*行（上/下额外空间+上状态栏+列标显示+主区域+下状态栏）+ 4*/
	pCGI->lines = pCGI->start_y + 0 + 0 + 1 + 0 + pCGI->CFI.bhigh * pCGI->row_num + 1 + 4;
	/*列（左/右额外空间+行标显示+主区域+3*/
	pCGI->cols = pCGI->start_x + 0 + 0 + 0 + pCGI->CFI.bwidth * pCGI->col_num + 3;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：画主游戏框架
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
  返 回 值：
  说    明：具体可参考demo的效果
***************************************************************************/
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO* const pCGI)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*设置cmd窗口的大小*/
	cct_setconsoleborder(pCGI->cols, pCGI->lines, pCGI->cols, pCGI->lines);
	/*设置字体*/
	cct_setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high, pCGI->CFT.font_size_width);
	/*将整个cmd窗口的颜色统一一波*/
	char color[9] = "color ";
	switch (pCGI->area_bgcolor)
	{
		case COLOR_BLACK:   //黑
			color[6] = (char)('0' + COLOR_BLACK);
			break;
		case COLOR_BLUE:    //蓝
			color[6] = (char)('0' + COLOR_BLUE);
			break;
		case  COLOR_GREEN:  //绿
			color[6] = (char)('0' + COLOR_GREEN);
			break;
		case  COLOR_CYAN:   //青
			color[6] = (char)('0' + COLOR_CYAN);
			break;
		case  COLOR_RED:	//红
			color[6] = (char)('0' + COLOR_RED);
			break;
		case  COLOR_PINK:	//粉
			color[6] = (char)('0' + COLOR_PINK);
			break;
		case  COLOR_YELLOW:	//黄
			color[6] = (char)('0' + COLOR_YELLOW);
			break;
		case  COLOR_WHITE:	//白
			color[6] = (char)('0' + COLOR_WHITE);
			break;
		case  COLOR_HBLACK:	//亮黑
			color[6] = (char)('0' + COLOR_HBLACK);
			break;
		case  COLOR_HBLUE:	//亮蓝
			color[6] = (char)('0' + COLOR_GREEN);
			break;
		case  COLOR_HGREEN:	//亮绿
			color[6] = (char)('A' + (COLOR_HGREEN - 10));
			break;
		case  COLOR_HCYAN:	//亮青
			color[6] = (char)('A' + (COLOR_HCYAN - 10));
			break;
		case  COLOR_HRED:	//亮红
			color[6] = (char)('A' + (COLOR_HRED - 10));
			break;
		case  COLOR_HPINK:	//亮粉
			color[6] = (char)('A' + (COLOR_HPINK - 10));
			break;
		case COLOR_HYELLOW:	//亮黄
			color[6] = (char)('A' + (COLOR_HYELLOW - 10));
			break;
		case  COLOR_HWHITE:	//亮白
			color[6] = (char)('A' + (COLOR_HWHITE - 10));
			break;
		default:
			break;
	}
	switch (pCGI->area_fgcolor)
	{
		case COLOR_BLACK:   //黑
			color[7] = (char)('0' + COLOR_BLACK);
			break;
		case COLOR_BLUE:    //蓝
			color[7] = (char)('0' + COLOR_BLUE);
			break;
		case  COLOR_GREEN:  //绿
			color[7] = (char)('0' + COLOR_GREEN);
			break;
		case  COLOR_CYAN:   //青
			color[7] = (char)('0' + COLOR_CYAN);
			break;
		case  COLOR_RED:	//红
			color[7] = (char)('0' + COLOR_RED);
			break;
		case  COLOR_PINK:	//粉
			color[7] = (char)('0' + COLOR_PINK);
			break;
		case  COLOR_YELLOW:	//黄
			color[7] = (char)('0' + COLOR_YELLOW);
			break;
		case  COLOR_WHITE:	//白
			color[7] = (char)('0' + COLOR_WHITE);
			break;
		case  COLOR_HBLACK:	//亮黑
			color[7] = (char)('0' + COLOR_HBLACK);
			break;
		case  COLOR_HBLUE:	//亮蓝
			color[7] = (char)('0' + COLOR_GREEN);
			break;
		case  COLOR_HGREEN:	//亮绿
			color[7] = (char)('A' + (COLOR_HGREEN - 10));
			break;
		case  COLOR_HCYAN:	//亮青
			color[7] = (char)('A' + (COLOR_HCYAN - 10));
			break;
		case  COLOR_HRED:	//亮红
			color[7] = (char)('A' + (COLOR_HRED - 10));
			break;
		case  COLOR_HPINK:	//亮粉
			color[7] = (char)('A' + (COLOR_HPINK - 10));
			break;
		case COLOR_HYELLOW:	//亮黄
			color[7] = (char)('A' + (COLOR_HYELLOW - 10));
			break;
		case  COLOR_HWHITE:	//亮白
			color[7] = (char)('A' + (COLOR_HWHITE - 10));
			break;
		default:
			break;
	}
	system(color);
	/*设置相关颜色*/
	cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);


	/*前往打印主框架的开头位置*/
	int row_sign = 0, col_sign = 0, top_status = 0, lower_status = 0;
	/*确定上下状态栏和行列标的显示情况
	  注意：上状态栏（0/1行）/下状态栏（0/1行）/行号显示（0/2列）/列标显示区域（0/1行）*/
	if (pCGI->draw_frame_with_row_no)//需要行标显示
		row_sign = 2;
	if (pCGI->draw_frame_with_col_no)//需要列标显示
		col_sign = 1;
	if (pCGI->top_status_line)//需要上状态栏
		top_status = 1;
	if (pCGI->lower_status_line)//需要下状态栏
		lower_status = 1;
	int x = pCGI->extern_left_cols;
	int y = pCGI->extern_up_lines + top_status;
	cct_gotoxy(x, y);//移动到开头位置（含色块集和行号列标）

	/*判断是否加分隔符*/
	int x_add = 0, y_add = 0;
	if (pCGI->CFI.separator)//符加色块间的边界为中文分隔符（两字节）
	{
		x_add = 2;
		y_add = 1;
	}
	/*确立每个色块对应的边框个数*/
	int width = pCGI->CFI.block_width / 2;//每个色块对应的包裹的字符个数
	int high = pCGI->CFI.block_high;// 每个色块对应的包裹的字符个数

	/*打印框架--列标*/
	if (pCGI->draw_frame_with_col_no)//此时需要列标
	{
		for (int i = 0; i <= pCGI->col_num - 1; i++)//打印列标
		{
			cct_gotoxy(x + row_sign + 2 + (width * 2 / 2 - 1) + (width * 2 + x_add) * i, y);
			if (i >= 100)
				cout << "**";
			else
				cout << i;
		}
		y++;
		cct_gotoxy(x, y);//写了一行列标后换行
	}


	/*打印框架--色块集和行标*/
	int y_0 = y;
	int lie = 1;
	if (pCGI->CFI.separator)//此时需要分割线
		for (int i = 1; i <= pCGI->row_num; i++)
		{
			cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);//设置框架颜色
			/*每一行的排头位置*/
			cct_gotoxy(x, y);
			for (int j = 1; j <= pCGI->col_num; j++)
			{
				if (pCGI->draw_frame_with_row_no && j == 1)//此时需要行号
				{
					cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
					cout << "  ";
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				}

				if (i == 1 && j == 1)
					cout << pCGI->CFI.top_left;
				else if (j == 1 && i != 1)
					cout << pCGI->CFI.v_left_separator;
				else if (i == 1 && j != 1)
					cout << pCGI->CFI.h_top_separator;
				else
					cout << pCGI->CFI.mid_separator;
				for (int k = 1; k <= width; k++)
					cout << pCGI->CFI.h_normal;
				if (j == pCGI->col_num)//最后的位置时多加一列
				{
					if (i == 1)
						cout << pCGI->CFI.top_right;
					else
						cout << pCGI->CFI.v_right_separator;
				}
				Sleep(pCGI->delay_of_draw_frame);//画框架时延时
			}
			y++;

			/*每一行的普通位置*/
			for (int same = 1; same <= high; same++)
			{
				cct_gotoxy(x, y);
				if (pCGI->draw_frame_with_row_no)//此时需要行号
				{
					cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
					if (same == (high + 1) / 2)
					{
						if (lie >= 1 && lie <= 26)
							cout << (char)('A' + lie - 1) << ' ';
						else if (lie >= 27 && lie <= 52)
							cout << (char)('a' + lie - 1) << ' ';
						else
							cout << "* ";
						lie++;
					}
					else
						cout << "  ";
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				}
				for (int j = 1; j <= pCGI->col_num; j++)
				{
					if (j == 1)
						cout << pCGI->CFI.v_normal;
					for (int k = 1; k <= width; k++)//此位置用空格代替
						cout << "  ";
					cout << pCGI->CFI.v_normal;
					Sleep(pCGI->delay_of_draw_frame);//画框架时延时
				}
				y++;
			}

			/*特别最后一行时再加一行*/
			if (i == pCGI->row_num)
			{
				cct_gotoxy(x, y);
				if (pCGI->draw_frame_with_row_no)//此时需要行号
				{
					cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
					cout << "  ";
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				}

				cout << pCGI->CFI.lower_left;
				for (int j = 1; j <= pCGI->col_num; j++)
				{
					for (int k = 1; k <= width; k++)
						cout << pCGI->CFI.h_normal;
					if (j == pCGI->col_num)
						cout << pCGI->CFI.lower_right;
					else
						cout << pCGI->CFI.h_lower_separator;
					Sleep(pCGI->delay_of_draw_frame);//画框架时延时	
				}
				y++;
			}
		}

	else//此时无需分割线
		for (int i = 1; i <= pCGI->row_num; i++)
		{
			cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);//设置颜色
			/*第一行的排头位置*/
			if (i == 1)
			{
				cct_gotoxy(x, y);
				for (int j = 1; j <= pCGI->col_num; j++)
				{
					if (pCGI->draw_frame_with_row_no && j == 1)//此时需要行号
					{
						cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
						cout << "  ";
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					}

					if (i == 1 && j == 1)
						cout << pCGI->CFI.top_left;
					for (int k = 1; k <= width; k++)
						cout << pCGI->CFI.h_normal;
					if (i == 1 && j == pCGI->col_num)//最后的位置时多加一列
						cout << pCGI->CFI.top_right;
					Sleep(pCGI->delay_of_draw_frame);//画框架时延时
				}
				y++;
			}

			/*每一行的普通位置*/
			for (int same = 1; same <= high; same++)
			{
				cct_gotoxy(x, y);
				if (pCGI->draw_frame_with_row_no)//此时需要行号
				{
					cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
					if (same == (high + 1) / 2)
					{
						if (lie >= 1 && lie <= 26)//A-Z
							cout << (char)('A' + lie - 1) << ' ';
						else if (lie >= 27 && lie <= 52)//a-z
							cout << (char)('a' + lie - 27) << ' ';
						else//*
							cout << "* ";
						lie++;
					}
					else
						cout << "  ";
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				}
				for (int j = 1; j <= pCGI->col_num; j++)
				{
					if (j == 1)
						cout << pCGI->CFI.v_normal;
					for (int k = 1; k <= width; k++)//此位置用空格代替
						cout << "  ";
					if (j == pCGI->col_num)
						cout << pCGI->CFI.v_normal;
					Sleep(pCGI->delay_of_draw_frame);//画框架时延时
				}
				y++;
			}

			/*特别最后一行时再加一行*/
			if (i == pCGI->row_num)
			{
				cct_gotoxy(x, y);
				if (pCGI->draw_frame_with_row_no)//此时需要行号
				{
					cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
					cout << "  ";
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				}

				cout << pCGI->CFI.lower_left;
				for (int j = 1; j <= pCGI->col_num; j++)
				{
					for (int k = 1; k <= width; k++)
						cout << pCGI->CFI.h_normal;
					if (j == pCGI->col_num)
						cout << pCGI->CFI.lower_right;
					Sleep(pCGI->delay_of_draw_frame);//画框架时延时	
				}
				y++;
			}
		}

	cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//恢复初始颜色

	return 0;
}

/***************************************************************************
  函数名称：
  功    能：在状态栏上显示信息
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type							：指定是上/下状态栏
		   const char *msg						    ：正常信息
		   const char *catchy_msg					：需要特别标注的信息（在正常信息前显示）
  返 回 值：
  说    明：1、输出宽度限定为主框架的宽度（含行号列标位置），超出则截去
			2、如果最后一个字符是某汉字的前半个，会导致后面乱码，要处理
***************************************************************************/
int gmw_status_line(const CONSOLE_GRAPHICS_INFO* const pCGI, const int type, const char* msg, const char* catchy_msg)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*处理字符串长度问题*/
	int length_nor = (int)strlen(msg);//不含尾0的字符串长度
	int length_cat;
	if (catchy_msg == NULL)
		length_cat = 0;
	else
		length_cat = (int)strlen(catchy_msg);//不含尾0的字符串长度
	int max = -1;
	bool out_cat_ = true;//默认长度可以满足输出亮色信息


	/*注：先显示亮色区域的内容！！！再显示正常区域的内容*/
	if (catchy_msg != NULL && length_cat >= pCGI->SLI.width)//长度不满足cat输出
	{
		/*处理最后一个字符为汉字的情况*/
		if (msg[pCGI->SLI.width - 1] > 127)//此时末尾为汉字的情况
			max = pCGI->SLI.width - 1;
		out_cat_ = false;//此时无法输出亮色信息
	}
	else if (length_nor + length_cat >= pCGI->SLI.width)//长度不完全满足nor输出
	{
		max = pCGI->SLI.width - length_cat;
		/*处理最后一个字符为汉字的情况*/
		if (catchy_msg != NULL && catchy_msg[max - 1] > 127)
			max = max - 1;
		out_cat_ = true;//此时可以输出亮色信息
	}


	/*在状态栏上显示信息*/
	switch (type)
	{
		case TOP_STATUS_LINE://上状态栏
			if (!pCGI->top_status_line)//如果关闭上状态栏 不打印
				break;
			cct_gotoxy(pCGI->SLI.top_start_x, pCGI->SLI.top_start_y);//清除该位置的打印字符
			for (int i = 1; i <= pCGI->SLI.width; i++)
				cout << ' ';
			cct_gotoxy(pCGI->SLI.top_start_x, pCGI->SLI.top_start_y);//前往相应的输出位置
			cct_setcolor(pCGI->SLI.top_catchy_bgcolor, pCGI->SLI.top_catchy_fgcolor);//先打印亮色区域
			if (out_cat_ == false)//此时仅能输出亮色区域
				for (int i = 0; i <= max - 1; i++)
					cout << catchy_msg[i];
			else
			{
				if (catchy_msg != NULL)
					cout << catchy_msg;
				cct_setcolor(pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor);
				if (max == -1)//正常部分能去完全显示
					cout << msg;
				else
					for (int i = 0; i <= max - 1; i++)
						cout << msg[i];
			}
			cct_setcolor(pCGI->area_bgcolor, pCGI->area_bgcolor);//恢复原来颜色
			break;

		case LOWER_STATUS_LINE://下状态栏
			if (!pCGI->lower_status_line)//如果关闭下状态栏 打印
				break;
			cct_gotoxy(pCGI->SLI.lower_start_x, pCGI->SLI.lower_start_y);//清除该位置的打印字符
			for (int i = 1; i <= pCGI->SLI.width; i++)
				cout << ' ';
			cct_gotoxy(pCGI->SLI.lower_start_x, pCGI->SLI.lower_start_y);//前往相应的输出位置
			cct_setcolor(pCGI->SLI.lower_catchy_bgcolor, pCGI->SLI.lower_catchy_fgcolor);//先打印亮色区域
			if (out_cat_ == false)//此时仅能输出亮色区域
				for (int i = 0; i <= max - 1; i++)
					cout << catchy_msg[i];
			else
			{
				if (catchy_msg != NULL)
					cout << catchy_msg;
				cct_setcolor(pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor);
				if (max == -1)//正常部分能去完全显示
					cout << msg;
				else
					for (int i = 0; i <= max - 1; i++)
						cout << msg[i];
			}
			cct_setcolor(pCGI->area_bgcolor, pCGI->area_bgcolor);//恢复原来颜色
			break;

		default:
			return -1;
	}

	cout << endl;//清空输出缓冲区 避免数据残留打印错误！！！！！

	return 0;
}

/***************************************************************************
  函数名称：
  功    能：显示某一个色块(内容为字符串，坐标为row/col)
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						    ：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						    ：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const BLOCK_DISPLAY_INFO *const bdi		：存放该值对应的显示信息的结构体数组
  返 回 值：
  说    明：1、BLOCK_DISPLAY_INFO 的含义见头文件，用法参考测试样例
			2、bdi_value为 BDI_VALUE_BLANK 表示空白块，要特殊处理
***************************************************************************/
int gmw_draw_block(const CONSOLE_GRAPHICS_INFO* const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO* const bdi)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*value表示需要打印的色块数组的位置*/
	int value = 0;
	while (bdi[value].value != BDI_VALUE_END)
	{
		if (bdi[value].value == bdi_value)
			break;
		value++;

	}

	int x_add = 0, y_add = 0;
	if (pCGI->CFI.separator)//符加色块间的边界为中文分隔符（两字节）
	{
		x_add = 2;
		y_add = 1;
	}
	/*移动光标到对应位置*/
	/*注：start_x和start_y的是游戏主页面的排头位置*/
	int x = pCGI->start_x + 2 + col_no * (pCGI->CFI.block_width + x_add);
	int y = pCGI->start_y + 1 + row_no * (pCGI->CFI.block_high + y_add);
	cct_gotoxy(x, y);//前往所需打印的排头位置

	/*进行打印*/

	/*更改颜色*/
	int color_bg = 0, color_fg = 0;
	if (bdi[value].bgcolor == -1)//默认背景色
		color_bg = pCGI->CFI.bgcolor;//颜色为框架的颜色
	else
		color_bg = bdi[value].bgcolor;
	if (bdi[value].fgcolor == -1)//默认背景色
		color_fg = pCGI->CFI.fgcolor;//颜色为框架的颜色
	else
		color_fg = bdi[value].fgcolor;
	cct_setcolor(color_bg, color_fg);//设置颜色

	/*无需判断一波 打印边框的要求是否合理 将认为保证色块打印合理*/
	//if (pCGI->CBI.block_border && ((pCGI->CFI.block_high < 3) || (pCGI->CFI.block_width < 6) || (int)(strlen(bdi->content) > (pCGI->CFI.block_width / 2 - 2))))
		//return -1;

	/*进行字符的打印*/
	if (bdi_value == BDI_VALUE_BLANK)//特判需要打印空白块的时候
	{
		for (int j = 1; j <= pCGI->CFI.block_high; j++)
		{
			cct_gotoxy(x, y);
			for (int i = 1; i <= pCGI->CFI.block_width; i++)
				cout << ' ';
			Sleep(pCGI->delay_of_draw_block);//打印色块时延时
			y++;
		}
	}
	else//非空白块的时候
	{
		/*计算需要打印色块的总长宽*/
		int width = pCGI->CFI.block_width;
		int high = pCGI->CFI.block_high;

		/*需要边框时*/
		if (pCGI->CBI.block_border)//注：人为保证需要边框时，字符的width>=6&&字符的high>=3!!!!!!
		{
			/*计算内数组值的位数（默认内部数组值的长度在能显示的长度范围内！！！）*/
			int site = 1;//表示内部数组值的位数
			int mom = 10;
			if (bdi[value].content == NULL)//需要打印内部数组
				while (1)
				{
					if (bdi[value].value / mom == 0)
						break;
					mom = mom * 10;
					site++;
				}

			cct_gotoxy(x, y);
			for (int i = 1; i <= width / 2; i++)//第一行
			{
				if (i == 1)
					cout << pCGI->CBI.top_left;
				else if (i == width / 2)
					cout << pCGI->CBI.top_right;
				else
					cout << pCGI->CBI.h_normal;
				Sleep(pCGI->delay_of_draw_block);//打印色块时延时
			}
			y++;;

			for (int k = 1; k <= (high - 2); k++)//其余普通行
			{
				cct_gotoxy(x, y);
				for (int i = 1; i <= width; i++)
				{
					if (i == 1)
					{
						cout << pCGI->CBI.v_normal;//打印边框
						i++;//补上一个位数
					}
					else if (i == width - 1)
					{
						cout << pCGI->CBI.v_normal;//打印边框
						i++;//补上一个位数
					}
					else
					{
						if (bdi[value].content == NULL)//打印内部数组值
						{
							if (k == ((high - 2 + 1) / 2) && i == ((width - 4 - site) / 2 + 1) + 2)//处于相对中心位置
							{
								/*内部数组值存在多种位数情况！！！*/
								cout << bdi[value].value;
								i = i + site - 1;//补上相应的位数
							}
							else
								cout << ' ';//空格
						}
						else//打印对应图形(注意：本处默认为两字节的中文字符图形！！！！)
						{
							if (k == ((high - 2) / 2 + 1) && i == width / 2)//处于中心位置
							{
								cout << bdi[value].content;
								i++;//打印的为两字节 故需要补充一个空格位
							}
							else
								cout << ' ';//空格
						}
					}
					Sleep(pCGI->delay_of_draw_block);//打印色块时延时
				}
				y++;
			}

			cct_gotoxy(x, y);
			for (int i = 1; i <= width / 2; i++)//最后一行
			{

				if (i == 1)
					cout << pCGI->CBI.lower_left;
				else if (i == width / 2)
					cout << pCGI->CBI.lower_right;
				else
					cout << pCGI->CBI.h_normal;
				Sleep(pCGI->delay_of_draw_block);//打印色块时延时

			}
			y++;
		}

		/*无需边框时*/
		else
		{
			/*计算内数组值的位数（默认内部数组值的长度在能显示的长度范围内！！！）*/
			int site = 1;//表示内部数组值的位数
			int mom = 10;
			if (bdi[value].content == NULL)//需要打印内部数组
				while (1)
				{
					if (bdi[value].value / mom == 0)
						break;
					mom = mom * 10;
					site++;
				}
			for (int j = 1; j <= high; j++)
			{
				cct_gotoxy(x, y);
				for (int i = 1; i <= width; i++)
				{
					if (bdi[value].content == NULL)//打印内部数组值
					{
						if (j == ((high + 1) / 2) && i == ((width - site) / 2 + 1))//处于相对中心位置
						{
							/*内部数组值存在多种位数情况！！！*/
							cout << bdi[value].value;
							i = i + site - 1;//补上相应的位数
						}
						else
							cout << ' ';//空格
					}
					else//打印对应图形(注意：本处默认为两字节的中文字符图形！！！！)
					{
						if (j == ((high + 1) / 2) && i == width / 2)//处于中心位置
						{

							cout << bdi[value].content;
							i++;//打印的为两字节 故需要补充一个空格位
						}
						else
							cout << ' ';//空格
					}
					Sleep(pCGI->delay_of_draw_block);//打印色块时延时
				}
				y++;
			}
		}
	}

	cct_setcolor(pCGI->area_bgcolor, pCGI->area_bgcolor);//恢复背景颜色设置

	return 0;
}
/***************************************************************************
  函数名称：
  功    能：在某个位置(x,y)显示一个色块
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int bdi_value						：需要显示的值
		   int const x_0                            ：需要打印的位置的x坐标
		   int const y_0                            ：需要打印的位置的y坐标
		   const int blank_bdi_value				：移动过程中用于动画效果显示时用于表示空白的值（一般为0，此处做为参数代入，是考虑到可能出现的特殊情况）
		   const BLOCK_DISPLAY_INFO *const bdi		：存放显示值/空白值对应的显示信息的结构体数组
  返 回 值：
  说    明：bdi_value为 BDI_VALUE_BLANK 表示空白块，要特殊处理
***************************************************************************/
static int gmw_inner_block_move(const CONSOLE_GRAPHICS_INFO* const pCGI, int const x_0, int const y_0, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO* const bdi)
{

	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*value表示需要打印的色块数组的位置*/
	int value = 0;
	while (bdi[value].value != BDI_VALUE_END)
	{
		if (bdi[value].value == bdi_value)
			break;
		value++;

	}

	/*更改颜色*/
	int color_bg = 0, color_fg = 0;
	if (bdi[value].bgcolor == -1)//默认背景色
		color_bg = pCGI->CFI.bgcolor;//颜色为框架的颜色
	else
		color_bg = bdi[value].bgcolor;
	if (bdi[value].fgcolor == -1)//默认背景色
		color_fg = pCGI->CFI.fgcolor;//颜色为框架的颜色
	else
		color_fg = bdi[value].fgcolor;
	cct_setcolor(color_bg, color_fg);

	int x = x_0, y = y_0;

	/*进行字符的打印*/
	if (bdi_value == BDI_VALUE_BLANK)//特判需要打印空白块的时候
	{
		for (int j = 1; j <= pCGI->CFI.block_high; j++)
		{
			cct_gotoxy(x, y);
			for (int i = 1; i <= pCGI->CFI.block_width; i++)
				cout << ' ';
			Sleep(pCGI->delay_of_draw_block);//打印色块时延时
			y++;
		}
	}
	else//非空白块的时候
	{
		/*计算需要打印色块的总长宽*/
		int width = pCGI->CFI.block_width;
		int high = pCGI->CFI.block_high;

		/*需要边框时*/
		if (pCGI->CBI.block_border)//注：人为保证需要边框时，字符的width>=6&&字符的high>=3!!!!!!
		{
			/*计算内数组值的位数（默认内部数组值的长度在能显示的长度范围内！！！）*/
			int site = 1;//表示内部数组值的位数
			int mom = 10;
			if (bdi[value].content == NULL)//需要打印内部数组
				while (1)
				{
					if (bdi[value].value / mom == 0)
						break;
					mom = mom * 10;
					site++;
				}

			cct_gotoxy(x, y);
			for (int i = 1; i <= width / 2; i++)//第一行
			{
				if (i == 1)
					cout << pCGI->CBI.top_left;
				else if (i == width / 2)
					cout << pCGI->CBI.top_right;
				else
					cout << pCGI->CBI.h_normal;
				Sleep(pCGI->delay_of_draw_block);//打印色块时延时
			}
			y++;;

			for (int k = 1; k <= (high - 2); k++)//其余普通行
			{
				cct_gotoxy(x, y);
				for (int i = 1; i <= width; i++)
				{
					if (i == 1)
					{
						cout << pCGI->CBI.v_normal;//打印边框
						i++;//补上一个位数
					}
					else if (i == width - 1)
					{
						cout << pCGI->CBI.v_normal;//打印边框
						i++;//补上一个位数
					}
					else
					{
						if (bdi[value].content == NULL)//打印内部数组值
						{
							if (k == ((high - 2 + 1) / 2) && i == ((width - 4 - site) / 2 + 1) + 2)//处于相对中心位置
							{
								/*内部数组值存在多种位数情况！！！*/
								cout << bdi[value].value;
								i = i + site - 1;//补上相应的位数
							}
							else
								cout << ' ';//空格
						}
						else//打印对应图形(注意：本处默认为两字节的中文字符图形！！！！)
						{
							if (k == ((high - 2) / 2 + 1) && i == width / 2)//处于中心位置
							{
								cout << bdi[value].content;
								i++;//打印的为两字节 故需要补充一个空格位
							}
							else
								cout << ' ';//空格
						}
					}
					Sleep(pCGI->delay_of_draw_block);//打印色块时延时
				}
				y++;
			}

			cct_gotoxy(x, y);
			for (int i = 1; i <= width / 2; i++)//最后一行
			{

				if (i == 1)
					cout << pCGI->CBI.lower_left;
				else if (i == width / 2)
					cout << pCGI->CBI.lower_right;
				else
					cout << pCGI->CBI.h_normal;
				Sleep(pCGI->delay_of_draw_block);//打印色块时延时

			}
			y++;
		}

		/*无需边框时*/
		else
		{
			/*计算内数组值的位数（默认内部数组值的长度在能显示的长度范围内！！！）*/
			int site = 1;//表示内部数组值的位数
			int mom = 10;
			if (bdi[value].content == NULL)//需要打印内部数组
				while (1)
				{
					if (bdi[value].value / mom == 0)
						break;
					mom = mom * 10;
					site++;
				}
			for (int j = 1; j <= high; j++)
			{
				cct_gotoxy(x, y);
				for (int i = 1; i <= width; i++)
				{
					if (bdi[value].content == NULL)//打印内部数组值
					{
						if (j == ((high + 1) / 2) && i == ((width - site) / 2 + 1))//处于相对中心位置
						{
							/*内部数组值存在多种位数情况！！！*/
							cout << bdi[value].value;
							i = i + site - 1;//补上相应的位数
						}
						else
							cout << ' ';//空格
					}
					else//打印对应图形(注意：本处默认为两字节的中文字符图形！！！！)
					{
						if (j == ((high + 1) / 2) && i == width / 2)//处于中心位置
						{
							cout << bdi[value].content;
							i++;//打印的为两字节 故需要补充一个空格位
						}
						else
							cout << ' ';//空格
					}
					Sleep(pCGI->delay_of_draw_block);//打印色块时延时
				}
				y++;
			}
		}
	}

	cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//恢复默认的背景颜色

	return 0;
}

/***************************************************************************
  函数名称：
  功    能：移动某一个色块
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						    ：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						    ：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const int blank_bdi_value				：移动过程中用于动画效果显示时用于表示空白的值（一般为0，此处做为参数代入，是考虑到可能出现的特殊情况）
		   const BLOCK_DISPLAY_INFO *const bdi		：存放显示值/空白值对应的显示信息的结构体数组
		   const int direction						：移动方向，一共四种，具体见cmd_gmw_tools.h
		   const int distance						：移动距离（从1开始，人为保证正确性，程序不检查）
  返 回 值：
  说    明：
***************************************************************************/
int gmw_move_block(const CONSOLE_GRAPHICS_INFO* const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO* const bdi, const int direction, const int distance)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	int x_add = 0, y_add = 0;
	if (pCGI->CFI.separator)//符加色块间的边界为中文分隔符（两字节）
	{
		x_add = 2;
		y_add = 1;
	}

	/*移动光标到对应位置*/
	/*注：start_x和start_y的是游戏主页面的排头位置*/
	int x = pCGI->start_x + 2 + col_no * (pCGI->CFI.block_width + x_add);
	int y = pCGI->start_y + 1 + row_no * (pCGI->CFI.block_high + y_add);
	cct_gotoxy(x, y);//光标前往所需打印的排头位置

	int distance_1 = 0;
	/*开始移动（人为保证移动距离的正确性！！！）*/
	if (pCGI->CFI.separator)//此时有分隔线的情况
	{

		switch (direction)
		{
			case DOWN_TO_UP://有分割线 上移
				distance_1 = distance + distance * pCGI->CFI.block_high;
				for (int dis = 1; dis <= distance_1; dis++)
				{
					y--;
					gmw_inner_block_move(pCGI, x, y, bdi_value, blank_bdi_value, bdi);//上移动一格并打印色块
					if (dis % (pCGI->CFI.block_high + y_add) == 0)//当移动满一格时
					{
						cct_gotoxy(x, (y + pCGI->CFI.block_high));
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);//使用框架的颜色设置
						for (int k = 1; k <= (pCGI->CFI.block_width / 2); k++)
							cout << pCGI->CFI.h_normal;
						cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//恢复默认颜色
					}
					else//当移动未满一格时
					{
						cct_gotoxy(x, (y + pCGI->CFI.block_high));
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);//使用框架的颜色设置
						for (int k = 1; k <= (pCGI->CFI.block_width / 2); k++)
							cout << "  ";//一次两格
						cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//恢复默认颜色
					}
					Sleep(pCGI->delay_of_block_moved);//移动延时

				}
				break;

			case UP_TO_DOWN://有分割线 下移
				distance_1 = distance + distance * pCGI->CFI.block_high;
				for (int dis = 1; dis <= distance_1; dis++)
				{
					y++;
					gmw_inner_block_move(pCGI, x, y, bdi_value, blank_bdi_value, bdi);//下移动一格并打印色块
					if (dis % (pCGI->CFI.block_high + y_add) == 0)//当移动满一格时
					{
						cct_gotoxy(x, (y - 1));
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);//使用色块集的颜色选择
						for (int k = 1; k <= (pCGI->CFI.block_width / 2); k++)
							cout << pCGI->CFI.h_normal;
						cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//恢复默认颜色
					}
					else//当移动未满一格时
					{
						cct_gotoxy(x, (y - 1));
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						for (int k = 1; k <= (pCGI->CFI.block_width / 2); k++)
							cout << "  ";//一次两格
						cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//恢复默认颜色
					}
					Sleep(pCGI->delay_of_block_moved);//移动延时
				}
				break;

			case RIGHT_TO_LEFT://有分割线 左移
				distance_1 = distance + distance * (pCGI->CFI.block_width / 2);
				for (int dis = 1; dis <= distance_1; dis++)
				{
					/*一次移动两格*/
					x--;
					x--;
					gmw_inner_block_move(pCGI, x, y, bdi_value, blank_bdi_value, bdi);//左移动一格并打印色块
					if (dis % ((pCGI->CFI.block_width + x_add) / 2) == 0)//当移动满一格时
					{
						cct_gotoxy((x + pCGI->CFI.block_width), y);
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						for (int k = 1; k <= pCGI->CFI.block_high; k++)
						{
							cct_gotoxy((x + pCGI->CFI.block_width), (y + k - 1));
							cout << pCGI->CFI.v_normal;
						}
						cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//恢复默认颜色
					}
					else//当移动未满一格时
					{
						cct_gotoxy((x + pCGI->CFI.block_width), y);
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						for (int k = 1; k <= pCGI->CFI.block_high; k++)
						{
							cct_gotoxy((x + pCGI->CFI.block_width), (y + k - 1));
							cout << "  ";//一次两格
						}
						cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//恢复默认颜色
					}
					Sleep(pCGI->delay_of_block_moved);//移动延时
				}
				break;

			case LEFT_TO_RIGHT://有分割线 右移
				distance_1 = distance + distance * (pCGI->CFI.block_width / 2);
				for (int dis = 1; dis <= distance_1; dis++)
				{
					x++;
					x++;
					gmw_inner_block_move(pCGI, x, y, bdi_value, blank_bdi_value, bdi);//右移动一格并打印色块
					if (dis % ((pCGI->CFI.block_width + x_add) / 2) == 0)//当移动满一格时
					{
						cct_gotoxy((x - 2), y);
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						for (int k = 1; k <= pCGI->CFI.block_high; k++)
						{
							cct_gotoxy((x - 2), (y + k - 1));
							cout << pCGI->CFI.v_normal;
						}
						cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//恢复默认颜色
					}
					else//当移动未满一格时
					{
						cct_gotoxy((x - 2), y);
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						for (int k = 1; k <= pCGI->CFI.block_high; k++)
						{
							cct_gotoxy((x - 2), (y + k - 1));
							cout << "  ";//一次两格
						}
						cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//恢复默认颜色
					}
					Sleep(pCGI->delay_of_block_moved);//移动延时
				}
				break;

			default:
				return -1;

		}
	}

	else//此时为无分隔线的情况
	{
		switch (direction)
		{
			case DOWN_TO_UP://无分割线上移
				distance_1 = pCGI->CFI.block_high * distance;
				for (int dis = 1; dis <= distance_1; dis++)
				{
					y--;
					gmw_inner_block_move(pCGI, x, y, bdi_value, blank_bdi_value, bdi);//上移动一格并打印色块
					cct_gotoxy(x, (y + pCGI->CFI.block_high));
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					for (int k = 1; k <= (pCGI->CFI.block_width / 2); k++)//填补移动的空缺
						cout << "  ";//一次两格
					cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//恢复默认颜色
					Sleep(pCGI->delay_of_block_moved);//移动延时
				}
				break;

			case UP_TO_DOWN://无分割线下移
				distance_1 = pCGI->CFI.block_high * distance;
				for (int dis = 1; dis <= distance_1; dis++)
				{
					y++;
					gmw_inner_block_move(pCGI, x, y, bdi_value, blank_bdi_value, bdi);//下移动一格并打印色块
					cct_gotoxy(x, (y - 1));
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					for (int k = 1; k <= (pCGI->CFI.block_width / 2); k++)//填补移动的空缺
						cout << "  ";//一次两格
					cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//恢复默认颜色
					Sleep(pCGI->delay_of_block_moved);//移动延时
				}
				break;

			case RIGHT_TO_LEFT://无分割线左移
				distance_1 = (pCGI->CFI.block_width / 2) * distance;
				for (int dis = 1; dis <= distance_1; dis++)
				{
					/*一次移动两格*/
					x--;
					x--;
					gmw_inner_block_move(pCGI, x, y, bdi_value, blank_bdi_value, bdi);//左移动一格并打印色块
					cct_gotoxy((x + pCGI->CFI.block_width), y);
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					for (int k = 1; k <= pCGI->CFI.block_high; k++)
					{
						cct_gotoxy((x + pCGI->CFI.block_width), (y + k - 1));
						cout << "  ";//一次两格
					}
					cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//恢复默认颜色
					Sleep(pCGI->delay_of_block_moved);//移动延时
				}
				break;

			case LEFT_TO_RIGHT://无分割线右移
				distance_1 = (pCGI->CFI.block_width / 2) * distance;
				for (int dis = 1; dis <= distance_1; dis++)
				{
					/*一次移动两格*/
					x++;
					x++;
					gmw_inner_block_move(pCGI, x, y, bdi_value, blank_bdi_value, bdi);//左移动一格并打印色块
					cct_gotoxy((x - 2), y);
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					for (int k = 1; k <= pCGI->CFI.block_high; k++)
					{
						cct_gotoxy((x - 2), (y + k - 1));
						cout << "  ";//一次两格
					}
					cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//恢复默认颜色
					Sleep(pCGI->delay_of_block_moved);//移动延时
				}
				break;

			default:
				return -1;
		}

	}

	return 0;
}


/***************************************************************************
  函数名称：
  功    能：根据坐标，判断所处的行列位置
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   int const x_0                            ：输入的位置的x坐标
		   int const y_0                            ：输入的位置的y坐标
		   char &row                                ：得到的所处位置排数
		   int &col                                 ：得到的所处位置列数
  返 回 值：
  说    明：如果row/col均为-1，则此时处于非法位置
***************************************************************************/
static int gmw_inner_judge_hl(const CONSOLE_GRAPHICS_INFO* const pCGI, const int x_0, const int y_0, int& row, int& col)
{
	/*注：分隔符默认为两字节中文字符*/

	/*初始排头位置坐标*/
	int x = pCGI->start_x + 2;
	int y = pCGI->start_y + 1;

	int block_row = -1;//相应的行号
	int block_col = -1;//相应的列标

	/*列标*/
	for (int i = 1; i <= pCGI->col_num; i++)
	{
		if (x_0 >= x && x_0 <= (x + pCGI->CFI.block_width - 1))
		{
			block_col = i - 1;
			break;
		}
		if (pCGI->CFI.separator)
			x = x + (pCGI->CFI.block_width + pCGI->CFI.block_width_ext);
		else
			x = x + pCGI->CFI.block_width;
	}

	/*行号*/
	for (int j = 1; j <= pCGI->row_num; j++)
	{
		if (y_0 >= y && y_0 <= (y + pCGI->CFI.block_high - 1))
		{
			block_row = j - 1;
			break;
		}
		if (pCGI->CFI.separator)
			y = y + (pCGI->CFI.block_high + pCGI->CFI.block_high_ext);
		else
			y = y + pCGI->CFI.block_high;
	}

	/*进行引用的赋值*/
	row = block_row;
	col = block_col;

	return 0;

}


/***************************************************************************
  函数名称：
  功    能：读键盘或鼠标
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针

		   int &MAction							    ：如果返回 CCT_MOUSE_EVENT，则此值有效，为 MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK 三者之一
													  如果返回 CCT_KEYBOARD_EVENT，则此值无效

		   int &MRow								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的行号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）

		   int &MCol								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的列号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）

		   int &KeyCode1							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第一个）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）

		   int &KeyCode2							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第二个，如果是单键码，则为0）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）

		   const bool update_lower_status_line		：鼠标移动时，是否要在本函数中显示"[当前光标] *行*列/位置非法"的信息（true=显示，false=不显示，缺省为true）

  返 回 值：函数返回约定
		   1、如果是鼠标移动，得到的MRow/MCol与传入的相同(鼠标指针微小的移动)，则不返回，继续读
							  得到行列非法位置，则不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"
							  得到的MRow/MCol与传入的不同(行列至少一个变化)，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] *行*列"，再返回MOUSE_ONLY_MOVED（有些游戏返回后要处理色块的不同颜色显示）

		   2、如果是按下鼠标左键，且当前鼠标指针停留在主游戏区域的*行*列上，则返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_LEFT_BUTTON_CLICK, MRow 为行号，MCol 为列标
								  且当前鼠标指针停留在非法区域（非游戏区域，游戏区域中的分隔线），则不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"

		   3、如果是按下鼠标右键，则不判断鼠标指针停留区域是否合法，直接返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_RIGHT_BUTTON_CLICK, MRow、MCol取当前值

		   4、如果按下键盘上的某键（含双键码按键），则直接返回 CCT_KEYBOARD_EVENT，KeyCode1/KeyCode2中为对应的键码值

 说    明：通过调用 cmd_console_tools.cpp 中的 read_keyboard_and_mouse 函数实现
***************************************************************************/
int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO* const pCGI, int& MAction, int& MRow, int& MCol, int& KeyCode1, int& KeyCode2, const bool update_lower_status_line)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*部分基础设置*/
	cct_enable_mouse();//允许使用鼠标
	cct_setcursor(CURSOR_INVISIBLE);//取消使用光标

	/*要读取的位置*/
	int col = -1;
	int row = -1;
	/*传入的位置*/
	int old_row = MRow;
	int old_col = MCol;
	/*避免[当前光标]的额外多次打印！！！*/
	int sign = 0;
	int sign_site = 0;

	while (1)
	{
		int x, y;
		int back = cct_read_keyboard_and_mouse(x, y, MAction, KeyCode1, KeyCode2);//读取鼠标和键盘事件
		switch (back)
		{
			case CCT_MOUSE_EVENT:
				gmw_inner_judge_hl(pCGI, x, y, row, col);//根据坐标得出相应的位置编号

				if (MAction == MOUSE_RIGHT_BUTTON_CLICK)//如果是按下鼠标右键，则不判断鼠标指针停留区域是否合法，直接返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_RIGHT_BUTTON_CLICK, MRow、MCol取当前值
				{
					gmw_inner_judge_hl(pCGI, x, y, MRow, MCol);//直接取当前值
					return CCT_MOUSE_EVENT;
				}

				else//此时为按下鼠标左键和鼠标移动
				{
					if (row == -1 || col == -1)//此时处于非法位置处
					{
						cct_setcolor(pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor);//设置为下状态栏的颜色
						cct_gotoxy(pCGI->SLI.lower_start_x+sign_site, pCGI->SLI.lower_start_y);//前往下状态栏处
						for (int i = 1; i <= pCGI->SLI.width; i++)//清除该位置的打印字符
							cout << ' ';
						cct_gotoxy(pCGI->SLI.lower_start_x + sign_site, pCGI->SLI.lower_start_y);//前往下状态栏处
						if (sign == 0)
						{
							cout << "[当前光标]" << " ";
							sign = 1;
							sign_site = 11;
						}
						cout << "位置非法";
						cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//恢复原来的颜色

						break;//继续读取

					}

					else if ((old_row == row) && (old_col == col) && (MAction == MOUSE_ONLY_MOVED))
						break;//鼠标移动、横纵编号未改变、读取位置正确---重新读取

					else//此时处于合理位置处且横纵编号改变
					{
						cct_setcolor(pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor);//设置为下状态栏的颜色
						cct_gotoxy(pCGI->SLI.lower_start_x+sign_site, pCGI->SLI.lower_start_y);//前往下状态栏处
						for (int i = 1; i <= pCGI->SLI.width; i++)//清除该位置的打印字符
							cout << ' ';
						cct_gotoxy(pCGI->SLI.lower_start_x+sign_site, pCGI->SLI.lower_start_y);//前往下状态栏处

						if (sign == 0)//保证仅打印一次光标标志
						{
							cout << "[当前光标]" << " ";
							sign = 1;
							sign_site = 11;
						}

						int row_0 = row + 1;
						if (row_0 >= 1 && row_0 <= 26)
							cout << (char)('A' + row_0 - 1);
						else if (row_0 >= 27 && row_0 <= 52)
							cout << (char)('a' + row_0 - 27);
						else
							cout << '*';
						cout << "行";
						if (col >= 100)
							cout << '*' << "列";
						else
							cout << col << "列";
						cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//恢复原来的颜色
						/*进行赋值*/
						MRow = row;
						MCol = col;
						return CCT_MOUSE_EVENT;//此时均返回鼠标事件

					}

				}
				break;

			case CCT_KEYBOARD_EVENT://如果按下键盘上的某键（含双键码按键），则直接返回 CCT_KEYBOARD_EVENT，KeyCode1/KeyCode2中为对应的键码值
				return CCT_KEYBOARD_EVENT;

			default:
				break;
		}

	}

	return 0;
}
