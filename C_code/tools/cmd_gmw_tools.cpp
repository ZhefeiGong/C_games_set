/* 2052532 ���ܷ� ��04 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <time.h>
#include <climits>
#include <conio.h>
#include "../include/cmd_console_tools.h"//���������е��µĹ��ߺ���
#include "../include/cmd_gmw_tools.h"//����αͼ�ν����µĹ��ߺ���
using namespace std;

/* --------------------------------------------------
		�˴����Ը�����Ҫ�ľ�̬ȫ�ֱ������������٣����û�У�����̬ȫ��ֻ������/�궨�壨�������ޣ���
   -------------------------------------------------- */


   /* --------------------------------------------------
		   �˴����Ը�����Ҫ���ڲ��������ߺ���
		   1�����������ޣ�����Ϊ gmw_inner_*
		   2����������
		   3��������static������ȷ��ֻ�ڱ�Դ�ļ���ʹ��
	  -------------------------------------------------- */


   /* -----------------------------------------------
		   ʵ����������ĺ���������������׼����
   ----------------------------------------------- */
/***************************************************************************
  �������ƣ�
  ��    �ܣ�������Ϸ����ܵ�������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		    const int row						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
		    const int col						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
  �� �� ֵ��
  ˵    ����1��ָ��������Ϸ�ľ������������ֵ
		    2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
 ***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO* const pCGI, const int row, const int col)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*��������*/
	pCGI->row_num = row;//������Ϸ������
	pCGI->col_num = col;//������Ϸ������

	/*�޸�cmd���ڵĴ�С*/
	int row_sign = 0, col_sign = 0, top_status = 0, lower_status = 0, special_row = 0, special_col = 0;
	/*ȷ������״̬�������б����ʾ���
	  ע�⣺��״̬����0/1�У�/��״̬����0/1�У�/�к���ʾ��0/2�У�/�б���ʾ����0/1�У�*/
	if (pCGI->draw_frame_with_row_no)//��Ҫ�б���ʾ
		row_sign = 2;
	if (pCGI->draw_frame_with_col_no)//��Ҫ�б���ʾ
		col_sign = 1;
	if (pCGI->top_status_line)//��Ҫ��״̬��
		top_status = 1;
	if (pCGI->lower_status_line)//��Ҫ��״̬��
		lower_status = 1;
	if (!pCGI->CFI.separator)//��ʱ�޷ָ���
	{
		special_row = 1;
		special_col = 2;
	}

	/*�У���/�¶���ռ�+��״̬��+�б���ʾ+������+��״̬����+ 4*/
	pCGI->lines = special_row + (pCGI->CFI.bhigh * row + 1) + (pCGI->extern_down_lines + pCGI->extern_up_lines) + (col_sign + top_status + lower_status) + 4;

	/*�У���/�Ҷ���ռ�+�б���ʾ+������+3*/
	pCGI->cols = special_col + (pCGI->CFI.bwidth * col) + (pCGI->extern_left_cols + pCGI->extern_right_cols) + (row_sign)+3;

	/*��������״̬������ʼ����*/
	pCGI->SLI.top_start_x = pCGI->extern_left_cols;
	pCGI->SLI.top_start_y = pCGI->extern_up_lines;
	pCGI->SLI.lower_start_x = pCGI->extern_left_cols;
	pCGI->SLI.lower_start_y = pCGI->extern_up_lines + special_row + (pCGI->CFI.bhigh * pCGI->row_num + 1) + col_sign + top_status;

	/*״̬������ռ���*/
	pCGI->SLI.width = special_col + pCGI->CFI.bwidth * pCGI->col_num + row_sign + 3;

	/*��Ϸ����ܵ���ʼλ��(��ʾɫ��ͼ��������ʼλ��)*/
	/*ע����Ϸ����ܣ������а���m��n�е�ɫ�������⣬������״̬����0/1�У�/��״̬����0/1�У�/�к���ʾ��0/2�У�/�б���ʾ����0/1�У�
	   -     ������ֵ������ͬ������£�start_x/start_y��ֵ�ǲ�ͬ��*/
	pCGI->start_x = pCGI->extern_left_cols + row_sign;
	pCGI->start_y = pCGI->extern_up_lines + top_status + col_sign;

	return 0; //����0��ʾ���óɹ�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������������ڣ�����Ϸ�������������ڵ�����cmd���ڣ�����ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int bg_color					��ǰ��ɫ��ȱʡCOLOR_BLACK��
		   const int fg_color					������ɫ��ȱʡCOLOR_WHITE��
		   const bool cascade					���Ƿ�����ȱʡΪtrue-������
  �� �� ֵ��
  ˵    ����1��cascade = trueʱ
				ͬ���޸���Ϸ���������ɫ
				ͬ���޸�����״̬���������ı��ı���ɫ��ǰ��ɫ����Ŀ�ı��ı���ɫ��ǰ��ɫ���䣩
			2���������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15
					ǰ��ɫ����ɫ��ֵһ�µ����޷���������
					ǰ��ɫ������״̬����Ŀǰ��ɫ�������޷�������Ŀ��ʾ
					...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO* const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*���ýṹ��pCGI������ͼ�ν������ɫ����*/
	pCGI->area_bgcolor = bgcolor;
	pCGI->area_fgcolor = fgcolor;

	/*��Ҫ����ʱ*/
	if (cascade)
	{
		/*������Ϸ���������ɫ*/
		pCGI->CFI.bgcolor = bgcolor;
		pCGI->CFI.fgcolor = fgcolor;
		/*��������״̬������Ϣ��ɫ*/
		pCGI->SLI.top_normal_bgcolor = bgcolor;
		pCGI->SLI.top_normal_fgcolor = fgcolor;
		pCGI->SLI.lower_normal_bgcolor = bgcolor;
		pCGI->SLI.lower_normal_fgcolor = fgcolor;
		pCGI->SLI.top_catchy_bgcolor = bgcolor;    //���ı���ɫӴ
		pCGI->SLI.lower_catchy_bgcolor = bgcolor;  //���ı���ɫӴ

	}

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ô��ڵ�����
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const char *fontname					���������ƣ�ֻ����"Terminal"��"������"���֣������򷵻�-1�����ı����壩
		   const int fs_high					������߶ȣ�ȱʡ������Ϊ16�������������ƣ���Ϊ��֤��
		   const int fs_width					������߶ȣ�ȱʡ������Ϊ8�������������ƣ���Ϊ��֤��
  �� �� ֵ��
  ˵    ����1����cmd_console_tools�е�setfontsize���ƣ�Ŀǰֻ֧�֡��������塱�͡������塱
			2������������������ֱ�ӷ��أ�����ԭ�������ò���
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO* const pCGI, const char* fontname, const int fs_high, const int fs_width)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	/*��¼���������Ϣ*/
	char font1[] = "��������";
	char font2[] = "������";
	if (strcmp(fontname, font1) == 0 || strcmp(fontname, font2) == 0)//�ж������Ƿ���Ҫ�ı�
		strcpy(pCGI->CFT.font_type, fontname);//��¼��Ϣ���ṹ����
	pCGI->CFT.font_size_high = fs_high;
	pCGI->CFT.font_size_width = fs_width;
	/*������������*/
	cct_setfontsize(fontname, fs_high, fs_width);

	return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������ʱ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int type						����ʱ�����ͣ�ĿǰΪ3�֣�
		   const int delay_ms					����msΪ��λ����ʱ
			   ���߿����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
			   ��ɫ�����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
			   ɫ���ƶ�����ʱ��BLOCK_MOVED_DELAY_MS ~ �������ޣ���Ϊ��֤��ȷ�� <BLOCK_MOVED_DELAY_MS ���� BLOCK_MOVED_DELAY_MS��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO* const pCGI, const int type, const int delay_ms)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	switch (type)
	{
		case DELAY_OF_DRAW_FRAME://���߿����ʱ
			if (delay_ms < 0)
				pCGI->delay_of_draw_frame = 0;
			else
				pCGI->delay_of_draw_frame = delay_ms;
			break;
		case DELAY_OF_DRAW_BLOCK://��ɫ�����ʱ
			if (delay_ms < 0)
				pCGI->delay_of_draw_block = 0;
			else
				pCGI->delay_of_draw_block = delay_ms;
			break;
		case DELAY_OF_BLOCK_MOVED://ɫ���ƶ�����ʱ
			if (delay_ms < BLOCK_MOVED_DELAY_MS)
				pCGI->delay_of_block_moved = BLOCK_MOVED_DELAY_MS;
			else
				pCGI->delay_of_block_moved = delay_ms;
			break;
		default:
			break;
	}

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ���������������Ϸ����ܽṹ֮����Ҫ�����Ķ�������
  ��    �ܣ�CONSOLE_GRAPHICS_INFO *const pCGI	��
		   const int up_lines					���ϲ�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int down_lines				���²�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int left_cols					����߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int right_cols				���ұ߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
  �� �� ֵ��
  ˵    �����������еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
			��cmd���ڵ�����ֵ�ᷢ���ı䣡����
