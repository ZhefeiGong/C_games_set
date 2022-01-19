/* 2052532 信04 龚哲飞 */
#include<iostream>
#include <conio.h>

using namespace std;

#include"../include/common_output.h"
#include"../include/cmd_console_tools.h"


/***************************************************************************
  函数名称：shuzu_out
  功    能：命令行下打印数组
  输入参数：int width, int length, int line_max, int between, int start, char* p
  返 回 值：void
  说    明：相关打印规则：
           1.雷：              *  直接打印雷
                               +  此时雷隐藏，用X打印该位置以隐藏雷
           2.数字：       正常打印     反显打印 
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
		   3.特殊字符：    $   表示此位置无雷，被标记，打印红底X
						   %   表示此位置有雷，被标记，打印红底X
***************************************************************************/
void shuzu_out(int width, int length, int line_max, int between, int start, char* p)//注意*p为二维数组的首元素地址
{
    int i, j, k;
	cout << "  |";//打印前导的空格
	for (i = start; i <= (length - 1 + start); i++)
	{
		for (k = 1; k <= between; k++)
            cout << ' ';//打印字符间的空格
        if (i > 9)
            cout << (char)(i - 10 + 'a');//此时超过9，打印字母
        else
            cout << i;//此时打印数字
    }
	cout << "\n--+";
	for (i = start; i <= length; i++)
	{
		for (k = 1; k <= between; k++)
			cout << '-';
        cout << '-';
	}   
    cout << "-\n";//换行
    /*打印数组的内部结构*/
    for (i = 0; i <= (width - 1); i++)
    {
        cout << (char)(65 + i) << " |";//输出了前面的排头位置
        for (j = 0; j <= (length - 1); j++)
        {
            for (k = 1; k <= between; k++)//打印不同命令间的空格
                cout << ' ';
            if (*p >= '0' && *p <= '9')//正常打印相关的数字
                cout << *p;
			else if (*p >= 'a' && *p <= 'j')//此时数字反显
			{
				cct_setcolor(14, (int)(*p - 'a'));//使得数字的颜色不同
				cout << (char)(*p - ('a' - '0'));
				cct_setcolor(0, 7);//色彩恢复原状
			}
			else if (*p == '$' || *p == '%')
			{
				cct_setcolor(4, 7);//将标记的背景色设置为红色
				cout << 'X';
				cct_setcolor(0, 7);//颜色恢复原状
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
  函数名称：menu_out
  功    能：打印命令行底下的菜单界面，并根据输入返回执行项
  输入参数：
  返 回 值：返回值为一个字符，范围在A-Z.用A-Z表示各项命令对应的值
  说    明：仅支持输入标志为字母的情况
***************************************************************************/
char menu_out( struct menu* p,int max)
{
	int i, longest = strlen(p->name);//首先假定第一个为最长的字符串
    struct menu* start = p;
	for (; start <= (p + max-1); start++)//求出最长的字符串，便于打印刚好覆盖文档的上下横线
		if (longest < (int)strlen(start->name))
			longest = strlen(start->name);
	cout << "--";//打印上部分的横线
	for (i = 1; i <= longest; i++)
		cout << "-";
	cout << "--\n";
    for (start = p; start <= (p + max-1); start++)//打印相应字符串
        cout << start->sign << '.' << start->name << endl;
    cout << "--";//打印下部分空格
    for (i = 1; i <= longest; i++)
        cout << "-";
    cout << "--\n";
    cout << "[请选择:] ";
    while (1)
	{
		char shuru = _getch();
		if( (shuru >= 'a' && shuru <= 'z')|| (shuru >= 'A' && shuru <= 'Z'))//此处是为了体现输入的大小写不敏感
		{
			char shuru_;
			if (shuru >= 'a' && shuru <= 'z')
				shuru_ = shuru - 32;//赋值一个大写
			else
				shuru_ = shuru + 32;//赋值一个小写
			for (start = p; start <= (p + max - 1); start++)
				if (shuru == start->sign || shuru_ == start->sign)//此时表示字母大小写均可
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
	return 'a';//起始本函数不可能执行至此,因此如果返回a表示程序出错
}

/***************************************************************************
  函数名称：site_spread
  功    能：由某一位置的数据开始向外寻找相同的数据,并将扩散的位置处反显
  输入参数：char* p, int length, int width, int hang, int lie,int hang_max,int direction,int neighbor
  返 回 值：-1表示未进行扩散，输入位置为不可扩散点；0表示未执行扩散，输入位置为可扩散点；1表示进行了扩散
  说    明：
            相关参数说明：
            char*  p                ：       扩散数组的首元素地址
			int    length 和 width  ：       是表观上的长宽最大值int 
			int    hang   和 lie    ：       是数组中所处位置的行数和列数   
			int    length_max       ：       所开数组的一行元素的数目最大值
			int    direction        ：       确定由几个方向开始向外扩散（东 南 西 北 东南 西南 西北 东北）
			int    neighbor         ：       当首个元素周围无相同元素时，是否需要反显（0：不反显；1：反显）
          注：反显的位置也可是实现扩散操作（
		      * + $ %  为初始位置时 无法进行扩散
		  注：当从某个位置为反显位置时，先将该位置变为非反显，再开始以非反显的形态扩散（此反显->非反显的变化时永久的）！！！
***************************************************************************/
int site_spread(char* p, int length, int width, int hang, int lie,int length_max_0,int direction,int neighbor)
{
	
	/*定义八个方向：东 南 西 北 东南 西南 西北 东北 */
	int hang_move[8] = { 1,0,-1, 0,1,-1,-1, 1 };
	int lie_move[8] = { 0,1, 0,-1,1, 1,-1,-1 };
	int i;
	char* site = p + lie * length_max_0 + hang;//根据指针定位点开的位置
	if (*site == '*' || *site == '$' || *site == '%' || *site == '+' )//此时表示点开位置无法进行向周围遍历的条件
		return -1;
	/*将a-j统一为字符0-9*/
	char start;
	if (*site >= 'a' && *site <= 'j')
		start = (char)(*site - ('a' - '0'));
	else
		start = *site;//此时start只存在0-9的情况
	int number = 0;
	/*开始遍历*/
	for (i = 0; i <= (direction - 1); i++)
	{
		int next_hang = hang + hang_move[i];//代表位置坐标的x
		int next_lie = lie + lie_move[i];//代表位置坐标的y
		if (next_hang >= 0 && next_hang <= (length - 1) && next_lie >= 0 && next_lie <= (width - 1))//只有长和宽在该范围内才可计算
		{
			char* next_site = p + next_lie * length_max_0 + next_hang;//此时的位置
			if (*next_site == start)
			{
				number++;
				*next_site += 'a' - '0';//将该位置反显
				site_spread(p, length, width, next_hang, next_lie, length_max_0, direction, 1);//此时进入递归，因此当周围均无一样字符时，该位置仍需反显
			}
		}
	}
	if (number == 0)//此时周围均无一样的数字，未进入递归
	{
		if (neighbor == 0)//由参数决定在是否在点开位置反显
			;
		else
			if (*site >= '0' && *site <= '9')//确定是否需要反显
				*site += 'a' - '0';//反显
		return 0;
	}
	else
		if (*site >= '0' && *site <= '9')//确定是否需要反显
			*site += 'a' - '0';//反显
	return 1;
}

/***************************************************************************
  函数名称：judge_game_win
  功    能：判断数字周围（四个方向/八个方向）是否有相同的数据：有：还未成功；无：已经成功
  输入参数：char* p, int length, int width,int direction
  返 回 值：0 表示还未成功；1 表示已经成功
  说    明：注：1.当一个位置周围无相同数据时即为成功
                2.仅遍历数字0-9，其余数据不考虑
            数组内容：0  1  2  3  4  5  6  7  8  9 
			          a  b  c  d  e  f  g  h  i  j
					  *  +  %  $	    
***************************************************************************/
int judge_game_win(char* p, int length, int width,int length_max_0,int direction)
{
	/*定义八个方向：东 南 西 北 东南 西南 西北 东北 */
	int hang_move[8] = { 1,0,-1, 0,1,-1,-1, 1 };
	int lie_move[8] = { 0,1, 0,-1,1, 1,-1,-1 };
	/*遍历整个数组内的数据(数字0-9)*/
	int i, j, k;
	char* site = p;
	for (j = 0; j <= width - 1; j++)
	{
		for (i = 0; i <= length - 1; i++)
		{
			site = p + length_max_0 * j + i;
			if (*site >= '0' && *site <= '9')//仅当为数字0-9时才开始遍历
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
  函数名称：
  功    能：
  输入参数：char* hint, char* input, bool distinguish(默认字母大小写不区分)
  返 回 值：
  说    明：输入某一字符表示结束该段程序
***************************************************************************/
void end(const char* hint,  char* input, const bool distinguish)
{
	cin.clear();//重置缓冲区
	//cin.ignore(1024, '\n');//清除缓冲区
	char ERROR_HINT[128] = "输入错误，请重新输入";
	cout << hint;
	int x, y;
	cct_getxy(x, y);
	while (1) {
		cct_gotoxy(x, y);
		char in[1024] = { '\0' };
		cin >> in;
		if (distinguish) {//区分字母大小写
			if (strcmp(input, in) == 0)
				return;
		}
		else {//不区分大小写
			/*先将内容中的字母全部转换为大写字母*/
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
		cout << "                       ";//清除输入
		cin.clear();//重置缓冲区
		cin.ignore(1024, '\n');//清除缓冲区
	}//end of while

}





