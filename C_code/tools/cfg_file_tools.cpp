/*2052532 信04 龚哲飞*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
#include <io.h>
#include <iomanip>
#include "../include/cfg_file_tools.h"
using namespace std;

/* ---------------------------------------------------------------
	 允许加入其它需要static函数（内部工具用）
   ---------------------------------------------------------------- */

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：返回0 表示该行为空行；返回1 表示该位置为组名；返回2 表示该位置为项目名
  说    明：提取一行的关键信息
            截断;及#开始的注释，处理前后空格/tab，
****************************************************************************/
static int deal_line(string& in)
{
	int length = in.size();
	string sample = in;        //in暂时不加改动
	string result = "";        //储存最终结果
	string temp = "";          //储存空格的缓冲区
	bool front = true;
	
	//去除注释行
	int mode = 0;//mode=1 组名；mode=2项目名；0为空行
	for (int num = 0; num < length; num++) {
		if (sample[num] == ';' || sample[num] == '#') {
			sample.erase(num);//此时之后的内容全部作废
			break;
		}

	}

	//忽略前导或后部的不可见字符
	int count = 0;
	for (int i = 0; i < (int)sample.size(); i++) {
		if ((sample[i] >= 0 && sample[i] <= 32) || (sample[i] == 127))
			count++;
		else {
			sample.erase(0, count);
			break;
		}
	}
	for (int i = sample.size() - 1; i >= 0; i--) {
		if ((sample[i] >= 0 && sample[i] <= 32) || (sample[i] == 127))
			sample.erase(i, 1);
		else
			break;
	}

	//判断属性
	length = sample.size();//最大长度改变
	for (int num = 0; num < length; num++)
	{
		if ((sample[num] >= 0 && sample[num] <= 32) || (sample[num] == 127))//忽略不可见字符
			;
		else if (sample[num] == '[') {//进入'['特判
			for (int n = length - 1; n >= 0; n--) {
				if ((sample[n] >= 0 && sample[n] <= 32) || (sample[n] == 127))//忽略不可见字符
					;
				else if (sample[n] == ']') {
					mode = GROUP_LINE;
					break;
				}
				else {
					mode = PROJECT_LINE;
					break;
				}
			}
			break;
		}
		else {
			mode = PROJECT_LINE;
			break;
		}
	}


	/*提取有效数据*/

	//项目行
	if (mode == PROJECT_LINE) {
		string before = sample;
		string after = sample;
		int length = sample.size();
		int site = 0;
		for (int i = length - 1; i >= 0; i--) {
			if (sample[i] == '=') {
				site = i;
				break;
			}
		}
		before.erase(site);
		after.erase(0, site + 1);
		//项目名
		for (int i = before.size() - 1; i >= 0; i--)
			if (!((before[i] >= 0 && before[i] <= 32) || (before[i] == 127))) {//忽略不可见字符
				before.erase(i + 1);
				break;
			}
		//值名
		bool temp= false;
		int count = 0;
		for (int i = 0; i < (int)after.size(); i++) {
			if (((after[i] >= 0 && after[i] <= 32) || (after[i] == 127)) &&temp==false) {//忽略前导的空格或者TAB键等不可见字符
				count++;
			}
			else {
				temp = true;
				if ((after[i] >= 0 && after[i] <= 32) || (after[i] == 127)) {//字符串中不含空格等不可见字符
					after.erase(i);
					break;
				}
			}
		}
		after.erase(0, count);
		result = before + '=' + after;
	}
    //组名行
	else if (mode == GROUP_LINE) {
		int n = length - 1;//忽略后空格和]内的不可见字符
		for (; n >= 0; n--) {
			if (sample[n] != ']' && !((sample[n] >= 0 && sample[n] <= 32) || (sample[n] == 127)))
				break;
		}
		int num = 0;
		for (; num <=n; num++) {
			if (sample[num] != '[' && !((sample[num] >= 0 && sample[num] <= 32) || (sample[num] == 127)))
				break;
		}
		for (int i = num; i <= n; i++)
			temp += sample[i];
		result = '[' + temp + ']';
	}

	in = result;

	return mode;
			
}

/***************************************************************************
  函数名称：seek_exist
  功    能：寻找某个项目是否存在
  输入参数：
  返 回 值：
  说    明：返回值说明：
            group_name=NULL 表示查找简单配置文件
                     SIM_FALSE            -1           //需要简单配置文件 却为普通配置文件
                     SIM_NONE             -2           //简单配置文件不存在
                     SIM_BE               -3           //简单配置文件存在
            group_name!=NULL 表示查找普通配置文件
                     COM_GROUP_NONE        1           //普通配置文件组不存在
                     COM_FILE_NONE         2           //普通配置文件组存在 项目不存在
                     COM_BE                3           //普通配置文件存在      
***************************************************************************/
int CFT::seek_exist(const char* group_name, const char* item_name)
{
	fp.clear();
	fp.seekg(0, ios::beg);        //文件指针移动到开始处

	//简单配置文件
	if (group_name == NULL) {
		string item = item_name;
		while (1) {//每行
			char line[LINE_MAX] = { '\0' };
			fp.getline(line, LINE_MAX, '\n');
			if (fp.eof())
				break;
			string strline = line;
			int state = deal_line(strline);//提取一行的有效数据
			if (state == GROUP_LINE) {//组名
				fp.clear();
				fp.seekg(0, ios::beg);//指针移动到开头位置

				return SIM_FALSE;//需要简单配置文件 却为普通配置文件
			}
			else if (state == PROJECT_LINE) {//项目名
				for (int i = 0; i < (int)strline.size(); i++)
					if (strline[i] == '=') {
						strline.erase(i);
						break;//跳出循环
					}
				if (strline == item) {//存在该项目
					fp.clear();
					fp.seekg(0, ios::beg);//指针移动到开头位置

					return SIM_BE;//简单配置文件存在
				}

			}
		}
		//未找到
		if (fp.eof()) {
			fp.clear();
			fp.seekg(0, ios::beg);//文件指针移动到开头

			return SIM_NONE;//简单配置文件不存在
		}
	}
	//普通配置文件
	else {
		string group = group_name;
		group = '[' + group + ']';
		string  item = item_name;
		//查找是否存在该行
		while (1) {//每行
			char line[LINE_MAX] = { '\0' };
			fp.getline(line, LINE_MAX, '\n');
			if (fp.eof()) {
				fp.clear();
				fp.seekg(0, ios::beg);

				return COM_GROUP_NONE;//组都不存在
			}
			int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
			if (line[line_length] == '\r')
				line[line_length] = '\0';
			string strline = line;
			int state = deal_line(strline);//提取一行的有效数据
			if (state == GROUP_LINE) {
				if (strline == group) {
					//进入组二轮搜寻
					while (1) {
						char line_s[LINE_MAX] = { '\0' };
						fp.getline(line_s, LINE_MAX, '\n');
						if (fp.eof()) {
							fp.clear();
							fp.seekg(0, ios::beg);//指针移动到开头

							return COM_FILE_NONE;//组存在 项目不存在
						}
						int line_length_s = strlen(line_s) - 1;//消除windows和linux之间的差异\r
						if (line_s[line_length_s] == '\r')
							line_s[line_length_s] = '\0';
						string strline_s = line_s;
						int state_s = deal_line(strline_s);//提取一行的有效数据
						if (state_s == GROUP_LINE) {
							fp.clear();
							fp.seekg(0, ios::beg);//指针移动到开头

							return COM_FILE_NONE;//组存在 项目不存在
						}
						else if (state_s == PROJECT_LINE) {//判断是否存在
							for (int num = 0; num < (int)strline_s.size(); num++) {
								if (strline_s[num] == '=') {
									strline_s.erase(num);//截断此后内容 提取项目名
									break;//跳出循环
								}
							}
							if (strline_s == item) {
								fp.clear();
								fp.seekg(0, ios::beg);//指针移动到开头

								return COM_BE;//项目存在
							}
						}
						
					}//end of while
				}
			}
		}
	}

	return 0;//此时出错
}

/***************************************************************************
  函数名称：无参构造
  功    能：
  输入参数：
  返 回 值：
  说    明：如有必要，按需加入其它内容
***************************************************************************/
CFT::CFT()
{
	filename = "";
}

/***************************************************************************
  函数名称：虚构函数
  功    能：
  输入参数：
  返 回 值：
  说    明：如有必要，按需加入其它内容
***************************************************************************/
CFT::~CFT()
{
	if (fp.is_open())
		fp.close();
}