***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO* const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*������ظ�ֵ*/
	pCGI->extern_right_cols = right_cols;
	pCGI->extern_left_cols = left_cols;
	pCGI->extern_up_lines = up_lines;
	pCGI->extern_down_lines = down_lines;

	/*cmd���ڵĴ�С�������ı�*/
	int row_sign = 0, col_sign = 0, top_status = 0, lower_status = 0, special_row = 0, special_col = 0;
	/*ȷ������״̬�������б����ʾ���
	  ע�⣺��״̬����0/1�У�/��״̬����0/1�У�/�к���ʾ��0/2�У�/�б���ʾ����0/1�У�*/
	if (pCGI->draw_frame_with_row_no)//��Ҫ�б���ʾ
		row_sign = 2;
	if (pCGI->draw_frame_with_col_no)//��Ҫ�б���ʾ
		col_sign = 1;
	if (pCGI->top_status_line)//��Ҫ��״̬��
		top_status = 1;
	if (pCGI->lower_status_line)//��Ҫ��״̬��
		lower_status = 1;
	if (!pCGI->CFI.separator)//��ʱ�޷ָ���
	{
		special_row = 1;
		special_col = 2;
	}

	/*�У���/�¶���ռ�+��״̬��+�б���ʾ+������+��״̬����+ 4*/
	pCGI->lines = special_row + (pCGI->CFI.bhigh * pCGI->row_num + 1) + (pCGI->extern_down_lines + pCGI->extern_up_lines) + (col_sign + top_status + lower_status) + 4;

	/*�У���/�Ҷ���ռ�+�б���ʾ+������+3*/
	pCGI->cols = special_col + (pCGI->CFI.bwidth * pCGI->col_num) + (pCGI->extern_left_cols + pCGI->extern_right_cols) + (row_sign)+3;

	/*��������״̬������ʼ����*/
	pCGI->SLI.top_start_x = pCGI->extern_left_cols;
	pCGI->SLI.top_start_y = pCGI->extern_up_lines;
	pCGI->SLI.lower_start_x = pCGI->extern_left_cols;
	pCGI->SLI.lower_start_y = special_row + pCGI->extern_up_lines + (pCGI->CFI.bhigh * pCGI->row_num + 1) + col_sign + top_status;

	/*״̬������ռ���*/
	pCGI->SLI.width = special_col + pCGI->CFI.bwidth * pCGI->col_num + row_sign + 3;

	/*��Ϸ����ܵ���ʼλ��(��ʾɫ��ͼ��������ʼλ��)*/
	/*ע����Ϸ����ܣ������а���m��n�е�ɫ�������⣬������״̬����0/1�У�/��״̬����0/1�У�/�к���ʾ��0/2�У�/�б���ʾ����0/1�У�
	   -     ������ֵ������ͬ������£�start_x/start_y��ֵ�ǲ�ͬ��*/
	pCGI->start_x = pCGI->extern_left_cols + row_sign;
	pCGI->start_y = pCGI->extern_up_lines + top_status + col_sign;

	return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�11�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��1 - ȫ�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const int type)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	switch (type)//����ȱʡ����صı߿�ֵ
	{
		case 1://ȫ��
			strcpy(pCGI->CFI.top_left, "�X");
			strcpy(pCGI->CFI.lower_left, "�^");
			strcpy(pCGI->CFI.top_right, "�[");
			strcpy(pCGI->CFI.lower_right, "�a");
			strcpy(pCGI->CFI.h_normal, "�T");
			strcpy(pCGI->CFI.v_normal, "�U");
			strcpy(pCGI->CFI.h_top_separator, "�j");
			strcpy(pCGI->CFI.h_lower_separator, "�m");
			strcpy(pCGI->CFI.v_left_separator, "�d");
			strcpy(pCGI->CFI.v_right_separator, "�g");
			strcpy(pCGI->CFI.mid_separator, "�p");
			break;
		case 2://ȫ����
			strcpy(pCGI->CFI.top_left, "��");
			strcpy(pCGI->CFI.lower_left, "��");
			strcpy(pCGI->CFI.top_right, "��");
			strcpy(pCGI->CFI.lower_right, "��");
			strcpy(pCGI->CFI.h_normal, "��");
			strcpy(pCGI->CFI.v_normal, "��");
			strcpy(pCGI->CFI.h_top_separator, "��");
			strcpy(pCGI->CFI.h_lower_separator, "��");
			strcpy(pCGI->CFI.v_left_separator, "��");
			strcpy(pCGI->CFI.v_right_separator, "��");
			strcpy(pCGI->CFI.mid_separator, "��");
			break;
		case 3://��˫����
			strcpy(pCGI->CFI.top_left, "�V");
			strcpy(pCGI->CFI.lower_left, "�\");
			strcpy(pCGI->CFI.top_right, "�Y");
			strcpy(pCGI->CFI.lower_right, "�_");
			strcpy(pCGI->CFI.h_normal, "�T");
			strcpy(pCGI->CFI.v_normal, "��");
			strcpy(pCGI->CFI.h_top_separator, "�h");
			strcpy(pCGI->CFI.h_lower_separator, "�k");
			strcpy(pCGI->CFI.v_left_separator, "�b");
			strcpy(pCGI->CFI.v_right_separator, "�e");
			strcpy(pCGI->CFI.mid_separator, "�n");
			break;
		case 4://�ᵥ��˫
			strcpy(pCGI->CFI.top_left, "�W");
			strcpy(pCGI->CFI.lower_left, "�]");
			strcpy(pCGI->CFI.top_right, "�Z");
			strcpy(pCGI->CFI.lower_right, "�`");
			strcpy(pCGI->CFI.h_normal, "��");
			strcpy(pCGI->CFI.v_normal, "�U");
			strcpy(pCGI->CFI.h_top_separator, "�i");
			strcpy(pCGI->CFI.h_lower_separator, "�l");
			strcpy(pCGI->CFI.v_left_separator, "�c");
			strcpy(pCGI->CFI.v_right_separator, "�f");
			strcpy(pCGI->CFI.mid_separator, "�o");
			break;
		default:
			cout << "error������" << endl;
			return -1;
	}
	return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�11������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const char *...						����11�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
			1������2�ֽ���ֻȡǰ2�ֽ�
			2�������NULL���������ո����
			3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const char* top_left, const char* lower_left, const char* top_right,
	const char* lower_right, const char* h_normal, const char* v_normal, const char* h_top_separator,
	const char* h_lower_separator, const char* v_left_separator, const char* v_right_separator, const char* mid_separator)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	/*�޸Ľṹ�еĸ�������*/

	/*top_left*/
	if (strlen(top_left) == NULL)
		strcpy(pCGI->CFI.top_left, "  ");//�������ո�
	else if (strlen(top_left) == 1)
	{
		pCGI->CFI.top_left[0] = top_left[0];
		pCGI->CFI.top_left[1] = ' ';//��һ���ո�
	}
	else
		strncpy(pCGI->CFI.top_left, top_left, 2);

	/*lower_left*/
	if (strlen(lower_left) == NULL)
		strcpy(pCGI->CFI.lower_left, "  ");//�������ո�
	else if (strlen(lower_left) == 1)
	{
		pCGI->CFI.lower_left[0] = lower_left[0];
		pCGI->CFI.lower_left[1] = ' ';//��һ���ո�
	}
	else
		strncpy(pCGI->CFI.lower_left, lower_left, 2);

	/*top_right*/
	if (strlen(top_right) == NULL)
		strcpy(pCGI->CFI.top_right, "  ");//�������ո�
	else if (strlen(top_right) == 1)
	{
		pCGI->CFI.top_right[0] = top_right[0];
		pCGI->CFI.top_right[1] = ' ';//��һ���ո�
	}
	else
		strncpy(pCGI->CFI.top_right, top_right, 2);

	/*lower_right*/
	if (strlen(lower_right) == NULL)
		strcpy(pCGI->CFI.lower_right, "  ");//�������ո�
	else if (strlen(lower_right) == 1)
	{
		pCGI->CFI.lower_right[0] = lower_right[0];
		pCGI->CFI.lower_right[1] = ' ';//��һ���ո�
	}
	else
		strncpy(pCGI->CFI.lower_right, lower_right, 2);

	/*h_normal*/
	if (strlen(h_normal) == NULL)
		strcpy(pCGI->CFI.h_normal, "  ");//�������ո�
	else if (strlen(h_normal) == 1)
	{
		pCGI->CFI.h_normal[0] = h_normal[0];
		pCGI->CFI.h_normal[1] = ' ';//��һ���ո�
	}
	else
		strncpy(pCGI->CFI.h_normal, h_normal, 2);

	/*v_normal*/
	if (strlen(v_normal) == NULL)
		strcpy(pCGI->CFI.v_normal, "  ");//�������ո�
	else if (strlen(v_normal) == 1)
	{
		pCGI->CFI.v_normal[0] = v_normal[0];
		pCGI->CFI.v_normal[1] = ' ';//��һ���ո�
	}
	else
		strncpy(pCGI->CFI.v_normal, v_normal, 2);

	/*h_top_separator*/
	if (strlen(h_top_separator) == NULL)
		strcpy(pCGI->CFI.h_top_separator, "  ");//�������ո�
	else if (strlen(h_top_separator) == 1)
	{
		pCGI->CFI.h_top_separator[0] = h_top_separator[0];
		pCGI->CFI.h_top_separator[1] = ' ';//��һ���ո�
	}
	else
		strncpy(pCGI->CFI.h_top_separator, h_top_separator, 2);

	/*h_lower_separator*/
	if (strlen(h_lower_separator) == NULL)
		strcpy(pCGI->CFI.h_lower_separator, "  ");//�������ո�
	else if (strlen(h_lower_separator) == 1)
	{
		pCGI->CFI.h_lower_separator[0] = h_lower_separator[0];
		pCGI->CFI.h_lower_separator[1] = ' ';//��һ���ո�
	}
	else
		strncpy(pCGI->CFI.h_lower_separator, h_lower_separator, 2);

	/*v_left_separator*/
	if (strlen(v_left_separator) == NULL)
		strcpy(pCGI->CFI.v_left_separator, "  ");//�������ո�
	else if (strlen(v_left_separator) == 1)
	{
		pCGI->CFI.v_left_separator[0] = v_left_separator[0];
		pCGI->CFI.v_left_separator[1] = ' ';//��һ���ո�
	}
	else
		strncpy(pCGI->CFI.v_left_separator, v_left_separator, 2);

	/*v_right_separator*/
	if (strlen(v_right_separator) == NULL)
		strcpy(pCGI->CFI.v_right_separator, "  ");//�������ո�
	else if (strlen(v_right_separator) == 1)
	{
		pCGI->CFI.v_right_separator[0] = v_right_separator[0];
		pCGI->CFI.v_right_separator[1] = ' ';//��һ���ո�
	}
	else
		strncpy(pCGI->CFI.v_right_separator, v_right_separator, 2);

	/*mid_separator*/
	if (strlen(mid_separator) == NULL)
		strcpy(pCGI->CFI.mid_separator, "  ");//�������ո�
	else if (strlen(mid_separator) == 1)
	{
		pCGI->CFI.mid_separator[0] = mid_separator[0];
		pCGI->CFI.mid_separator[1] = ' ';//��һ���ո�
	}
	else
		strncpy(pCGI->CFI.mid_separator, mid_separator, 2);

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPEO �ṹ�е�ɫ��������С���Ƿ���Ҫ�ָ��ߵ�
  ������������������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int block_width						����ȣ�����ȱʡ2����ΪԼ�������Ϊ�����Ʊ�����������������Ҫ+1��
			const int block_high						���߶ȣ�����ȱʡ1��
			const bool separator						���Ƿ���Ҫ�ָ��ߣ�ȱʡΪtrue����Ҫ�ָ��ߣ�
  �� �� ֵ��
  ˵    ������ܴ�С/�Ƿ���Ҫ�ָ��ߵȵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO* const pCGI, const int block_width, const int block_high, const bool separator)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	/*������ظ�ֵ����*/
	if (block_width % 2 == 1)//���Ϊ����Ҫת��Ϊż��
		pCGI->CFI.block_width = block_width + 1;
	else
		pCGI->CFI.block_width = block_width;

	pCGI->CFI.block_high = block_high;

	pCGI->CFI.separator = separator;//�޸��Ƿ���Ҫ�ָ���

	/*�޸���ص�ֵ*/
	int high_, width_;
	if (pCGI->CFI.separator) {
		width_ = 2;
		high_ = 1;
	}
	else {
		width_ = 0;
		high_ = 0;
	}//ע����ʱ��ʹ��block_width_ext/block_high_ext
	pCGI->CFI.block_high_ext = 1;
	pCGI->CFI.block_width_ext = 2;
	pCGI->CFI.bhigh = pCGI->CFI.block_high + high_;
	pCGI->CFI.bwidth = pCGI->CFI.block_width + width_;

	/*�޸�cmd���ڵĴ�С*/
	int row_sign = 0, col_sign = 0, top_status = 0, lower_status = 0, special_row = 0, special_col = 0;
	/*ȷ������״̬�������б����ʾ���
	  ע�⣺��״̬����0/1�У�/��״̬����0/1�У�/�к���ʾ��0/2�У�/�б���ʾ����0/1�У�*/
	if (pCGI->draw_frame_with_row_no)//��Ҫ�б���ʾ
		row_sign = 2;
	if (pCGI->draw_frame_with_col_no)//��Ҫ�б���ʾ
		col_sign = 1;
	if (pCGI->top_status_line)//��Ҫ��״̬��
		top_status = 1;
	if (pCGI->lower_status_line)//��Ҫ��״̬��
		lower_status = 1;
	if (!pCGI->CFI.separator)//��ʱ�޷ָ���
	{
		special_row = 1;
		special_col = 2;
	}


	/*�У���/�¶���ռ�+��״̬��+�б���ʾ+������+��״̬����+ 4*/
	pCGI->lines = special_row + (pCGI->CFI.bhigh * pCGI->row_num + 1) + (pCGI->extern_down_lines + pCGI->extern_up_lines) + (col_sign + top_status + lower_status) + 4;

	/*�У���/�Ҷ���ռ�+�б���ʾ+������+3*/
	pCGI->cols = special_col + (pCGI->CFI.bwidth * pCGI->col_num) + (pCGI->extern_left_cols + pCGI->extern_right_cols) + (row_sign)+3;

	/*��������״̬������ʼ����*/
	pCGI->SLI.top_start_x = pCGI->extern_left_cols;
	pCGI->SLI.top_start_y = pCGI->extern_up_lines;
	pCGI->SLI.lower_start_x = pCGI->extern_left_cols;
	pCGI->SLI.lower_start_y = special_row + pCGI->extern_up_lines + (pCGI->CFI.bhigh * pCGI->row_num + 1) + col_sign + top_status;

	/*״̬������ռ���*/
	pCGI->SLI.width = special_col + pCGI->CFI.bwidth * pCGI->col_num + row_sign + 3;

	/*��Ϸ����ܵ���ʼλ��(��ʾɫ��ͼ��������ʼλ��)*/
	/*ע����Ϸ����ܣ������а���m��n�е�ɫ�������⣬������״̬����0/1�У�/��״̬����0/1�У�/�к���ʾ��0/2�У�/�б���ʾ����0/1�У�
	   -     ������ֵ������ͬ������£�start_x/start_y��ֵ�ǲ�ͬ��*/
	pCGI->start_x = pCGI->extern_left_cols + row_sign;
	pCGI->start_y = pCGI->extern_up_lines + top_status + col_sign;

	return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BORDER_TYPE �ṹ�е���ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int bg_color					������ɫ��ȱʡ -1��ʾ�ô��ڱ���ɫ��
			const int fg_color					��ǰ��ɫ��ȱʡ -1��ʾ�ô���ǰ��ɫ��
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO* const pCGI, const int bgcolor, const int fgcolor)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*�޸�CONSOLE_FRAME_INFO�����������ɫ*/
	pCGI->CFI.bgcolor = bgcolor;
	pCGI->CFI.fgcolor = fgcolor;

	return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��1 - ȫ˫�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const int type)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	switch (type)
	{
		case 1://ȫ˫��
			strcpy(pCGI->CBI.top_left, "�X");
			strcpy(pCGI->CBI.lower_left, "�^");
			strcpy(pCGI->CBI.top_right, "�[");
			strcpy(pCGI->CBI.lower_right, "�a");
			strcpy(pCGI->CBI.h_normal, "�T");
			strcpy(pCGI->CBI.v_normal, "�U");
			break;
		case 2://ȫ����
			strcpy(pCGI->CBI.top_left, "��");
			strcpy(pCGI->CBI.lower_left, "��");
			strcpy(pCGI->CBI.top_right, "��");
			strcpy(pCGI->CBI.lower_right, "��");
			strcpy(pCGI->CBI.h_normal, "��");
			strcpy(pCGI->CBI.v_normal, "��");
			break;
		case 3://��˫����
			strcpy(pCGI->CBI.top_left, "�V");
			strcpy(pCGI->CBI.lower_left, "�\");
			strcpy(pCGI->CBI.top_right, "�Y");
			strcpy(pCGI->CBI.lower_right, "�_");
			strcpy(pCGI->CBI.h_normal, "�T");
			strcpy(pCGI->CBI.v_normal, "��");
			break;
		case 4://�ᵥ��˫
			strcpy(pCGI->CBI.top_left, "�W");
			strcpy(pCGI->CBI.lower_left, "�]");
			strcpy(pCGI->CBI.top_right, "�Z");
			strcpy(pCGI->CBI.lower_right, "�`");
			strcpy(pCGI->CBI.h_normal, "��");
			strcpy(pCGI->CBI.v_normal, "�U");
			break;
		default:
			cout << "error������" << endl;
			return -1;
	}

	return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const char *...					����6�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
			1������2�ֽ���ֻȡǰ2�ֽ�
			2�������NULL���������ո����
			3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const char* top_left, const char* lower_left, const char* top_right, const char* lower_right, const char* h_normal, const char* v_normal)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*top_left*/
	if (strlen(top_left) == NULL)
		strcpy(pCGI->CBI.top_left, "  ");//�������ո�
	else if (strlen(top_left) == 1)
	{
		pCGI->CBI.top_left[0] = top_left[0];
		pCGI->CBI.top_left[1] = ' ';//��һ���ո�
	}
	else
		strncpy(pCGI->CBI.top_left, top_left, 2);

	/*lower_left*/
	if (strlen(lower_left) == NULL)
		strcpy(pCGI->CBI.lower_left, "  ");//�������ո�
	else if (strlen(lower_left) == 1)
	{
		pCGI->CBI.lower_left[0] = lower_left[0];
		pCGI->CBI.lower_left[1] = ' ';//��һ���ո�
	}
	else
		strncpy(pCGI->CBI.lower_left, lower_left, 2);

	/*top_right*/
	if (strlen(top_right) == NULL)
		strcpy(pCGI->CBI.top_right, "  ");//�������ո�
	else if (strlen(top_right) == 1)
	{
		pCGI->CBI.top_right[0] = top_right[0];
		pCGI->CBI.top_right[1] = ' ';//��һ���ո�
	}
	else
		strncpy(pCGI->CBI.top_right, top_right, 2);

	/*lower_right*/
	if (strlen(lower_right) == NULL)
		strcpy(pCGI->CBI.lower_right, "  ");//�������ո�
	else if (strlen(lower_right) == 1)
	{
		pCGI->CBI.lower_right[0] = lower_right[0];
		pCGI->CBI.lower_right[1] = ' ';//��һ���ո�
	}
	else
		strncpy(pCGI->CBI.lower_right, lower_right, 2);

	/*h_normal*/
	if (strlen(h_normal) == NULL)
		strcpy(pCGI->CBI.h_normal, "  ");//�������ո�
	else if (strlen(h_normal) == 1)
	{
		pCGI->CBI.h_normal[0] = h_normal[0];
		pCGI->CBI.h_normal[1] = ' ';//��һ���ո�
	}
	else
		strncpy(pCGI->CBI.h_normal, h_normal, 2);

	/*v_normal*/
	if (strlen(v_normal) == NULL)
		strcpy(pCGI->CBI.v_normal, "  ");//�������ո�
	else if (strlen(v_normal) == 1)
	{
		pCGI->CBI.v_normal[0] = v_normal[0];
		pCGI->CBI.v_normal[1] = ' ';//��һ���ո�
	}
	else
		strncpy(pCGI->CBI.v_normal, v_normal, 2);

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ÿ����Ϸɫ��(����)�Ƿ���ҪС�߿�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const bool on_off					��true - ��Ҫ flase - ����Ҫ��ȱʡfalse��
  �� �� ֵ��
  ˵    �����߿�Լ��Ϊ�����Ʊ����˫��
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO* const pCGI, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->CBI.block_border = on_off;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ����״̬��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��״̬�����ͣ���/�£�(0��/1��)
			const bool on_off					��true - ��Ҫ flase - ����Ҫ��ȱʡtrue��
  �� �� ֵ��
  ˵    ����1��״̬�������Լ�����£�
			   1.1����״̬��ֻ��һ�У������������Ϸ�����/�б�����棬Ϊ��������ʼһ�У�����������Ͻ����������״̬�������꣩
			   1.2����״̬��ֻ��һ�У������������·����ߵ�����
			   1.3��״̬���Ŀ��Ϊ�������ȣ������Ϣ������ض�
		   2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO* const pCGI, const int type, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*�޸��Ƿ���Ҫ״̬���Ĳ���*/
	switch (type)
	{
		case TOP_STATUS_LINE://��״̬��
			pCGI->top_status_line = on_off;
			break;
		case LOWER_STATUS_LINE://��״̬��
			pCGI->lower_status_line = on_off;
			break;
		default:
			return -1;
	}

	/*�޸�cmd���ڵĴ�С*/
	int row_sign = 0, col_sign = 0, top_status = 0, lower_status = 0, special_row = 0, special_col = 0;
	/*ȷ������״̬�������б����ʾ���
	  ע�⣺��״̬����0/1�У�/��״̬����0/1�У�/�к���ʾ��0/2�У�/�б���ʾ����0/1�У�*/
	if (pCGI->draw_frame_with_row_no)//��Ҫ�б���ʾ
		row_sign = 2;
	if (pCGI->draw_frame_with_col_no)//��Ҫ�б���ʾ
		col_sign = 1;
	if (pCGI->top_status_line)//��Ҫ��״̬��
		top_status = 1;
	if (pCGI->lower_status_line)//��Ҫ��״̬��
		lower_status = 1;
	if (!pCGI->CFI.separator)//��ʱ�޷ָ���
	{
		special_row = 1;
		special_col = 2;
	}

	/*�У���/�¶���ռ�+��״̬��+�б���ʾ+������+��״̬����+ 4*/
	pCGI->lines = special_row + (pCGI->CFI.bhigh * pCGI->row_num + 1) + (pCGI->extern_down_lines + pCGI->extern_up_lines) + (col_sign + top_status + lower_status) + 4;

	/*�У���/�Ҷ���ռ�+�б���ʾ+������+3*/
	pCGI->cols = special_col + (pCGI->CFI.bwidth * pCGI->col_num) + (pCGI->extern_left_cols + pCGI->extern_right_cols) + (row_sign)+3;

	/*��������״̬������ʼ����*/
	pCGI->SLI.top_start_x = pCGI->extern_left_cols;
	pCGI->SLI.top_start_y = pCGI->extern_up_lines;
	pCGI->SLI.lower_start_x = pCGI->extern_left_cols;
	pCGI->SLI.lower_start_y = special_row + pCGI->extern_up_lines + (pCGI->CFI.bhigh * pCGI->row_num + 1) + col_sign + top_status;

	/*״̬������ռ���*/
	pCGI->SLI.width = special_col + pCGI->CFI.bwidth * pCGI->col_num + row_sign + 3;

	/*��Ϸ����ܵ���ʼλ��(��ʾɫ��ͼ��������ʼλ��)*/
	/*ע����Ϸ����ܣ������а���m��n�е�ɫ�������⣬������״̬����0/1�У�/��״̬����0/1�У�/�к���ʾ��0/2�У�/�б���ʾ����0/1�У�
	   -     ������ֵ������ͬ������£�start_x/start_y��ֵ�ǲ�ͬ��*/
	pCGI->start_x = pCGI->extern_left_cols + row_sign;
	pCGI->start_y = pCGI->extern_up_lines + top_status + col_sign;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���������״̬������ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��״̬�����ͣ���/�£�(0��/1��)
			const int normal_bgcolor			�������ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
			const int normal_fgcolor			�������ı�ǰ��ɫ��ȱʡ -1��ʾʹ�ô���ǰ��ɫ��
			const int catchy_bgcolor			����Ŀ�ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
			const int catchy_fgcolor			����Ŀ�ı�ǰ��ɫ��ȱʡ -1��ʾʹ������ɫ��
  ���������
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO* const pCGI, const int type, const int normal_bgcolor, const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*������ɫ��ȱʡ����*/
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
	if (catchy_fgcolor == -1)//��Ŀ����ȱʡǰ��ɫΪ����ɫ
		cat_fgcolor = COLOR_HYELLOW;
	else
		cat_fgcolor = catchy_fgcolor;


	switch (type)
	{
		case TOP_STATUS_LINE://��״̬��
			pCGI->SLI.top_normal_bgcolor = nor_bgcolor;
			pCGI->SLI.top_normal_fgcolor = nor_fgcolor;
			pCGI->SLI.top_catchy_bgcolor = cat_bgcolor;
			pCGI->SLI.top_catchy_fgcolor = cat_fgcolor;
			break;
		case  LOWER_STATUS_LINE://��״̬��
			pCGI->SLI.lower_normal_bgcolor = nor_bgcolor;
			pCGI->SLI.lower_normal_fgcolor = nor_fgcolor;
			pCGI->SLI.lower_catchy_bgcolor = cat_bgcolor;
			pCGI->SLI.lower_catchy_fgcolor = cat_fgcolor;
		default:
			return -1;
	}

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�к�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const bool on_off					��true - ��ʾ flase - ����ʾ��ȱʡfalse��
  �� �� ֵ��
  ˵    ����1���к�Լ��Ϊ��ĸA��ʼ�������У��������26�����a��ʼ������52��ͳһΪ*��ʵ��Ӧ�ò����ܣ�
			2���Ƿ���ʾ�кŵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO* const pCGI, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	/*�����Ƿ���Ҫ�к�*/
	pCGI->draw_frame_with_row_no = on_off;

	/*�޸�cmd�����еĴ�С*/
	int row_sign = 0, special_row = 0, special_col = 0;
	/*ȷ������״̬�������б����ʾ���
	  ע�⣺��״̬����0/1�У�/��״̬����0/1�У�/�к���ʾ��0/2�У�/�б���ʾ����0/1�У�*/
	if (pCGI->draw_frame_with_row_no)//��Ҫ�б���ʾ
		row_sign = 2;
	if (!pCGI->CFI.separator)//��ʱ�޷ָ���
	{
		special_row = 1;
		special_col = 2;
	}


	/*�У���/�Ҷ���ռ�+�б���ʾ+������+3*/
	pCGI->cols = special_col + (pCGI->CFI.bwidth * pCGI->col_num) + (pCGI->extern_left_cols + pCGI->extern_right_cols) + (row_sign)+3;


	/*״̬������ռ���*/
	pCGI->SLI.width = special_col + pCGI->CFI.bwidth * pCGI->col_num + row_sign + 3;//��Ⱥ��кŵ�λ��

	/*��Ϸ����ܵ���ʼλ��(��ʾɫ��ͼ��������ʼλ��)*/
	/*ע����Ϸ����ܣ������а���m��n�е�ɫ�������⣬������״̬����0/1�У�/��״̬����0/1�У�/�к���ʾ��0/2�У�/�б���ʾ����0/1�У�
	   -     ������ֵ������ͬ������£�start_x/start_y��ֵ�ǲ�ͬ��*/
	pCGI->start_x = pCGI->extern_left_cols + row_sign;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�б�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const bool on_off					��true - ��ʾ flase - ����ʾ��ȱʡfalse��
  �� �� ֵ��
  ˵    ����1���б�Լ��Ϊ����0��ʼ�������У�����0-99������99ͳһΪ**��ʵ��Ӧ�ò����ܣ�
			2���Ƿ���ʾ�б�ı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO* const pCGI, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*�����Ƿ���Ҫ�б�*/
	pCGI->draw_frame_with_col_no = on_off;

	/*�ı�cmd�����еĴ�С*/
	int  col_sign = 0, top_status = 0, lower_status = 0, special_row = 0, special_col = 0;
	/*ȷ������״̬�������б����ʾ���
	  ע�⣺��״̬����0/1�У�/��״̬����0/1�У�/�к���ʾ��0/2�У�/�б���ʾ����0/1�У�*/
	if (pCGI->draw_frame_with_col_no)//��Ҫ�б���ʾ
		col_sign = 1;
	if (pCGI->top_status_line)//��Ҫ��״̬��
		top_status = 1;
	if (pCGI->lower_status_line)//��Ҫ��״̬��
		lower_status = 1;
	if (!pCGI->CFI.separator)//��ʱ�޷ָ���
	{
		special_row = 1;
		special_col = 2;
	}


	/*�У���/�¶���ռ�+��״̬��+�б���ʾ+������+��״̬����+ 4*/
	pCGI->lines = special_row + (pCGI->CFI.bhigh * pCGI->row_num + 1) + (pCGI->extern_down_lines + pCGI->extern_up_lines) + (col_sign + top_status + lower_status) + 4;

	/*��������״̬������ʼ����*/
	pCGI->SLI.top_start_x = pCGI->extern_left_cols;
	pCGI->SLI.top_start_y = pCGI->extern_up_lines;
	pCGI->SLI.lower_start_x = pCGI->extern_left_cols;
	pCGI->SLI.lower_start_y = special_row + pCGI->extern_up_lines + (pCGI->CFI.bhigh * pCGI->row_num + 1) + col_sign + top_status;

	/*��Ϸ����ܵ���ʼλ��(��ʾɫ��ͼ��������ʼλ��)*/
	/*ע����Ϸ����ܣ������а���m��n�е�ɫ�������⣬������״̬����0/1�У�/��״̬����0/1�У�/�к���ʾ��0/2�У�/�б���ʾ����0/1�У�
	   -     ������ֵ������ͬ������£�start_x/start_y��ֵ�ǲ�ͬ��*/
	pCGI->start_y = pCGI->extern_up_lines + top_status + col_sign;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ӡ CONSOLE_GRAPHICS_INFO �ṹ���еĸ���Աֵ
  ���������
  �� �� ֵ��
  ˵    ����1�����������ã���ӡ��ʽ�Զ���
			2������������������δ���ù������Բ�ʵ��
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO* const pCGI)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*��ʱ��ʵ��*/

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��� CONSOLE_GRAPHICS_INFO �ṹ����ȱʡֵ���г�ʼ��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI������ṹָ��
		   const int row					����Ϸ����ɫ��������ȱʡ10��
		   const int col					����Ϸ����ɫ��������ȱʡ10��
		   const int bgcolor				���������ڱ���ɫ��ȱʡ COLOR_BLACK��
		   const int fgcolor				���������ڱ���ɫ��ȱʡ COLOR_WHITE��
  �� �� ֵ��
  ˵    �������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ��
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO* const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	/* �����ñ�� */
	pCGI->inited = CGI_INITED;//ִ�д˺������Ѿ����й���ʼ��

	/*���г�ʼ��*/

	/*����ȫ����Ϣ�ṹ���е������ֵĳ�ʼ��*/
	pCGI->extern_up_lines = 0;//�޶�������
	pCGI->extern_down_lines = 0;//�޶�������
	pCGI->extern_left_cols = 0;//�޶�������
	pCGI->extern_right_cols = 0;//�޶�������
	pCGI->row_num = row;
	pCGI->col_num = col;
	pCGI->area_bgcolor = bgcolor;
	pCGI->area_fgcolor = fgcolor;
	pCGI->top_status_line = true;//����״̬������
	pCGI->lower_status_line = true;//����״̬������
	pCGI->draw_frame_with_row_no = false;//�����к�
	pCGI->draw_frame_with_col_no = false;//�����б�
	pCGI->delay_of_block_moved = BLOCK_MOVED_DELAY_MS;
	pCGI->delay_of_draw_block = 0;
	pCGI->delay_of_draw_frame = 0;
	pCGI->start_x = 0;//��ʱ�޶���ռ䣬���к��б꣬������״̬��
	pCGI->start_y = 1;

	/*������Ϸ���������Ľṹ��(CFI)�ĳ�ʼ��*/
	strcpy(pCGI->CFI.top_left, "�X");
	strcpy(pCGI->CFI.lower_left, "�^");
	strcpy(pCGI->CFI.top_right, "�[");
	strcpy(pCGI->CFI.lower_right, "�a");
	strcpy(pCGI->CFI.h_normal, "�T");
	strcpy(pCGI->CFI.v_normal, "�U");
	strcpy(pCGI->CFI.h_top_separator, "�j");
	strcpy(pCGI->CFI.h_lower_separator, "�m");
	strcpy(pCGI->CFI.v_left_separator, "�d");
	strcpy(pCGI->CFI.v_right_separator, "�g");
	strcpy(pCGI->CFI.mid_separator, "�p");
	pCGI->CFI.bgcolor = bgcolor;
	pCGI->CFI.fgcolor = fgcolor;
	pCGI->CFI.block_width = 2;
	pCGI->CFI.block_high = 1;
	pCGI->CFI.separator = true;   //ȱʡɫ��֮����ָ���
	pCGI->CFI.block_width_ext = 2;
	pCGI->CFI.block_high_ext = 1;
	pCGI->CFI.bwidth = pCGI->CFI.block_width + pCGI->CFI.block_width_ext;
	pCGI->CFI.bhigh = pCGI->CFI.block_high + pCGI->CFI.block_high_ext;

	/*����ɫ����Ϣ�ṹ��(CBI)�ĳ�ʼ��*/
	strcpy(pCGI->CBI.top_left, "�X");
	strcpy(pCGI->CBI.lower_left, "�^");
	strcpy(pCGI->CBI.top_right, "�[");
	strcpy(pCGI->CBI.lower_right, "�a");
	strcpy(pCGI->CBI.h_normal, "�T");
	strcpy(pCGI->CBI.v_normal, "�U");
	pCGI->CBI.block_border = false;//ɫ���ڲ���С�߿�

	/*����״̬����Ϣ�ṹ��(SLI)�ĳ�ʼ��*/
	pCGI->SLI.is_lower_status_line = true;//����״̬��
	pCGI->SLI.is_top_status_line = true;//����״̬��
	pCGI->SLI.top_normal_bgcolor = bgcolor;
	pCGI->SLI.top_catchy_bgcolor = bgcolor;
	pCGI->SLI.lower_normal_bgcolor = bgcolor;
	pCGI->SLI.lower_catchy_bgcolor = bgcolor;
	pCGI->SLI.top_normal_fgcolor = fgcolor;
	pCGI->SLI.top_catchy_fgcolor = COLOR_HYELLOW;     //ȱʡΪ����ɫ
	pCGI->SLI.lower_catchy_fgcolor = COLOR_HYELLOW;   //ȱʡΪ����ɫ
	pCGI->SLI.lower_normal_fgcolor = fgcolor;
	pCGI->SLI.top_start_x = 0;
	pCGI->SLI.top_start_y = 0;
	pCGI->SLI.lower_start_x = 0;
	pCGI->SLI.lower_start_y = (pCGI->CFI.bhigh * pCGI->row_num + 1) + (pCGI->extern_down_lines + pCGI->extern_up_lines) + 1;
	pCGI->SLI.width = pCGI->CFI.bwidth * pCGI->col_num + 3;//width����Ϸ����Ŀ�ȣ���������չ������Ϣ���������ض�

	/*����������Ϣ�ṹ��(CFT)�ĳ�ʼ��*/
	pCGI->CFT.font_size_high = 16;
	pCGI->CFT.font_size_width = 8;
	strcpy(pCGI->CFT.font_type, "��������");

	/*������ ���и�ֵ*/
	/*�У���/�¶���ռ�+��״̬��+�б���ʾ+������+��״̬����+ 4*/
	pCGI->lines = pCGI->start_y + 0 + 0 + 1 + 0 + pCGI->CFI.bhigh * pCGI->row_num + 1 + 4;
	/*�У���/�Ҷ���ռ�+�б���ʾ+������+3*/
	pCGI->cols = pCGI->start_x + 0 + 0 + 0 + pCGI->CFI.bwidth * pCGI->col_num + 3;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������Ϸ���
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
  �� �� ֵ��
  ˵    ��������ɲο�demo��Ч��
