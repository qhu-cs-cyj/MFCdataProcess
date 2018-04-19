#include "源.h"
#include <iostream>
#include <vector> 
#include <string> 
#include <fstream> 
#include <iostream> 
using namespace std;

vector <string> datas;

//得到data数据文件的总长度，告诉用户输入时不要越界
int get_longest() {
	ifstream myfile(".\\data.txt");
	bool judge = true;
	if (!myfile.is_open())
	{
		judge = false;
		cout << "无法打开data.txt数据文件！！" << endl;
	}
	string temp;
	while (judge && (getline(myfile, temp)))
	{
		datas.push_back(temp);
	}
	myfile.close();
	return datas.size() - 102;//前101个推不到，最后一个也推不到
}

//根据用户需要的行数，来从原data中截取对应行数（从最后开始）
void get_need_shoot(int sum) {
	ofstream file_need_shoot(".\\need_shoot.txt", ios::trunc);//使用trunc，每次写入之前先删除原文件
	for (int i = datas.size() - sum; i < datas.size(); i++)
	{
		file_need_shoot << datas[i];
		file_need_shoot << endl;
	}
	file_need_shoot.close();
}

//因为只能从后往前算，所以得到的temp要进行反转,反转后保存为want.txt
void text_incersion() {
	vector<string> nums;
	ifstream myfile(".\\temp.txt");
	ofstream outfile(".\\want.txt", ios::trunc);
	bool judge = true;
	if (!myfile.is_open())
	{
		cout << "未成功打开文件" << endl;
		judge = false;
	}
	string temp1;
	while (judge && getline(myfile, temp1))
	{
		nums.push_back(temp1);
	}

	for (int i = nums.size() - 1; i >= 0; i--)
	{
		outfile << nums[i];
		outfile << endl;
	}

	myfile.close();
	outfile.close();
}

//将want.txt和need_shoot.txt进行命中
void get_shoot() {
	ifstream need_shoot(".\\need_shoot.txt");
	ifstream myfile(".\\want.txt");
	ofstream shooted(".\\been_shoot.txt", ios::trunc);
	string temp1, temp2;
	bool judge = true;
	if (!need_shoot.is_open() && !myfile.is_open())
	{
		cout << "未成功打开文件aaa" << endl;
		judge = false;
	}
	while (judge && getline(need_shoot, temp1) && getline(myfile, temp2))
	{
		int s1 = 0;
		for (int i = temp1.size() - 29, j = 0; j < 10; i = i + 3, j++)
		{
			if ((((temp1[i] - 48) * 10 + (temp1[i + 1] - 48)) % 2) == (temp2[j] - 48))
				s1 = s1 + 10;
		}
		shooted << s1;
		shooted << " ";
	}
	need_shoot.close();
	myfile.close();
	shooted.close();
}


void main1(string filename, int temp)
{
	//btnLoadDate("data.txt");
	btnLoadDate(filename);
	//int temp;
	//cout << "你要跑多少行：";
	//cin >> temp;
	GenParseLsData(temp);

	get_longest();
	get_need_shoot(temp);
	text_incersion();
	get_shoot();
	datas.clear();
}