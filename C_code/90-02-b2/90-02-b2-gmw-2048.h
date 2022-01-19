/* 2052532 ���ܷ� ��04 */
#pragma once

/*���������׵����������*/
#define       random(x)                       (rand()%x)//�궨��һ��ȡ������ĺ�����д�ĺ���

/*��ֵ����*/
#define ROW_MAX                           8
#define ROW_MIN                           4
#define COL_MIN                           4
#define COL_MAX                           10
/*����ÿ��ɫ��Ĵ�С*/
#define BLOCK_HIGH                        5        
#define BLOCK_WIDTH                       12
/*�ƶ�����*/
#define BLANK                             0
#define DIRECTION                         4
/*�ӳ����õĲ���*/
#define DELAY_0                           0
#define DELAY_1                           2
#define DELAY_2                           5
#define DELAY_3                           10
#define DELAY_4                           20
#define DELAY_5                           50
/*�ƶ�����Ĳ���*/
#define L_R                               1
#define R_L                               2
#define U_D                               3
#define D_U                               4

/*��غ���������*/
int             First                     (void);
int             my2048_mid                (CONSOLE_GRAPHICS_INFO* const  M2048_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi, const int score_goal, const int score_highest, int& score);
int             my2048_end                (CONSOLE_GRAPHICS_INFO* const  M2048_CGI);
int             judge_win                 (CONSOLE_GRAPHICS_INFO* const  M2048_CGI, int inbase[ROW_MAX][COL_MAX]);
int             move_out                  (CONSOLE_GRAPHICS_INFO* const  M2048_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi, int type);
void            level                     (CONSOLE_GRAPHICS_INFO* const  M2048_CGI, int& score);
void            my2048_start              (CONSOLE_GRAPHICS_INFO* const  M2048_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi, const int score_goal, const int score_highest);
void            status_out                (CONSOLE_GRAPHICS_INFO* const  M2048_CGI, int score_now, int score_goal, int score_highest, int start_time);
void            creat                     (CONSOLE_GRAPHICS_INFO* const  M2048_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi);