***************************************************************************/
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO* const pCGI)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*����cmd���ڵĴ�С*/
	cct_setconsoleborder(pCGI->cols, pCGI->lines, pCGI->cols, pCGI->lines);
	/*��������*/
	cct_setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high, pCGI->CFT.font_size_width);
	/*������cmd���ڵ���ɫͳһһ��*/
	char color[9] = "color ";
	switch (pCGI->area_bgcolor)
	{
		case COLOR_BLACK:   //��
			color[6] = (char)('0' + COLOR_BLACK);
			break;
		case COLOR_BLUE:    //��
			color[6] = (char)('0' + COLOR_BLUE);
			break;
		case  COLOR_GREEN:  //��
			color[6] = (char)('0' + COLOR_GREEN);
			break;
		case  COLOR_CYAN:   //��
			color[6] = (char)('0' + COLOR_CYAN);
			break;
		case  COLOR_RED:	//��
			color[6] = (char)('0' + COLOR_RED);
			break;
		case  COLOR_PINK:	//��
			color[6] = (char)('0' + COLOR_PINK);
			break;
		case  COLOR_YELLOW:	//��
			color[6] = (char)('0' + COLOR_YELLOW);
			break;
		case  COLOR_WHITE:	//��
			color[6] = (char)('0' + COLOR_WHITE);
			break;
		case  COLOR_HBLACK:	//����
			color[6] = (char)('0' + COLOR_HBLACK);
			break;
		case  COLOR_HBLUE:	//����
			color[6] = (char)('0' + COLOR_GREEN);
			break;
		case  COLOR_HGREEN:	//����
			color[6] = (char)('A' + (COLOR_HGREEN - 10));
			break;
		case  COLOR_HCYAN:	//����
			color[6] = (char)('A' + (COLOR_HCYAN - 10));
			break;
		case  COLOR_HRED:	//����
			color[6] = (char)('A' + (COLOR_HRED - 10));
			break;
		case  COLOR_HPINK:	//����
			color[6] = (char)('A' + (COLOR_HPINK - 10));
			break;
		case COLOR_HYELLOW:	//����
			color[6] = (char)('A' + (COLOR_HYELLOW - 10));
			break;
		case  COLOR_HWHITE:	//����
			color[6] = (char)('A' + (COLOR_HWHITE - 10));
			break;
		default:
			break;
	}
	switch (pCGI->area_fgcolor)
	{
		case COLOR_BLACK:   //��
			color[7] = (char)('0' + COLOR_BLACK);
			break;
		case COLOR_BLUE:    //��
			color[7] = (char)('0' + COLOR_BLUE);
			break;
		case  COLOR_GREEN:  //��
			color[7] = (char)('0' + COLOR_GREEN);
			break;
		case  COLOR_CYAN:   //��
			color[7] = (char)('0' + COLOR_CYAN);
			break;
		case  COLOR_RED:	//��
			color[7] = (char)('0' + COLOR_RED);
			break;
		case  COLOR_PINK:	//��
			color[7] = (char)('0' + COLOR_PINK);
			break;
		case  COLOR_YELLOW:	//��
			color[7] = (char)('0' + COLOR_YELLOW);
			break;
		case  COLOR_WHITE:	//��
			color[7] = (char)('0' + COLOR_WHITE);
			break;
		case  COLOR_HBLACK:	//����
			color[7] = (char)('0' + COLOR_HBLACK);
			break;
		case  COLOR_HBLUE:	//����
			color[7] = (char)('0' + COLOR_GREEN);
			break;
		case  COLOR_HGREEN:	//����
			color[7] = (char)('A' + (COLOR_HGREEN - 10));
			break;
		case  COLOR_HCYAN:	//����
			color[7] = (char)('A' + (COLOR_HCYAN - 10));
			break;
		case  COLOR_HRED:	//����
			color[7] = (char)('A' + (COLOR_HRED - 10));
			break;
		case  COLOR_HPINK:	//����
			color[7] = (char)('A' + (COLOR_HPINK - 10));
			break;
		case COLOR_HYELLOW:	//����
			color[7] = (char)('A' + (COLOR_HYELLOW - 10));
			break;
		case  COLOR_HWHITE:	//����
			color[7] = (char)('A' + (COLOR_HWHITE - 10));
			break;
		default:
			break;
	}
	system(color);
	/*���������ɫ*/
	cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);


	/*ǰ����ӡ����ܵĿ�ͷλ��*/
	int row_sign = 0, col_sign = 0, top_status = 0, lower_status = 0;
	/*ȷ������״̬�������б����ʾ���
	  ע�⣺��״̬����0/1�У�/��״̬����0/1�У�/�к���ʾ��0/2�У�/�б���ʾ����0/1�У�*/
	if (pCGI->draw_frame_with_row_no)//��Ҫ�б���ʾ
		row_sign = 2;
	if (pCGI->draw_frame_with_col_no)//��Ҫ�б���ʾ
		col_sign = 1;
	if (pCGI->top_status_line)//��Ҫ��״̬��
		top_status = 1;
	if (pCGI->lower_status_line)//��Ҫ��״̬��
		lower_status = 1;
	int x = pCGI->extern_left_cols;
	int y = pCGI->extern_up_lines + top_status;
	cct_gotoxy(x, y);//�ƶ�����ͷλ�ã���ɫ�鼯���к��б꣩

	/*�ж��Ƿ�ӷָ���*/
	int x_add = 0, y_add = 0;
	if (pCGI->CFI.separator)//����ɫ���ı߽�Ϊ���ķָ��������ֽڣ�
	{
		x_add = 2;
		y_add = 1;
	}
	/*ȷ��ÿ��ɫ���Ӧ�ı߿����*/
	int width = pCGI->CFI.block_width / 2;//ÿ��ɫ���Ӧ�İ������ַ�����
	int high = pCGI->CFI.block_high;// ÿ��ɫ���Ӧ�İ������ַ�����

	/*��ӡ���--�б�*/
	if (pCGI->draw_frame_with_col_no)//��ʱ��Ҫ�б�
	{
		for (int i = 0; i <= pCGI->col_num - 1; i++)//��ӡ�б�
		{
			cct_gotoxy(x + row_sign + 2 + (width * 2 / 2 - 1) + (width * 2 + x_add) * i, y);
			if (i >= 100)
				cout << "**";
			else
				cout << i;
		}
		y++;
		cct_gotoxy(x, y);//д��һ���б����
	}


	/*��ӡ���--ɫ�鼯���б�*/
	int y_0 = y;
	int lie = 1;
	if (pCGI->CFI.separator)//��ʱ��Ҫ�ָ���
		for (int i = 1; i <= pCGI->row_num; i++)
		{
			cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);//���ÿ����ɫ
			/*ÿһ�е���ͷλ��*/
			cct_gotoxy(x, y);
			for (int j = 1; j <= pCGI->col_num; j++)
			{
				if (pCGI->draw_frame_with_row_no && j == 1)//��ʱ��Ҫ�к�
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
				if (j == pCGI->col_num)//����λ��ʱ���һ��
				{
					if (i == 1)
						cout << pCGI->CFI.top_right;
					else
						cout << pCGI->CFI.v_right_separator;
				}
				Sleep(pCGI->delay_of_draw_frame);//�����ʱ��ʱ
			}
			y++;

			/*ÿһ�е���ͨλ��*/
			for (int same = 1; same <= high; same++)
			{
				cct_gotoxy(x, y);
				if (pCGI->draw_frame_with_row_no)//��ʱ��Ҫ�к�
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
					for (int k = 1; k <= width; k++)//��λ���ÿո����
						cout << "  ";
					cout << pCGI->CFI.v_normal;
					Sleep(pCGI->delay_of_draw_frame);//�����ʱ��ʱ
				}
				y++;
			}

			/*�ر����һ��ʱ�ټ�һ��*/
			if (i == pCGI->row_num)
			{
				cct_gotoxy(x, y);
				if (pCGI->draw_frame_with_row_no)//��ʱ��Ҫ�к�
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
					Sleep(pCGI->delay_of_draw_frame);//�����ʱ��ʱ	
				}
				y++;
			}
		}

	else//��ʱ����ָ���
		for (int i = 1; i <= pCGI->row_num; i++)
		{
			cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);//������ɫ
			/*��һ�е���ͷλ��*/
			if (i == 1)
			{
				cct_gotoxy(x, y);
				for (int j = 1; j <= pCGI->col_num; j++)
				{
					if (pCGI->draw_frame_with_row_no && j == 1)//��ʱ��Ҫ�к�
					{
						cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
						cout << "  ";
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					}

					if (i == 1 && j == 1)
						cout << pCGI->CFI.top_left;
					for (int k = 1; k <= width; k++)
						cout << pCGI->CFI.h_normal;
					if (i == 1 && j == pCGI->col_num)//����λ��ʱ���һ��
						cout << pCGI->CFI.top_right;
					Sleep(pCGI->delay_of_draw_frame);//�����ʱ��ʱ
				}
				y++;
			}

			/*ÿһ�е���ͨλ��*/
			for (int same = 1; same <= high; same++)
			{
				cct_gotoxy(x, y);
				if (pCGI->draw_frame_with_row_no)//��ʱ��Ҫ�к�
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
					for (int k = 1; k <= width; k++)//��λ���ÿո����
						cout << "  ";
					if (j == pCGI->col_num)
						cout << pCGI->CFI.v_normal;
					Sleep(pCGI->delay_of_draw_frame);//�����ʱ��ʱ
				}
				y++;
			}

			/*�ر����һ��ʱ�ټ�һ��*/
			if (i == pCGI->row_num)
			{
				cct_gotoxy(x, y);
				if (pCGI->draw_frame_with_row_no)//��ʱ��Ҫ�к�
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
					Sleep(pCGI->delay_of_draw_frame);//�����ʱ��ʱ	
				}
				y++;
			}
		}

	cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//�ָ���ʼ��ɫ

	return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���״̬������ʾ��Ϣ
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int type							��ָ������/��״̬��
		   const char *msg						    ��������Ϣ
		   const char *catchy_msg					����Ҫ�ر��ע����Ϣ����������Ϣǰ��ʾ��
  �� �� ֵ��
  ˵    ����1���������޶�Ϊ����ܵĿ�ȣ����к��б�λ�ã����������ȥ
			2��������һ���ַ���ĳ���ֵ�ǰ������ᵼ�º������룬Ҫ����
