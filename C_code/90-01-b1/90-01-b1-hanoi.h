/* 2052532 ��04 ���ܷ� */
#pragma once

/* -----------------------------------------

     ���ļ����ܣ�
	1��Ϊ�˱�֤ hanoi_main.cpp/hanoi_menu.cpp/hanoi_multiple_solutions.cpp ���໥���ʺ����ĺ�������
	2��һ�����ϵ�cpp���õ��ĺ궨�壨#define����ȫ��ֻ����const����������������
	3�����Բο� cmd_console_tools.h ��д���������Ķ�����ᣩ
   ----------------------------------------- */


/*������Ŀ��Ӧ��ִ�к���*/
void   item_1                                       (void);
void   item_2                                       (void);
void   item_3                                       (void); 
void   item_4                                       (void);
void   item_5                                       (void);
void   item_6                                       (void);
void   item_7                                       (void);
void   item_8                                       (void);
void   item_9                                       (void);


/*��ŵ���ݹ��Լ�����غ���*/
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


/*���뺯���Լ��������ú���*/
void   input                                        (int* floor, char* start, char* mid, char* goal, int* yan_shi, int yby);
void   data_renew                                   (void);


/*���ڲ˵�ҳ�ĺ���*/
int    menu_out_hanoi                                (void);


/*��ӡͼ�κ�ŵ�������ƶ��ĺ���*/
void   tower                                        (void);
void   start_plate                                  (void);
void   plate_move                                   (char src, char dst);
void   plate_move_data                              (int* start_floor, int* start_zhu, int* end_floor, int* end_zhu, int* lump, char start, char goal);
void   plate_move_level                             (int start, int goal, int lump);
void   plate_move_vertical_up                       (int zhu, int floor, int lump);
void   plate_move_vertical_down                     (int zhu, int floor, int lump);
void   sleep                                        (void);


/*��Ŀ9������¼�����*/
int    item_9_in                                    (char* start, char* goal);
void   item_9_pdcs                                  (char start, int* cengshu);
int    item_9_examine                               (int shuru, int cengshu);
void   item_9_out                                   (int zhixing, char start, char goal, int cengshu, char dst, int floor);
int    item_9_judge                                 (char dst, int floor);




