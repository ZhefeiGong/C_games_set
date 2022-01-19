/* 2052532 ���ܷ� ��04 */

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include "../include/cmd_hdc_tools.h"
using namespace std;

#if !HDC_SERIES_BY_TEACHER	//δ���������²Ŵ���������

/* ���������Ҫ��ͷ�ļ� */
#include <cmath>
#include <stdlib.h>

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow(); //VS�д˴����в����ߣ���Ӱ������ʹ��

/* �Ѷ���ľ�̬ȫ�ֱ��� */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;
static int _Delay_ms;

/* �����ʶ�ʹ�þ�̬ȫ�ֱ�����������static�����ⲿ���ɼ� */

/* �˴���������Զ��庯����������static�����ⲿ���ɼ� */
/***************************************************************************
  �������ƣ�
  ��    �ܣ����Ƕ�ת��ΪϵͳĬ������Ƕ�
  ���������const int begin 
            const int end
		    int& start
			int& over
  �� �� ֵ��
  ˵    ����
***************************************************************************/
static void angle_change(const int begin, const int end, int& start, int& over)
{
	/*start��overΪϵͳ�ı�׼��ת����Ͳ�������
	ϵͳ��׼�������꣺˳���� + ˮƽx�᷽��Ϊ������*/
	int temp_begin = begin - ANGLE_BETWEEN;
	int temp_end = end - ANGLE_BETWEEN;
	/*�������޶���[0,360)��������*/
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

/* ��������˼�����������������ʵ�֣���Ҫ�Ķ� */
/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʼ��
  ���������const int bgcolor������ɫ
            const int fgcolor��ǰ��ɫ
			const int width  ����Ļ��ȣ�����
			const int high   ����Ļ�߶ȣ�����
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high)
{
	/* ���ͷţ���ֹ��release���ٴ�init��hdc_release�����룩 */
	hdc_release();

	/* ����init����һ����̬ȫ������¼������hdc_cls()���õ� */
	_BgColor_ = bgcolor;
	_FgColor_ = fgcolor;
	_Width_ = width;
	_High = high;

	hdc = GetDC(hWnd);

	cct_setcursor(CURSOR_INVISIBLE);
	cct_setcolor(bgcolor, fgcolor);
	cct_setfontsize("��������", 16);
	cct_setconsoleborder(width / 8 + !!(width % 8), high / 16 + !!(high % 16)); //������Ŀ�ȼ��߶�ת��Ϊ�ض��������������!!�ĺ��壺�������8/16�ı��������ж�+1
	cct_cls();
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ͷŻ�ͼ��Դ
  ���������
  �� �� ֵ��
  ˵    ����������
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_delay(int ms)
{
	if (ms > 0)
		_Delay_ms = ms;
	else
		_Delay_ms = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����û�����ɫ������RGBֵ
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����û�����ɫ������RGB��ɫ��ֵ0-255
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ļ�����е�ͼ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_cls()
{
    /* ����һ��Ҫ��һ����ɫ��ʼ������������ص㣬�ҵ�����㷽����ͬѧ����֪ͨ�� */
	hdc_init(_BgColor_, (_FgColor_  + 1) % 16, _Width_, _High);

	hdc_init(_BgColor_, _FgColor_, _Width_, _High);

	/* ���ֻ���������demoʱ�������hdc_cls()���һ������ȱ�ߵ����������ʱ����
	   ������ֻ������л������⣬���ߴ���ʱֵ */
	Sleep(30);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ����һ�����ص�
  ���������const int x��x���꣬���Ͻ�Ϊ(0,0)
            const int y��y���꣬���Ͻ�Ϊ(0,0)
  �� �� ֵ��
  ˵    ������ɫֱ���õ�ǰ�趨�������ڵ��ô�ָ��
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
	MoveToEx(hdc, x - 1, y - 1, NULL);
	LineTo(hdc, x, y);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x1,y1)-(x2,y2)֮�仭��һ�����ص������
  ���������const int x1�����x���꣬���Ͻ�Ϊ(0,0)
			const int y1�����y���꣬���Ͻ�Ϊ(0,0)
			const int x2���յ�y���꣬���Ͻ�Ϊ(0,0)
			const int y2���յ�y���꣬���Ͻ�Ϊ(0,0)
  �� �� ֵ��
  ˵    ������ɫֱ���õ�ǰ�趨
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ��һ��ָ����ϸ�ĵ�(�û�ʵ��Բ��ģ��)
  ���������const int x			��x���꣬���Ͻ�Ϊ(0,0)
            const int y			��y���꣬���Ͻ�Ϊ(0,0)
			const int thickness	����Ĵ�ϸ������λ1�����޲���(�����ᵼ�²���ȫ���)����ȱʡֵ��
			const int RGB_value	�������ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    �����Ľ�����Ч��
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//��������
	/* ������ֱ�������ޣ����ǵ����޳���һ����С�ǣ�������Բ����λ��δʵ�� */

	const int tn_end = (tn <= 1) ? 1 : tn/2;

	int angle, level;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	/* �û�Բ�ķ�ʽģ��ֵ� */
	for (level = 1; level <= tn_end; level++) {
		for (angle = 0; angle <= 360; angle ++) {
			/* ע�⣬ϵͳ�������ᣬ0����Բ�����·� */
			x1 = x + (int)(level * sin(angle * PI / 180));
			y1 = y - (int)(level * cos(angle * PI / 180));

			/* ���뾶��Сʱ���Ƕȱ仯��������int�͵�x1/y1�仯����˼��ж���䣬�����ظ���ͬһ���ص� */
			if (x1 != old_x1 || y1 != old_y1) {
				old_x1 = x1;
				old_y1 = y1;
				hdc_base_point(x1, y1);
				++count;
			}
		}
	}
#if 0
	/* �ſ���ע�ͣ����Կ���һ���ֵ����˼������ص���� */
	cct_gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
}

/* -------------------------------------------------------------------------
	�������к�����ʵ�֣���������������׼�Ķ�
	1������Ҫ����ϵͳ�������й�ͼ�β����ĺ���
	2�����к�����ʵ�ֶ�����hdc_base_point/hdc_base_line���������
	3����һ�룬�ǲ������к�������Ҫ����������ʵ�ֹ��̣�Ӧ����ʵ����Щ������
	4�������ʵ������ӵĲ���
	5��ϵͳ�ĽǶ��뺯�������еĽǶȺ������180��
   ------------------------------------------------------------------------- */

/***************************************************************************
  �������ƣ�
  ��    �ܣ����߶�
  ���������const int x1		������x
            const int y1		������y
			const int x2		���յ��x
			const int y2		���յ��y
			const int thickness	���߶εĴ�ϸ����ȱʡֵ��
			const int RGB_value	���߶ε���ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ���������������û���hdc_point����ϣ��ٶ�����
                  Ҳ������hdc_base_point/hdc_base_line����ϣ��ٶȿ죩
***************************************************************************/
void hdc_line(const int x1, const int y1, const int x2, const int y2, const int thickness, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);//���û�����ɫ

	/*��ƽ��ֱ����*/
	if (x1 == x2) {
		for (int num = 0; num < thickness; num++)
			hdc_base_line(x1+num, y1, x2+num, y2);
	}
	else if (y1 == y2) {
		for (int num = 0; num < thickness; num++)
			hdc_base_line(x1, y1 + num, x2, y2 + num);
	}
	/*б��*/
	double k = fabs(((double)y1 - (double)y2) / ((double)x1 - (double)x2));
	int thick_min = 0;//ʹ�ÿ��Ϊ���Ҿ��ӵ�
	int thick_max = 0;//ʹ�ÿ��Ϊ���Ҿ��ӵ�
	if (thickness % 2 == 1) {//Ϊ����
		thick_min = -(thickness - 1) / 2;
		thick_max = (thickness - 1) / 2;
	}
	else {//Ϊż��
		thick_min = -(thickness / 2 - 1);
		thick_max = thickness / 2;
	}
	if (fabs(k) - 1 >= 1e-6)//��Ǵ���45��
		for (int num = thick_min; num <= thick_max; num++)
			hdc_base_line(x1 + num, y1, x2 + num, y2);
	else//���С��45��
		for (int num = thick_min; num <= thick_max; num++)
			hdc_base_line(x1, y1 + num, x2, y2 + num);

	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������������꣬��һ��������
  ���������const int x1		����1�����x
            const int y1		����1�����y
			const int x2		����2�����x
			const int y2		����2�����y
			const int x3		����3�����x
			const int y3		����3�����y
			bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness	���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value	����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    �������ж������Ƿ��ߣ�������ߣ�����һ��ֱ�߼���
***************************************************************************/
void hdc_triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, bool filled, const int thickness, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);//���û�����ɫ

	/*�Ȼ��߿�*/
	hdc_line(x1, y1, x2, y2, thickness, RGB_value);
	hdc_line(x2, y2, x3, y3, thickness, RGB_value);
	hdc_line(x3, y3, x1, y1, thickness, RGB_value);

	/*�����Ҫ���*/
	if (filled) {
		int comx[4] = { 0,x1,x2,x3 };
		int comy[4] = { 0,y1,y2,y3 };

		//������������� ��С�������� �ɵ͵������򣡣���
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

		/*��ʼ���*/
		double k1 = 0, k2 = 0, k3 = 0;
		int delt_y = 0;
		int tempx1 = 0;
		int tempx2 = 0;
		int tempy = 0;
		if (comy[1] == comy[2] && comy[2] == comy[3]) {//һ��ֱ��
			hdc_line(comx[1], comy[1], comx[2], comy[2], thickness, RGB_value);
			hdc_line(comx[2], comy[2], comx[3], comy[3], thickness, RGB_value);
			hdc_line(comx[3], comy[3], comx[1], comy[1], thickness, RGB_value);
		}
		else if (comy[1] == comy[2]) {//��ƽ
			k1 = ((double)comx[1] - (double)comx[3]) / ((double)comy[1] - (double)comy[3]);
			k2 = ((double)comx[2] - (double)comx[3]) / ((double)comy[2] - (double)comy[3]);
			for (; comy[1] + delt_y <= comy[3]; delt_y++) {//�������ϲ���
				int delt_x1 = (int)(delt_y * k1);
				int delt_x2 = (int)(delt_y * k2);
				tempx1 = comx[1] + delt_x1;
				tempx2 = comx[2] + delt_x2;
				tempy = comy[1] + delt_y;
				hdc_base_line(tempx1, tempy, tempx2, tempy);//����������߽������
			}
		}
		else if (comy[2] == comy[3]) {//��ƽ
			k1 = ((double)comx[1] - (double)comx[2]) / ((double)comy[1] - (double)comy[2]);
			k3 = ((double)comx[1] - (double)comx[3]) / ((double)comy[1] - (double)comy[3]);
			for (; comy[1] + delt_y <= comy[2]; delt_y++) {//�������ϲ���
				int delt_x1 = (int)(delt_y * k1);
				int delt_x2 = (int)(delt_y * k3);
				tempx1 = comx[1] + delt_x1;
				tempx2 = comx[1] + delt_x2;
				tempy = comy[1] + delt_y;
				hdc_base_line(tempx1, tempy, tempx2, tempy);//����������߽������
			}

		}
		else {//���������
			k1 = ((double)comx[1] - (double)comx[2]) / ((double)comy[1] - (double)comy[2]);
			k2 = ((double)comx[2] - (double)comx[3]) / ((double)comy[2] - (double)comy[3]);
			k3 = ((double)comx[3] - (double)comx[1]) / ((double)comy[3] - (double)comy[1]);
			/*��ʼ���*/
			for (; comy[1] + delt_y <= comy[2]; delt_y++) {//�������ϲ���
				int delt_x1 = (int)(delt_y * k1);
				int delt_x2 = (int)(delt_y * k3);
				tempx1 = comx[1] + delt_x1;
				tempx2 = comx[1] + delt_x2;
				tempy = comy[1] + delt_y;
				hdc_base_line(tempx1, tempy, tempx2, tempy);//����������߽������
			}
			delt_y = 0;
			int next_x1 = tempx1;
			int next_x2 = tempx2;
			for (; comy[2] + delt_y <= comy[3]; delt_y++) {//�������²���
				int delt_x1 = (int)(delt_y * k2);
				int delt_x2 = (int)(delt_y * k3);
				tempx1 = next_x1 + delt_x1;
				tempx2 = next_x2 + delt_x2;
				tempy = comy[2] + delt_y;
				hdc_base_line(tempx1, tempy, tempx2, tempy);//����������߽������
			}
		}
	}

	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��������Ͻ����꼰��ȡ��߶ȣ�����һ��������
  ���������const int left_up_x			�����Ͻ�x
            const int left_up_y			�����Ͻ�y
			const int width				�����
			const int high				���߶�
			const int rotation_angles	�������Ͻ�Ϊ֧�㣬��x�����ת��ǣ�����תΪ����
			bool filled					���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_rectangle(const int left_up_x, const int left_up_y, const int width, const int high, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	/*��Ӧ�ĳ��������꣺
	    A(x1,y1)     |width|     B(x2,y2)
	     ��----------------------��
		 ��                      ��|high|
		 ��----------------------��
		C(x3,y3)                 D(x4,y4)  
	*/

	int x1, x2, x3, x4;
	int y1, y2, y3, y4;
	double turn = (double)rotation_angles / 180.0 * PI;
	/*A��*/
	x1 = left_up_x;
	y1 = left_up_y;
	/*B��*/
	x2 = (int)(left_up_x + width * cos(turn));
	y2 = (int)(left_up_y + width * sin(turn));
	/*C��*/
	x3 = (int)(left_up_x - high * sin(turn));
	y3 = (int)(left_up_y + high * cos(turn));
	/*D��*/
	x4 = (int)(x3 + width * cos(turn));
	y4 = (int)(y3 + width * sin(turn));

	/*�Ȼ��߿�*/
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);//���û�����ɫ
	hdc_line(x1, y1, x2, y2, thickness, RGB_value);
	hdc_line(x1, y1, x3, y3, thickness, RGB_value);
	hdc_line(x2, y2, x4, y4, thickness, RGB_value);
	hdc_line(x3, y3, x4, y4, thickness, RGB_value);

	/*�����Ҫ���*/
	if (filled) {

		int comx[5] = { 0,x1,x2,x3,x4 };
		int comy[5] = { 0,y1,y2,y3,y4 };

		//������������� ��С�������� �ɵ͵������򣡣���
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
		/*�����ƽ��ֱ*/
		if (comy[1] == comy[2]) {
			for (int i = 0; i < (comy[4] - comy[1]); i++)
				hdc_base_line(comx[1], comy[1] + i, comx[2], comy[2] + i);//����������߽������
		}
		/*�Ǻ�ƽ��ֱ*/
		else {
			/*����kֵ*/
			double k1, k2, k3, k4;//��ʱkֵ�����ܲ�����
			k1 = ((double)comx[1] - (double)comx[2]) / ((double)comy[1] - (double)comy[2]);
			k2 = ((double)comx[2] - (double)comx[4]) / ((double)comy[2] - (double)comy[4]);
			k3 = ((double)comx[4] - (double)comx[3]) / ((double)comy[4] - (double)comy[3]);
			k4 = ((double)comx[3] - (double)comx[1]) / ((double)comy[3] - (double)comy[1]);

			/*��ʼ���*/
			int delt_y = 0;
			int tempx1 = 0;
			int tempx2 = 0;
			int tempy = 0;
			int next_x1, next_x2;
			for (; comy[1] + delt_y <= comy[2]; delt_y++) {//�����ϲ���
				int delt_x1 = (int)(delt_y * k1);
				int delt_x2 = (int)(delt_y * k4);
				tempx1 = comx[1] + delt_x1;
				tempx2 = comx[1] + delt_x2;
				tempy = comy[1] + delt_y;
				hdc_base_line(tempx1, tempy, tempx2, tempy);//����������߽������
			}
			delt_y = 0;
			next_x1 = tempx1;
			next_x2 = tempx2;
			for (; comy[2] + delt_y <= comy[3]; delt_y++) {//�����в���
				int delt_x1 = (int)(delt_y * k2);
				int delt_x2 = (int)(delt_y * k4);
				tempx1 = next_x1 + delt_x1;
				tempx2 = next_x2 + delt_x2;
				tempy = comy[2] + delt_y;
				hdc_base_line(tempx1, tempy, tempx2, tempy);//����������߽������
			}
			delt_y = 0;
			next_x1 = tempx1;
			next_x2 = tempx2;
			for (; comy[3] + delt_y <= comy[4]; delt_y++) {//�����²���
				int delt_x1 = (int)(delt_y * k2);
				int delt_x2 = (int)(delt_y * k3);
				tempx1 = next_x1 + delt_x1;
				tempx2 = next_x2 + delt_x2;
				tempy = comy[3] + delt_y;
				hdc_base_line(tempx1, tempy, tempx2, tempy);//����������߽������
			}
		}
		return;
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��������Ͻ����꼰�߳�������һ��������
  ���������const int left_up_x			�����Ͻ�x
			const int left_up_y			�����Ͻ�y
			const int length			���߳�
			const int rotation_angles	�������Ͻ�Ϊ֧�㣬��x�����ת��ǣ�����תΪ��������ȱʡֵ��
			const bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_square(const int left_up_x, const int left_up_y, const int length, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	hdc_rectangle(left_up_x, left_up_y, length, length, rotation_angles, filled, thickness, RGB_value);
	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ��Բ��
  ���������const int point_x		��Բ�ĵ�x
            const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const int angle_begin	����ʼ�Ƕȣ���Բ�����Ϸ�Ϊ0�㣬˳ʱ��Ϊ����ע�⣺��ϵͳ�������180��!!!������ȱʡֵ��
			const int angle_end		�������Ƕȣ�ͬ�ϣ�����ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_arc(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const int thickness, const int RGB_value)
{
	/*start��overΪϵͳ�ı�׼��ת����Ͳ�������
	ϵͳ��׼�������꣺˳���� + ˮƽx�᷽��Ϊ������*/
	int angle_start, angle_over;//��Χ��0-360��֮��
	angle_change(angle_begin, angle_end, angle_start, angle_over);

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);//���û�����ɫ

	/*ͨ����ʮ����ʵ�ֺ��*/
	int len_min, len_max;
	if (thickness % 2 == 1) {//Ϊ����
		len_min = -(thickness - 1) / 2 ;
		len_max = (thickness - 1) / 2 + 1;
	}
	else {//Ϊż��
		len_min = -(thickness / 2 - 1);
		len_max = thickness / 2 + 1;
	}
	int temp;
	int x, y, old_x = -1, old_y = -1;
	int compace;
	if (radius <= LENGTH_ADD)//���ݰ뾶�͵Ļ����ϵ���һ�ȷֳɶ���С��
		compace = 1;
	else
		compace = radius / LENGTH_ADD;
	for (temp = angle_start;; ) {
		for (int i = 1; i <= compace; i++) {
			double every = 1.0 / (double)compace;
			double angle = ((double)(temp - 1) + i * every) / 180.0 * PI;
			x = (int)(point_x + radius * cos(angle));
			y = (int)(point_y + radius * sin(angle));
			/*ʮ��ǰ����ӡ*/
			if (x != old_x || y != old_y) {//�����ظ���һ�����ص�
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
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ������
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const int angle_begin	����ʼ�Ƕȣ���Բ�����Ϸ�Ϊ0�㣬˳ʱ��Ϊ����ע�⣺��ϵͳ�������180��!!!������ȱʡֵ��
			const int angle_end		�������Ƕȣ�ͬ�ϣ�����ȱʡֵ��
			const bool filled		���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  ���������
  �� �� ֵ��
  ˵    ��������ʼ/�����ǶȲ�ֵΪ360�ı���ʱ������������
***************************************************************************/
void hdc_sector(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const bool filled, const int thickness, const int RGB_value)
{
	/*start��overΪϵͳ�ı�׼��ת����Ͳ�������
	ϵͳ��׼�������꣺˳���� + ˮƽx�᷽��Ϊ������*/
	int angle_start, angle_over;//��Χ��0-360��֮��
	angle_change(angle_begin, angle_end, angle_start, angle_over);

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);//���û�����ɫ

	/*�Ƿ񻭱�*/
	if (angle_start != angle_over && filled == false) {//��ʼ/�����ǶȲ����Ҳ����ʱ�Ż�������
		double turn_start = (double)angle_start / 180.0 * PI;
		double turn_over = (double)angle_over / 180.0 * PI;
		int x1 = (int)(point_x + radius * cos(turn_start));
		int y1 = (int)(point_y + radius * sin(turn_start));
		int x2 = (int)(point_x + radius * cos(turn_over));
		int y2 = (int)(point_y + radius * sin(turn_over));
		hdc_line(x1, y1, point_x, point_y, thickness, RGB_value);
		hdc_line(x2, y2, point_x, point_y, thickness, RGB_value);
	}
	/*������*/
	hdc_arc(point_x, point_y, radius, angle_start + ANGLE_BETWEEN, angle_over + ANGLE_BETWEEN, thickness, RGB_value);

	/*���*/
	if (filled) {
		int temp, x, y, old_x = -1, old_y = -1;
		int compace;
		if (radius <= LENGTH_ADD)//���ݰ뾶�͵Ļ����ϵ���һ�ȷֳɶ���С��
			compace = 1;
		else
			compace = radius / LENGTH_ADD;
		for (temp = angle_start;; ) {
			for (int i = 1; i <= compace; i++) {
				double every = 1.0 / (double)compace;
				double angle = ((double)(temp - 1) + i * every) / 180.0 * PI;
				x = (int)(point_x + radius * cos(angle));
				y = (int)(point_y + radius * sin(angle));
				/*��ת���*/
				if (x != old_x || y != old_y) {//�����ظ���һ�����ص�
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
		hdc_point(point_x, point_y, 4);//�����м�δ��������
	}

	return;

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ��Բ
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const bool filled		���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_circle(const int point_x, const int point_y, const int radius, const bool filled, const int thickness, const int RGB_value)
{
	/*��Ҫ���*/
	if (filled)
		hdc_sector(point_x, point_y, radius, 0, 0, filled, thickness, RGB_value);
	/*�������*/
	else
		hdc_arc(point_x, point_y, radius, 0, 0, thickness, RGB_value);

	return;

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ����Բ
  ���������const int point_x			��Բ�ĵ�x
			const int point_y			��Բ�ĵ�y
			const int radius_a			��ƽ����X��İ뾶
			const int radius_b			��ƽ����Y��İ뾶
			const int rotation_angles	����Բ��Ϊ֧�㣬��x�����ת��ǣ�����תΪ��������ȱʡֵ��
			const bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			����ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_ellipse(const int point_x, const int point_y, const int radius_a, const int radius_b, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);//���û�����ɫ

	double turn = (double)rotation_angles / 180.0 * PI;//ת���ǶȻ���ɻ�����

	/*���ݽϳ��������뾶ȷ������Ϊ����ϸ�ֲ���ʵ����ȫ���*/
	int max = (radius_a >= radius_b ? radius_a : radius_b);//�ҵ��ϴ���Ǹ��뾶
	int compace;
	if (radius_a <= LENGTH_ADD && radius_b < LENGTH_ADD)//���ݰ뾶�͵Ļ����ϵ���һ�ȷֳɶ���С��
		compace = 1;
	else
		compace = max / LENGTH_ADD;
	/*ȷ�������ߵĺ��*/
	int len_min, len_max;
	if (thickness % 2 == 1) {//Ϊ����
		len_min = -(thickness - 1) / 2;
		len_max = (thickness - 1) / 2 + 1;
	}
	else {//Ϊż��
		len_min = -(thickness / 2 - 1);
		len_max = thickness / 2 + 1;
	}

	int angle_large;
	int x, y, old_x = -1, old_y = -1;

	/*��Բ����*/
	for (angle_large = 1; angle_large <= 360; angle_large++) {
		for (int i = 1; i <= compace; i++) {
			double every = 1.0 / (double)compace;
			double angle = ((double)(angle_large - 1) + i * every) / 180.0 * PI;
			x = (int)(point_x + radius_a * cos(angle) * cos(turn) - radius_b * sin(angle) * sin(turn));
			y = (int)(point_y + radius_a * cos(angle) * sin(turn) + radius_b * sin(angle) * cos(turn));
			/*��ʮ��ǰ��*/
			if (x != old_x || y != old_y) {//�����ظ���һ�����ص�
				hdc_base_line(x + len_min, y, x + len_max, y);
				hdc_base_line(x, y + len_min, x, y + len_max);
				old_x = x;
				old_y = y;
			}	
		}
	}

	/*���*/
	old_x = -1;
	old_y = -1;
	if (filled) {
		for (angle_large = 1; angle_large <= 360; angle_large++) {
			for (int i = 1; i <= compace; i++) {
				double every = 1.0 / (double)compace;
				double angle = ((double)(angle_large - 1) + i * every) / 180.0 * PI;
				x = (int)(point_x + radius_a * cos(angle) * cos(turn) - radius_b * sin(angle) * sin(turn));
				y = (int)(point_y + radius_a * cos(angle) * sin(turn) + radius_b * sin(angle) * cos(turn));
				/*��ת���*/
				if (x != old_x || y != old_y) {//�����ظ���һ�����ص�
					hdc_line(x, y, point_x, point_y, 3, RGB_value);//��ת���
					old_x = x;
					old_y = y;
				}
			}
		}
		hdc_point(point_x, point_y, 4);//�����м�δ��������
	}

	return;

}

#endif !HDC_SERIES_BY_TEACHER