***************************************************************************/
int gmw_status_line(const CONSOLE_GRAPHICS_INFO* const pCGI, const int type, const char* msg, const char* catchy_msg)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*�����ַ�����������*/
	int length_nor = (int)strlen(msg);//����β0���ַ�������
	int length_cat;
	if (catchy_msg == NULL)
		length_cat = 0;
	else
		length_cat = (int)strlen(catchy_msg);//����β0���ַ�������
	int max = -1;
	bool out_cat_ = true;//Ĭ�ϳ��ȿ������������ɫ��Ϣ


	/*ע������ʾ��ɫ��������ݣ���������ʾ�������������*/
	if (catchy_msg != NULL && length_cat >= pCGI->SLI.width)//���Ȳ�����cat���
	{
		/*�������һ���ַ�Ϊ���ֵ����*/
		if (msg[pCGI->SLI.width - 1] > 127)//��ʱĩβΪ���ֵ����
			max = pCGI->SLI.width - 1;
		out_cat_ = false;//��ʱ�޷������ɫ��Ϣ
	}
	else if (length_nor + length_cat >= pCGI->SLI.width)//���Ȳ���ȫ����nor���
	{
		max = pCGI->SLI.width - length_cat;
		/*�������һ���ַ�Ϊ���ֵ����*/
		if (catchy_msg != NULL && catchy_msg[max - 1] > 127)
			max = max - 1;
		out_cat_ = true;//��ʱ���������ɫ��Ϣ
	}


	/*��״̬������ʾ��Ϣ*/
	switch (type)
	{
		case TOP_STATUS_LINE://��״̬��
			if (!pCGI->top_status_line)//����ر���״̬�� ����ӡ
				break;
			cct_gotoxy(pCGI->SLI.top_start_x, pCGI->SLI.top_start_y);//�����λ�õĴ�ӡ�ַ�
			for (int i = 1; i <= pCGI->SLI.width; i++)
				cout << ' ';
			cct_gotoxy(pCGI->SLI.top_start_x, pCGI->SLI.top_start_y);//ǰ����Ӧ�����λ��
			cct_setcolor(pCGI->SLI.top_catchy_bgcolor, pCGI->SLI.top_catchy_fgcolor);//�ȴ�ӡ��ɫ����
			if (out_cat_ == false)//��ʱ���������ɫ����
				for (int i = 0; i <= max - 1; i++)
					cout << catchy_msg[i];
			else
			{
				if (catchy_msg != NULL)
					cout << catchy_msg;
				cct_setcolor(pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor);
				if (max == -1)//����������ȥ��ȫ��ʾ
					cout << msg;
				else
					for (int i = 0; i <= max - 1; i++)
						cout << msg[i];
			}
			cct_setcolor(pCGI->area_bgcolor, pCGI->area_bgcolor);//�ָ�ԭ����ɫ
			break;

		case LOWER_STATUS_LINE://��״̬��
			if (!pCGI->lower_status_line)//����ر���״̬�� ��ӡ
				break;
			cct_gotoxy(pCGI->SLI.lower_start_x, pCGI->SLI.lower_start_y);//�����λ�õĴ�ӡ�ַ�
			for (int i = 1; i <= pCGI->SLI.width; i++)
				cout << ' ';
			cct_gotoxy(pCGI->SLI.lower_start_x, pCGI->SLI.lower_start_y);//ǰ����Ӧ�����λ��
			cct_setcolor(pCGI->SLI.lower_catchy_bgcolor, pCGI->SLI.lower_catchy_fgcolor);//�ȴ�ӡ��ɫ����
			if (out_cat_ == false)//��ʱ���������ɫ����
				for (int i = 0; i <= max - 1; i++)
					cout << catchy_msg[i];
			else
			{
				if (catchy_msg != NULL)
					cout << catchy_msg;
				cct_setcolor(pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor);
				if (max == -1)//����������ȥ��ȫ��ʾ
					cout << msg;
				else
					for (int i = 0; i <= max - 1; i++)
						cout << msg[i];
			}
			cct_setcolor(pCGI->area_bgcolor, pCGI->area_bgcolor);//�ָ�ԭ����ɫ
			break;

		default:
			return -1;
	}

	cout << endl;//������������ �������ݲ�����ӡ���󣡣�������

	return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʾĳһ��ɫ��(����Ϊ�ַ���������Ϊrow/col)
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int row_no						    ���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int col_no						    ���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const BLOCK_DISPLAY_INFO *const bdi		����Ÿ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
  �� �� ֵ��
  ˵    ����1��BLOCK_DISPLAY_INFO �ĺ����ͷ�ļ����÷��ο���������
			2��bdi_valueΪ BDI_VALUE_BLANK ��ʾ�հ׿飬Ҫ���⴦��
