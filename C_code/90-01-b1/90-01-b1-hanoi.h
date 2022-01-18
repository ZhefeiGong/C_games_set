/* 2052532 信04 龚哲飞 */
#pragma once

/* -----------------------------------------

     本文件功能：
	1、为了保证 hanoi_main.cpp/hanoi_menu.cpp/hanoi_multiple_solutions.cpp 能相互访问函数的函数声明
	2、一个以上的cpp中用到的宏定义（#define）或全局只读（const）变量，个数不限
	3、可以参考 cmd_console_tools.h 的写法（认真阅读并体会）
   ----------------------------------------- */


/*各个项目对应的执行函数*/
void   item_1                                       (void);
void   item_2                                       (void);
void   item_3                                       (void); 
void   item_4                                       (void);
void   item_5                                       (void);
void   item_6                                       (void);
void   item_7                                       (void);
void   item_8                                       (void);
void   item_9                                       (void);


/*汉诺塔递归以及其相关函数*/
void   hanoi                                        (int n, char src, char tmp, char dst, int mode);
void   hanoi_out                                    (int n, char src, char tmp, char dst, int mode);
void   column_out_48                                (int n, char src, char dst, int mode, int start);
void   row_out_348                                  (int n, char src, char dst, int mode, int start);
void   start_row_48                                 (void);
void   row_out_1                                    (int n, char src, char dst);
void   row_out_2                                    (int n, char src, char dst);
void   row_inner                                    (void);
void   column_inner                                 (int mode);
void   stop                                         (void);
void   change                                       (char start, char goal);
void   initial_zhu                                  (int floor, char start);


/*输入函数以及数据重置函数*/
void   input                                        (int* floor, char* start, char* mid, char* goal, int* yan_shi, int yby);
void   data_renew                                   (void);


/*用于菜单页的函数*/
int    menu_out_hanoi                                (void);


/*打印图形汉诺塔及其移动的函数*/
void   tower                                        (void);
void   start_plate                                  (void);
void   plate_move                                   (char src, char dst);
void   plate_move_data                              (int* start_floor, int* start_zhu, int* end_floor, int* end_zhu, int* lump, char start, char goal);
void   plate_move_level                             (int start, int goal, int lump);
void   plate_move_vertical_up                       (int zhu, int floor, int lump);
void   plate_move_vertical_down                     (int zhu, int floor, int lump);
void   sleep                                        (void);


/*项目9的相关下级函数*/
int    item_9_in                                    (char* start, char* goal);
void   item_9_pdcs                                  (char start, int* cengshu);
int    item_9_examine                               (int shuru, int cengshu);
void   item_9_out                                   (int zhixing, char start, char goal, int cengshu, char dst, int floor);
int    item_9_judge                                 (char dst, int floor);




