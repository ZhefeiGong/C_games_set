/* 2052532 龚哲飞 信04 */
#pragma once

/*定义生成雷的随机数函数*/
#define random(x)                           (rand()%x)//宏定义一个取随机数的函数己写的函数

/*定义各个难度下的长宽和雷数*/
#define LEVEL_1                             1
#define ROW_1                               9
#define COL_1                               9
#define MINE_1                              10
#define LEVEL_2                             2
#define ROW_2                               16
#define COL_2                               16
#define MINE_2                              40
#define LEVEL_MAX                           3
#define ROW_MAX                             16//定义存放扫雷内部数组的数组的最大规格
#define COL_MAX                             30//定义存放扫雷内部数组的数组的最大规格
#define MINE_MAX                            99


/*定义需要用到的特殊数字*/
#define BLANK                               0
#define MINE_SIGN                           9
#define TAB_MINE_SIGN                       99
#define TAB_NOT_SIGN                        88
#define COVER_SIGN                          999
#define BLOCK_WIDTH                         6
#define BLOCK_HIGH                          3

/*相关函数声明*/
int         level                           (void);
int         First                           (void);
int         judge_win                       (CONSOLE_GRAPHICS_INFO* const MINE_CGI, const int inbase[ROW_MAX][COL_MAX]);
int         mine_sweeper_mid                (CONSOLE_GRAPHICS_INFO* const MINE_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* const bdi, int num);
void        mine_sweeper_end                (CONSOLE_GRAPHICS_INFO* const MINE_CGI);
void        mine_sweeper_start              (CONSOLE_GRAPHICS_INFO* const MINE_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* const bdi, const int row, const int col, const int num);
void        make_inbase                     (CONSOLE_GRAPHICS_INFO* const MINE_CGI, int inbase[ROW_MAX][COL_MAX], const int num, const int st_row, const int st_col);
void        spread                          (CONSOLE_GRAPHICS_INFO* const MINE_CGI, int inbase[ROW_MAX][COL_MAX], int row, int col);
void        mine_count                      (CONSOLE_GRAPHICS_INFO* const MINE_CGI, const int inbase[ROW_MAX][COL_MAX], int mine_num);
void        time_out                        (CONSOLE_GRAPHICS_INFO* const MINE_CGI, const clock_t start_time, const int top_lower, const bool win_los=false);//默认情况为失败