***************************************************************************/
int gmw_draw_block(const CONSOLE_GRAPHICS_INFO* const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO* const bdi)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*value��ʾ��Ҫ��ӡ��ɫ�������λ��*/
	int value = 0;
	while (bdi[value].value != BDI_VALUE_END)
	{
		if (bdi[value].value == bdi_value)
			break;
		value++;

	}

	int x_add = 0, y_add = 0;
	if (pCGI->CFI.separator)//����ɫ���ı߽�Ϊ���ķָ��������ֽڣ�
	{
		x_add = 2;
		y_add = 1;
	}
	/*�ƶ���굽��Ӧλ��*/
	/*ע��start_x��start_y������Ϸ��ҳ�����ͷλ��*/
	int x = pCGI->start_x + 2 + col_no * (pCGI->CFI.block_width + x_add);
	int y = pCGI->start_y + 1 + row_no * (pCGI->CFI.block_high + y_add);
	cct_gotoxy(x, y);//ǰ�������ӡ����ͷλ��

	/*���д�ӡ*/

	/*������ɫ*/
	int color_bg = 0, color_fg = 0;
	if (bdi[value].bgcolor == -1)//Ĭ�ϱ���ɫ
		color_bg = pCGI->CFI.bgcolor;//��ɫΪ��ܵ���ɫ
	else
		color_bg = bdi[value].bgcolor;
	if (bdi[value].fgcolor == -1)//Ĭ�ϱ���ɫ
		color_fg = pCGI->CFI.fgcolor;//��ɫΪ��ܵ���ɫ
	else
		color_fg = bdi[value].fgcolor;
	cct_setcolor(color_bg, color_fg);//������ɫ

	/*�����ж�һ�� ��ӡ�߿��Ҫ���Ƿ���� ����Ϊ��֤ɫ���ӡ����*/
	//if (pCGI->CBI.block_border && ((pCGI->CFI.block_high < 3) || (pCGI->CFI.block_width < 6) || (int)(strlen(bdi->content) > (pCGI->CFI.block_width / 2 - 2))))
		//return -1;

	/*�����ַ��Ĵ�ӡ*/
	if (bdi_value == BDI_VALUE_BLANK)//������Ҫ��ӡ�հ׿��ʱ��
	{
		for (int j = 1; j <= pCGI->CFI.block_high; j++)
		{
			cct_gotoxy(x, y);
			for (int i = 1; i <= pCGI->CFI.block_width; i++)
				cout << ' ';
			Sleep(pCGI->delay_of_draw_block);//��ӡɫ��ʱ��ʱ
			y++;
		}
	}
	else//�ǿհ׿��ʱ��
	{
		/*������Ҫ��ӡɫ����ܳ���*/
		int width = pCGI->CFI.block_width;
		int high = pCGI->CFI.block_high;

		/*��Ҫ�߿�ʱ*/
		if (pCGI->CBI.block_border)//ע����Ϊ��֤��Ҫ�߿�ʱ���ַ���width>=6&&�ַ���high>=3!!!!!!
		{
			/*����������ֵ��λ����Ĭ���ڲ�����ֵ�ĳ���������ʾ�ĳ��ȷ�Χ�ڣ�������*/
			int site = 1;//��ʾ�ڲ�����ֵ��λ��
			int mom = 10;
			if (bdi[value].content == NULL)//��Ҫ��ӡ�ڲ�����
				while (1)
				{
					if (bdi[value].value / mom == 0)
						break;
					mom = mom * 10;
					site++;
				}

			cct_gotoxy(x, y);
			for (int i = 1; i <= width / 2; i++)//��һ��
			{
				if (i == 1)
					cout << pCGI->CBI.top_left;
				else if (i == width / 2)
					cout << pCGI->CBI.top_right;
				else
					cout << pCGI->CBI.h_normal;
				Sleep(pCGI->delay_of_draw_block);//��ӡɫ��ʱ��ʱ
			}
			y++;;

			for (int k = 1; k <= (high - 2); k++)//������ͨ��
			{
				cct_gotoxy(x, y);
				for (int i = 1; i <= width; i++)
				{
					if (i == 1)
					{
						cout << pCGI->CBI.v_normal;//��ӡ�߿�
						i++;//����һ��λ��
					}
					else if (i == width - 1)
					{
						cout << pCGI->CBI.v_normal;//��ӡ�߿�
						i++;//����һ��λ��
					}
					else
					{
						if (bdi[value].content == NULL)//��ӡ�ڲ�����ֵ
						{
							if (k == ((high - 2 + 1) / 2) && i == ((width - 4 - site) / 2 + 1) + 2)//�����������λ��
							{
								/*�ڲ�����ֵ���ڶ���λ�����������*/
								cout << bdi[value].value;
								i = i + site - 1;//������Ӧ��λ��
							}
							else
								cout << ' ';//�ո�
						}
						else//��ӡ��Ӧͼ��(ע�⣺����Ĭ��Ϊ���ֽڵ������ַ�ͼ�Σ�������)
						{
							if (k == ((high - 2) / 2 + 1) && i == width / 2)//��������λ��
							{
								cout << bdi[value].content;
								i++;//��ӡ��Ϊ���ֽ� ����Ҫ����һ���ո�λ
							}
							else
								cout << ' ';//�ո�
						}
					}
					Sleep(pCGI->delay_of_draw_block);//��ӡɫ��ʱ��ʱ
				}
				y++;
			}

			cct_gotoxy(x, y);
			for (int i = 1; i <= width / 2; i++)//���һ��
			{

				if (i == 1)
					cout << pCGI->CBI.lower_left;
				else if (i == width / 2)
					cout << pCGI->CBI.lower_right;
				else
					cout << pCGI->CBI.h_normal;
				Sleep(pCGI->delay_of_draw_block);//��ӡɫ��ʱ��ʱ

			}
			y++;
		}

		/*����߿�ʱ*/
		else
		{
			/*����������ֵ��λ����Ĭ���ڲ�����ֵ�ĳ���������ʾ�ĳ��ȷ�Χ�ڣ�������*/
			int site = 1;//��ʾ�ڲ�����ֵ��λ��
			int mom = 10;
			if (bdi[value].content == NULL)//��Ҫ��ӡ�ڲ�����
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
					if (bdi[value].content == NULL)//��ӡ�ڲ�����ֵ
					{
						if (j == ((high + 1) / 2) && i == ((width - site) / 2 + 1))//�����������λ��
						{
							/*�ڲ�����ֵ���ڶ���λ�����������*/
							cout << bdi[value].value;
							i = i + site - 1;//������Ӧ��λ��
						}
						else
							cout << ' ';//�ո�
					}
					else//��ӡ��Ӧͼ��(ע�⣺����Ĭ��Ϊ���ֽڵ������ַ�ͼ�Σ�������)
					{
						if (j == ((high + 1) / 2) && i == width / 2)//��������λ��
						{

							cout << bdi[value].content;
							i++;//��ӡ��Ϊ���ֽ� ����Ҫ����һ���ո�λ
						}
						else
							cout << ' ';//�ո�
					}
					Sleep(pCGI->delay_of_draw_block);//��ӡɫ��ʱ��ʱ
				}
				y++;
			}
		}
	}

	cct_setcolor(pCGI->area_bgcolor, pCGI->area_bgcolor);//�ָ�������ɫ����

	return 0;
}
/***************************************************************************
  �������ƣ�
  ��    �ܣ���ĳ��λ��(x,y)��ʾһ��ɫ��
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   int const x_0                            ����Ҫ��ӡ��λ�õ�x����
		   int const y_0                            ����Ҫ��ӡ��λ�õ�y����
		   const int blank_bdi_value				���ƶ����������ڶ���Ч����ʾʱ���ڱ�ʾ�հ׵�ֵ��һ��Ϊ0���˴���Ϊ�������룬�ǿ��ǵ����ܳ��ֵ����������
		   const BLOCK_DISPLAY_INFO *const bdi		�������ʾֵ/�հ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
  �� �� ֵ��
  ˵    ����bdi_valueΪ BDI_VALUE_BLANK ��ʾ�հ׿飬Ҫ���⴦��
***************************************************************************/
static int gmw_inner_block_move(const CONSOLE_GRAPHICS_INFO* const pCGI, int const x_0, int const y_0, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO* const bdi)
{

	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*value��ʾ��Ҫ��ӡ��ɫ�������λ��*/
	int value = 0;
	while (bdi[value].value != BDI_VALUE_END)
	{
		if (bdi[value].value == bdi_value)
			break;
		value++;

	}

	/*������ɫ*/
	int color_bg = 0, color_fg = 0;
	if (bdi[value].bgcolor == -1)//Ĭ�ϱ���ɫ
		color_bg = pCGI->CFI.bgcolor;//��ɫΪ��ܵ���ɫ
	else
		color_bg = bdi[value].bgcolor;
	if (bdi[value].fgcolor == -1)//Ĭ�ϱ���ɫ
		color_fg = pCGI->CFI.fgcolor;//��ɫΪ��ܵ���ɫ
	else
		color_fg = bdi[value].fgcolor;
	cct_setcolor(color_bg, color_fg);

	int x = x_0, y = y_0;

	/*�����ַ��Ĵ�ӡ*/
	if (bdi_value == BDI_VALUE_BLANK)//������Ҫ��ӡ�հ׿��ʱ��
	{
		for (int j = 1; j <= pCGI->CFI.block_high; j++)
		{
			cct_gotoxy(x, y);
			for (int i = 1; i <= pCGI->CFI.block_width; i++)
				cout << ' ';
			Sleep(pCGI->delay_of_draw_block);//��ӡɫ��ʱ��ʱ
			y++;
		}
	}
	else//�ǿհ׿��ʱ��
	{
		/*������Ҫ��ӡɫ����ܳ���*/
		int width = pCGI->CFI.block_width;
		int high = pCGI->CFI.block_high;

		/*��Ҫ�߿�ʱ*/
		if (pCGI->CBI.block_border)//ע����Ϊ��֤��Ҫ�߿�ʱ���ַ���width>=6&&�ַ���high>=3!!!!!!
		{
			/*����������ֵ��λ����Ĭ���ڲ�����ֵ�ĳ���������ʾ�ĳ��ȷ�Χ�ڣ�������*/
			int site = 1;//��ʾ�ڲ�����ֵ��λ��
			int mom = 10;
			if (bdi[value].content == NULL)//��Ҫ��ӡ�ڲ�����
				while (1)
				{
					if (bdi[value].value / mom == 0)
						break;
					mom = mom * 10;
					site++;
				}

			cct_gotoxy(x, y);
			for (int i = 1; i <= width / 2; i++)//��һ��
			{
				if (i == 1)
					cout << pCGI->CBI.top_left;
				else if (i == width / 2)
					cout << pCGI->CBI.top_right;
				else
					cout << pCGI->CBI.h_normal;
				Sleep(pCGI->delay_of_draw_block);//��ӡɫ��ʱ��ʱ
			}
			y++;;

			for (int k = 1; k <= (high - 2); k++)//������ͨ��
			{
				cct_gotoxy(x, y);
				for (int i = 1; i <= width; i++)
				{
					if (i == 1)
					{
						cout << pCGI->CBI.v_normal;//��ӡ�߿�
						i++;//����һ��λ��
					}
					else if (i == width - 1)
					{
						cout << pCGI->CBI.v_normal;//��ӡ�߿�
						i++;//����һ��λ��
					}
					else
					{
						if (bdi[value].content == NULL)//��ӡ�ڲ�����ֵ
						{
							if (k == ((high - 2 + 1) / 2) && i == ((width - 4 - site) / 2 + 1) + 2)//�����������λ��
							{
								/*�ڲ�����ֵ���ڶ���λ�����������*/
								cout << bdi[value].value;
								i = i + site - 1;//������Ӧ��λ��
							}
							else
								cout << ' ';//�ո�
						}
						else//��ӡ��Ӧͼ��(ע�⣺����Ĭ��Ϊ���ֽڵ������ַ�ͼ�Σ�������)
						{
							if (k == ((high - 2) / 2 + 1) && i == width / 2)//��������λ��
							{
								cout << bdi[value].content;
								i++;//��ӡ��Ϊ���ֽ� ����Ҫ����һ���ո�λ
							}
							else
								cout << ' ';//�ո�
						}
					}
					Sleep(pCGI->delay_of_draw_block);//��ӡɫ��ʱ��ʱ
				}
				y++;
			}

			cct_gotoxy(x, y);
			for (int i = 1; i <= width / 2; i++)//���һ��
			{

				if (i == 1)
					cout << pCGI->CBI.lower_left;
				else if (i == width / 2)
					cout << pCGI->CBI.lower_right;
				else
					cout << pCGI->CBI.h_normal;
				Sleep(pCGI->delay_of_draw_block);//��ӡɫ��ʱ��ʱ

			}
			y++;
		}

		/*����߿�ʱ*/
		else
		{
			/*����������ֵ��λ����Ĭ���ڲ�����ֵ�ĳ���������ʾ�ĳ��ȷ�Χ�ڣ�������*/
			int site = 1;//��ʾ�ڲ�����ֵ��λ��
			int mom = 10;
			if (bdi[value].content == NULL)//��Ҫ��ӡ�ڲ�����
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
					if (bdi[value].content == NULL)//��ӡ�ڲ�����ֵ
					{
						if (j == ((high + 1) / 2) && i == ((width - site) / 2 + 1))//�����������λ��
						{
							/*�ڲ�����ֵ���ڶ���λ�����������*/
							cout << bdi[value].value;
							i = i + site - 1;//������Ӧ��λ��
						}
						else
							cout << ' ';//�ո�
					}
					else//��ӡ��Ӧͼ��(ע�⣺����Ĭ��Ϊ���ֽڵ������ַ�ͼ�Σ�������)
					{
						if (j == ((high + 1) / 2) && i == width / 2)//��������λ��
						{
							cout << bdi[value].content;
							i++;//��ӡ��Ϊ���ֽ� ����Ҫ����һ���ո�λ
						}
						else
							cout << ' ';//�ո�
					}
					Sleep(pCGI->delay_of_draw_block);//��ӡɫ��ʱ��ʱ
				}
				y++;
			}
		}
	}

	cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//�ָ�Ĭ�ϵı�����ɫ

	return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ƶ�ĳһ��ɫ��
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int row_no						    ���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int col_no						    ���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const int blank_bdi_value				���ƶ����������ڶ���Ч����ʾʱ���ڱ�ʾ�հ׵�ֵ��һ��Ϊ0���˴���Ϊ�������룬�ǿ��ǵ����ܳ��ֵ����������
		   const BLOCK_DISPLAY_INFO *const bdi		�������ʾֵ/�հ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
		   const int direction						���ƶ�����һ�����֣������cmd_gmw_tools.h
		   const int distance						���ƶ����루��1��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_move_block(const CONSOLE_GRAPHICS_INFO* const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO* const bdi, const int direction, const int distance)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	int x_add = 0, y_add = 0;
	if (pCGI->CFI.separator)//����ɫ���ı߽�Ϊ���ķָ��������ֽڣ�
	{
		x_add = 2;
		y_add = 1;
	}

	/*�ƶ���굽��Ӧλ��*/
	/*ע��start_x��start_y������Ϸ��ҳ�����ͷλ��*/
	int x = pCGI->start_x + 2 + col_no * (pCGI->CFI.block_width + x_add);
	int y = pCGI->start_y + 1 + row_no * (pCGI->CFI.block_high + y_add);
	cct_gotoxy(x, y);//���ǰ�������ӡ����ͷλ��

	int distance_1 = 0;
	/*��ʼ�ƶ�����Ϊ��֤�ƶ��������ȷ�ԣ�������*/
	if (pCGI->CFI.separator)//��ʱ�зָ��ߵ����
	{

		switch (direction)
		{
			case DOWN_TO_UP://�зָ��� ����
				distance_1 = distance + distance * pCGI->CFI.block_high;
				for (int dis = 1; dis <= distance_1; dis++)
				{
					y--;
					gmw_inner_block_move(pCGI, x, y, bdi_value, blank_bdi_value, bdi);//���ƶ�һ�񲢴�ӡɫ��
					if (dis % (pCGI->CFI.block_high + y_add) == 0)//���ƶ���һ��ʱ
					{
						cct_gotoxy(x, (y + pCGI->CFI.block_high));
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);//ʹ�ÿ�ܵ���ɫ����
						for (int k = 1; k <= (pCGI->CFI.block_width / 2); k++)
							cout << pCGI->CFI.h_normal;
						cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//�ָ�Ĭ����ɫ
					}
					else//���ƶ�δ��һ��ʱ
					{
						cct_gotoxy(x, (y + pCGI->CFI.block_high));
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);//ʹ�ÿ�ܵ���ɫ����
						for (int k = 1; k <= (pCGI->CFI.block_width / 2); k++)
							cout << "  ";//һ������
						cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//�ָ�Ĭ����ɫ
					}
					Sleep(pCGI->delay_of_block_moved);//�ƶ���ʱ

				}
				break;

			case UP_TO_DOWN://�зָ��� ����
				distance_1 = distance + distance * pCGI->CFI.block_high;
				for (int dis = 1; dis <= distance_1; dis++)
				{
					y++;
					gmw_inner_block_move(pCGI, x, y, bdi_value, blank_bdi_value, bdi);//���ƶ�һ�񲢴�ӡɫ��
					if (dis % (pCGI->CFI.block_high + y_add) == 0)//���ƶ���һ��ʱ
					{
						cct_gotoxy(x, (y - 1));
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);//ʹ��ɫ�鼯����ɫѡ��
						for (int k = 1; k <= (pCGI->CFI.block_width / 2); k++)
							cout << pCGI->CFI.h_normal;
						cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//�ָ�Ĭ����ɫ
					}
					else//���ƶ�δ��һ��ʱ
					{
						cct_gotoxy(x, (y - 1));
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						for (int k = 1; k <= (pCGI->CFI.block_width / 2); k++)
							cout << "  ";//һ������
						cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//�ָ�Ĭ����ɫ
					}
					Sleep(pCGI->delay_of_block_moved);//�ƶ���ʱ
				}
				break;

			case RIGHT_TO_LEFT://�зָ��� ����
				distance_1 = distance + distance * (pCGI->CFI.block_width / 2);
				for (int dis = 1; dis <= distance_1; dis++)
				{
					/*һ���ƶ�����*/
					x--;
					x--;
					gmw_inner_block_move(pCGI, x, y, bdi_value, blank_bdi_value, bdi);//���ƶ�һ�񲢴�ӡɫ��
					if (dis % ((pCGI->CFI.block_width + x_add) / 2) == 0)//���ƶ���һ��ʱ
					{
						cct_gotoxy((x + pCGI->CFI.block_width), y);
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						for (int k = 1; k <= pCGI->CFI.block_high; k++)
						{
							cct_gotoxy((x + pCGI->CFI.block_width), (y + k - 1));
							cout << pCGI->CFI.v_normal;
						}
						cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//�ָ�Ĭ����ɫ
					}
					else//���ƶ�δ��һ��ʱ
					{
						cct_gotoxy((x + pCGI->CFI.block_width), y);
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						for (int k = 1; k <= pCGI->CFI.block_high; k++)
						{
							cct_gotoxy((x + pCGI->CFI.block_width), (y + k - 1));
							cout << "  ";//һ������
						}
						cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//�ָ�Ĭ����ɫ
					}
					Sleep(pCGI->delay_of_block_moved);//�ƶ���ʱ
				}
				break;

			case LEFT_TO_RIGHT://�зָ��� ����
				distance_1 = distance + distance * (pCGI->CFI.block_width / 2);
				for (int dis = 1; dis <= distance_1; dis++)
				{
					x++;
					x++;
					gmw_inner_block_move(pCGI, x, y, bdi_value, blank_bdi_value, bdi);//���ƶ�һ�񲢴�ӡɫ��
					if (dis % ((pCGI->CFI.block_width + x_add) / 2) == 0)//���ƶ���һ��ʱ
					{
						cct_gotoxy((x - 2), y);
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						for (int k = 1; k <= pCGI->CFI.block_high; k++)
						{
							cct_gotoxy((x - 2), (y + k - 1));
							cout << pCGI->CFI.v_normal;
						}
						cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//�ָ�Ĭ����ɫ
					}
					else//���ƶ�δ��һ��ʱ
					{
						cct_gotoxy((x - 2), y);
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						for (int k = 1; k <= pCGI->CFI.block_high; k++)
						{
							cct_gotoxy((x - 2), (y + k - 1));
							cout << "  ";//һ������
						}
						cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//�ָ�Ĭ����ɫ
					}
					Sleep(pCGI->delay_of_block_moved);//�ƶ���ʱ
				}
				break;

			default:
				return -1;

		}
	}

	else//��ʱΪ�޷ָ��ߵ����
	{
		switch (direction)
		{
			case DOWN_TO_UP://�޷ָ�������
				distance_1 = pCGI->CFI.block_high * distance;
				for (int dis = 1; dis <= distance_1; dis++)
				{
					y--;
					gmw_inner_block_move(pCGI, x, y, bdi_value, blank_bdi_value, bdi);//���ƶ�һ�񲢴�ӡɫ��
					cct_gotoxy(x, (y + pCGI->CFI.block_high));
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					for (int k = 1; k <= (pCGI->CFI.block_width / 2); k++)//��ƶ��Ŀ�ȱ
						cout << "  ";//һ������
					cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//�ָ�Ĭ����ɫ
					Sleep(pCGI->delay_of_block_moved);//�ƶ���ʱ
				}
				break;

			case UP_TO_DOWN://�޷ָ�������
				distance_1 = pCGI->CFI.block_high * distance;
				for (int dis = 1; dis <= distance_1; dis++)
				{
					y++;
					gmw_inner_block_move(pCGI, x, y, bdi_value, blank_bdi_value, bdi);//���ƶ�һ�񲢴�ӡɫ��
					cct_gotoxy(x, (y - 1));
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					for (int k = 1; k <= (pCGI->CFI.block_width / 2); k++)//��ƶ��Ŀ�ȱ
						cout << "  ";//һ������
					cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//�ָ�Ĭ����ɫ
					Sleep(pCGI->delay_of_block_moved);//�ƶ���ʱ
				}
				break;

			case RIGHT_TO_LEFT://�޷ָ�������
				distance_1 = (pCGI->CFI.block_width / 2) * distance;
				for (int dis = 1; dis <= distance_1; dis++)
				{
					/*һ���ƶ�����*/
					x--;
					x--;
					gmw_inner_block_move(pCGI, x, y, bdi_value, blank_bdi_value, bdi);//���ƶ�һ�񲢴�ӡɫ��
					cct_gotoxy((x + pCGI->CFI.block_width), y);
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					for (int k = 1; k <= pCGI->CFI.block_high; k++)
					{
						cct_gotoxy((x + pCGI->CFI.block_width), (y + k - 1));
						cout << "  ";//һ������
					}
					cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//�ָ�Ĭ����ɫ
					Sleep(pCGI->delay_of_block_moved);//�ƶ���ʱ
				}
				break;

			case LEFT_TO_RIGHT://�޷ָ�������
				distance_1 = (pCGI->CFI.block_width / 2) * distance;
				for (int dis = 1; dis <= distance_1; dis++)
				{
					/*һ���ƶ�����*/
					x++;
					x++;
					gmw_inner_block_move(pCGI, x, y, bdi_value, blank_bdi_value, bdi);//���ƶ�һ�񲢴�ӡɫ��
					cct_gotoxy((x - 2), y);
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					for (int k = 1; k <= pCGI->CFI.block_high; k++)
					{
						cct_gotoxy((x - 2), (y + k - 1));
						cout << "  ";//һ������
					}
					cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//�ָ�Ĭ����ɫ
					Sleep(pCGI->delay_of_block_moved);//�ƶ���ʱ
				}
				break;

			default:
				return -1;
		}

	}

	return 0;
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ��������꣬�ж�����������λ��
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   int const x_0                            �������λ�õ�x����
		   int const y_0                            �������λ�õ�y����
		   char &row                                ���õ�������λ������
		   int &col                                 ���õ�������λ������
  �� �� ֵ��
  ˵    �������row/col��Ϊ-1�����ʱ���ڷǷ�λ��
