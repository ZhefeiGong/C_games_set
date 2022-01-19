/* 2052532 ��04 ���ܷ� */
#include<iostream>
#include <conio.h>

using namespace std;

#include"../include/common_output.h"
#include"../include/cmd_console_tools.h"


/***************************************************************************
  �������ƣ�shuzu_out
  ��    �ܣ��������´�ӡ����
  ���������int width, int length, int line_max, int between, int start, char* p
  �� �� ֵ��void
  ˵    ������ش�ӡ����
           1.�ף�              *  ֱ�Ӵ�ӡ��
                               +  ��ʱ�����أ���X��ӡ��λ����������
           2.���֣�       ������ӡ     ���Դ�ӡ 
                             0            a
                             1            b
                             2            c
                             3            d
                             4            e
                             5            f
                             6            g
                             7            h
                             8            i
                             9            j
		   3.�����ַ���    $   ��ʾ��λ�����ף�����ǣ���ӡ���X
						   %   ��ʾ��λ�����ף�����ǣ���ӡ���X
***************************************************************************/
void shuzu_out(int width, int length, int line_max, int between, int start, char* p)//ע��*pΪ��ά�������Ԫ�ص�ַ
{
    int i, j, k;
	cout << "  |";//��ӡǰ���Ŀո�
	for (i = start; i <= (length - 1 + start); i++)
	{
		for (k = 1; k <= between; k++)
            cout << ' ';//��ӡ�ַ���Ŀո�
        if (i > 9)
            cout << (char)(i - 10 + 'a');//��ʱ����9����ӡ��ĸ
        else
            cout << i;//��ʱ��ӡ����
    }
	cout << "\n--+";
	for (i = start; i <= length; i++)
	{
		for (k = 1; k <= between; k++)
			cout << '-';
        cout << '-';
	}   
    cout << "-\n";//����
    /*��ӡ������ڲ��ṹ*/
    for (i = 0; i <= (width - 1); i++)
    {
        cout << (char)(65 + i) << " |";//�����ǰ�����ͷλ��
        for (j = 0; j <= (length - 1); j++)
        {
            for (k = 1; k <= between; k++)//��ӡ��ͬ�����Ŀո�
                cout << ' ';
            if (*p >= '0' && *p <= '9')//������ӡ��ص�����
                cout << *p;
			else if (*p >= 'a' && *p <= 'j')//��ʱ���ַ���
			{
				cct_setcolor(14, (int)(*p - 'a'));//ʹ�����ֵ���ɫ��ͬ
				cout << (char)(*p - ('a' - '0'));
				cct_setcolor(0, 7);//ɫ�ʻָ�ԭ״
			}
			else if (*p == '$' || *p == '%')
			{
				cct_setcolor(4, 7);//����ǵı���ɫ����Ϊ��ɫ
				cout << 'X';
				cct_setcolor(0, 7);//��ɫ�ָ�ԭ״
			}
			else if (*p == 'X' || *p == '+')
				cout << 'X';
			else if (*p == '*')
				cout << '*';
			else
				cout << "error!!!";
            p++;
		}
        if (length < line_max)
            p += (line_max - length);
		cout << endl;
	}
    return;
}

