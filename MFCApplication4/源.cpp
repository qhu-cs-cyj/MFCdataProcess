#include "Դ.h"
#include <iostream>
#include <vector> 
#include <string> 
#include <fstream> 
#include <iostream> 
using namespace std;

vector <string> datas;

//�õ�data�����ļ����ܳ��ȣ������û�����ʱ��ҪԽ��
int get_longest() {
	ifstream myfile(".\\data.txt");
	bool judge = true;
	if (!myfile.is_open())
	{
		judge = false;
		cout << "�޷���data.txt�����ļ�����" << endl;
	}
	string temp;
	while (judge && (getline(myfile, temp)))
	{
		datas.push_back(temp);
	}
	myfile.close();
	return datas.size() - 102;//ǰ101���Ʋ��������һ��Ҳ�Ʋ���
}

//�����û���Ҫ������������ԭdata�н�ȡ��Ӧ�����������ʼ��
void get_need_shoot(int sum) {
	ofstream file_need_shoot(".\\need_shoot.txt", ios::trunc);//ʹ��trunc��ÿ��д��֮ǰ��ɾ��ԭ�ļ�
	for (int i = datas.size() - sum; i < datas.size(); i++)
	{
		file_need_shoot << datas[i];
		file_need_shoot << endl;
	}
	file_need_shoot.close();
}

//��Ϊֻ�ܴӺ���ǰ�㣬���Եõ���tempҪ���з�ת,��ת�󱣴�Ϊwant.txt
void text_incersion() {
	vector<string> nums;
	ifstream myfile(".\\temp.txt");
	ofstream outfile(".\\want.txt", ios::trunc);
	bool judge = true;
	if (!myfile.is_open())
	{
		cout << "δ�ɹ����ļ�" << endl;
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

//��want.txt��need_shoot.txt��������
void get_shoot() {
	ifstream need_shoot(".\\need_shoot.txt");
	ifstream myfile(".\\want.txt");
	ofstream shooted(".\\been_shoot.txt", ios::trunc);
	string temp1, temp2;
	bool judge = true;
	if (!need_shoot.is_open() && !myfile.is_open())
	{
		cout << "δ�ɹ����ļ�aaa" << endl;
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
	//cout << "��Ҫ�ܶ����У�";
	//cin >> temp;
	GenParseLsData(temp);

	get_longest();
	get_need_shoot(temp);
	text_incersion();
	get_shoot();
	datas.clear();
}