/* 2052532 ��04 ���ܷ� */
#pragma once




/*�Զ�����������*/

struct menu//�˽ṹ��Ϊ��ӡ�˵����棬ÿ����Ŀ��Ӧ�Ľṹ��
{
	char sign;
	char name[1024];
};



/*��д������*/

char               menu_out                         (struct menu* p, int max);
void               shuzu_out                        (int width, int length, int line_max, int between, int start, char* p);
int                site_spread                      (char* p, int length, int width, int hang, int lie, int length_max_0, int direction, int neighbor);
int                judge_game_win                   (char* p, int length, int width, int length_max_0, int direction);
void               end                              (const char* hint, char* input, const bool distinguish = false);