***************************************************************************/
static int gmw_inner_judge_hl(const CONSOLE_GRAPHICS_INFO* const pCGI, const int x_0, const int y_0, int& row, int& col)
{
	/*ע���ָ���Ĭ��Ϊ���ֽ������ַ�*/

	/*��ʼ��ͷλ������*/
	int x = pCGI->start_x + 2;
	int y = pCGI->start_y + 1;

	int block_row = -1;//��Ӧ���к�
	int block_col = -1;//��Ӧ���б�

	/*�б�*/
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

	/*�к�*/
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

	/*�������õĸ�ֵ*/
	row = block_row;
	col = block_col;

	return 0;

}


/***************************************************************************
  �������ƣ�
  ��    �ܣ������̻����
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��

		   int &MAction							    ��������� CCT_MOUSE_EVENT�����ֵ��Ч��Ϊ MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK ����֮һ
													  ������� CCT_KEYBOARD_EVENT�����ֵ��Ч

		   int &MRow								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��

		   int &MCol								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��

		   int &KeyCode1							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ��һ����
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��

		   int &KeyCode2							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ�ڶ���������ǵ����룬��Ϊ0��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��

		   const bool update_lower_status_line		������ƶ�ʱ���Ƿ�Ҫ�ڱ���������ʾ"[��ǰ���] *��*��/λ�÷Ƿ�"����Ϣ��true=��ʾ��false=����ʾ��ȱʡΪtrue��

  �� �� ֵ����������Լ��
		   1�����������ƶ����õ���MRow/MCol�봫�����ͬ(���ָ��΢С���ƶ�)���򲻷��أ�������
							  �õ����зǷ�λ�ã��򲻷��أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] λ�÷Ƿ�"
							  �õ���MRow/MCol�봫��Ĳ�ͬ(��������һ���仯)������ update_lower_status_line ����������״̬����ʾ"[��ǰ���] *��*��"���ٷ���MOUSE_ONLY_MOVED����Щ��Ϸ���غ�Ҫ����ɫ��Ĳ�ͬ��ɫ��ʾ��

		   2������ǰ������������ҵ�ǰ���ָ��ͣ��������Ϸ�����*��*���ϣ��򷵻� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_LEFT_BUTTON_CLICK, MRow Ϊ�кţ�MCol Ϊ�б�
								  �ҵ�ǰ���ָ��ͣ���ڷǷ����򣨷���Ϸ������Ϸ�����еķָ��ߣ����򲻷��أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] λ�÷Ƿ�"

		   3������ǰ�������Ҽ������ж����ָ��ͣ�������Ƿ�Ϸ���ֱ�ӷ��� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_RIGHT_BUTTON_CLICK, MRow��MColȡ��ǰֵ

		   4��������¼����ϵ�ĳ������˫���밴��������ֱ�ӷ��� CCT_KEYBOARD_EVENT��KeyCode1/KeyCode2��Ϊ��Ӧ�ļ���ֵ

 ˵    ����ͨ������ cmd_console_tools.cpp �е� read_keyboard_and_mouse ����ʵ��
***************************************************************************/
int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO* const pCGI, int& MAction, int& MRow, int& MCol, int& KeyCode1, int& KeyCode2, const bool update_lower_status_line)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*���ֻ�������*/
	cct_enable_mouse();//����ʹ�����
	cct_setcursor(CURSOR_INVISIBLE);//ȡ��ʹ�ù��

	/*Ҫ��ȡ��λ��*/
	int col = -1;
	int row = -1;
	/*�����λ��*/
	int old_row = MRow;
	int old_col = MCol;
	/*����[��ǰ���]�Ķ����δ�ӡ������*/
	int sign = 0;
	int sign_site = 0;

	while (1)
	{
		int x, y;
		int back = cct_read_keyboard_and_mouse(x, y, MAction, KeyCode1, KeyCode2);//��ȡ���ͼ����¼�
		switch (back)
		{
			case CCT_MOUSE_EVENT:
				gmw_inner_judge_hl(pCGI, x, y, row, col);//��������ó���Ӧ��λ�ñ��

				if (MAction == MOUSE_RIGHT_BUTTON_CLICK)//����ǰ�������Ҽ������ж����ָ��ͣ�������Ƿ�Ϸ���ֱ�ӷ��� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_RIGHT_BUTTON_CLICK, MRow��MColȡ��ǰֵ
				{
					gmw_inner_judge_hl(pCGI, x, y, MRow, MCol);//ֱ��ȡ��ǰֵ
					return CCT_MOUSE_EVENT;
				}

				else//��ʱΪ����������������ƶ�
				{
					if (row == -1 || col == -1)//��ʱ���ڷǷ�λ�ô�
					{
						cct_setcolor(pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor);//����Ϊ��״̬������ɫ
						cct_gotoxy(pCGI->SLI.lower_start_x+sign_site, pCGI->SLI.lower_start_y);//ǰ����״̬����
						for (int i = 1; i <= pCGI->SLI.width; i++)//�����λ�õĴ�ӡ�ַ�
							cout << ' ';
						cct_gotoxy(pCGI->SLI.lower_start_x + sign_site, pCGI->SLI.lower_start_y);//ǰ����״̬����
						if (sign == 0)
						{
							cout << "[��ǰ���]" << " ";
							sign = 1;
							sign_site = 11;
						}
						cout << "λ�÷Ƿ�";
						cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//�ָ�ԭ������ɫ

						break;//������ȡ

					}

					else if ((old_row == row) && (old_col == col) && (MAction == MOUSE_ONLY_MOVED))
						break;//����ƶ������ݱ��δ�ı䡢��ȡλ����ȷ---���¶�ȡ

					else//��ʱ���ں���λ�ô��Һ��ݱ�Ÿı�
					{
						cct_setcolor(pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor);//����Ϊ��״̬������ɫ
						cct_gotoxy(pCGI->SLI.lower_start_x+sign_site, pCGI->SLI.lower_start_y);//ǰ����״̬����
						for (int i = 1; i <= pCGI->SLI.width; i++)//�����λ�õĴ�ӡ�ַ�
							cout << ' ';
						cct_gotoxy(pCGI->SLI.lower_start_x+sign_site, pCGI->SLI.lower_start_y);//ǰ����״̬����

						if (sign == 0)//��֤����ӡһ�ι���־
						{
							cout << "[��ǰ���]" << " ";
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
						cout << "��";
						if (col >= 100)
							cout << '*' << "��";
						else
							cout << col << "��";
						cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);//�ָ�ԭ������ɫ
						/*���и�ֵ*/
						MRow = row;
						MCol = col;
						return CCT_MOUSE_EVENT;//��ʱ����������¼�

					}

				}
				break;

			case CCT_KEYBOARD_EVENT://������¼����ϵ�ĳ������˫���밴��������ֱ�ӷ��� CCT_KEYBOARD_EVENT��KeyCode1/KeyCode2��Ϊ��Ӧ�ļ���ֵ
				return CCT_KEYBOARD_EVENT;

			default:
				break;
		}

	}

	return 0;
}
