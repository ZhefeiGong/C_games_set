/* 2052532 ���ܷ� ��04 */
#pragma once

/*���������׵����������*/
#define random(x)                           (rand()%x)//�궨��һ��ȡ������ĺ�����д�ĺ���

/*���ֺ궨��*/
#define BLANK                                    0
#define DIRECTION                                4
#define DELAY_FRAME                              3
#define DELAY_MOVE                               5
#define BETWEEN                                  10
#define SLEEP_BLANK                              10

/*�������ǵ�������ֵ*/
#define ROW_MIN                                  8
#define COL_MIN                                  8
#define ROW_MAX                                  10
#define COL_MAX                                  10

/*ÿ��ɫ��Ĵ�С*/
#define BLOCK_HIGH                               3
#define BLOCK_WIDTH                              6

/*��غ���������*/
int                 First                        (void);
int                 judge_win                    (CONSOLE_GRAPHICS_INFO* const  STAR_CGI, const int inbase[ROW_MAX][COL_MAX]);
int                 same_spread                  (CONSOLE_GRAPHICS_INFO* const  STAR_CGI, int inbase[ROW_MAX][COL_MAX], const int row, const int col);
int                 popstar_end                  (CONSOLE_GRAPHICS_INFO* const  STAR_CGI, const int inbase[ROW_MAX][COL_MAX], int& score);
void                popstar_start                (CONSOLE_GRAPHICS_INFO* const  STAR_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi);
void                popstar_mid                  (CONSOLE_GRAPHICS_INFO* const  STAR_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi, int& score);
void                back                         (CONSOLE_GRAPHICS_INFO* const  STAR_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi);
void                move                         (CONSOLE_GRAPHICS_INFO* const  STAR_CGI, int inbase[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* bdi);
void                rowcol                       (CONSOLE_GRAPHICS_INFO* const  STAR_CGI);
void                count                        (CONSOLE_GRAPHICS_INFO* const  STAR_CGI, const int inbase[ROW_MAX][COL_MAX], int& score);
void                same_spread_in               (CONSOLE_GRAPHICS_INFO* const  STAR_CGI, const int inbase[ROW_MAX][COL_MAX], const int row, const int col, int in[ROW_MAX][COL_MAX]);





