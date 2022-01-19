/* 2052532 龚哲飞 信04 */

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include "../include/cmd_hdc_tools.h"
using namespace std;

#if !HDC_SERIES_BY_TEACHER	//未定义的情况下才打开条件编译

/* 允许添加需要的头文件 */
#include <cmath>
#include <stdlib.h>

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow(); //VS中此处会有波浪线，不影响正常使用

/* 已定义的静态全局变量 */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;
static int _Delay_ms;

/* 允许适度使用静态全局变量，但仅限static，即外部不可见 */

/* 此处允许添加自定义函数，但仅限static，即外部不可见 */
/***************************************************************************
  函数名称：
  功    能：将角度转换为系统默认坐标角度
  输入参数：const int begin 
            const int end
		    int& start
			int& over
  返 回 值：
  说    明：
***************************************************************************/
static void angle_change(const int begin, const int end, int& start, int& over)
{
	/*start和over为系统的标准旋转方向和参数坐标
	系统标准方向坐标：顺序针 + 水平x轴方向为正方向*/
	int temp_begin = begin - ANGLE_BETWEEN;
	int temp_end = end - ANGLE_BETWEEN;
	/*将数据限定在[0,360)的区间内*/
	while (1) {
		if (temp_begin < 0) {
			temp_begin += 360;
			continue;
		}
		else if (temp_begin >= 360) {
			temp_begin -= 360;
			continue;
		}
		else
			break;
	}
	while (1) {
		if (temp_end < 0) {
			temp_end += 360;
			continue;
		}
		else if (temp_end >= 360) {
			temp_end -= 360;
			continue;
		}
		else
			break;
	}

	start = temp_begin;
	over = temp_end;
}

/* 下面给出了几个基本函数的完整实现，不要改动 */
/***************************************************************************
  函数名称：
  功    能：初始化
  输入参数：const int bgcolor：背景色
            const int fgcolor：前景色
			const int width  ：屏幕宽度（点阵）
			const int high   ：屏幕高度（点阵）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high)
{
	/* 先释放，防止不release而再次init（hdc_release可重入） */
	hdc_release();

	/* 窗口init后，用一个静态全局量记录，后续hdc_cls()会用到 */
	_BgColor_ = bgcolor;
	_FgColor_ = fgcolor;
	_Width_ = width;
	_High = high;

	hdc = GetDC(hWnd);

	cct_setcursor(CURSOR_INVISIBLE);
	cct_setcolor(bgcolor, fgcolor);
	cct_setfontsize("点阵字体", 16);
	cct_setconsoleborder(width / 8 + !!(width % 8), high / 16 + !!(high % 16)); //将点阵的宽度及高度转换为特定字体的行列数，!!的含义：如果不是8/16的倍数，行列多+1
	cct_cls();
}