/***************************************************************************
  �������ƣ�menu_out
  ��    �ܣ���ӡ�����е��µĲ˵����棬���������뷵��ִ����
  ���������
  �� �� ֵ������ֵΪһ���ַ�����Χ��A-Z.��A-Z��ʾ���������Ӧ��ֵ
  ˵    ������֧�������־Ϊ��ĸ�����
***************************************************************************/
char menu_out( struct menu* p,int max)
{
	int i, longest = strlen(p->name);//���ȼٶ���һ��Ϊ����ַ���
    struct menu* start = p;
	for (; start <= (p + max-1); start++)//�������ַ��������ڴ�ӡ�պø����ĵ������º���
		if (longest < (int)strlen(start->name))
			longest = strlen(start->name);
	cout << "--";//��ӡ�ϲ��ֵĺ���
	for (i = 1; i <= longest; i++)
		cout << "-";
	cout << "--\n";
    for (start = p; start <= (p + max-1); start++)//��ӡ��Ӧ�ַ���
        cout << start->sign << '.' << start->name << endl;
    cout << "--";//��ӡ�²��ֿո�
    for (i = 1; i <= longest; i++)
        cout << "-";
    cout << "--\n";
    cout << "[��ѡ��:] ";
    while (1)
	{
		char shuru = _getch();
		if( (shuru >= 'a' && shuru <= 'z')|| (shuru >= 'A' && shuru <= 'Z'))//�˴���Ϊ����������Ĵ�Сд������
		{
			char shuru_;
			if (shuru >= 'a' && shuru <= 'z')
				shuru_ = shuru - 32;//��ֵһ����д
			else
				shuru_ = shuru + 32;//��ֵһ��Сд
			for (start = p; start <= (p + max - 1); start++)
				if (shuru == start->sign || shuru_ == start->sign)//��ʱ��ʾ��ĸ��Сд����
				{
					cout << shuru;
					return (char)('A' + (start - p));
				}

		}
        else
			for (start = p; start <= (p + max-1); start++)
			if (shuru == start->sign)
			{
				cout << shuru;
				return (char)('A' + (start - p));
			}
    }
	return 'a';//��ʼ������������ִ������,����������a��ʾ�������
}

/***************************************************************************
  �������ƣ�site_spread
  ��    �ܣ���ĳһλ�õ����ݿ�ʼ����Ѱ����ͬ������,������ɢ��λ�ô�����
  ���������char* p, int length, int width, int hang, int lie,int hang_max,int direction,int neighbor
  �� �� ֵ��-1��ʾδ������ɢ������λ��Ϊ������ɢ�㣻0��ʾδִ����ɢ������λ��Ϊ����ɢ�㣻1��ʾ��������ɢ
  ˵    ����
            ��ز���˵����
            char*  p                ��       ��ɢ�������Ԫ�ص�ַ
			int    length �� width  ��       �Ǳ���ϵĳ������ֵint 
			int    hang   �� lie    ��       ������������λ�õ�����������   
			int    length_max       ��       ���������һ��Ԫ�ص���Ŀ���ֵ
			int    direction        ��       ȷ���ɼ�������ʼ������ɢ���� �� �� �� ���� ���� ���� ������
			int    neighbor         ��       ���׸�Ԫ����Χ����ͬԪ��ʱ���Ƿ���Ҫ���ԣ�0�������ԣ�1�����ԣ�
          ע�����Ե�λ��Ҳ����ʵ����ɢ������
		      * + $ %  Ϊ��ʼλ��ʱ �޷�������ɢ
		  ע������ĳ��λ��Ϊ����λ��ʱ���Ƚ���λ�ñ�Ϊ�Ƿ��ԣ��ٿ�ʼ�ԷǷ��Ե���̬��ɢ���˷���->�Ƿ��Եı仯ʱ���õģ�������
***************************************************************************/
int site_spread(char* p, int length, int width, int hang, int lie,int length_max_0,int direction,int neighbor)
{
	
	/*����˸����򣺶� �� �� �� ���� ���� ���� ���� */
	int hang_move[8] = { 1,0,-1, 0,1,-1,-1, 1 };
	int lie_move[8] = { 0,1, 0,-1,1, 1,-1,-1 };
	int i;
	char* site = p + lie * length_max_0 + hang;//����ָ�붨λ�㿪��λ��
	if (*site == '*' || *site == '$' || *site == '%' || *site == '+' )//��ʱ��ʾ�㿪λ���޷���������Χ����������
		return -1;
	/*��a-jͳһΪ�ַ�0-9*/
	char start;
	if (*site >= 'a' && *site <= 'j')
		start = (char)(*site - ('a' - '0'));
	else
		start = *site;//��ʱstartֻ����0-9�����
	int number = 0;
	/*��ʼ����*/
	for (i = 0; i <= (direction - 1); i++)
	{
		int next_hang = hang + hang_move[i];//����λ�������x
		int next_lie = lie + lie_move[i];//����λ�������y
		if (next_hang >= 0 && next_hang <= (length - 1) && next_lie >= 0 && next_lie <= (width - 1))//ֻ�г��Ϳ��ڸ÷�Χ�ڲſɼ���
		{
			char* next_site = p + next_lie * length_max_0 + next_hang;//��ʱ��λ��
			if (*next_site == start)
			{
				number++;
				*next_site += 'a' - '0';//����λ�÷���
				site_spread(p, length, width, next_hang, next_lie, length_max_0, direction, 1);//��ʱ����ݹ飬��˵���Χ����һ���ַ�ʱ����λ�����跴��
			}
		}
	}
	if (number == 0)//��ʱ��Χ����һ�������֣�δ����ݹ�
	{
		if (neighbor == 0)//�ɲ����������Ƿ��ڵ㿪λ�÷���
			;
		else
			if (*site >= '0' && *site <= '9')//ȷ���Ƿ���Ҫ����
				*site += 'a' - '0';//����
		return 0;
	}
	else
		if (*site >= '0' && *site <= '9')//ȷ���Ƿ���Ҫ����
			*site += 'a' - '0';//����
	return 1;
}

