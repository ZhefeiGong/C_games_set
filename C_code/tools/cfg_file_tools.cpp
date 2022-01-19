/*2052532 ��04 ���ܷ�*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
#include <io.h>
#include <iomanip>
#include "../include/cfg_file_tools.h"
using namespace std;

/* ---------------------------------------------------------------
	 �������������Ҫstatic�������ڲ������ã�
   ---------------------------------------------------------------- */

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ������0 ��ʾ����Ϊ���У�����1 ��ʾ��λ��Ϊ����������2 ��ʾ��λ��Ϊ��Ŀ��
  ˵    ������ȡһ�еĹؼ���Ϣ
            �ض�;��#��ʼ��ע�ͣ�����ǰ��ո�/tab��
****************************************************************************/
static int deal_line(string& in)
{
	int length = in.size();
	string sample = in;        //in��ʱ���ӸĶ�
	string result = "";        //�������ս��
	string temp = "";          //����ո�Ļ�����
	bool front = true;
	
	//ȥ��ע����
	int mode = 0;//mode=1 ������mode=2��Ŀ����0Ϊ����
	for (int num = 0; num < length; num++) {
		if (sample[num] == ';' || sample[num] == '#') {
			sample.erase(num);//��ʱ֮�������ȫ������
			break;
		}

	}

	//����ǰ����󲿵Ĳ��ɼ��ַ�
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

	//�ж�����
	length = sample.size();//��󳤶ȸı�
	for (int num = 0; num < length; num++)
	{
		if ((sample[num] >= 0 && sample[num] <= 32) || (sample[num] == 127))//���Բ��ɼ��ַ�
			;
		else if (sample[num] == '[') {//����'['����
			for (int n = length - 1; n >= 0; n--) {
				if ((sample[n] >= 0 && sample[n] <= 32) || (sample[n] == 127))//���Բ��ɼ��ַ�
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


	/*��ȡ��Ч����*/

	//��Ŀ��
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
		//��Ŀ��
		for (int i = before.size() - 1; i >= 0; i--)
			if (!((before[i] >= 0 && before[i] <= 32) || (before[i] == 127))) {//���Բ��ɼ��ַ�
				before.erase(i + 1);
				break;
			}
		//ֵ��
		bool temp= false;
		int count = 0;
		for (int i = 0; i < (int)after.size(); i++) {
			if (((after[i] >= 0 && after[i] <= 32) || (after[i] == 127)) &&temp==false) {//����ǰ���Ŀո����TAB���Ȳ��ɼ��ַ�
				count++;
			}
			else {
				temp = true;
				if ((after[i] >= 0 && after[i] <= 32) || (after[i] == 127)) {//�ַ����в����ո�Ȳ��ɼ��ַ�
					after.erase(i);
					break;
				}
			}
		}
		after.erase(0, count);
		result = before + '=' + after;
	}
    //������
	else if (mode == GROUP_LINE) {
		int n = length - 1;//���Ժ�ո��]�ڵĲ��ɼ��ַ�
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
  �������ƣ�seek_exist
  ��    �ܣ�Ѱ��ĳ����Ŀ�Ƿ����
  ���������
  �� �� ֵ��
  ˵    ��������ֵ˵����
            group_name=NULL ��ʾ���Ҽ������ļ�
                     SIM_FALSE            -1           //��Ҫ�������ļ� ȴΪ��ͨ�����ļ�
                     SIM_NONE             -2           //�������ļ�������
                     SIM_BE               -3           //�������ļ�����
            group_name!=NULL ��ʾ������ͨ�����ļ�
                     COM_GROUP_NONE        1           //��ͨ�����ļ��鲻����
                     COM_FILE_NONE         2           //��ͨ�����ļ������ ��Ŀ������
                     COM_BE                3           //��ͨ�����ļ�����      
***************************************************************************/
int CFT::seek_exist(const char* group_name, const char* item_name)
{
	fp.clear();
	fp.seekg(0, ios::beg);        //�ļ�ָ���ƶ�����ʼ��

	//�������ļ�
	if (group_name == NULL) {
		string item = item_name;
		while (1) {//ÿ��
			char line[LINE_MAX] = { '\0' };
			fp.getline(line, LINE_MAX, '\n');
			if (fp.eof())
				break;
			string strline = line;
			int state = deal_line(strline);//��ȡһ�е���Ч����
			if (state == GROUP_LINE) {//����
				fp.clear();
				fp.seekg(0, ios::beg);//ָ���ƶ�����ͷλ��

				return SIM_FALSE;//��Ҫ�������ļ� ȴΪ��ͨ�����ļ�
			}
			else if (state == PROJECT_LINE) {//��Ŀ��
				for (int i = 0; i < (int)strline.size(); i++)
					if (strline[i] == '=') {
						strline.erase(i);
						break;//����ѭ��
					}
				if (strline == item) {//���ڸ���Ŀ
					fp.clear();
					fp.seekg(0, ios::beg);//ָ���ƶ�����ͷλ��

					return SIM_BE;//�������ļ�����
				}

			}
		}
		//δ�ҵ�
		if (fp.eof()) {
			fp.clear();
			fp.seekg(0, ios::beg);//�ļ�ָ���ƶ�����ͷ

			return SIM_NONE;//�������ļ�������
		}
	}
	//��ͨ�����ļ�
	else {
		string group = group_name;
		group = '[' + group + ']';
		string  item = item_name;
		//�����Ƿ���ڸ���
		while (1) {//ÿ��
			char line[LINE_MAX] = { '\0' };
			fp.getline(line, LINE_MAX, '\n');
			if (fp.eof()) {
				fp.clear();
				fp.seekg(0, ios::beg);

				return COM_GROUP_NONE;//�鶼������
			}
			int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
			if (line[line_length] == '\r')
				line[line_length] = '\0';
			string strline = line;
			int state = deal_line(strline);//��ȡһ�е���Ч����
			if (state == GROUP_LINE) {
				if (strline == group) {
					//�����������Ѱ
					while (1) {
						char line_s[LINE_MAX] = { '\0' };
						fp.getline(line_s, LINE_MAX, '\n');
						if (fp.eof()) {
							fp.clear();
							fp.seekg(0, ios::beg);//ָ���ƶ�����ͷ

							return COM_FILE_NONE;//����� ��Ŀ������
						}
						int line_length_s = strlen(line_s) - 1;//����windows��linux֮��Ĳ���\r
						if (line_s[line_length_s] == '\r')
							line_s[line_length_s] = '\0';
						string strline_s = line_s;
						int state_s = deal_line(strline_s);//��ȡһ�е���Ч����
						if (state_s == GROUP_LINE) {
							fp.clear();
							fp.seekg(0, ios::beg);//ָ���ƶ�����ͷ

							return COM_FILE_NONE;//����� ��Ŀ������
						}
						else if (state_s == PROJECT_LINE) {//�ж��Ƿ����
							for (int num = 0; num < (int)strline_s.size(); num++) {
								if (strline_s[num] == '=') {
									strline_s.erase(num);//�ضϴ˺����� ��ȡ��Ŀ��
									break;//����ѭ��
								}
							}
							if (strline_s == item) {
								fp.clear();
								fp.seekg(0, ios::beg);//ָ���ƶ�����ͷ

								return COM_BE;//��Ŀ����
							}
						}
						
					}//end of while
				}
			}
		}
	}

	return 0;//��ʱ����
}

/***************************************************************************
  �������ƣ��޲ι���
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������б�Ҫ�����������������
***************************************************************************/
CFT::CFT()
{
	filename = "";
}

/***************************************************************************
  �������ƣ��鹹����
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������б�Ҫ�����������������
***************************************************************************/
CFT::~CFT()
{
	if (fp.is_open())
		fp.close();
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ʒ�ʽ��
  ���������
  �� �� ֵ��
  ˵    �������ι��죬����ͬopen
***************************************************************************/
CFT::CFT(const char* cfgname, int opt)
{
	/*��ֻ����ʽ���ļ� �򲻿���ʧ��*/
	if (opt == OPEN_OPT_RDONLY) {
		fp.open(cfgname, ios::in | ios::binary);//�Զ�����ֻ����ʽ���ļ�
		if (!fp) {//�����ʧ��
			filename = "";
			open_mode = OPEN_ERROR;//��ʧ��
		}
		else {
			filename = cfgname;
			open_mode = OPEN_OPT_RDONLY;//ֻ����ʽ��
		}
	}
	/*�Զ�д��ʽ���ļ� �򲻿��򴴽�*/
	else if (opt == OPEN_OPT_RDWR) {
		fp.open(cfgname, ios::out | ios::in | ios::binary);//�Զ����ƶ�д��ʽ���ļ�
		if (!fp) {//�����ʧ��
			fp.open(cfgname, ios::out | ios::binary);//����д��ʽ�򿪣��൱�ڴ���һ���ļ���
			if (!fp) {
				filename = "";//��ʾ��ʧ��
				open_mode = OPEN_ERROR;//��ʧ��
			}
			else
				fp.close();//�ر��ļ�
			fp.open(cfgname, ios::out | ios::in | ios::binary);//���Զ�д��ʽ��
			if (!fp) {
				filename = "";//��ʾ��ʧ��
				open_mode = OPEN_ERROR;//��ʧ��
			}
			else {
				filename = cfgname;
				open_mode = OPEN_OPT_RDWR;//��д��ʽ��
			}
		}
		else {
			filename = cfgname;
			open_mode = OPEN_OPT_RDWR;//��д��ʽ��
		}
	}
	/*���벻����Ҫ��*/
	else {
		filename = "";
		open_mode = OPEN_ERROR;//��ʧ��
	}


}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ʒ�ʽ��
  ���������
  �� �� ֵ��
  ˵    ����filename=""��ʾ�ļ���ʧ��
***************************************************************************/
void CFT::open(const char* cfgname, int opt)
{
	/*��ֻ����ʽ���ļ� �򲻿���ʧ��*/
	if (opt == OPEN_OPT_RDONLY) {
		fp.open(cfgname, ios::in | ios::binary);//�Զ�����ֻ����ʽ���ļ�
		if (!fp) {//�����ʧ��
			filename = "";
			open_mode = OPEN_ERROR;//��ʧ��
		}
		else {
			filename = cfgname;
			open_mode = OPEN_OPT_RDONLY;//ֻ����ʽ��
		}
	}
	/*�Զ�д��ʽ���ļ� �򲻿��򴴽�*/
	else if (opt == OPEN_OPT_RDWR) {
		fp.open(cfgname, ios::out | ios::in | ios::binary );//�Զ����ƶ�д��ʽ���ļ�
		if (!fp) {//�����ʧ��
			fp.open(cfgname, ios::out | ios::binary );//����д��ʽ�򿪣��൱�ڴ���һ���ļ���
			if (!fp) {
				filename = "";//��ʾ��ʧ��
				open_mode = OPEN_ERROR;//��ʧ��
			}
			else
				fp.close();//�ر��ļ�
			fp.open(cfgname, ios::out | ios::in | ios::binary);//���Զ�д��ʽ��
			if (!fp) {
				filename = "";//��ʾ��ʧ��
				open_mode = OPEN_ERROR;//��ʧ��
			}
			else {
				filename = cfgname;
				open_mode = OPEN_OPT_RDWR;//��д��ʽ��
			}
		}
		else {
			filename = cfgname;
			open_mode = OPEN_OPT_RDWR;//��д��ʽ��
		}
	}
	/*���벻����Ҫ��*/
	else {
		filename = "";
		open_mode = OPEN_ERROR;//��ʧ��
	}

	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �����򿪳ɹ�����true�����򷵻�false
***************************************************************************/
bool CFT::is_open(void)
{
	if (filename == "" || open_mode == OPEN_ERROR || open_mode == OPEN_DOWN)
		return false;
	else
		return true;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �����ر��ļ�
***************************************************************************/
void CFT::close()
{
	if (filename != "") {
		fp.close();//�ر��ļ�
		open_mode = OPEN_DOWN;//����Ϊ�ļ��ر�
		/*������һ�ε��ļ�����*/
	}
	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����C++17����filesystem::resize_fileֱ�ӿ����޸��ļ���С��֮ǰ�İ汾��
                   ��˱�����������C�ľɷ���
	    ��ʵ�֣���Ҫ��
***************************************************************************/
int CFT::file_resize(int newsize)
{
	/* �����С�Ǹ�����ֱ�ӷ���-1 */
	if (newsize < 0)
		return -1;

	if (!fp.good())
		fp.clear();

	/* ȡfp��ǰָ��λ�� */
	int now_pos = int(fp.tellg());

	/* ���ȣ�C++��ʽ�ر� */
	this->close();

	/* ��Σ�C��ʽ�򿪲��ı��ļ���С */
	FILE* fp_cstyle;
	fp_cstyle = fopen(filename.c_str(), "rb+"); //rw��ʽ��
	if (fp_cstyle) {
		_chsize(_fileno(fp_cstyle), newsize);
		fclose(fp_cstyle);
	}
	else
		cout << "open [" << filename << "] for resize failed" << endl;

	/* �ٴΣ�C++��ʽ���´� */
	fp.open(filename, ios::in | ios::out | ios::binary);
	if (fp.is_open()==0) {
		cout << "reopen [" << filename << "] after resize failed" << endl;
		return -1;
	}
	open_mode = OPEN_OPT_RDWR;//�����ļ�״ֵ̬

	/* �ļ�ָ���ƶ���ԭλ�� */
	fp.seekg(now_pos < newsize ? now_pos:newsize, ios::beg);

	return 0;
}

/* ---------------------------------------------------------------
	 ����AAT���Զ����Ա������ʵ�֣�protected��
   ---------------------------------------------------------------- */


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����������
            getline��ȡʱ�������з���ֹͣ ���������з�����������
 ***************************************************************************/
int CFT::group_add(const char* group_name)
{
	/*���Զ�д��ʽ���ļ�ʱ*/
	if (open_mode == OPEN_OPT_RDWR) {
		string standard = group_name ;
		standard = '[' + standard + ']';
		string get = "";
		fp.seekp(0, ios::beg);//��ָ���Ƶ���ʼλ��
		while (1) {//ÿ��
			char line[LINE_MAX] = { '\0' };
			fp.getline(line, LINE_MAX, '\n');
			if (fp.eof()) {
				fp.clear();
				break;
			}
			int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
			if (line[line_length] == '\r')
				line[line_length] = '\0';
			string strline = line;
			int line_mode = deal_line(strline);
			if (line_mode == GROUP_LINE) {
				if (standard == strline)//���ڸ���
					return 0;
			}
		}
		fp.seekp(0, ios::end);//ָ���ƶ����ļ�ĩβ
		fp << standard << "\r\n";//ÿ���һ��
		fp.seekp(0, ios::beg);//ָ���ƶ�����ʼ��
	}	

	return 1; 
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������"��"Ϊ��λɾ��
***************************************************************************/
int CFT::group_del(const char* group_name)
{
	int cut_num = 0;//ɾ��������
	/*���Զ�д��ʽ��ʱ*/
	if (open_mode == OPEN_OPT_RDWR) {

		fp.seekp(0, ios::beg);            //�ļ��ƶ����ļ���ʼλ��
		int size_of_file = 0;             //��¼��Ҫ�ض�����֮ǰ���ļ���С
		string stan = group_name ;        //������Ҫ�жϵ�����   
		stan = '[' + stan + ']';

		/*���е�һ�α���*/
		while (1) {
			char line[LINE_MAX] = { '\0' };
			fp.getline(line, LINE_MAX, '\n');
			if (fp.eof())
				break;
			else
				size_of_file = size_of_file + strlen(line) + 1;
			int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
			if (line[line_length] == '\r')
				line[line_length] = '\0';
			string strline = line;
			int line_mode = deal_line(strline);
			if (line_mode == GROUP_LINE) {
				if (stan == strline) {
					cut_num++;
					size_of_file = size_of_file - (line_length + 1 + 1);
					break;//�ҵ�ɾ����
				}
			}
		}

		//��������β��δ��ȡɾ����
		if (fp.eof()) {//�ļ�����
			fp.clear();
			fp.seekp(0, ios::beg);//ָ���ƶ�����ʼ��
			return cut_num;
		}

		/*��ȡ����ɾ����Ŀ*/
		string data = "";
		if (cut_num == 1) {
			//ɾ�����������
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				//����getline���
				if (fp.eof())
					break;
				string original = line;
				int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
				if (line[line_length] == '\r') 
					line[line_length] = '\0';
				string line_str = line;
				int line_mode = deal_line(line_str);
				if (line_mode==GROUP_LINE) {
					if (line_str != stan) {
						data = data + original + '\n';
						break;//����whileѭ��������
					}
					else
						cut_num++;//���������Ϊɾ����
				}
			}

			if (fp.eof()) {//�ļ�����
				fp.clear();
				file_resize(size_of_file);//�ض��ļ�
				fp.seekp(0, ios::beg);//ָ���ƶ�����ͷ
				fp.clear();
				return cut_num;
			}

			//����������ȡ�˺�����
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				//����getline���
				if (fp.eof())
					break;
				string original = line;
				int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string line_str = line;
				int line_mode = deal_line(line_str);
				if (line_mode==GROUP_LINE) {
					if (line_str != stan)
						data = data + original + '\n';
					else {//��Ϊɾ����
						cut_num++;
						while (1) {//����ɾ��ƥ����
							char line_next[LINE_MAX] = { '\0' };
							fp.getline(line_next, LINE_MAX, '\n');
							//����getline���
							if (fp.eof())
								break;
							string original_next = line_next;
							int line_length_next = strlen(line_next) - 1;//����windows��linux֮��Ĳ���\r
							if (line_next[line_length_next] == '\r')
								line_next[line_length_next] = '\0';
							string line_str_next = line_next;
							int line_mode_next = deal_line(line_str_next);
							if (line_mode_next==GROUP_LINE) {
								if (line_str_next != stan) {
									data = data + original_next + '\n';
									break;//����small whileѭ��������
								}
								else
									cut_num++;
							}
						}//end of small while
					}
				}
				else//��Ϊ�ļ���
					data = data + original + '\n';
			}//end of big while

			if (fp.eof()) {//�ļ�����
				fp.clear();
				file_resize(size_of_file);//�ض��ļ�
				fp.seekp(0, ios::end);//ָ���ƶ�����β��
				fp << data;
				fp.seekp(0, ios::beg);//ָ���ƶ�����ʼ��
				return cut_num;
			}
		}

	}
	return cut_num; //�˾������Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������int��
            ע�⣺�ļ�getlineʱ�������з�ֹͣ ���������з�
***************************************************************************/
int CFT::item_add(const char* group_name, const char* item_name, const int item_value)
{
	/*�Զ�д��ʽ��*/
	if (open_mode == OPEN_OPT_RDWR) {
		//�������ļ�
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			fp.clear();
			if (get == SIM_FALSE || get == SIM_BE) {//�ļ����ڻ�Ϊ��ͨ�����ļ�
				fp.seekp(0, ios::beg);
				return 0;
			}

			fp.seekp(0, ios::end);//ָ���ƶ�����ĩ
			fp << item_name << '=' << item_value << "\r\n";

			fp.clear();
			fp.seekp(0, ios::beg);//�ļ�ָ���ƶ�����ͷ
			return 1;
		}
		//��ͨ�����ļ�
		else {
			int size_of_file = 0;
			string group = group_name;
			group = '[' + group + ']';
			string add = "";
			string data = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_BE || get == COM_GROUP_NONE)//�ļ����ڻ��鲻����
				return 0;//δ���
            //��ʼ���
			fp.seekp(0, ios::beg);//ָ��ص���ʼλ��
			while (1) {//ÿ��
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof()) 
					break;
				else
					size_of_file += strlen(line) + 1;
				int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
				if (line[line_length] == '\r') 
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);//��ȡһ�е���Ч����
				if (state == GROUP_LINE) {
					if (strline == group) {
						ostringstream out;
						out << item_name << '=' << item_value << "\r\n";
						add = out.str();
						break;//����whileѭ��������
					}
				}
			}//end of while

			//�˺���Ϣ�������
			if (!fp.eof())//δ����β��
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;//�ļ���β
					string strline = line;
					data += strline + '\n';
				}

			//���и���
			fp.clear();
			fp.seekp(0, ios::beg);
			file_resize(size_of_file);
			fp.seekp(0, ios::end);//ָ���ƶ�������λ��
			fp << add;
			fp << data;
			fp.seekp(0, ios::beg);//ָ���ƶ�����ʼλ��

			return 1;
		}
	}

	return -1; //��ʱ����
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������double��
***************************************************************************/
int CFT::item_add(const char* group_name, const char* item_name, const double item_value)
{
	/*�Զ�д��ʽ��*/
	if (open_mode == OPEN_OPT_RDWR) {
		//�������ļ�
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			fp.clear();
			if (get == SIM_FALSE || get == SIM_BE) {//�ļ����ڻ�Ϊ��ͨ�����ļ�
				fp.seekp(0, ios::beg);
				return 0;
			}

			fp.seekp(0, ios::end);//ָ���ƶ�����ĩ
			fp << item_name << '=' << setiosflags(ios::fixed) << setprecision(6) << item_value << "\r\n";//����λС����ӡ

			fp.clear();
			fp.seekp(0, ios::beg);//�ļ�ָ���ƶ�����ͷ
			return 1;
		}
		//��ͨ�����ļ�
		else {
			int size_of_file = 0;
			string group = group_name;
			group = '[' + group + ']';
			string add = "";
			string data = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_BE || get == COM_GROUP_NONE)//�ļ����ڻ��鲻����
				return 0;//δ���
			//��ʼ���
			fp.seekp(0, ios::beg);//ָ��ص���ʼλ��
			while (1) {//ÿ��
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				else
					size_of_file += strlen(line) + 1;
				int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);//��ȡһ�е���Ч����
				if (state == GROUP_LINE) {
					if (strline == group) {
						ostringstream out;
						out << item_name << '=' << setiosflags(ios::fixed) << setprecision(6) << item_value << "\r\n";//����λС����ӡ
						add = out.str();
						break;//����whileѭ��������
					}
				}
			}//end of while

			//�˺���Ϣ�������
			if (!fp.eof())//δ����β��
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;//�ļ���β
					string strline = line;
					data += strline + '\n';
				}

			//���и���
			fp.clear();
			fp.seekp(0, ios::beg);
			file_resize(size_of_file);
			fp.seekp(0, ios::end);//ָ���ƶ�������λ��
			fp << add;
			fp << data;
			fp.seekp(0, ios::beg);//ָ���ƶ�����ʼλ��

			return 1;
		}
	}

	return -1; //��ʱ����
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������char������
***************************************************************************/
int CFT::item_add(const char* group_name, const char* item_name, const char *item_value)
{
	/*�Զ�д��ʽ��*/
	if (open_mode == OPEN_OPT_RDWR) {
		//�������ļ�
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			fp.clear();
			if (get == SIM_FALSE || get == SIM_BE) {//�ļ����ڻ�Ϊ��ͨ�����ļ�
				fp.seekp(0, ios::beg);
				return 0;
			}

			fp.seekp(0, ios::end);//ָ���ƶ�����ĩ
			fp << item_name << '=' << item_value << "\r\n";

			fp.clear();
			fp.seekp(0, ios::beg);//�ļ�ָ���ƶ�����ͷ
			return 1;
		}
		//��ͨ�����ļ�
		else {
			int size_of_file = 0;
			string group = group_name;
			group = '[' + group + ']';
			string add = "";
			string data = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_BE || get == COM_GROUP_NONE)//�ļ����ڻ��鲻����
				return 0;//δ���
			//��ʼ���
			fp.seekp(0, ios::beg);//ָ��ص���ʼλ��
			while (1) {//ÿ��
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				else
					size_of_file += strlen(line) + 1;
				int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);//��ȡһ�е���Ч����
				if (state == GROUP_LINE) {
					if (strline == group) {
						ostringstream out;
						out << item_name << '=' << item_value << "\r\n";
						add = out.str();
						break;//����whileѭ��������
					}
				}
			}//end of while

			//�˺���Ϣ�������
			if (!fp.eof())//δ����β��
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;//�ļ���β
					string strline = line;
					data += strline + '\n';
				}

			//���и���
			fp.clear();
			fp.seekp(0, ios::beg);
			file_resize(size_of_file);
			fp.seekp(0, ios::end);//ָ���ƶ�������λ��
			fp << add;
			fp << data;
			fp.seekp(0, ios::beg);//ָ���ƶ�����ʼλ��

			return 1;
		}
	}

	return -1; //��ʱ����
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������string��
***************************************************************************/
int CFT::item_add(const char* group_name, const char* item_name, const string &item_value)
{
	/*�Զ�д��ʽ��*/
	if (open_mode == OPEN_OPT_RDWR) {
		//�������ļ�
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			fp.clear();
			if (get == SIM_FALSE || get == SIM_BE) {//�ļ����ڻ�Ϊ��ͨ�����ļ�
				fp.seekp(0, ios::beg);
				return 0;
			}

			fp.seekp(0, ios::end);//ָ���ƶ�����ĩ
			fp << item_name << '=' << item_value << "\r\n";

			fp.clear();
			fp.seekp(0, ios::beg);//�ļ�ָ���ƶ�����ͷ
			return 1;
		}
		//��ͨ�����ļ�
		else {
			int size_of_file = 0;
			string group = group_name;
			group = '[' + group + ']';
			string add = "";
			string data = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_BE || get == COM_GROUP_NONE)//�ļ����ڻ��鲻����
				return 0;//δ���
			//��ʼ���
			fp.seekp(0, ios::beg);//ָ��ص���ʼλ��
			while (1) {//ÿ��
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				else
					size_of_file += strlen(line) + 1;
				int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);//��ȡһ�е���Ч����
				if (state == GROUP_LINE) {
					if (strline == group) {
						ostringstream out;
						out << item_name << '=' << item_value << "\r\n";
						add = out.str();
						break;//����whileѭ��������
					}
				}
			}//end of while

			//�˺���Ϣ�������
			if (!fp.eof())//δ����β��
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;//�ļ���β
					string strline = line;
					data += strline + '\n';
				}

			//���и���
			fp.clear();
			fp.seekp(0, ios::beg);
			file_resize(size_of_file);
			fp.seekp(0, ios::end);//ָ���ƶ�������λ��
			fp << add;
			fp << data;
			fp.seekp(0, ios::beg);//ָ���ƶ�����ʼλ��

			return 1;
		}
	}

	return -1; //��ʱ����
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������char��
***************************************************************************/
int CFT::item_add(const char* group_name, const char* item_name, const char item_value)
{
	/*�Զ�д��ʽ��*/
	if (open_mode == OPEN_OPT_RDWR) {
		//�������ļ�
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			fp.clear();
			if (get == SIM_FALSE || get == SIM_BE) {//�ļ����ڻ�Ϊ��ͨ�����ļ�
				fp.seekp(0, ios::beg);
				return 0;
			}

			fp.seekp(0, ios::end);//ָ���ƶ�����ĩ
			fp << item_name << '=' << item_value << "\r\n";

			fp.clear();
			fp.seekp(0, ios::beg);//�ļ�ָ���ƶ�����ͷ
			return 1;
		}
		//��ͨ�����ļ�
		else {
			int size_of_file = 0;
			string group = group_name;
			group = '[' + group + ']';
			string add = "";
			string data = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_BE || get == COM_GROUP_NONE)//�ļ����ڻ��鲻����
				return 0;//δ���
			//��ʼ���
			fp.seekp(0, ios::beg);//ָ��ص���ʼλ��
			while (1) {//ÿ��
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				else
					size_of_file += strlen(line) + 1;
				int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);//��ȡһ�е���Ч����
				if (state == GROUP_LINE) {
					if (strline == group) {
						ostringstream out;
						out << item_name << '=' << item_value << "\r\n";
						add = out.str();
						break;//����whileѭ��������
					}
				}
			}//end of while

			//�˺���Ϣ�������
			if (!fp.eof())//δ����β��
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;//�ļ���β
					string strline = line;
					data += strline + '\n';
				}

			//���и���
			fp.clear();
			fp.seekp(0, ios::beg);
			file_resize(size_of_file);
			fp.seekp(0, ios::end);//ָ���ƶ�������λ��
			fp << add;
			fp << data;
			fp.seekp(0, ios::beg);//ָ���ƶ�����ʼλ��

			return 1;
		}
	}

	return -1; //��ʱ����
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ӿ���
***************************************************************************/
int CFT::item_add(const char* group_name, const char* item_name)
{
	/*�Զ�д��ʽ��*/
	if (open_mode == OPEN_OPT_RDWR) {
		//�������ļ�
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			fp.clear();
			if (get == SIM_FALSE || get == SIM_BE) {//�ļ����ڻ�Ϊ��ͨ�����ļ�
				fp.seekp(0, ios::beg);
				return 0;
			}

			fp.seekp(0, ios::end);//ָ���ƶ�����ĩ
			fp << item_name << '=' << "\r\n";

			fp.clear();
			fp.seekp(0, ios::beg);//�ļ�ָ���ƶ�����ͷ
			return 1;
		}
		//��ͨ�����ļ�
		else {
			int size_of_file = 0;
			string group = group_name;
			group = '[' + group + ']';
			string add = "";
			string data = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_BE || get == COM_GROUP_NONE)//�ļ����ڻ��鲻����
				return 0;//δ���
			//��ʼ���
			fp.seekp(0, ios::beg);//ָ��ص���ʼλ��
			while (1) {//ÿ��
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				else
					size_of_file += strlen(line) + 1;
				int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);//��ȡһ�е���Ч����
				if (state == GROUP_LINE) {
					if (strline == group) {
						ostringstream out;
						out << item_name << '=' << "\r\n";
						add = out.str();
						break;//����whileѭ��������
					}
				}
			}//end of while

			//�˺���Ϣ�������
			if (!fp.eof())//δ����β��
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;//�ļ���β
					string strline = line;
					data += strline + '\n';
				}

			//���и���
			fp.clear();
			fp.seekp(0, ios::beg);
			file_resize(size_of_file);
			fp.seekp(0, ios::end);//ָ���ƶ�������λ��
			fp << add;
			fp << data;
			fp.seekp(0, ios::beg);//ָ���ƶ�����ʼλ��

			return 1;
		}
	}

	return -1; //��ʱ����
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����ɾ����
***************************************************************************/
int CFT::item_del(const char* group_name, const char* item_name)
{
	/*�Զ�д��ʽ��*/
	if (open_mode == OPEN_OPT_RDWR) {

		int size_of_file = 0;
		int cut_num = 0;
		fp.clear();
		fp.seekp(0, ios::beg);

		//�������ļ�
		if (group_name == NULL) {
			string data = "";
			const string item = item_name;
			int get = seek_exist(group_name, item_name);
			if (get == SIM_NONE)//�޿�ɾ����
				return cut_num;
			else{
				//�ҵ���һ��λ�õĽضϴ�
				fp.clear();
				fp.seekp(0, ios::beg);
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					else
						size_of_file += strlen(line) + 1;
					int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
					if (line[line_length] == '\r')
						line[line_length] = '\0';
					string strline = line;
					int state = deal_line(strline);
					if (state == PROJECT_LINE) {
						for(int i=0;i< (int)strline.size();i++)//����ȡ��Ŀ��
							if (strline[i] == '=') {
								strline.erase(i);//����˺�����
								break;//����ѭ��
							}
						if (strline == item) {
							size_of_file = size_of_file - (line_length + 1 + 1);
							cut_num++;
							break;//����whileѭ��������
						}
					}
				}//end of while
				//δ�ҵ���ɾ����
				if (fp.eof()) {
					fp.clear();
					fp.seekp(0, ios::beg);
					return cut_num;
				}
				//��ʼ��ȡ����
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;//����whleѭ��������
					string original = line;
					int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
					if (line[line_length] == '\r')
						line[line_length] = '\0';
					string strline = line;
					int state = deal_line(strline);

					if (state == PROJECT_LINE) {
						for (int i = 0; i < (int)strline.size(); i++)//����ȡ��Ŀ��
							if (strline[i] == '=') {
								strline.erase(i);//����˺�����
								break;
							}
						if (strline == item) {//��Ⱦ�ɾ��
							cut_num++;
							continue;
						}
					}
					data = data + original + '\n';
				}//end of while

				//��ʼɾ��
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);
				fp << data;
				fp.seekp(0, ios::beg);//ָ���ƶ����ļ�ͷ

				return cut_num;
			}
		}
		//��ͨ�����ļ�
		else {
			string data = "";
			string group =  group_name ;
			group = '[' + group + ']';
			string item = item_name;
			int get = seek_exist(group_name, item_name);
            //�鲻���ڻ�����������
			if (get == COM_GROUP_NONE || get == COM_FILE_NONE)
				return cut_num;
		    //�����Ŀ������
			else {

				//�ҵ���һ��λ�õĽضϴ�
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
					int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
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
								int line_length_twice = strlen(line_twice) - 1;//����windows��linux֮��Ĳ���\r
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
									if (strline_twice == item) {//�ҵ���ͬ��
										size_of_file = size_of_file - (line_length_twice + 1 + 1);
										cut_num++;
										break;//����samll whileѭ��������
									}
								}
							}//end of small while
							break;//����big whileѭ��������
						}
					}
				}//end of big while

				if (fp.eof())
					fp.clear();

				//��ʼ������ȡ ������Ϣ
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string orginal = line;
					int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
					if (line[line_length] == '\r')
						line[line_length] = '\0';
					string strline = line;
					int state = deal_line(strline);
					if (state == GROUP_LINE) {
						data = data + orginal + '\n';
						break;//����whileѭ��������
					}
					else if (state == PROJECT_LINE) {
						for (int i = 0; i < (int)strline.size(); i++)
							if (strline[i] == '=') {
								strline.erase(i);
								break;//����small whileѭ��������
							}
						if (strline == item)//������ɾ��
							cut_num++;
						else
							data = data + orginal + '\n';
					}
				}

				//�˺���Ϣ����ɾ��
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;//�ļ���β
					string strline = line;
					data = data + strline + '\n';
				}

				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
                fp.seekp(0, ios::end);//ָ���ƶ�������λ��
				fp << data;
				fp.seekp(0, ios::beg);//ָ���ƶ�����ʼλ��

				return cut_num;
			}
		}
	}

	return -1;//ִ�е��˾��ʾ����
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����������Ŀ
***************************************************************************/
int CFT::item_update(const char* group_name, const char* item_name, const int item_value)
{
	/*�Զ�д��ʽ��*/
	if (open_mode == OPEN_OPT_RDWR) {
		int size_of_file = 0;
		//�������ļ�
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			if (get == SIM_FALSE)//�ļ�����
				return 0;
			else if (get == SIM_NONE) {//��������Ŀ
				fp.clear();
				fp.seekp(0, ios::end);//ָ���Ƶ��ļ�β
				fp << item_name << '=' << item_value << "\r\n";
				fp.seekp(0, ios::beg);//ָ���Ƶ��ļ�ͷ
				return 1;
			}
			else if (get == SIM_BE) {//������Ŀ
				//�ҵ���Ҫ���ĵ���Ŀ
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
					int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
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
							break;//����whileѭ��������
						}	
					}
				}
				//�˺�����ݾ����ı�
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//���и���
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//ָ���ƶ�������λ��
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//ָ���ƶ�����ʼλ��

				return 1;
			}
		}
		//��ͨ�����ļ�
		else {
			string group =  group_name ;
			group = '[' + group + ']';
			string item = item_name;
			string data = "";
			string change = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_GROUP_NONE)//�����ڸ�����
				return 0;
			else if (get == COM_FILE_NONE) {//�����ڸ�����Ŀ
				this->item_add(group_name, item_name, item_value);
				return 1;
			}
			else if(get==COM_BE) {//���ڸ�����Ŀ
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					else
						size_of_file += strlen(line) + 1;
					int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
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
								int line_length_twice = strlen(line_twice) - 1;//����windows��linux֮��Ĳ���\r
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
									if (strline_twice == item) {//�ҵ���ͬ��
										size_of_file = size_of_file - (line_length_twice + 1 + 1);
										ostringstream out;
										out << item_name << '=' << item_value << "\r\n";
										change = out.str();
										break;//����small whileѭ��������
									}
								}
							}//end of small while
							break;//����big whileѭ��������
						}	
					}
				}//end of big while

				//�˺����ݾ�����ı�
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//���и���
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//ָ���ƶ�������λ��
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//ָ���ƶ�����ʼλ��

				return 1;
			}
		}
	}

	return -1; //ִ�е��˾�ʱ��ʾ����
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����������Ŀ
***************************************************************************/
int CFT::item_update(const char* group_name, const char* item_name, const double item_value)
{
	/*�Զ�д��ʽ��*/
	if (open_mode == OPEN_OPT_RDWR) {
		int size_of_file = 0;
		//�������ļ�
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			if (get == SIM_FALSE)//�ļ�����
				return 0;
			else if (get == SIM_NONE) {//��������Ŀ
				fp.clear();
				fp.seekp(0, ios::end);//ָ���Ƶ��ļ�β
				fp << item_name << '=' << setiosflags(ios::fixed) << setprecision(6) << item_value << "\r\n";//����λС����ӡ
				fp.seekp(0, ios::beg);//ָ���Ƶ��ļ�ͷ
				return 1;
			}
			else if (get == SIM_BE) {//������Ŀ
				//�ҵ���Ҫ���ĵ���Ŀ
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
					int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
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
							out << item_name << '=' << setiosflags(ios::fixed) << setprecision(6) << item_value << "\r\n";//����λС����ӡ
							change = out.str();
							break;//����whileѭ��������
						}
					}
				}
				//�˺�����ݾ����ı�
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//���и���
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//ָ���ƶ�������λ��
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//ָ���ƶ�����ʼλ��

				return 1;
			}
		}
		//��ͨ�����ļ�
		else {
			string group = group_name;
			group = '[' + group + ']';
			string item = item_name;
			string data = "";
			string change = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_GROUP_NONE)//�����ڸ�����
				return 0;
			else if (get == COM_FILE_NONE) {//�����ڸ�����Ŀ
				this->item_add(group_name, item_name, item_value);
				return 1;
			}
			else if (get == COM_BE) {//���ڸ�����Ŀ
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					else
						size_of_file += strlen(line) + 1;
					int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
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
								int line_length_twice = strlen(line_twice) - 1;//����windows��linux֮��Ĳ���\r
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
									if (strline_twice == item) {//�ҵ���ͬ��
										size_of_file = size_of_file - (line_length_twice + 1 + 1);
										ostringstream out;
										out << item_name << '=' << setiosflags(ios::fixed) << setprecision(6) << item_value << "\r\n";//����λС����ӡ
										change = out.str();
										break;//����small whileѭ��������
									}
								}
							}//end of small while
							break;//����big whileѭ��������
						}
					}
				}//end of big while

				//�˺����ݾ�����ı�
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//���и���
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//ָ���ƶ�������λ��
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//ָ���ƶ�����ʼλ��

				return 1;
			}
		}
	}

	return -1; //ִ�е��˾�ʱ��ʾ����
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����������Ŀ
***************************************************************************/
int CFT::item_update(const char* group_name, const char* item_name, const char* item_value)
{
	/*�Զ�д��ʽ��*/
	if (open_mode == OPEN_OPT_RDWR) {
		int size_of_file = 0;
		//�������ļ�
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			if (get == SIM_FALSE)//�ļ�����
				return 0;
			else if (get == SIM_NONE) {//��������Ŀ
				fp.clear();
				fp.seekp(0, ios::end);//ָ���Ƶ��ļ�β
				fp << item_name << '=' << item_value << "\r\n";
				fp.seekp(0, ios::beg);//ָ���Ƶ��ļ�ͷ
				return 1;
			}
			else if (get == SIM_BE) {//������Ŀ
				//�ҵ���Ҫ���ĵ���Ŀ
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
					int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
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
							break;//����whileѭ��������
						}
					}
				}
				//�˺�����ݾ����ı�
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//���и���
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//ָ���ƶ�������λ��
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//ָ���ƶ�����ʼλ��

				return 1;
			}
		}
		//��ͨ�����ļ�
		else {
			string group = group_name;
			group = '[' + group + ']';
			string item = item_name;
			string data = "";
			string change = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_GROUP_NONE)//�����ڸ�����
				return 0;
			else if (get == COM_FILE_NONE) {//�����ڸ�����Ŀ
				this->item_add(group_name, item_name, item_value);
				return 1;
			}
			else if (get == COM_BE) {//���ڸ�����Ŀ
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					else
						size_of_file += strlen(line) + 1;
					int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
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
								int line_length_twice = strlen(line_twice) - 1;//����windows��linux֮��Ĳ���\r
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
									if (strline_twice == item) {//�ҵ���ͬ��
										size_of_file = size_of_file - (line_length_twice + 1 + 1);
										ostringstream out;
										out << item_name << '=' << item_value << "\r\n";
										change = out.str();
										break;//����small whileѭ��������
									}
								}
							}//end of small while
							break;//����big whileѭ��������
						}
					}
				}//end of big while

				//�˺����ݾ�����ı�
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//���и���
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//ָ���ƶ�������λ��
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//ָ���ƶ�����ʼλ��

				return 1;
			}
		}
	}
	return -1; //ִ�е��˾�ʱ��ʾ����
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����������Ŀ
***************************************************************************/
int CFT::item_update(const char* group_name, const char* item_name, const string &item_value)
{
	/*�Զ�д��ʽ��*/
	if (open_mode == OPEN_OPT_RDWR) {
		int size_of_file = 0;
		//�������ļ�
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			if (get == SIM_FALSE)//�ļ�����
				return 0;
			else if (get == SIM_NONE) {//��������Ŀ
				fp.clear();
				fp.seekp(0, ios::end);//ָ���Ƶ��ļ�β
				fp << item_name << '=' << item_value << "\r\n";
				fp.seekp(0, ios::beg);//ָ���Ƶ��ļ�ͷ
				return 1;
			}
			else if (get == SIM_BE) {//������Ŀ
				//�ҵ���Ҫ���ĵ���Ŀ
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
					int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
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
							break;//����whileѭ��������
						}
					}
				}
				//�˺�����ݾ����ı�
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//���и���
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//ָ���ƶ�������λ��
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//ָ���ƶ�����ʼλ��

				return 1;
			}
		}
		//��ͨ�����ļ�
		else {
			string group = group_name;
			group = '[' + group + ']';
			string item = item_name;
			string data = "";
			string change = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_GROUP_NONE)//�����ڸ�����
				return 0;
			else if (get == COM_FILE_NONE) {//�����ڸ�����Ŀ
				this->item_add(group_name, item_name, item_value);
				return 1;
			}
			else if (get == COM_BE) {//���ڸ�����Ŀ
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					else
						size_of_file += strlen(line) + 1;
					int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
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
								int line_length_twice = strlen(line_twice) - 1;//����windows��linux֮��Ĳ���\r
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
									if (strline_twice == item) {//�ҵ���ͬ��
										size_of_file = size_of_file - (line_length_twice + 1 + 1);
										ostringstream out;
										out << item_name << '=' << item_value << "\r\n";
										change = out.str();
										break;//����small whileѭ��������
									}
								}
							}//end of small while
							break;//����big whileѭ��������
						}
					}
				}//end of big while

				//�˺����ݾ�����ı�
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//���и���
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//ָ���ƶ�������λ��
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//ָ���ƶ�����ʼλ��

				return 1;
			}
		}
	}
	return -1; //ִ�е��˾�ʱ��ʾ����
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����������Ŀ
***************************************************************************/
int CFT::item_update(const char* group_name, const char* item_name, const char item_value)
{
	/*�Զ�д��ʽ��*/
	if (open_mode == OPEN_OPT_RDWR) {
		int size_of_file = 0;
		//�������ļ�
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			if (get == SIM_FALSE)//�ļ�����
				return 0;
			else if (get == SIM_NONE) {//��������Ŀ
				fp.clear();
				fp.seekp(0, ios::end);//ָ���Ƶ��ļ�β
				fp << item_name << '=' << item_value << "\r\n";
				fp.seekp(0, ios::beg);//ָ���Ƶ��ļ�ͷ
				return 1;
			}
			else if (get == SIM_BE) {//������Ŀ
				//�ҵ���Ҫ���ĵ���Ŀ
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
					int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
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
							break;//����whileѭ��������
						}
					}
				}
				//�˺�����ݾ����ı�
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//���и���
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//ָ���ƶ�������λ��
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//ָ���ƶ�����ʼλ��

				return 1;
			}
		}
		//��ͨ�����ļ�
		else {
			string group = group_name;
			group = '[' + group + ']';
			string item = item_name;
			string data = "";
			string change = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_GROUP_NONE)//�����ڸ�����
				return 0;
			else if (get == COM_FILE_NONE) {//�����ڸ�����Ŀ
				this->item_add(group_name, item_name, item_value);
				return 1;
			}
			else if (get == COM_BE) {//���ڸ�����Ŀ
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					else
						size_of_file += strlen(line) + 1;
					int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
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
								int line_length_twice = strlen(line_twice) - 1;//����windows��linux֮��Ĳ���\r
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
									if (strline_twice == item) {//�ҵ���ͬ��
										size_of_file = size_of_file - (line_length_twice + 1 + 1);
										ostringstream out;
										out << item_name << '=' << item_value << "\r\n";
										change = out.str();
										break;//����small whileѭ��������
									}
								}
							}//end of small while
							break;//����big whileѭ��������
						}
					}
				}//end of big while

				//�˺����ݾ�����ı�
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//���и���
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//ָ���ƶ�������λ��
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//ָ���ƶ�����ʼλ��

				return 1;
			}
		}
	}
	return -1; //ִ�е��˾�ʱ��ʾ����
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����������Ŀ
***************************************************************************/
int CFT::item_update(const char* group_name, const char* item_name)
{
	/*�Զ�д��ʽ��*/
	if (open_mode == OPEN_OPT_RDWR) {
		int size_of_file = 0;
		//�������ļ�
		if (group_name == NULL) {
			int get = seek_exist(group_name, item_name);
			if (get == SIM_FALSE)//�ļ�����
				return 0;
			else if (get == SIM_NONE) {//��������Ŀ
				fp.clear();
				fp.seekp(0, ios::end);//ָ���Ƶ��ļ�β
				fp << item_name << '=' << "\r\n";
				fp.seekp(0, ios::beg);//ָ���Ƶ��ļ�ͷ
				return 1;
			}
			else if (get == SIM_BE) {//������Ŀ
				//�ҵ���Ҫ���ĵ���Ŀ
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
					int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
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
							break;//����whileѭ��������
						}
					}
				}
				//�˺�����ݾ����ı�
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//���и���
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//ָ���ƶ�������λ��
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//ָ���ƶ�����ʼλ��

				return 1;
			}
		}
		//��ͨ�����ļ�
		else {
			string group = group_name;
			group = '[' + group + ']';
			string item = item_name;
			string data = "";
			string change = "";
			int get = seek_exist(group_name, item_name);
			if (get == COM_GROUP_NONE)//�����ڸ�����
				return 0;
			else if (get == COM_FILE_NONE) {//�����ڸ�����Ŀ
				this->item_add(group_name, item_name);
				return 1;
			}
			else if (get == COM_BE) {//���ڸ�����Ŀ
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					else
						size_of_file += strlen(line) + 1;
					int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
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
								int line_length_twice = strlen(line_twice) - 1;//����windows��linux֮��Ĳ���\r
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
									if (strline_twice == item) {//�ҵ���ͬ��
										size_of_file = size_of_file - (line_length_twice + 1 + 1);
										ostringstream out;
										out << item_name << '=' << "\r\n";
										change = out.str();
										break;//����small whileѭ��������
									}
								}
							}//end of small while
							break;//����big whileѭ��������
						}
					}
				}//end of big while

				//�˺����ݾ�����ı�
				while (1) {
					char line[LINE_MAX] = { '\0' };
					fp.getline(line, LINE_MAX, '\n');
					if (fp.eof())
						break;
					string strline = line;
					data = data + strline + '\n';
				}
				//���и���
				fp.clear();
				fp.seekp(0, ios::beg);
				file_resize(size_of_file);
				fp.seekp(0, ios::end);//ָ���ƶ�������λ��
				fp << change;
				fp << data;
				fp.seekp(0, ios::beg);//ָ���ƶ�����ʼλ��

				return 1;
			}
		}
	}
	return -1; //ִ�е��˾�ʱ��ʾ����
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ȡֵ
***************************************************************************/
int CFT::item_get_value(const char* group_name, const char* item_name, int &item_value)
{
	/*�����߶�д��ʽ���ļ�����*/

	//�������ļ�
	if (group_name == NULL) {
		int get = seek_exist(group_name, item_name);
		if (get == SIM_FALSE || get == SIM_NONE)//��ȡ�����
			return 0;
		else if (get == SIM_BE) {//��ȡ�����
			string item = item_name;
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
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
					if (temp == item) {//�ҵ�����
						for (int i = 0; i < (int)strline.size(); i++)
							if (strline[i] == '=') {
								strline.erase(0, (i + 1));
								break;
							}
						istringstream in(strline);//��ȡ����
						in >> item_value;
						fp.clear();
						fp.seekg(0, ios::beg);

						return 1;//��ȡ���ļ�
					}
				}
			}//end of big while
		}
	}
	//��ͨ�����ļ�
	else {
		int get = seek_exist(group_name, item_name);
		if (get == COM_FILE_NONE || get == COM_GROUP_NONE)//��ȡ�����
			return 0;
		else if (get == COM_BE) {//��ȡ�����
			string group = group_name;
			group = '[' + group + ']';
			string item = item_name;
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);
				if (state == GROUP_LINE) {
					if (strline == group) {//�ҵ�����ʱ
						while (1) {//�ҵ�����ʱ
							char line_next[LINE_MAX] = { '\0' };
							fp.getline(line_next, LINE_MAX, '\n');
							if (fp.eof())
								break;
							int line_length_next = strlen(line_next) - 1;//����windows��linux֮��Ĳ���\r
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
								if (temp == item) {//�ҵ�����
									for (int i = 0; i < (int)strline_next.size(); i++)//����ȡ����
										if (strline_next[i] == '=') {
											strline_next.erase(0, (i + 1));
											break;
										}
									istringstream in(strline_next);//��ȡ����
									in >> item_value;
									fp.clear();
									fp.seekg(0, ios::beg);

									return 1;//��ȡ��ֵ
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


	return -1; //ִ�е��˾�ʱ����
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ȡֵ
***************************************************************************/
int CFT::item_get_value(const char* group_name, const char* item_name, double& item_value)
{
	/*�����߶�д��ʽ���ļ�����*/

	//�������ļ�
	if (group_name == NULL) {
		int get = seek_exist(group_name, item_name);
		if (get == SIM_FALSE || get == SIM_NONE)//��ȡ�����
			return 0;
		else if (get == SIM_BE) {//��ȡ�����
			string item = item_name;
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
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
					if (temp == item) {//�ҵ�����
						for (int i = 0; i < (int)strline.size(); i++)
							if (strline[i] == '=') {
								strline.erase(0, (i + 1));
								break;
							}
						istringstream in(strline);//��ȡ����
						in >> item_value;
						fp.clear();
						fp.seekg(0, ios::beg);

						return 1;//��ȡ���ļ�
					}
				}
			}//end of big while
		}
	}
	//��ͨ�����ļ�
	else {
		int get = seek_exist(group_name, item_name);
		if (get == COM_FILE_NONE || get == COM_GROUP_NONE)//��ȡ�����
			return 0;
		else if (get == COM_BE) {//��ȡ�����
			string group = group_name;
			group = '[' + group + ']';
			string item = item_name;
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);
				if (state == GROUP_LINE) {
					if (strline == group) {//�ҵ�����ʱ
						while (1) {//�ҵ�����ʱ
							char line_next[LINE_MAX] = { '\0' };
							fp.getline(line_next, LINE_MAX, '\n');
							if (fp.eof())
								break;
							int line_length_next = strlen(line_next) - 1;//����windows��linux֮��Ĳ���\r
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
								if (temp == item) {//�ҵ�����
									for (int i = 0; i < (int)strline_next.size(); i++)//����ȡ����
										if (strline_next[i] == '=') {
											strline_next.erase(0, (i + 1));
											break;
										}
									istringstream in(strline_next);//��ȡ����
									in >> item_value;
									fp.clear();
									fp.seekg(0, ios::beg);

									return 1;//��ȡ��ֵ
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


	return -1; //ִ�е��˾�ʱ����
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ȡֵ
***************************************************************************/
int CFT::item_get_value(const char* group_name, const char* item_name, char *item_value)
{
	/*�����߶�д��ʽ���ļ�����*/

	//�������ļ�
	if (group_name == NULL) {
		int get = seek_exist(group_name, item_name);
		if (get == SIM_FALSE || get == SIM_NONE)//��ȡ�����
			return 0;
		else if (get == SIM_BE) {//��ȡ�����
			string item = item_name;
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
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
					if (temp == item) {//�ҵ�����
						for (int i = 0; i < (int)strline.size(); i++)
							if (strline[i] == '=') {
								strline.erase(0, (i + 1));
								break;
							}
						strcpy(item_value, strline.data());
						fp.clear();
						fp.seekg(0, ios::beg);

						return 1;//��ȡ���ļ�
					}
				}
			}//end of big while
		}
	}
	//��ͨ�����ļ�
	else {
		int get = seek_exist(group_name, item_name);
		if (get == COM_FILE_NONE || get == COM_GROUP_NONE)//��ȡ�����
			return 0;
		else if (get == COM_BE) {//��ȡ�����
			string group = group_name;
			group = '[' + group + ']';
			string item = item_name;
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);
				if (state == GROUP_LINE) {
					if (strline == group) {//�ҵ�����ʱ
						while (1) {//�ҵ�����ʱ
							char line_next[LINE_MAX] = { '\0' };
							fp.getline(line_next, LINE_MAX, '\n');
							if (fp.eof())
								break;
							int line_length_next = strlen(line_next) - 1;//����windows��linux֮��Ĳ���\r
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
								if (temp == item) {//�ҵ�����
									for (int i = 0; i < (int)strline_next.size(); i++)//����ȡ����
										if (strline_next[i] == '=') {
											strline_next.erase(0, (i + 1));
											break;
										}
									strcpy(item_value, strline_next.data());
									fp.clear();
									fp.seekg(0, ios::beg);

									return 1;//��ȡ��ֵ
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


	return -1; //ִ�е��˾�ʱ����
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ȡֵ
***************************************************************************/
int CFT::item_get_value(const char* group_name, const char* item_name, string &item_value)
{
	/*�����߶�д��ʽ���ļ�����*/

	//�������ļ�
	if (group_name == NULL) {
		int get = seek_exist(group_name, item_name);
		if (get == SIM_FALSE || get == SIM_NONE)//��ȡ�����
			return 0;
		else if (get == SIM_BE) {//��ȡ�����
			string item = item_name;
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
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
					if (temp == item) {//�ҵ�����
						for (int i = 0; i < (int)strline.size(); i++)
							if (strline[i] == '=') {
								strline.erase(0, (i + 1));
								break;
							}
						item_value = strline;
						fp.clear();
						fp.seekg(0, ios::beg);

						return 1;//��ȡ���ļ�
					}
				}
			}//end of big while
		}
	}
	//��ͨ�����ļ�
	else {
		int get = seek_exist(group_name, item_name);
		if (get == COM_FILE_NONE || get == COM_GROUP_NONE)//��ȡ�����
			return 0;
		else if (get == COM_BE) {//��ȡ�����
			string group = group_name;
			group = '[' + group + ']';
			string item = item_name;
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);
				if (state == GROUP_LINE) {
					if (strline == group) {//�ҵ�����ʱ
						while (1) {//�ҵ�����ʱ
							char line_next[LINE_MAX] = { '\0' };
							fp.getline(line_next, LINE_MAX, '\n');
							if (fp.eof())
								break;
							int line_length_next = strlen(line_next) - 1;//����windows��linux֮��Ĳ���\r
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
								if (temp == item) {//�ҵ�����
									for (int i = 0; i < (int)strline_next.size(); i++)//����ȡ����
										if (strline_next[i] == '=') {
											strline_next.erase(0, (i + 1));
											break;
										}
									item_value = strline_next;
									fp.clear();
									fp.seekg(0, ios::beg);

									return 1;//��ȡ��ֵ
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


	return -1; //ִ�е��˾�ʱ����
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ȡֵ
***************************************************************************/
int CFT::item_get_value(const char* group_name, const char* item_name, char& item_value)
{
	/*�����߶�д��ʽ���ļ�����*/

	//�������ļ�
	if (group_name == NULL) {
		int get = seek_exist(group_name, item_name);
		if (get == SIM_FALSE || get == SIM_NONE)//��ȡ�����
			return 0;
		else if (get == SIM_BE) {//��ȡ�����
			string item = item_name;
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
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
					if (temp == item) {//�ҵ�����
						for (int i = 0; i < (int)strline.size(); i++)
							if (strline[i] == '=') {
								strline.erase(0, (i + 1));
								break;
							}
						istringstream in(strline);//��ȡ����
						in >> item_value;
						fp.clear();
						fp.seekg(0, ios::beg);

						return 1;//��ȡ���ļ�
					}
				}
			}//end of big while
		}
	}
	//��ͨ�����ļ�
	else {
		int get = seek_exist(group_name, item_name);
		if (get == COM_FILE_NONE || get == COM_GROUP_NONE)//��ȡ�����
			return 0;
		else if (get == COM_BE) {//��ȡ�����
			string group = group_name;
			group = '[' + group + ']';
			string item = item_name;
			while (1) {
				char line[LINE_MAX] = { '\0' };
				fp.getline(line, LINE_MAX, '\n');
				if (fp.eof())
					break;
				int line_length = strlen(line) - 1;//����windows��linux֮��Ĳ���\r
				if (line[line_length] == '\r')
					line[line_length] = '\0';
				string strline = line;
				int state = deal_line(strline);
				if (state == GROUP_LINE) {
					if (strline == group) {//�ҵ�����ʱ
						while (1) {//�ҵ�����ʱ
							char line_next[LINE_MAX] = { '\0' };
							fp.getline(line_next, LINE_MAX, '\n');
							if (fp.eof())
								break;
							int line_length_next = strlen(line_next) - 1;//����windows��linux֮��Ĳ���\r
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
								if (temp == item) {//�ҵ�����
									for (int i = 0; i < (int)strline_next.size(); i++)//����ȡ����
										if (strline_next[i] == '=') {
											strline_next.erase(0, (i + 1));
											break;
										}
									istringstream in(strline_next);//��ȡ����
									in >> item_value;
									fp.clear();
									fp.seekg(0, ios::beg);

									return 1;//��ȡ��ֵ
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


	return -1; //ִ�е��˾�ʱ����
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ȡֵ
***************************************************************************/
int CFT::item_get_value(const char* group_name, const char* item_name)
{
	/*�����߶�д��ʽ���ļ�����*/

	//�������ļ�
	if (group_name == NULL) {
		int get = seek_exist(group_name, item_name);
		fp.clear();
		fp.seekg(0, ios::beg);
		if (get == SIM_FALSE || get == SIM_NONE)//��ȡ�����
			return 0;
		else if (get == SIM_BE) //��ȡ�����
			return 1;
	}
	//��ͨ�����ļ�
	else {
		int get = seek_exist(group_name, item_name);
		fp.clear();
		fp.seekg(0, ios::beg);
		if (get == COM_FILE_NONE || get == COM_GROUP_NONE)//��ȡ�����
			return 0;
		else if (get == COM_BE)//��ȡ�����
			return 1;
	}

	return -1; //ִ�е��˾�ʱ����

}