/***************************************************************************
  函数名称：
  功    能：释放画图资源
  输入参数：
  返 回 值：
  说    明：可重入
***************************************************************************/
void hdc_release()
{
	if (hdc) {
		ReleaseDC(hWnd, hdc);
		hdc = NULL;
		cct_setcursor(CURSOR_VISIBLE_NORMAL);
	}
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_delay(int ms)
{
	if (ms > 0)
		_Delay_ms = ms;
	else
		_Delay_ms = 0;
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB值
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB三色，值0-255
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  函数名称：
  功    能：清除屏幕上现有的图形
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_cls()
{
    /* 发现一定要换一种颜色初始化才能清除像素点，找到更简便方法的同学可以通知我 */
	hdc_init(_BgColor_, (_FgColor_  + 1) % 16, _Width_, _High);

	hdc_init(_BgColor_, _FgColor_, _Width_, _High);

	/* 部分机器上运行demo时，会出现hdc_cls()后第一根线有缺线的情况，加延时即可
	   如果部分机器运行还有问题，调高此延时值 */
	Sleep(30);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画出一个像素点
  输入参数：const int x：x坐标，左上角为(0,0)
            const int y：y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定，可以在调用处指定
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
	MoveToEx(hdc, x - 1, y - 1, NULL);
	LineTo(hdc, x, y);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  函数名称：
  功    能：在(x1,y1)-(x2,y2)之间画出一个像素点的连线
  输入参数：const int x1：起点x坐标，左上角为(0,0)
			const int y1：起点y坐标，左上角为(0,0)
			const int x2：终点y坐标，左上角为(0,0)
			const int y2：终点y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画一个指定粗细的点(用画实心圆来模拟)
  输入参数：const int x			：x坐标，左上角为(0,0)
            const int y			：y坐标，左上角为(0,0)
			const int thickness	：点的粗细，下限位1，上限不限(如过大会导致不完全填充)（有缺省值）
			const int RGB_value	：点的颜色（有缺省值）
  返 回 值：
  说    明：改进运行效率
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//处理下限
	/* 不卡点直径的上限，但是单上限超过一定大小是，画出的圆部分位置未实心 */

	const int tn_end = (tn <= 1) ? 1 : tn/2;

	int angle, level;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	/* 用画圆的方式模拟粗点 */
	for (level = 1; level <= tn_end; level++) {
		for (angle = 0; angle <= 360; angle ++) {
			/* 注意，系统的坐标轴，0°在圆的最下方 */
			x1 = x + (int)(level * sin(angle * PI / 180));
			y1 = y - (int)(level * cos(angle * PI / 180));

			/* 当半径很小时，角度变化不会引起int型的x1/y1变化，因此加判断语句，避免重复画同一像素点 */
			if (x1 != old_x1 || y1 != old_y1) {
				old_x1 = x1;
				old_y1 = y1;
				hdc_base_point(x1, y1);
				++count;
			}
		}
	}
#if 0
	/* 放开此注释，可以看到一个粗点用了几个像素点组成 */
	cct_gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
}

/* -------------------------------------------------------------------------
	给出下列函数的实现，函数名及参数表不准改动
	1、不需要调用系统的其他有关图形操作的函数
	2、下列函数的实现都基于hdc_base_point/hdc_base_line函数的组合
	3、想一想，是不是所有函数都需要给出独立的实现过程？应该先实现哪些函数？
	4、填充是实现中最复杂的部分
	5、系统的角度与函数参数中的角度含义相差180°
   ------------------------------------------------------------------------- */

/***************************************************************************
  函数名称：
  功    能：画线段
  输入参数：const int x1		：起点的x
            const int y1		：起点的y
			const int x2		：终点的x
			const int y2		：终点的y
			const int thickness	：线段的粗细（有缺省值）
			const int RGB_value	：线段的颜色（有缺省值）
  返 回 值：
  说    明：本函数可以用基于hdc_point的组合（速度慢）
                  也可以用hdc_base_point/hdc_base_line的组合（速度快）
***************************************************************************/
void hdc_line(const int x1, const int y1, const int x2, const int y2, const int thickness, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);//设置画笔颜色

	/*横平竖直的线*/
	if (x1 == x2) {
		for (int num = 0; num < thickness; num++)
			hdc_base_line(x1+num, y1, x2+num, y2);
	}
	else if (y1 == y2) {
		for (int num = 0; num < thickness; num++)
			hdc_base_line(x1, y1 + num, x2, y2 + num);
	}
	/*斜线*/
	double k = fabs(((double)y1 - (double)y2) / ((double)x1 - (double)x2));
	int thick_min = 0;//使得宽度为左右均加的
	int thick_max = 0;//使得宽度为左右均加的
	if (thickness % 2 == 1) {//为奇数
		thick_min = -(thickness - 1) / 2;
		thick_max = (thickness - 1) / 2;
	}
	else {//为偶数
		thick_min = -(thickness / 2 - 1);
		thick_max = thickness / 2;
	}
	if (fabs(k) - 1 >= 1e-6)//倾角大于45度
		for (int num = thick_min; num <= thick_max; num++)
			hdc_base_line(x1 + num, y1, x2 + num, y2);
	else//倾角小于45度
		for (int num = thick_min; num <= thick_max; num++)
			hdc_base_line(x1, y1 + num, x2, y2 + num);

	return;
}

/***************************************************************************
  函数名称：
  功    能：给出三点的坐标，画一个三角形
  输入参数：const int x1		：第1个点的x
            const int y1		：第1个点的y
			const int x2		：第2个点的x
			const int y2		：第2个点的y
			const int x3		：第3个点的x
			const int y3		：第3个点的y
			bool filled			：是否需要填充（有缺省值）
			const int thickness	：边的粗细（有缺省值）
			const int RGB_value	：颜色（有缺省值）
  返 回 值：
  说    明：不判断三点是否共线，如果共线，划出一条直线即可
***************************************************************************/
void hdc_triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, bool filled, const int thickness, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);//设置画笔颜色

	/*先画边框*/
	hdc_line(x1, y1, x2, y2, thickness, RGB_value);
	hdc_line(x2, y2, x3, y3, thickness, RGB_value);
	hdc_line(x3, y3, x1, y1, thickness, RGB_value);

	/*如果需要填充*/
	if (filled) {
		int comx[4] = { 0,x1,x2,x3 };
		int comy[4] = { 0,y1,y2,y3 };

		//对数组进行排序 由小到大排序 由低到高排序！！！
		for (int i = 1; i <= 2; i++)
			for (int j = 1; j <= (3 - i); j++)
				if (comy[j] > comy[j + 1]) {
					comy[0] = comy[j];
					comy[j] = comy[j + 1];
					comy[j + 1] = comy[0];
					comx[0] = comx[j];
					comx[j] = comx[j + 1];
					comx[j + 1] = comx[0];
				}

		/*开始填充*/
		double k1 = 0, k2 = 0, k3 = 0;
		int delt_y = 0;
		int tempx1 = 0;
		int tempx2 = 0;
		int tempy = 0;
		if (comy[1] == comy[2] && comy[2] == comy[3]) {//一条直线
			hdc_line(comx[1], comy[1], comx[2], comy[2], thickness, RGB_value);
			hdc_line(comx[2], comy[2], comx[3], comy[3], thickness, RGB_value);
			hdc_line(comx[3], comy[3], comx[1], comy[1], thickness, RGB_value);
		}
		else if (comy[1] == comy[2]) {//上平
			k1 = ((double)comx[1] - (double)comx[3]) / ((double)comy[1] - (double)comy[3]);
			k2 = ((double)comx[2] - (double)comx[3]) / ((double)comy[2] - (double)comy[3]);
			for (; comy[1] + delt_y <= comy[3]; delt_y++) {//三角形上部分
				int delt_x1 = (int)(delt_y * k1);
				int delt_x2 = (int)(delt_y * k2);
				tempx1 = comx[1] + delt_x1;
				tempx2 = comx[2] + delt_x2;
				tempy = comy[1] + delt_y;
				hdc_base_line(tempx1, tempy, tempx2, tempy);//用最基础的线进行填充
			}
		}
		else if (comy[2] == comy[3]) {//下平
			k1 = ((double)comx[1] - (double)comx[2]) / ((double)comy[1] - (double)comy[2]);
			k3 = ((double)comx[1] - (double)comx[3]) / ((double)comy[1] - (double)comy[3]);
			for (; comy[1] + delt_y <= comy[2]; delt_y++) {//三角形上部分
				int delt_x1 = (int)(delt_y * k1);
				int delt_x2 = (int)(delt_y * k3);
				tempx1 = comx[1] + delt_x1;
				tempx2 = comx[1] + delt_x2;
				tempy = comy[1] + delt_y;
				hdc_base_line(tempx1, tempy, tempx2, tempy);//用最基础的线进行填充
			}

		}
		else {//非特殊情况
			k1 = ((double)comx[1] - (double)comx[2]) / ((double)comy[1] - (double)comy[2]);
			k2 = ((double)comx[2] - (double)comx[3]) / ((double)comy[2] - (double)comy[3]);
			k3 = ((double)comx[3] - (double)comx[1]) / ((double)comy[3] - (double)comy[1]);
			/*开始填充*/
			for (; comy[1] + delt_y <= comy[2]; delt_y++) {//三角形上部分
				int delt_x1 = (int)(delt_y * k1);
				int delt_x2 = (int)(delt_y * k3);
				tempx1 = comx[1] + delt_x1;
				tempx2 = comx[1] + delt_x2;
				tempy = comy[1] + delt_y;
				hdc_base_line(tempx1, tempy, tempx2, tempy);//用最基础的线进行填充
			}
			delt_y = 0;
			int next_x1 = tempx1;
			int next_x2 = tempx2;
			for (; comy[2] + delt_y <= comy[3]; delt_y++) {//三角形下部分
				int delt_x1 = (int)(delt_y * k2);
				int delt_x2 = (int)(delt_y * k3);
				tempx1 = next_x1 + delt_x1;
				tempx2 = next_x2 + delt_x2;
				tempy = comy[2] + delt_y;
				hdc_base_line(tempx1, tempy, tempx2, tempy);//用最基础的线进行填充
			}
		}
	}

	return;
}