/***************************************************************************
  函数名称：
  功    能：二进制方式打开
  输入参数：
  返 回 值：
  说    明：两参构造，功能同open
***************************************************************************/
CFT::CFT(const char* cfgname, int opt)
{
	/*以只读方式打开文件 打不开返失败*/
	if (opt == OPEN_OPT_RDONLY) {
		fp.open(cfgname, ios::in | ios::binary);//以二进制只读方式打开文件
		if (!fp) {//如果打开失败
			filename = "";
			open_mode = OPEN_ERROR;//打开失败
		}
		else {
			filename = cfgname;
			open_mode = OPEN_OPT_RDONLY;//只读方式打开
		}
	}
	/*以读写方式打开文件 打不开则创建*/
	else if (opt == OPEN_OPT_RDWR) {
		fp.open(cfgname, ios::out | ios::in | ios::binary);//以二进制读写方式打开文件
		if (!fp) {//如果打开失败
			fp.open(cfgname, ios::out | ios::binary);//先以写方式打开（相当于创建一个文件）
			if (!fp) {
				filename = "";//表示打开失败
				open_mode = OPEN_ERROR;//打开失败
			}
			else
				fp.close();//关闭文件
			fp.open(cfgname, ios::out | ios::in | ios::binary);//再以读写方式打开
			if (!fp) {
				filename = "";//表示打开失败
				open_mode = OPEN_ERROR;//打开失败
			}
			else {
				filename = cfgname;
				open_mode = OPEN_OPT_RDWR;//读写方式打开
			}
		}
		else {
			filename = cfgname;
			open_mode = OPEN_OPT_RDWR;//读写方式打开
		}
	}
	/*输入不满足要求*/
	else {
		filename = "";
		open_mode = OPEN_ERROR;//打开失败
	}


}

