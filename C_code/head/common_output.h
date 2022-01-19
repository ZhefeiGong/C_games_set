/* 2052532 信04 龚哲飞 */
#pragma once




/*自定义类型区域*/

struct menu//此结构体为打印菜单界面，每个项目对应的结构体
{
	char sign;
	char name[1024];
};



/*自写函数区*/

char               menu_out                         (struct menu* p, int max);
void               shuzu_out                        (int width, int length, int line_max, int between, int start, char* p);
int                site_spread                      (char* p, int length, int width, int hang, int lie, int length_max_0, int direction, int neighbor);
int                judge_game_win                   (char* p, int length, int width, int length_max_0, int direction);
void               end                              (const char* hint, char* input, const bool distinguish = false);