/***************************************************************************
  函数名称：
  功    能：给出左上角坐标及宽度、高度，画出一个长方形
  输入参数：const int left_up_x			：左上角x
            const int left_up_y			：左上角y
			const int width				：宽度
			const int high				：高度
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）
			bool filled					：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_rectangle(const int left_up_x, const int left_up_y, const int width, const int high, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	/*对应的长方形坐标：
	    A(x1,y1)     |width|     B(x2,y2)
	     ┏----------------------┓
		 ┃                      ┃|high|
		 ┗----------------------┛
		C(x3,y3)                 D(x4,y4)  
	*/

	int x1, x2, x3, x4;
	int y1, y2, y3, y4;
	double turn = (double)rotation_angles / 180.0 * PI;
	/*A点*/
	x1 = left_up_x;
	y1 = left_up_y;
	/*B点*/
	x2 = (int)(left_up_x + width * cos(turn));
	y2 = (int)(left_up_y + width * sin(turn));
	/*C点*/
	x3 = (int)(left_up_x - high * sin(turn));
	y3 = (int)(left_up_y + high * cos(turn));
	/*D点*/
	x4 = (int)(x3 + width * cos(turn));
	y4 = (int)(y3 + width * sin(turn));

	/*先画边框*/
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);//设置画笔颜色
	hdc_line(x1, y1, x2, y2, thickness, RGB_value);
	hdc_line(x1, y1, x3, y3, thickness, RGB_value);
	hdc_line(x2, y2, x4, y4, thickness, RGB_value);
	hdc_line(x3, y3, x4, y4, thickness, RGB_value);

	/*如果需要填充*/
	if (filled) {

		int comx[5] = { 0,x1,x2,x3,x4 };
		int comy[5] = { 0,y1,y2,y3,y4 };

		//对数组进行排序 由小到大排序 由低到高排序！！！
		for (int i = 1; i <= 3; i++)
			for (int j = 1; j <= (4 - i); j++)
				if (comy[j] > comy[j + 1]) {
					comy[0] = comy[j];
					comy[j] = comy[j + 1];
					comy[j + 1] = comy[0];
					comx[0] = comx[j];
					comx[j] = comx[j + 1];
					comx[j + 1] = comx[0];
				}
		/*如果横平竖直*/
		if (comy[1] == comy[2]) {
			for (int i = 0; i < (comy[4] - comy[1]); i++)
				hdc_base_line(comx[1], comy[1] + i, comx[2], comy[2] + i);//用最基础的线进行填充
		}
		/*非横平竖直*/
		else {
			/*计算k值*/
			double k1, k2, k3, k4;//此时k值不可能不存在
			k1 = ((double)comx[1] - (double)comx[2]) / ((double)comy[1] - (double)comy[2]);
			k2 = ((double)comx[2] - (double)comx[4]) / ((double)comy[2] - (double)comy[4]);
			k3 = ((double)comx[4] - (double)comx[3]) / ((double)comy[4] - (double)comy[3]);
			k4 = ((double)comx[3] - (double)comx[1]) / ((double)comy[3] - (double)comy[1]);

			/*开始填充*/
			int delt_y = 0;
			int tempx1 = 0;
			int tempx2 = 0;
			int tempy = 0;
			int next_x1, next_x2;
			for (; comy[1] + delt_y <= comy[2]; delt_y++) {//矩形上部分
				int delt_x1 = (int)(delt_y * k1);
				int delt_x2 = (int)(delt_y * k4);
				tempx1 = comx[1] + delt_x1;
				tempx2 = comx[1] + delt_x2;
				tempy = comy[1] + delt_y;
				hdc_base_line(tempx1, tempy, tempx2, tempy);//用最基础的线进行填充
			}
			delt_y = 0;
			next_x1 = tempx1;
			next_x2 = tempx2;
			for (; comy[2] + delt_y <= comy[3]; delt_y++) {//矩形中部分
				int delt_x1 = (int)(delt_y * k2);
				int delt_x2 = (int)(delt_y * k4);
				tempx1 = next_x1 + delt_x1;
				tempx2 = next_x2 + delt_x2;
				tempy = comy[2] + delt_y;
				hdc_base_line(tempx1, tempy, tempx2, tempy);//用最基础的线进行填充
			}
			delt_y = 0;
			next_x1 = tempx1;
			next_x2 = tempx2;
			for (; comy[3] + delt_y <= comy[4]; delt_y++) {//矩形下部分
				int delt_x1 = (int)(delt_y * k2);
				int delt_x2 = (int)(delt_y * k3);
				tempx1 = next_x1 + delt_x1;
				tempx2 = next_x2 + delt_x2;
				tempy = comy[3] + delt_y;
				hdc_base_line(tempx1, tempy, tempx2, tempy);//用最基础的线进行填充
			}
		}
		return;
	}
}