/***************************************************************************
  函数名称：
  功    能：二进制方式打开
  输入参数：
  返 回 值：
  说    明：filename=""表示文件打开失败
***************************************************************************/
void CFT::open(const char* cfgname, int opt)
{
	/*以只读方式打开文件 打不开返失败*/
	if (opt == OPEN_OPT_RDONLY) {
		fp.open(cfgname, ios::in | ios::binary);//以二进制只读方式打开文件
		if (!fp) {//如果打开失败
			filename = "";
			open_mode = OPEN_ERROR;//打开失败
		}
		else {
			filename = cfgname;
			open_mode = OPEN_OPT_RDONLY;//只读方式打开
		}
	}
	/*以读写方式打开文件 打不开则创建*/
	else if (opt == OPEN_OPT_RDWR) {
		fp.open(cfgname, ios::out | ios::in | ios::binary );//以二进制读写方式打开文件
		if (!fp) {//如果打开失败
			fp.open(cfgname, ios::out | ios::binary );//先以写方式打开（相当于创建一个文件）
			if (!fp) {
				filename = "";//表示打开失败
				open_mode = OPEN_ERROR;//打开失败
			}
			else
				fp.close();//关闭文件
			fp.open(cfgname, ios::out | ios::in | ios::binary);//再以读写方式打开
			if (!fp) {
				filename = "";//表示打开失败
				open_mode = OPEN_ERROR;//打开失败
			}
			else {
				filename = cfgname;
				open_mode = OPEN_OPT_RDWR;//读写方式打开
			}
		}
		else {
			filename = cfgname;
			open_mode = OPEN_OPT_RDWR;//读写方式打开
		}
	}
	/*输入不满足要求*/
	else {
		filename = "";
		open_mode = OPEN_ERROR;//打开失败
	}

	return;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：打开成功返回true，否则返回false
***************************************************************************/
bool CFT::is_open(void)
{
	if (filename == "" || open_mode == OPEN_ERROR || open_mode == OPEN_DOWN)
		return false;
	else
		return true;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：关闭文件
***************************************************************************/
void CFT::close()
{
	if (filename != "") {
		fp.close();//关闭文件
		open_mode = OPEN_DOWN;//设置为文件关闭
		/*保留上一次的文件名字*/
	}
	return;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：C++17中有filesystem::resize_file直接可以修改文件大小，之前的版本无
                   因此本函数扔沿用C的旧方法
	    已实现，不要动
***************************************************************************/
int CFT::file_resize(int newsize)
{
	/* 如果大小是负数则直接返回-1 */
	if (newsize < 0)
		return -1;

	if (!fp.good())
		fp.clear();

	/* 取fp当前指针位置 */
	int now_pos = int(fp.tellg());

	/* 首先：C++方式关闭 */
	this->close();

	/* 其次：C方式打开并改变文件大小 */
	FILE* fp_cstyle;
	fp_cstyle = fopen(filename.c_str(), "rb+"); //rw方式打开
	if (fp_cstyle) {
		_chsize(_fileno(fp_cstyle), newsize);
		fclose(fp_cstyle);
	}
	else
		cout << "open [" << filename << "] for resize failed" << endl;

	/* 再次：C++方式重新打开 */
	fp.open(filename, ios::in | ios::out | ios::binary);
	if (fp.is_open()==0) {
		cout << "reopen [" << filename << "] after resize failed" << endl;
		return -1;
	}
	open_mode = OPEN_OPT_RDWR;//更改文件状态值

	/* 文件指针移动到原位置 */
	fp.seekg(now_pos < newsize ? now_pos:newsize, ios::beg);

	return 0;
}

/* ---------------------------------------------------------------
	 允许AAT中自定义成员函数的实现（protected）
   ---------------------------------------------------------------- */


/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：增加组
            getline提取时遇到换行符号停止 并丢弃换行符！！！！！
 ***************************************************************************/
int CFT::group_add(const char* group_name)
{
	/*当以读写方式打开文件时*/
	if (open_mode == OPEN_OPT_RDWR) {
		string standard = group_name ;
		standard = '[' + standard + ']';
		string get = "";
		fp.seekp(0, ios::beg);//将指针移到起始位置
		while (1) {//每行
			char line[LINE_MAX] = { '\0' };
			fp.getline(line, LINE_MAX, '\n');
			if (fp.eof()) {
				fp.clear();
				break;
			}
			int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
			if (line[line_length] == '\r')
				line[line_length] = '\0';
			string strline = line;
			int line_mode = deal_line(strline);
			if (line_mode == GROUP_LINE) {
				if (standard == strline)//存在该组
					return 0;
			}
		}
		fp.seekp(0, ios::end);//指针移动到文件末尾
		fp << standard << "\r\n";//每组隔一行
		fp.seekp(0, ios::beg);//指针移动到开始处
	}	

	return 1; 
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：以"组"为单位删除
***************************************************************************/
int CFT::group_del(const char* group_name)
{
	int cut_num = 0;//删除的组数
	/*当以读写方式打开时*/
	if (open_mode == OPEN_OPT_RDWR) {

		fp.seekp(0, ios::beg);            //文件移动到文件起始位置
		int size_of_file = 0;             //记录需要截断数据之前的文件大小
		string stan = group_name ;        //储存需要判断的组名   
		stan = '[' + stan + ']';

		/*进行第一次遍历*/
		while (1) {
			char line[LINE_MAX] = { '\0' };
			fp.getline(line, LINE_MAX, '\n');
			if (fp.eof())
				break;
			else
				size_of_file = size_of_file + strlen(line) + 1;
			int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
			if (line[line_length] == '\r')
				line[line_length] = '\0';
			string strline = line;
			int line_mode = deal_line(strline);
			if (line_mode == GROUP_LINE) {
				if (stan == strline) {
					cut_num++;
					size_of_file = size_of_file - (line_length + 1 + 1);
					break;//找到删除组
				}
			}
		}

		//遍历至结尾仍未获取删除组
		if (fp.eof()) {//文件结束
			fp.clear();
			fp.seekp(0, ios::beg);//指针移动到开始处
			return cut_num;
		}

		/*提取无需删除项目*/
		string data = "";
		if (cut_num == 1) {
			//删除首组的内容
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				//处理getline情况
				if (fp.eof())
					break;
				string original = line;
				int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
				if (line[line_length] == '\r') 
					line[line_length] = '\0';
				string line_str = line;
				int line_mode = deal_line(line_str);
				if (line_mode==GROUP_LINE) {
					if (line_str != stan) {
						data = data + original + '\n';
						break;//跳出while循环《——
					}
					else
						cut_num++;//如果紧接着为删除组
				}
			}

			if (fp.eof()) {//文件结束
				fp.clear();
				file_resize(size_of_file);//截断文件
				fp.seekp(0, ios::beg);//指针移动到开头
				fp.clear();
				return cut_num;
			}

			//继续进行提取此后内容
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				//处理getline情况
				if (fp.eof())
					break;
				string original = line;
				int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string line_str = line;
				int line_mode = deal_line(line_str);
				if (line_mode==GROUP_LINE) {
					if (line_str != stan)
						data = data + original + '\n';
					else {//又为删除组
						cut_num++;
						while (1) {//继续删除匹配组
							char line_next[LINE_MAX] = { '\0' };
							fp.getline(line_next, LINE_MAX, '\n');
							//处理getline情况
							if (fp.eof())
								break;
							string original_next = line_next;
							int line_length_next = strlen(line_next) - 1;//消除windows和linux之间的差异\r
							if (line_next[line_length_next] == '\r')
								line_next[line_length_next] = '\0';
							string line_str_next = line_next;
							int line_mode_next = deal_line(line_str_next);
							if (line_mode_next==GROUP_LINE) {
								if (line_str_next != stan) {
									data = data + original_next + '\n';
									break;//跳出small while循环《——
								}
								else
									cut_num++;
							}
						}//end of small while
					}
				}
				else//不为文件名
					data = data + original + '\n';
			}//end of big while

			if (fp.eof()) {//文件结束
				fp.clear();
				file_resize(size_of_file);//截断文件
				fp.seekp(0, ios::end);//指针移动到结尾处
				fp << data;
				fp.seekp(0, ios::beg);//指针移动到开始处
				return cut_num;
			}
		}

	}
	return cut_num; //此句根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：添加int项
            注意：文件getline时遇到换行符停止 并丢弃换行符
***************************************************************************/
int CFT::item_add(const char* group_name, const char* item_name, const int item_value)
{
	/*以读写方式打开*/
	if (open_mode == OPEN_OPT_RDWR) {
		//简单配置文件
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			fp.clear();
			if (get == SIM_FALSE || get == SIM_BE) {//文件存在或为普通配置文件
				fp.seekp(0, ios::beg);
				return 0;
			}

			fp.seekp(0, ios::end);//指针移动到文末
			fp << item_name << '=' << item_value << "\r\n";

			fp.clear();
			fp.seekp(0, ios::beg);//文件指针移动到开头
			return 1;
		}
		//普通配置文件
		else {
			int size_of_file = 0;
			string group = group_name;
			group = '[' + group + ']';
			string add = "";
			string data = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_BE || get == COM_GROUP_NONE)//文件存在或组不存在
				return 0;//未添加
            //开始添加
			fp.seekp(0, ios::beg);//指针回到起始位置
			while (1) {//每行
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof()) 
					break;
				else
					size_of_file += strlen(line) + 1;
				int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
				if (line[line_length] == '\r') 
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);//提取一行的有效数据
				if (state == GROUP_LINE) {
					if (strline == group) {
						ostringstream out;
						out << item_name << '=' << item_value << "\r\n";
						add = out.str();
						break;//跳出while循环《——
					}
				}
			}//end of while

			//此后信息无需更改
			if (!fp.eof())//未到结尾处
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;//文件结尾
					string strline = line;
					data += strline + '\n';
				}

			//进行更改
			fp.clear();
			fp.seekp(0, ios::beg);
			file_resize(size_of_file);
			fp.seekp(0, ios::end);//指针移动到结束位置
			fp << add;
			fp << data;
			fp.seekp(0, ios::beg);//指针移动到起始位置

			return 1;
		}
	}

	return -1; //此时出错
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：添加double项
***************************************************************************/
int CFT::item_add(const char* group_name, const char* item_name, const double item_value)
{
	/*以读写方式打开*/
	if (open_mode == OPEN_OPT_RDWR) {
		//简单配置文件
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			fp.clear();
			if (get == SIM_FALSE || get == SIM_BE) {//文件存在或为普通配置文件
				fp.seekp(0, ios::beg);
				return 0;
			}

			fp.seekp(0, ios::end);//指针移动到文末
			fp << item_name << '=' << setiosflags(ios::fixed) << setprecision(6) << item_value << "\r\n";//以六位小数打印

			fp.clear();
			fp.seekp(0, ios::beg);//文件指针移动到开头
			return 1;
		}
		//普通配置文件
		else {
			int size_of_file = 0;
			string group = group_name;
			group = '[' + group + ']';
			string add = "";
			string data = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_BE || get == COM_GROUP_NONE)//文件存在或组不存在
				return 0;//未添加
			//开始添加
			fp.seekp(0, ios::beg);//指针回到起始位置
			while (1) {//每行
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				else
					size_of_file += strlen(line) + 1;
				int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);//提取一行的有效数据
				if (state == GROUP_LINE) {
					if (strline == group) {
						ostringstream out;
						out << item_name << '=' << setiosflags(ios::fixed) << setprecision(6) << item_value << "\r\n";//以六位小数打印
						add = out.str();
						break;//跳出while循环《——
					}
				}
			}//end of while

			//此后信息无需更改
			if (!fp.eof())//未到结尾处
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;//文件结尾
					string strline = line;
					data += strline + '\n';
				}

			//进行更改
			fp.clear();
			fp.seekp(0, ios::beg);
			file_resize(size_of_file);
			fp.seekp(0, ios::end);//指针移动到结束位置
			fp << add;
			fp << data;
			fp.seekp(0, ios::beg);//指针移动到起始位置

			return 1;
		}
	}

	return -1; //此时出错
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：添加char数组项
***************************************************************************/
int CFT::item_add(const char* group_name, const char* item_name, const char *item_value)
{
	/*以读写方式打开*/
	if (open_mode == OPEN_OPT_RDWR) {
		//简单配置文件
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			fp.clear();
			if (get == SIM_FALSE || get == SIM_BE) {//文件存在或为普通配置文件
				fp.seekp(0, ios::beg);
				return 0;
			}

			fp.seekp(0, ios::end);//指针移动到文末
			fp << item_name << '=' << item_value << "\r\n";

			fp.clear();
			fp.seekp(0, ios::beg);//文件指针移动到开头
			return 1;
		}
		//普通配置文件
		else {
			int size_of_file = 0;
			string group = group_name;
			group = '[' + group + ']';
			string add = "";
			string data = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_BE || get == COM_GROUP_NONE)//文件存在或组不存在
				return 0;//未添加
			//开始添加
			fp.seekp(0, ios::beg);//指针回到起始位置
			while (1) {//每行
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				else
					size_of_file += strlen(line) + 1;
				int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);//提取一行的有效数据
				if (state == GROUP_LINE) {
					if (strline == group) {
						ostringstream out;
						out << item_name << '=' << item_value << "\r\n";
						add = out.str();
						break;//跳出while循环《——
					}
				}
			}//end of while

			//此后信息无需更改
			if (!fp.eof())//未到结尾处
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;//文件结尾
					string strline = line;
					data += strline + '\n';
				}

			//进行更改
			fp.clear();
			fp.seekp(0, ios::beg);
			file_resize(size_of_file);
			fp.seekp(0, ios::end);//指针移动到结束位置
			fp << add;
			fp << data;
			fp.seekp(0, ios::beg);//指针移动到起始位置

			return 1;
		}
	}

	return -1; //此时出错
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：添加string项
***************************************************************************/
int CFT::item_add(const char* group_name, const char* item_name, const string &item_value)
{
	/*以读写方式打开*/
	if (open_mode == OPEN_OPT_RDWR) {
		//简单配置文件
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			fp.clear();
			if (get == SIM_FALSE || get == SIM_BE) {//文件存在或为普通配置文件
				fp.seekp(0, ios::beg);
				return 0;
			}

			fp.seekp(0, ios::end);//指针移动到文末
			fp << item_name << '=' << item_value << "\r\n";

			fp.clear();
			fp.seekp(0, ios::beg);//文件指针移动到开头
			return 1;
		}
		//普通配置文件
		else {
			int size_of_file = 0;
			string group = group_name;
			group = '[' + group + ']';
			string add = "";
			string data = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_BE || get == COM_GROUP_NONE)//文件存在或组不存在
				return 0;//未添加
			//开始添加
			fp.seekp(0, ios::beg);//指针回到起始位置
			while (1) {//每行
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				else
					size_of_file += strlen(line) + 1;
				int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);//提取一行的有效数据
				if (state == GROUP_LINE) {
					if (strline == group) {
						ostringstream out;
						out << item_name << '=' << item_value << "\r\n";
						add = out.str();
						break;//跳出while循环《——
					}
				}
			}//end of while

			//此后信息无需更改
			if (!fp.eof())//未到结尾处
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;//文件结尾
					string strline = line;
					data += strline + '\n';
				}

			//进行更改
			fp.clear();
			fp.seekp(0, ios::beg);
			file_resize(size_of_file);
			fp.seekp(0, ios::end);//指针移动到结束位置
			fp << add;
			fp << data;
			fp.seekp(0, ios::beg);//指针移动到起始位置

			return 1;
		}
	}

	return -1; //此时出错
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：添加char项
***************************************************************************/
int CFT::item_add(const char* group_name, const char* item_name, const char item_value)
{
	/*以读写方式打开*/
	if (open_mode == OPEN_OPT_RDWR) {
		//简单配置文件
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			fp.clear();
			if (get == SIM_FALSE || get == SIM_BE) {//文件存在或为普通配置文件
				fp.seekp(0, ios::beg);
				return 0;
			}

			fp.seekp(0, ios::end);//指针移动到文末
			fp << item_name << '=' << item_value << "\r\n";

			fp.clear();
			fp.seekp(0, ios::beg);//文件指针移动到开头
			return 1;
		}
		//普通配置文件
		else {
			int size_of_file = 0;
			string group = group_name;
			group = '[' + group + ']';
			string add = "";
			string data = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_BE || get == COM_GROUP_NONE)//文件存在或组不存在
				return 0;//未添加
			//开始添加
			fp.seekp(0, ios::beg);//指针回到起始位置
			while (1) {//每行
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				else
					size_of_file += strlen(line) + 1;
				int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);//提取一行的有效数据
				if (state == GROUP_LINE) {
					if (strline == group) {
						ostringstream out;
						out << item_name << '=' << item_value << "\r\n";
						add = out.str();
						break;//跳出while循环《——
					}
				}
			}//end of while

			//此后信息无需更改
			if (!fp.eof())//未到结尾处
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;//文件结尾
					string strline = line;
					data += strline + '\n';
				}

			//进行更改
			fp.clear();
			fp.seekp(0, ios::beg);
			file_resize(size_of_file);
			fp.seekp(0, ios::end);//指针移动到结束位置
			fp << add;
			fp << data;
			fp.seekp(0, ios::beg);//指针移动到起始位置

			return 1;
		}
	}

	return -1; //此时出错
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：添加空项
***************************************************************************/
int CFT::item_add(const char* group_name, const char* item_name)
{
	/*以读写方式打开*/
	if (open_mode == OPEN_OPT_RDWR) {
		//简单配置文件
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			fp.clear();
			if (get == SIM_FALSE || get == SIM_BE) {//文件存在或为普通配置文件
				fp.seekp(0, ios::beg);
				return 0;
			}

			fp.seekp(0, ios::end);//指针移动到文末
			fp << item_name << '=' << "\r\n";

			fp.clear();
			fp.seekp(0, ios::beg);//文件指针移动到开头
			return 1;
		}
		//普通配置文件
		else {
			int size_of_file = 0;
			string group = group_name;
			group = '[' + group + ']';
			string add = "";
			string data = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_BE || get == COM_GROUP_NONE)//文件存在或组不存在
				return 0;//未添加
			//开始添加
			fp.seekp(0, ios::beg);//指针回到起始位置
			while (1) {//每行
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				else
					size_of_file += strlen(line) + 1;
				int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);//提取一行的有效数据
				if (state == GROUP_LINE) {
					if (strline == group) {
						ostringstream out;
						out << item_name << '=' << "\r\n";
						add = out.str();
						break;//跳出while循环《——
					}
				}
			}//end of while

			//此后信息无需更改
			if (!fp.eof())//未到结尾处
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;//文件结尾
					string strline = line;
					data += strline + '\n';
				}

			//进行更改
			fp.clear();
			fp.seekp(0, ios::beg);
			file_resize(size_of_file);
			fp.seekp(0, ios::end);//指针移动到结束位置
			fp << add;
			fp << data;
			fp.seekp(0, ios::beg);//指针移动到起始位置

			return 1;
		}
	}

	return -1; //此时出错
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：删除项
***************************************************************************/
int CFT::item_del(const char* group_name, const char* item_name)
{
	/*以读写方式打开*/
	if (open_mode == OPEN_OPT_RDWR) {

		int size_of_file = 0;
		int cut_num = 0;
		fp.clear();
		fp.seekp(0, ios::beg);

		//简单配置文件
		if (group_name == NULL) {
			string data = "";
			const string item = item_name;
			int get = seek_exist(group_name, item_name);
			if (get == SIM_NONE)//无可删除项
				return cut_num;
			else{
				//找到第一个位置的截断处
				fp.clear();
				fp.seekp(0, ios::beg);
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					else
						size_of_file += strlen(line) + 1;
					int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
					if (line[line_length] == '\r')
						line[line_length] = '\0';
					string strline = line;
					int state = deal_line(strline);
					if (state == PROJECT_LINE) {
						for(int i=0;i< (int)strline.size();i++)//纯获取项目名
							if (strline[i] == '=') {
								strline.erase(i);//清除此后数据
								break;//跳出循环
							}
						if (strline == item) {
							size_of_file = size_of_file - (line_length + 1 + 1);
							cut_num++;
							break;//跳出while循环《——
						}
					}
				}//end of while
				//未找到需删除项
				if (fp.eof()) {
					fp.clear();
					fp.seekp(0, ios::beg);
					return cut_num;
				}
				//开始提取数据
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;//跳出whle循环《——
					string original = line;
					int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
					if (line[line_length] == '\r')
						line[line_length] = '\0';
					string strline = line;
					int state = deal_line(strline);

					if (state == PROJECT_LINE) {
						for (int i = 0; i < (int)strline.size(); i++)//纯获取项目名
							if (strline[i] == '=') {
								strline.erase(i);//清除此后数据
								break;
							}
						if (strline == item) {//相等就删除
							cut_num++;
							continue;
						}
					}
					data = data + original + '\n';
				}//end of while

				//开始删除
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);
				fp << data;
				fp.seekp(0, ios::beg);//指针移动到文件头

				return cut_num;
			}
		}
		//普通配置文件
		else {
			string data = "";
			string group =  group_name ;
			group = '[' + group + ']';
			string item = item_name;
			int get = seek_exist(group_name, item_name);
            //组不存在或组存在项不存在
			if (get == COM_GROUP_NONE || get == COM_FILE_NONE)
				return cut_num;
		    //组和项目均存在
			else {

				//找到第一个位置的截断处
				fp.clear();
				fp.seekp(0, ios::beg);
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					else
						size_of_file += strlen(line) + 1;
					string original = line;
					int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
					if (line[line_length] == '\r')
						line[line_length] = '\0';
					string strline = line;
					int state = deal_line(strline);
					if (state == GROUP_LINE) {
						if (group == strline) {
							while (1) {
								char line_twice[LINE_MAX] = { '\0' };
								fp.getline(line_twice, LINE_MAX, '\n');
								if (fp.eof())
									break;
								else
									size_of_file += strlen(line_twice) + 1;
								int line_length_twice = strlen(line_twice) - 1;//消除windows和linux之间的差异\r
								if (line_twice[line_length_twice] == '\r')
									line_twice[line_length_twice] = '\0';
								string strline_twice = line_twice;
								int state_twice = deal_line(strline_twice);
								if (state_twice == PROJECT_LINE) {
									for (int i = 0; i < (int)strline_twice.size(); i++)
										if (strline_twice[i] == '=') {
											strline_twice.erase(i);
											break;
										}
									if (strline_twice == item) {//找到相同项
										size_of_file = size_of_file - (line_length_twice + 1 + 1);
										cut_num++;
										break;//跳出samll while循环《——
									}
								}
							}//end of small while
							break;//跳出big while循环《——
						}
					}
				}//end of big while

				if (fp.eof())
					fp.clear();

				//开始二次提取 组内信息
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string orginal = line;
					int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
					if (line[line_length] == '\r')
						line[line_length] = '\0';
					string strline = line;
					int state = deal_line(strline);
					if (state == GROUP_LINE) {
						data = data + orginal + '\n';
						break;//跳出while循环《——
					}
					else if (state == PROJECT_LINE) {
						for (int i = 0; i < (int)strline.size(); i++)
							if (strline[i] == '=') {
								strline.erase(i);
								break;//跳出small while循环《——
							}
						if (strline == item)//相似则删除
							cut_num++;
						else
							data = data + orginal + '\n';
					}
				}

				//此后信息均不删除
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;//文件结尾
					string strline = line;
					data = data + strline + '\n';
				}

				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
                fp.seekp(0, ios::end);//指针移动到结束位置
				fp << data;
				fp.seekp(0, ios::beg);//指针移动到起始位置

				return cut_num;
			}
		}
	}

	return -1;//执行到此句表示出错
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：更新项目
***************************************************************************/
int CFT::item_update(const char* group_name, const char* item_name, const int item_value)
{
	/*以读写方式打开*/
	if (open_mode == OPEN_OPT_RDWR) {
		int size_of_file = 0;
		//简单配置文件
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			if (get == SIM_FALSE)//文件错误
				return 0;
			else if (get == SIM_NONE) {//不存在项目
				fp.clear();
				fp.seekp(0, ios::end);//指针移到文件尾
				fp << item_name << '=' << item_value << "\r\n";
				fp.seekp(0, ios::beg);//指针移到文件头
				return 1;
			}
			else if (get == SIM_BE) {//存在项目
				//找到需要更改的项目
				string item = item_name;
				string change = "";
				string data = "";
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					else
						size_of_file += strlen(line) + 1;
					int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
					if (line[line_length] == '\r')
						line[line_length] = '\0';
					string strline = line;
					int state = deal_line(strline);
					if (state == PROJECT_LINE) {
						for(int i=0;i< (int)strline.size();i++)
							if (strline[i] == '=') {
								strline.erase(i);
								break;
							}
						if (strline == item) {
							size_of_file = size_of_file - (line_length + 1 + 1);
							ostringstream out;
							out << item_name << '=' << item_value << "\r\n";
							change = out.str();
							break;//跳出while循环《——
						}	
					}
				}
				//此后的内容均不改变
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//进行更改
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//指针移动到结束位置
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//指针移动到起始位置

				return 1;
			}
		}
		//普通配置文件
		else {
			string group =  group_name ;
			group = '[' + group + ']';
			string item = item_name;
			string data = "";
			string change = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_GROUP_NONE)//不存在更新组
				return 0;
			else if (get == COM_FILE_NONE) {//不存在更新项目
				this->item_add(group_name, item_name, item_value);
				return 1;
			}
			else if(get==COM_BE) {//存在更新项目
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					else
						size_of_file += strlen(line) + 1;
					int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
					if (line[line_length] == '\r')
						line[line_length] = '\0';
					string strline = line;
					int state = deal_line(strline);
					if (state == GROUP_LINE) {
						if (strline == group) {
							while (1) {
								char line_twice[LINE_MAX] = { '\0' };
								fp.getline(line_twice, LINE_MAX, '\n');
								if (fp.eof())
									break;
								else
									size_of_file += strlen(line_twice) + 1;
								int line_length_twice = strlen(line_twice) - 1;//消除windows和linux之间的差异\r
								if (line_twice[line_length_twice] == '\r')
									line_twice[line_length_twice] = '\0';
								string strline_twice = line_twice;
								int state_twice = deal_line(strline_twice);
								if (state_twice == PROJECT_LINE) {
									for (int i = 0; i < (int)strline_twice.size(); i++)
										if (strline_twice[i] == '=') {
											strline_twice.erase(i);
											break;
										}
									if (strline_twice == item) {//找到相同项
										size_of_file = size_of_file - (line_length_twice + 1 + 1);
										ostringstream out;
										out << item_name << '=' << item_value << "\r\n";
										change = out.str();
										break;//跳出small while循环《——
									}
								}
							}//end of small while
							break;//跳出big while循环《——
						}	
					}
				}//end of big while

				//此后内容均无需改变
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//进行更改
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//指针移动到结束位置
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//指针移动到起始位置

				return 1;
			}
		}
	}

	return -1; //执行到此句时表示出错
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：更新项目
***************************************************************************/
int CFT::item_update(const char* group_name, const char* item_name, const double item_value)
{
	/*以读写方式打开*/
	if (open_mode == OPEN_OPT_RDWR) {
		int size_of_file = 0;
		//简单配置文件
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			if (get == SIM_FALSE)//文件错误
				return 0;
			else if (get == SIM_NONE) {//不存在项目
				fp.clear();
				fp.seekp(0, ios::end);//指针移到文件尾
				fp << item_name << '=' << setiosflags(ios::fixed) << setprecision(6) << item_value << "\r\n";//以六位小数打印
				fp.seekp(0, ios::beg);//指针移到文件头
				return 1;
			}
			else if (get == SIM_BE) {//存在项目
				//找到需要更改的项目
				string item = item_name;
				string change = "";
				string data = "";
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					else
						size_of_file += strlen(line) + 1;
					int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
					if (line[line_length] == '\r')
						line[line_length] = '\0';
					string strline = line;
					int state = deal_line(strline);
					if (state == PROJECT_LINE) {
						for (int i = 0; i < (int)strline.size(); i++)
							if (strline[i] == '=') {
								strline.erase(i);
								break;
							}
						if (strline == item) {
							size_of_file = size_of_file - (line_length + 1 + 1);
							ostringstream out;
							out << item_name << '=' << setiosflags(ios::fixed) << setprecision(6) << item_value << "\r\n";//以六位小数打印
							change = out.str();
							break;//跳出while循环《——
						}
					}
				}
				//此后的内容均不改变
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//进行更改
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//指针移动到结束位置
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//指针移动到起始位置

				return 1;
			}
		}
		//普通配置文件
		else {
			string group = group_name;
			group = '[' + group + ']';
			string item = item_name;
			string data = "";
			string change = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_GROUP_NONE)//不存在更新组
				return 0;
			else if (get == COM_FILE_NONE) {//不存在更新项目
				this->item_add(group_name, item_name, item_value);
				return 1;
			}
			else if (get == COM_BE) {//存在更新项目
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					else
						size_of_file += strlen(line) + 1;
					int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
					if (line[line_length] == '\r')
						line[line_length] = '\0';
					string strline = line;
					int state = deal_line(strline);
					if (state == GROUP_LINE) {
						if (strline == group) {
							while (1) {
								char line_twice[LINE_MAX] = { '\0' };
								fp.getline(line_twice, LINE_MAX, '\n');
								if (fp.eof())
									break;
								else
									size_of_file += strlen(line_twice) + 1;
								int line_length_twice = strlen(line_twice) - 1;//消除windows和linux之间的差异\r
								if (line_twice[line_length_twice] == '\r')
									line_twice[line_length_twice] = '\0';
								string strline_twice = line_twice;
								int state_twice = deal_line(strline_twice);
								if (state_twice == PROJECT_LINE) {
									for (int i = 0; i < (int)strline_twice.size(); i++)
										if (strline_twice[i] == '=') {
											strline_twice.erase(i);
											break;
										}
									if (strline_twice == item) {//找到相同项
										size_of_file = size_of_file - (line_length_twice + 1 + 1);
										ostringstream out;
										out << item_name << '=' << setiosflags(ios::fixed) << setprecision(6) << item_value << "\r\n";//以六位小数打印
										change = out.str();
										break;//跳出small while循环《——
									}
								}
							}//end of small while
							break;//跳出big while循环《——
						}
					}
				}//end of big while

				//此后内容均无需改变
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//进行更改
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//指针移动到结束位置
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//指针移动到起始位置

				return 1;
			}
		}
	}

	return -1; //执行到此句时表示出错
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：更新项目
***************************************************************************/
int CFT::item_update(const char* group_name, const char* item_name, const char* item_value)
{
	/*以读写方式打开*/
	if (open_mode == OPEN_OPT_RDWR) {
		int size_of_file = 0;
		//简单配置文件
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			if (get == SIM_FALSE)//文件错误
				return 0;
			else if (get == SIM_NONE) {//不存在项目
				fp.clear();
				fp.seekp(0, ios::end);//指针移到文件尾
				fp << item_name << '=' << item_value << "\r\n";
				fp.seekp(0, ios::beg);//指针移到文件头
				return 1;
			}
			else if (get == SIM_BE) {//存在项目
				//找到需要更改的项目
				string item = item_name;
				string change = "";
				string data = "";
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					else
						size_of_file += strlen(line) + 1;
					int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
					if (line[line_length] == '\r')
						line[line_length] = '\0';
					string strline = line;
					int state = deal_line(strline);
					if (state == PROJECT_LINE) {
						for (int i = 0; i < (int)strline.size(); i++)
							if (strline[i] == '=') {
								strline.erase(i);
								break;
							}
						if (strline == item) {
							size_of_file = size_of_file - (line_length + 1 + 1);
							ostringstream out;
							out << item_name << '=' << item_value << "\r\n";
							change = out.str();
							break;//跳出while循环《——
						}
					}
				}
				//此后的内容均不改变
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//进行更改
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//指针移动到结束位置
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//指针移动到起始位置

				return 1;
			}
		}
		//普通配置文件
		else {
			string group = group_name;
			group = '[' + group + ']';
			string item = item_name;
			string data = "";
			string change = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_GROUP_NONE)//不存在更新组
				return 0;
			else if (get == COM_FILE_NONE) {//不存在更新项目
				this->item_add(group_name, item_name, item_value);
				return 1;
			}
			else if (get == COM_BE) {//存在更新项目
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					else
						size_of_file += strlen(line) + 1;
					int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
					if (line[line_length] == '\r')
						line[line_length] = '\0';
					string strline = line;
					int state = deal_line(strline);
					if (state == GROUP_LINE) {
						if (strline == group) {
							while (1) {
								char line_twice[LINE_MAX] = { '\0' };
								fp.getline(line_twice, LINE_MAX, '\n');
								if (fp.eof())
									break;
								else
									size_of_file += strlen(line_twice) + 1;
								int line_length_twice = strlen(line_twice) - 1;//消除windows和linux之间的差异\r
								if (line_twice[line_length_twice] == '\r')
									line_twice[line_length_twice] = '\0';
								string strline_twice = line_twice;
								int state_twice = deal_line(strline_twice);
								if (state_twice == PROJECT_LINE) {
									for (int i = 0; i < (int)strline_twice.size(); i++)
										if (strline_twice[i] == '=') {
											strline_twice.erase(i);
											break;
										}
									if (strline_twice == item) {//找到相同项
										size_of_file = size_of_file - (line_length_twice + 1 + 1);
										ostringstream out;
										out << item_name << '=' << item_value << "\r\n";
										change = out.str();
										break;//跳出small while循环《——
									}
								}
							}//end of small while
							break;//跳出big while循环《——
						}
					}
				}//end of big while

				//此后内容均无需改变
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//进行更改
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//指针移动到结束位置
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//指针移动到起始位置

				return 1;
			}
		}
	}
	return -1; //执行到此句时表示出错
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：更新项目
***************************************************************************/
int CFT::item_update(const char* group_name, const char* item_name, const string &item_value)
{
	/*以读写方式打开*/
	if (open_mode == OPEN_OPT_RDWR) {
		int size_of_file = 0;
		//简单配置文件
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			if (get == SIM_FALSE)//文件错误
				return 0;
			else if (get == SIM_NONE) {//不存在项目
				fp.clear();
				fp.seekp(0, ios::end);//指针移到文件尾
				fp << item_name << '=' << item_value << "\r\n";
				fp.seekp(0, ios::beg);//指针移到文件头
				return 1;
			}
			else if (get == SIM_BE) {//存在项目
				//找到需要更改的项目
				string item = item_name;
				string change = "";
				string data = "";
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					else
						size_of_file += strlen(line) + 1;
					int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
					if (line[line_length] == '\r')
						line[line_length] = '\0';
					string strline = line;
					int state = deal_line(strline);
					if (state == PROJECT_LINE) {
						for (int i = 0; i < (int)strline.size(); i++)
							if (strline[i] == '=') {
								strline.erase(i);
								break;
							}
						if (strline == item) {
							size_of_file = size_of_file - (line_length + 1 + 1);
							ostringstream out;
							out << item_name << '=' << item_value << "\r\n";
							change = out.str();
							break;//跳出while循环《——
						}
					}
				}
				//此后的内容均不改变
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//进行更改
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//指针移动到结束位置
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//指针移动到起始位置

				return 1;
			}
		}
		//普通配置文件
		else {
			string group = group_name;
			group = '[' + group + ']';
			string item = item_name;
			string data = "";
			string change = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_GROUP_NONE)//不存在更新组
				return 0;
			else if (get == COM_FILE_NONE) {//不存在更新项目
				this->item_add(group_name, item_name, item_value);
				return 1;
			}
			else if (get == COM_BE) {//存在更新项目
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					else
						size_of_file += strlen(line) + 1;
					int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
					if (line[line_length] == '\r')
						line[line_length] = '\0';
					string strline = line;
					int state = deal_line(strline);
					if (state == GROUP_LINE) {
						if (strline == group) {
							while (1) {
								char line_twice[LINE_MAX] = { '\0' };
								fp.getline(line_twice, LINE_MAX, '\n');
								if (fp.eof())
									break;
								else
									size_of_file += strlen(line_twice) + 1;
								int line_length_twice = strlen(line_twice) - 1;//消除windows和linux之间的差异\r
								if (line_twice[line_length_twice] == '\r')
									line_twice[line_length_twice] = '\0';
								string strline_twice = line_twice;
								int state_twice = deal_line(strline_twice);
								if (state_twice == PROJECT_LINE) {
									for (int i = 0; i < (int)strline_twice.size(); i++)
										if (strline_twice[i] == '=') {
											strline_twice.erase(i);
											break;
										}
									if (strline_twice == item) {//找到相同项
										size_of_file = size_of_file - (line_length_twice + 1 + 1);
										ostringstream out;
										out << item_name << '=' << item_value << "\r\n";
										change = out.str();
										break;//跳出small while循环《——
									}
								}
							}//end of small while
							break;//跳出big while循环《——
						}
					}
				}//end of big while

				//此后内容均无需改变
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//进行更改
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//指针移动到结束位置
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//指针移动到起始位置

				return 1;
			}
		}
	}
	return -1; //执行到此句时表示出错
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：更新项目
***************************************************************************/
int CFT::item_update(const char* group_name, const char* item_name, const char item_value)
{
	/*以读写方式打开*/
	if (open_mode == OPEN_OPT_RDWR) {
		int size_of_file = 0;
		//简单配置文件
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			if (get == SIM_FALSE)//文件错误
				return 0;
			else if (get == SIM_NONE) {//不存在项目
				fp.clear();
				fp.seekp(0, ios::end);//指针移到文件尾
				fp << item_name << '=' << item_value << "\r\n";
				fp.seekp(0, ios::beg);//指针移到文件头
				return 1;
			}
			else if (get == SIM_BE) {//存在项目
				//找到需要更改的项目
				string item = item_name;
				string change = "";
				string data = "";
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					else
						size_of_file += strlen(line) + 1;
					int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
					if (line[line_length] == '\r')
						line[line_length] = '\0';
					string strline = line;
					int state = deal_line(strline);
					if (state == PROJECT_LINE) {
						for (int i = 0; i < (int)strline.size(); i++)
							if (strline[i] == '=') {
								strline.erase(i);
								break;
							}
						if (strline == item) {
							size_of_file = size_of_file - (line_length + 1 + 1);
							ostringstream out;
							out << item_name << '=' << item_value << "\r\n";
							change = out.str();
							break;//跳出while循环《——
						}
					}
				}
				//此后的内容均不改变
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//进行更改
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//指针移动到结束位置
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//指针移动到起始位置

				return 1;
			}
		}
		//普通配置文件
		else {
			string group = group_name;
			group = '[' + group + ']';
			string item = item_name;
			string data = "";
			string change = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_GROUP_NONE)//不存在更新组
				return 0;
			else if (get == COM_FILE_NONE) {//不存在更新项目
				this->item_add(group_name, item_name, item_value);
				return 1;
			}
			else if (get == COM_BE) {//存在更新项目
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					else
						size_of_file += strlen(line) + 1;
					int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
					if (line[line_length] == '\r')
						line[line_length] = '\0';
					string strline = line;
					int state = deal_line(strline);
					if (state == GROUP_LINE) {
						if (strline == group) {
							while (1) {
								char line_twice[LINE_MAX] = { '\0' };
								fp.getline(line_twice, LINE_MAX, '\n');
								if (fp.eof())
									break;
								else
									size_of_file += strlen(line_twice) + 1;
								int line_length_twice = strlen(line_twice) - 1;//消除windows和linux之间的差异\r
								if (line_twice[line_length_twice] == '\r')
									line_twice[line_length_twice] = '\0';
								string strline_twice = line_twice;
								int state_twice = deal_line(strline_twice);
								if (state_twice == PROJECT_LINE) {
									for (int i = 0; i < (int)strline_twice.size(); i++)
										if (strline_twice[i] == '=') {
											strline_twice.erase(i);
											break;
										}
									if (strline_twice == item) {//找到相同项
										size_of_file = size_of_file - (line_length_twice + 1 + 1);
										ostringstream out;
										out << item_name << '=' << item_value << "\r\n";
										change = out.str();
										break;//跳出small while循环《——
									}
								}
							}//end of small while
							break;//跳出big while循环《——
						}
					}
				}//end of big while

				//此后内容均无需改变
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//进行更改
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//指针移动到结束位置
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//指针移动到起始位置

				return 1;
			}
		}
	}
	return -1; //执行到此句时表示出错
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：更新项目
***************************************************************************/
int CFT::item_update(const char* group_name, const char* item_name)
{
	/*以读写方式打开*/
	if (open_mode == OPEN_OPT_RDWR) {
		int size_of_file = 0;
		//简单配置文件
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			if (get == SIM_FALSE)//文件错误
				return 0;
			else if (get == SIM_NONE) {//不存在项目
				fp.clear();
				fp.seekp(0, ios::end);//指针移到文件尾
				fp << item_name << '=' << "\r\n";
				fp.seekp(0, ios::beg);//指针移到文件头
				return 1;
			}
			else if (get == SIM_BE) {//存在项目
				//找到需要更改的项目
				string item = item_name;
				string change = "";
				string data = "";
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					else
						size_of_file += strlen(line) + 1;
					int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
					if (line[line_length] == '\r')
						line[line_length] = '\0';
					string strline = line;
					int state = deal_line(strline);
					if (state == PROJECT_LINE) {
						for (int i = 0; i < (int)strline.size(); i++)
							if (strline[i] == '=') {
								strline.erase(i);
								break;
							}
						if (strline == item) {
							size_of_file = size_of_file - (line_length + 1 + 1);
							ostringstream out;
							out << item_name << '='<< "\r\n";
							change = out.str();
							break;//跳出while循环《——
						}
					}
				}
				//此后的内容均不改变
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//进行更改
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//指针移动到结束位置
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//指针移动到起始位置

				return 1;
			}
		}
		//普通配置文件
		else {
			string group = group_name;
			group = '[' + group + ']';
			string item = item_name;
			string data = "";
			string change = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_GROUP_NONE)//不存在更新组
				return 0;
			else if (get == COM_FILE_NONE) {//不存在更新项目
				this->item_add(group_name, item_name);
				return 1;
			}
			else if (get == COM_BE) {//存在更新项目
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					else
						size_of_file += strlen(line) + 1;
					int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
					if (line[line_length] == '\r')
						line[line_length] = '\0';
					string strline = line;
					int state = deal_line(strline);
					if (state == GROUP_LINE) {
						if (strline == group) {
							while (1) {
								char line_twice[LINE_MAX] = { '\0' };
								fp.getline(line_twice, LINE_MAX, '\n');
								if (fp.eof())
									break;
								else
									size_of_file += strlen(line_twice) + 1;
								int line_length_twice = strlen(line_twice) - 1;//消除windows和linux之间的差异\r
								if (line_twice[line_length_twice] == '\r')
									line_twice[line_length_twice] = '\0';
								string strline_twice = line_twice;
								int state_twice = deal_line(strline_twice);
								if (state_twice == PROJECT_LINE) {
									for (int i = 0; i < (int)strline_twice.size(); i++)
										if (strline_twice[i] == '=') {
											strline_twice.erase(i);
											break;
										}
									if (strline_twice == item) {//找到相同项
										size_of_file = size_of_file - (line_length_twice + 1 + 1);
										ostringstream out;
										out << item_name << '=' << "\r\n";
										change = out.str();
										break;//跳出small while循环《——
									}
								}
							}//end of small while
							break;//跳出big while循环《——
						}
					}
				}//end of big while

				//此后内容均无需改变
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//进行更改
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//指针移动到结束位置
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//指针移动到起始位置

				return 1;
			}
		}
	}
	return -1; //执行到此句时表示出错
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：获取值
***************************************************************************/
int CFT::item_get_value(const char* group_name, const char* item_name, int &item_value)
{
	/*读或者读写方式打开文件均可*/

	//简单配置文件
	if (group_name == NULL) {
		int get = seek_exist(group_name, item_name);
		if (get == SIM_FALSE || get == SIM_NONE)//提取项不存在
			return 0;
		else if (get == SIM_BE) {//提取项存在
			string item = item_name;
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);
				if (state == PROJECT_LINE) {
					string temp = strline;
					for (int i = 0; i < (int)temp.size(); i++)
						if (temp[i] == '=') {
							temp.erase(i);
							break;
						}
					if (temp == item) {//找到该项
						for (int i = 0; i < (int)strline.size(); i++)
							if (strline[i] == '=') {
								strline.erase(0, (i + 1));
								break;
							}
						istringstream in(strline);//提取数据
						in >> item_value;
						fp.clear();
						fp.seekg(0, ios::beg);

						return 1;//提取到文件
					}
				}
			}//end of big while
		}
	}
	//普通配置文件
	else {
		int get = seek_exist(group_name, item_name);
		if (get == COM_FILE_NONE || get == COM_GROUP_NONE)//提取项不存在
			return 0;
		else if (get == COM_BE) {//提取项存在
			string group = group_name;
			group = '[' + group + ']';
			string item = item_name;
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);
				if (state == GROUP_LINE) {
					if (strline == group) {//找到该组时
						while (1) {//找到该项时
							char line_next[LINE_MAX] = { '\0' };
							fp.getline(line_next, LINE_MAX, '\n');
							if (fp.eof())
								break;
							int line_length_next = strlen(line_next) - 1;//消除windows和linux之间的差异\r
							if (line_next[line_length_next] == '\r')
								line_next[line_length_next] = '\0';
							string strline_next = line_next;
							int state_next = deal_line(strline_next);
							if (state_next == PROJECT_LINE) {
								string temp = strline_next;
								for (int i = 0; i < (int)temp.size(); i++)
									if (temp[i] == '=') {
										temp.erase(i);
										break;
									}
								if (temp == item) {//找到该项
									for (int i = 0; i < (int)strline_next.size(); i++)//仅提取数据
										if (strline_next[i] == '=') {
											strline_next.erase(0, (i + 1));
											break;
										}
									istringstream in(strline_next);//提取数据
									in >> item_value;
									fp.clear();
									fp.seekg(0, ios::beg);

									return 1;//提取到值
								}
							}
						}//end of small while
					}
				}
			}//end of big while

			if (fp.eof())
				fp.clear();
			fp.seekg(0, ios::beg);
		}
	}


	return -1; //执行到此句时出错
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：获取值
***************************************************************************/
int CFT::item_get_value(const char* group_name, const char* item_name, double& item_value)
{
	/*读或者读写方式打开文件均可*/

	//简单配置文件
	if (group_name == NULL) {
		int get = seek_exist(group_name, item_name);
		if (get == SIM_FALSE || get == SIM_NONE)//提取项不存在
			return 0;
		else if (get == SIM_BE) {//提取项存在
			string item = item_name;
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);
				if (state == PROJECT_LINE) {
					string temp = strline;
					for (int i = 0; i < (int)temp.size(); i++)
						if (temp[i] == '=') {
							temp.erase(i);
							break;
						}
					if (temp == item) {//找到该项
						for (int i = 0; i < (int)strline.size(); i++)
							if (strline[i] == '=') {
								strline.erase(0, (i + 1));
								break;
							}
						istringstream in(strline);//提取数据
						in >> item_value;
						fp.clear();
						fp.seekg(0, ios::beg);

						return 1;//提取到文件
					}
				}
			}//end of big while
		}
	}
	//普通配置文件
	else {
		int get = seek_exist(group_name, item_name);
		if (get == COM_FILE_NONE || get == COM_GROUP_NONE)//提取项不存在
			return 0;
		else if (get == COM_BE) {//提取项存在
			string group = group_name;
			group = '[' + group + ']';
			string item = item_name;
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);
				if (state == GROUP_LINE) {
					if (strline == group) {//找到该组时
						while (1) {//找到该项时
							char line_next[LINE_MAX] = { '\0' };
							fp.getline(line_next, LINE_MAX, '\n');
							if (fp.eof())
								break;
							int line_length_next = strlen(line_next) - 1;//消除windows和linux之间的差异\r
							if (line_next[line_length_next] == '\r')
								line_next[line_length_next] = '\0';
							string strline_next = line_next;
							int state_next = deal_line(strline_next);
							if (state_next == PROJECT_LINE) {
								string temp = strline_next;
								for (int i = 0; i < (int)temp.size(); i++)
									if (temp[i] == '=') {
										temp.erase(i);
										break;
									}
								if (temp == item) {//找到该项
									for (int i = 0; i < (int)strline_next.size(); i++)//仅提取数据
										if (strline_next[i] == '=') {
											strline_next.erase(0, (i + 1));
											break;
										}
									istringstream in(strline_next);//提取数据
									in >> item_value;
									fp.clear();
									fp.seekg(0, ios::beg);

									return 1;//提取到值
								}
							}
						}//end of small while
					}
				}
			}//end of big while

			if (fp.eof())
				fp.clear();
			fp.seekg(0, ios::beg);
		}
	}


	return -1; //执行到此句时出错
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：获取值
***************************************************************************/
int CFT::item_get_value(const char* group_name, const char* item_name, char *item_value)
{
	/*读或者读写方式打开文件均可*/

	//简单配置文件
	if (group_name == NULL) {
		int get = seek_exist(group_name, item_name);
		if (get == SIM_FALSE || get == SIM_NONE)//提取项不存在
			return 0;
		else if (get == SIM_BE) {//提取项存在
			string item = item_name;
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);
				if (state == PROJECT_LINE) {
					string temp = strline;
					for (int i = 0; i < (int)temp.size(); i++)
						if (temp[i] == '=') {
							temp.erase(i);
							break;
						}
					if (temp == item) {//找到该项
						for (int i = 0; i < (int)strline.size(); i++)
							if (strline[i] == '=') {
								strline.erase(0, (i + 1));
								break;
							}
						strcpy(item_value, strline.data());
						fp.clear();
						fp.seekg(0, ios::beg);

						return 1;//提取到文件
					}
				}
			}//end of big while
		}
	}
	//普通配置文件
	else {
		int get = seek_exist(group_name, item_name);
		if (get == COM_FILE_NONE || get == COM_GROUP_NONE)//提取项不存在
			return 0;
		else if (get == COM_BE) {//提取项存在
			string group = group_name;
			group = '[' + group + ']';
			string item = item_name;
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);
				if (state == GROUP_LINE) {
					if (strline == group) {//找到该组时
						while (1) {//找到该项时
							char line_next[LINE_MAX] = { '\0' };
							fp.getline(line_next, LINE_MAX, '\n');
							if (fp.eof())
								break;
							int line_length_next = strlen(line_next) - 1;//消除windows和linux之间的差异\r
							if (line_next[line_length_next] == '\r')
								line_next[line_length_next] = '\0';
							string strline_next = line_next;
							int state_next = deal_line(strline_next);
							if (state_next == PROJECT_LINE) {
								string temp = strline_next;
								for (int i = 0; i < (int)temp.size(); i++)
									if (temp[i] == '=') {
										temp.erase(i);
										break;
									}
								if (temp == item) {//找到该项
									for (int i = 0; i < (int)strline_next.size(); i++)//仅提取数据
										if (strline_next[i] == '=') {
											strline_next.erase(0, (i + 1));
											break;
										}
									strcpy(item_value, strline_next.data());
									fp.clear();
									fp.seekg(0, ios::beg);

									return 1;//提取到值
								}
							}
						}//end of small while
					}
				}
			}//end of big while

			if (fp.eof())
				fp.clear();
			fp.seekg(0, ios::beg);
		}
	}


	return -1; //执行到此句时出错
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：获取值
***************************************************************************/
int CFT::item_get_value(const char* group_name, const char* item_name, string &item_value)
{
	/*读或者读写方式打开文件均可*/

	//简单配置文件
	if (group_name == NULL) {
		int get = seek_exist(group_name, item_name);
		if (get == SIM_FALSE || get == SIM_NONE)//提取项不存在
			return 0;
		else if (get == SIM_BE) {//提取项存在
			string item = item_name;
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);
				if (state == PROJECT_LINE) {
					string temp = strline;
					for (int i = 0; i < (int)temp.size(); i++)
						if (temp[i] == '=') {
							temp.erase(i);
							break;
						}
					if (temp == item) {//找到该项
						for (int i = 0; i < (int)strline.size(); i++)
							if (strline[i] == '=') {
								strline.erase(0, (i + 1));
								break;
							}
						item_value = strline;
						fp.clear();
						fp.seekg(0, ios::beg);

						return 1;//提取到文件
					}
				}
			}//end of big while
		}
	}
	//普通配置文件
	else {
		int get = seek_exist(group_name, item_name);
		if (get == COM_FILE_NONE || get == COM_GROUP_NONE)//提取项不存在
			return 0;
		else if (get == COM_BE) {//提取项存在
			string group = group_name;
			group = '[' + group + ']';
			string item = item_name;
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);
				if (state == GROUP_LINE) {
					if (strline == group) {//找到该组时
						while (1) {//找到该项时
							char line_next[LINE_MAX] = { '\0' };
							fp.getline(line_next, LINE_MAX, '\n');
							if (fp.eof())
								break;
							int line_length_next = strlen(line_next) - 1;//消除windows和linux之间的差异\r
							if (line_next[line_length_next] == '\r')
								line_next[line_length_next] = '\0';
							string strline_next = line_next;
							int state_next = deal_line(strline_next);
							if (state_next == PROJECT_LINE) {
								string temp = strline_next;
								for (int i = 0; i < (int)temp.size(); i++)
									if (temp[i] == '=') {
										temp.erase(i);
										break;
									}
								if (temp == item) {//找到该项
									for (int i = 0; i < (int)strline_next.size(); i++)//仅提取数据
										if (strline_next[i] == '=') {
											strline_next.erase(0, (i + 1));
											break;
										}
									item_value = strline_next;
									fp.clear();
									fp.seekg(0, ios::beg);

									return 1;//提取到值
								}
							}
						}//end of small while
					}
				}
			}//end of big while

			if (fp.eof())
				fp.clear();
			fp.seekg(0, ios::beg);
		}
	}


	return -1; //执行到此句时出错
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：获取值
***************************************************************************/
int CFT::item_get_value(const char* group_name, const char* item_name, char& item_value)
{
	/*读或者读写方式打开文件均可*/

	//简单配置文件
	if (group_name == NULL) {
		int get = seek_exist(group_name, item_name);
		if (get == SIM_FALSE || get == SIM_NONE)//提取项不存在
			return 0;
		else if (get == SIM_BE) {//提取项存在
			string item = item_name;
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);
				if (state == PROJECT_LINE) {
					string temp = strline;
					for (int i = 0; i < (int)temp.size(); i++)
						if (temp[i] == '=') {
							temp.erase(i);
							break;
						}
					if (temp == item) {//找到该项
						for (int i = 0; i < (int)strline.size(); i++)
							if (strline[i] == '=') {
								strline.erase(0, (i + 1));
								break;
							}
						istringstream in(strline);//提取数据
						in >> item_value;
						fp.clear();
						fp.seekg(0, ios::beg);

						return 1;//提取到文件
					}
				}
			}//end of big while
		}
	}
	//普通配置文件
	else {
		int get = seek_exist(group_name, item_name);
		if (get == COM_FILE_NONE || get == COM_GROUP_NONE)//提取项不存在
			return 0;
		else if (get == COM_BE) {//提取项存在
			string group = group_name;
			group = '[' + group + ']';
			string item = item_name;
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				int line_length = strlen(line) - 1;//消除windows和linux之间的差异\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);
				if (state == GROUP_LINE) {
					if (strline == group) {//找到该组时
						while (1) {//找到该项时
							char line_next[LINE_MAX] = { '\0' };
							fp.getline(line_next, LINE_MAX, '\n');
							if (fp.eof())
								break;
							int line_length_next = strlen(line_next) - 1;//消除windows和linux之间的差异\r
							if (line_next[line_length_next] == '\r')
								line_next[line_length_next] = '\0';
							string strline_next = line_next;
							int state_next = deal_line(strline_next);
							if (state_next == PROJECT_LINE) {
								string temp = strline_next;
								for (int i = 0; i < (int)temp.size(); i++)
									if (temp[i] == '=') {
										temp.erase(i);
										break;
									}
								if (temp == item) {//找到该项
									for (int i = 0; i < (int)strline_next.size(); i++)//仅提取数据
										if (strline_next[i] == '=') {
											strline_next.erase(0, (i + 1));
											break;
										}
									istringstream in(strline_next);//提取数据
									in >> item_value;
									fp.clear();
									fp.seekg(0, ios::beg);

									return 1;//提取到值
								}
							}
						}//end of small while
					}
				}
			}//end of big while

			if (fp.eof())
				fp.clear();
			fp.seekg(0, ios::beg);
		}
	}


	return -1; //执行到此句时出错
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：获取值
***************************************************************************/
int CFT::item_get_value(const char* group_name, const char* item_name)
{
	/*读或者读写方式打开文件均可*/

	//简单配置文件
	if (group_name == NULL) {
		int get = seek_exist(group_name, item_name);
		fp.clear();
		fp.seekg(0, ios::beg);
		if (get == SIM_FALSE || get == SIM_NONE)//提取项不存在
			return 0;
		else if (get == SIM_BE) //提取项存在
			return 1;
	}
	//普通配置文件
	else {
		int get = seek_exist(group_name, item_name);
		fp.clear();
		fp.seekg(0, ios::beg);
		if (get == COM_FILE_NONE || get == COM_GROUP_NONE)//提取项不存在
			return 0;
		else if (get == COM_BE)//提取项存在
			return 1;
	}

	return -1; //执行到此句时出错

}