/***************************************************************************
  �������ƣ�judge_game_win
  ��    �ܣ��ж�������Χ���ĸ�����/�˸������Ƿ�����ͬ�����ݣ��У���δ�ɹ����ޣ��Ѿ��ɹ�
  ���������char* p, int length, int width,int direction
  �� �� ֵ��0 ��ʾ��δ�ɹ���1 ��ʾ�Ѿ��ɹ�
  ˵    ����ע��1.��һ��λ����Χ����ͬ����ʱ��Ϊ�ɹ�
                2.����������0-9���������ݲ�����
            �������ݣ�0  1  2  3  4  5  6  7  8  9 
			          a  b  c  d  e  f  g  h  i  j
					  *  +  %  $	    
***************************************************************************/
int judge_game_win(char* p, int length, int width,int length_max_0,int direction)
{
	/*����˸����򣺶� �� �� �� ���� ���� ���� ���� */
	int hang_move[8] = { 1,0,-1, 0,1,-1,-1, 1 };
	int lie_move[8] = { 0,1, 0,-1,1, 1,-1,-1 };
	/*�������������ڵ�����(����0-9)*/
	int i, j, k;
	char* site = p;
	for (j = 0; j <= width - 1; j++)
	{
		for (i = 0; i <= length - 1; i++)
		{
			site = p + length_max_0 * j + i;
			if (*site >= '0' && *site <= '9')//����Ϊ����0-9ʱ�ſ�ʼ����
				for (k = 0; k <= direction - 1; k++)
				{
					int i_next = i + hang_move[k];
					int j_next = j + lie_move[k];
					if (i_next >= 0 && i_next <= length - 1 && j_next >= 0 && j_next <= width - 1)
					{
						char* site_next = p + length_max_0 * j_next + i_next;
						if (*site_next == *site)
							return 0;
					}
				}
		}
	}
	return 1;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������char* hint, char* input, bool distinguish(Ĭ����ĸ��Сд������)
  �� �� ֵ��
  ˵    ��������ĳһ�ַ���ʾ�����öγ���
***************************************************************************/
void end(const char* hint,  char* input, const bool distinguish)
{
	cin.clear();//���û�����
	//cin.ignore(1024, '\n');//���������
	char ERROR_HINT[128] = "�����������������";
	cout << hint;
	int x, y;
	cct_getxy(x, y);
	while (1) {
		cct_gotoxy(x, y);
		char in[1024] = { '\0' };
		cin >> in;
		if (distinguish) {//������ĸ��Сд
			if (strcmp(input, in) == 0)
				return;
		}
		else {//�����ִ�Сд
			/*�Ƚ������е���ĸȫ��ת��Ϊ��д��ĸ*/
			for (int i = 0; input[i] != '\0'; i++) {
				if (input[i] >= 'a' && input[i] <= 'z')
					input[i] -= 'a' - 'A';
			}
			for (int i = 0; in[i] != '\0'; i++) {
				if (in[i] >= 'a' && in[i] <= 'z')
					in[i] -= 'a' - 'A';
			}
			if (strcmp(input, in) == 0)
				return;
		}
		cout << ERROR_HINT << endl;
		cct_gotoxy(x, y);
		cout << "                       ";//�������
		cin.clear();//���û�����
		cin.ignore(1024, '\n');//���������
	}//end of while

}