/***************************************************************************
  函数名称：
  功    能：给出左上角坐标及边长，画出一个正方形
  输入参数：const int left_up_x			：左上角x
			const int left_up_y			：左上角y
			const int length			：边长
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_square(const int left_up_x, const int left_up_y, const int length, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	hdc_rectangle(left_up_x, left_up_y, length, length, rotation_angles, filled, thickness, RGB_value);
	return;
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一段圆弧
  输入参数：const int point_x		：圆心的x
            const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_arc(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const int thickness, const int RGB_value)
{
	/*start和over为系统的标准旋转方向和参数坐标
	系统标准方向坐标：顺序针 + 水平x轴方向为正方向*/
	int angle_start, angle_over;//范围在0-360度之间
	angle_change(angle_begin, angle_end, angle_start, angle_over);

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);//设置画笔颜色

	/*通过画十字来实现厚度*/
	int len_min, len_max;
	if (thickness % 2 == 1) {//为奇数
		len_min = -(thickness - 1) / 2 ;
		len_max = (thickness - 1) / 2 + 1;
	}
	else {//为偶数
		len_min = -(thickness / 2 - 1);
		len_max = thickness / 2 + 1;
	}
	int temp;
	int x, y, old_x = -1, old_y = -1;
	int compace;
	if (radius <= LENGTH_ADD)//根据半径和的换算关系求出一度分成多少小份
		compace = 1;
	else
		compace = radius / LENGTH_ADD;
	for (temp = angle_start;; ) {
		for (int i = 1; i <= compace; i++) {
			double every = 1.0 / (double)compace;
			double angle = ((double)(temp - 1) + i * every) / 180.0 * PI;
			x = (int)(point_x + radius * cos(angle));
			y = (int)(point_y + radius * sin(angle));
			/*十字前进打印*/
			if (x != old_x || y != old_y) {//避免重复画一个像素点
				hdc_base_line(x + len_min, y, x + len_max, y);
				hdc_base_line(x, y + len_min, x, y + len_max);
				old_x = x;
				old_y = y;
			}
		}
		if (temp == 359)
			temp = -1;
		temp++;
		if (temp == angle_over)
			break;
	}

	return;

}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个扇形
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：当起始/结束角度差值为360的倍数时，不画两条边
***************************************************************************/
void hdc_sector(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const bool filled, const int thickness, const int RGB_value)
{
	/*start和over为系统的标准旋转方向和参数坐标
	系统标准方向坐标：顺序针 + 水平x轴方向为正方向*/
	int angle_start, angle_over;//范围在0-360度之间
	angle_change(angle_begin, angle_end, angle_start, angle_over);

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);//设置画笔颜色

	/*是否画边*/
	if (angle_start != angle_over && filled == false) {//起始/结束角度不等且不填充时才画两条边
		double turn_start = (double)angle_start / 180.0 * PI;
		double turn_over = (double)angle_over / 180.0 * PI;
		int x1 = (int)(point_x + radius * cos(turn_start));
		int y1 = (int)(point_y + radius * sin(turn_start));
		int x2 = (int)(point_x + radius * cos(turn_over));
		int y2 = (int)(point_y + radius * sin(turn_over));
		hdc_line(x1, y1, point_x, point_y, thickness, RGB_value);
		hdc_line(x2, y2, point_x, point_y, thickness, RGB_value);
	}
	/*画弧线*/
	hdc_arc(point_x, point_y, radius, angle_start + ANGLE_BETWEEN, angle_over + ANGLE_BETWEEN, thickness, RGB_value);

	/*填充*/
	if (filled) {
		int temp, x, y, old_x = -1, old_y = -1;
		int compace;
		if (radius <= LENGTH_ADD)//根据半径和的换算关系求出一度分成多少小份
			compace = 1;
		else
			compace = radius / LENGTH_ADD;
		for (temp = angle_start;; ) {
			for (int i = 1; i <= compace; i++) {
				double every = 1.0 / (double)compace;
				double angle = ((double)(temp - 1) + i * every) / 180.0 * PI;
				x = (int)(point_x + radius * cos(angle));
				y = (int)(point_y + radius * sin(angle));
				/*旋转填充*/
				if (x != old_x || y != old_y) {//避免重复画一个像素点
					hdc_line(x, y, point_x, point_y, 3, RGB_value);
					old_x = x;
					old_y = y;
				}
			}
			if (temp == 359)
				temp = -1;
			temp++;
			if (temp == angle_over)
				break;
		}
		hdc_point(point_x, point_y, 4);//补充中间未填充的区域
	}

	return;

}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个圆
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_circle(const int point_x, const int point_y, const int radius, const bool filled, const int thickness, const int RGB_value)
{
	/*需要填充*/
	if (filled)
		hdc_sector(point_x, point_y, radius, 0, 0, filled, thickness, RGB_value);
	/*无需填充*/
	else
		hdc_arc(point_x, point_y, radius, 0, 0, thickness, RGB_value);

	return;

}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个椭圆
  输入参数：const int point_x			：圆心的x
			const int point_y			：圆心的y
			const int radius_a			：平行于X轴的半径
			const int radius_b			：平行于Y轴的半径
			const int rotation_angles	：以圆心为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_ellipse(const int point_x, const int point_y, const int radius_a, const int radius_b, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);//设置画笔颜色

	double turn = (double)rotation_angles / 180.0 * PI;//转动角度换算成弧度制

	/*根据较长的那条半径确定划分为多少细分才能实现完全填充*/
	int max = (radius_a >= radius_b ? radius_a : radius_b);//找到较大的那个半径
	int compace;
	if (radius_a <= LENGTH_ADD && radius_b < LENGTH_ADD)//根据半径和的换算关系求出一度分成多少小份
		compace = 1;
	else
		compace = max / LENGTH_ADD;
	/*确立画弧线的厚度*/
	int len_min, len_max;
	if (thickness % 2 == 1) {//为奇数
		len_min = -(thickness - 1) / 2;
		len_max = (thickness - 1) / 2 + 1;
	}
	else {//为偶数
		len_min = -(thickness / 2 - 1);
		len_max = thickness / 2 + 1;
	}

	int angle_large;
	int x, y, old_x = -1, old_y = -1;

	/*椭圆弧线*/
	for (angle_large = 1; angle_large <= 360; angle_large++) {
		for (int i = 1; i <= compace; i++) {
			double every = 1.0 / (double)compace;
			double angle = ((double)(angle_large - 1) + i * every) / 180.0 * PI;
			x = (int)(point_x + radius_a * cos(angle) * cos(turn) - radius_b * sin(angle) * sin(turn));
			y = (int)(point_y + radius_a * cos(angle) * sin(turn) + radius_b * sin(angle) * cos(turn));
			/*画十字前进*/
			if (x != old_x || y != old_y) {//避免重复画一个像素点
				hdc_base_line(x + len_min, y, x + len_max, y);
				hdc_base_line(x, y + len_min, x, y + len_max);
				old_x = x;
				old_y = y;
			}	
		}
	}

	/*填充*/
	old_x = -1;
	old_y = -1;
	if (filled) {
		for (angle_large = 1; angle_large <= 360; angle_large++) {
			for (int i = 1; i <= compace; i++) {
				double every = 1.0 / (double)compace;
				double angle = ((double)(angle_large - 1) + i * every) / 180.0 * PI;
				x = (int)(point_x + radius_a * cos(angle) * cos(turn) - radius_b * sin(angle) * sin(turn));
				y = (int)(point_y + radius_a * cos(angle) * sin(turn) + radius_b * sin(angle) * cos(turn));
				/*旋转填充*/
				if (x != old_x || y != old_y) {//避免重复画一个像素点
					hdc_line(x, y, point_x, point_y, 3, RGB_value);//旋转填充
					old_x = x;
					old_y = y;
				}
			}
		}
		hdc_point(point_x, point_y, 4);//补充中间未填充的区域
	}

	return;

}

#endif !HDC_SERIES_BY_TEACHER
