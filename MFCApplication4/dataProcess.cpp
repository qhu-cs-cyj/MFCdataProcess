//#pragma hdrstop
#include "stdafx.h"
#include "dataProcess.h"

//
//#pragma link "CSPIN"
//#pragma link "SHDocVw_OCX"
//#pragma resource "*.dfm"
int lsflagidx;


int ParseLotter3Txt(const char s[1024], TLotter3Stru_& inlt)
{
	int i = 0;
	if (s[i] == '\0') return 0;

	while (!isdigit(s[i])) { ++i;  if (s[i] == '\0') return 0; }
	int j = i + 1;
	while (isdigit(s[j])) { ++j;   if (s[i] == '\0') return 0; }

	if ((j - i) <= 4) return 0; ///

	char a_[20] = { 0 };
	memcpy(a_, s + i, j - i);
	char *p = NULL;
	inlt.seqno = std::strtol(a_, &p, 10);

	int pos_ = 0; ///
	int flag[11] = { 0 };
	do
	{
		i = j + 1;
		while (!isdigit(s[i]))++i;
		j = i + 1;
		while (isdigit(s[j]))++j;
		char c_ = s[j - 1];

		if (!isdigit(c_)) return 0; ///

		inlt.a[pos_] = (isdigit(c_) ? (c_ == '0' ? 10 : c_ - '0') : 0);

		if (flag[inlt.a[pos_]]) return 0;
		else flag[inlt.a[pos_]] = 1;

		pos_ += 1; ///

		if (s[i] == '\0' || s[i] == '\n') break;
	} while (pos_ < 10);

	return (pos_ >= 10);
}

int ParseLotter3XMLRom2_(const std::string& s, TLotter3Stru_& inlt)
{
	if (s.empty())return 0;
	if (s.size() < 1) return 0;

	size_t pos = s.find("<row");
	if (pos == std::string::npos) return 0;

	pos = s.find("expect");
	if (pos == std::string::npos) return 0;

	while (!isdigit(s[pos])) pos += 1;
	char *p = NULL;
	inlt.seqno = std::strtol(s.substr(pos).c_str(), &p, 10);

	size_t pos2 = s.find("opencode");
	if (pos2 == std::string::npos) return 0;
	while (!isdigit(s[pos2])) pos2 += 1;

	int pos_ = 0; ///
	char c_ = 0;
	do
	{
		if (s[pos2] == ' ') { ++pos2; continue; }

		if ((s[pos2] == ',' || s[pos2] == '\"') && pos_ < 10)
		{
			int t = (c_ ? (c_ == '0' ? 10 : c_ - '0') : 0);
			if (t == 0) t = 10;
			inlt.a[pos_++] = t; ///(c_ ? (c_ == '0' ? 10 : c_ - '0') : 0);
			c_ = 0; ///
		}
		else
		{
			c_ = s[pos2]; ///
		}
		++pos2;

		if (s[pos2] == '\"' && pos_ >= 10) break;

	} while (pos_ < 10);

	pos = s.find("opentime");
	if (pos != std::string::npos)
	{
		while (!isdigit(s[pos])) pos += 1;
		int j = pos + 1;
		while (s[j] != '\"')++j;
		memcpy(inlt.tmstr, s.substr(pos, j - pos).c_str(), j - pos);
	}

	return 1;

}


int ParseLotter3XMLRom(const std::string& s, TLotter3Stru_& inlt)
{
	if (s.empty())return 0;
	if (s.size() < 1) return 0;

	///int len = s.length();

	int i = 0;
	while (s[i] == ' ')++i;
	const char* p_ = s.c_str();
	if (memcmp(p_ + i, "<row", 4)) return 0;
	i += 4;

	while (s[i] != 'e' && s[i] != 'o')++i;

	int count_ = 0;
	while (count_ < 3)
	{
		if (memcmp(p_ + i, "expect", 6) == 0)
		{
			count_ += 1;

			i += 6;
			while (s[i] != '\"')++i;
			int j = i + 1;
			while (s[j] != '\"')++j;
			char a_[20] = { 0 };
			memcpy(a_, p_ + i + 1, j - i);

			i = j + 1; ///

			char *p = NULL;
			inlt.seqno = std::strtol(a_, &p, 10);
		}
		else if (memcmp(p_ + i, "opencode", 8) == 0)
		{
			count_ += 1;

			i += 8;
			while (s[i] != '\"')++i;
			i += 1; ///
			int pos_ = 0; ///
			char c_ = 0;
			do
			{
				if (s[i] == ' ') { ++i; continue; }

				if ((s[i] == ',' || s[i] == '\"') && pos_ < 10)
				{

					inlt.a[pos_++] = (c_ ? (c_ == '0' ? 10 : c_ - '0') : 0);
					c_ = 0; ///

				}
				else
				{
					c_ = s[i]; ///
				}
				++i;

				if (s[i] == '\"' && pos_ >= 10) break;

			} while (pos_ < 10);
		}
		else if (memcmp(p_ + i, "opentime", 8) == 0)
		{
			count_ += 1;

			i += 8;
			while (s[i] != '\"')++i;
			int j = i + 1;
			while (s[j] != '\"')++j;
			memcpy(inlt.tmstr, p_ + i + 1, j - i);

			i = j + 1;
		}
		else
		{
			i += 1;
		}
	}

	return 1;

}

int TLotter3Stru_::chkO_(TLotter3Stru_& rhv, const int inChkNextFlag)
{
	return 1;
}

int TLotter3Stru_::SSQSumAvg = 102;





std::map<unsigned int, TLotter3Stru_> mapI2LsStru;
std::map<unsigned int, int> mapI2I_; ///
std::vector<TLotter3Stru_> vGenDataLsStru;
std::vector<TLsFlagStru_>  vLsFlagStru;

bool GetLsDataStruNum(int inAddEdtFlag)
{
	if (vGenDataLsStru.size() > LS_RECCHK_MAX_SIZE)
	{
		int lsflagcnt = 0;
		const int stepa[9] = { 0, 1, 1, 2, 3, 5, 8, 13, 21 }; ///
		const int isize_ = vGenDataLsStru.size();
		lsflagidx = 0;
		for (int iloop = 0; iloop < LS_CHK_SIZE; ++iloop)
		{
			///for (int irow = 0; irow < 10; ++irow)
			{
				lsflagidx = 0;
				for (int istep = 2; istep < 9; ++istep, ++lsflagidx)
				{
					for (int i = isize_ - 1 - iloop - inAddEdtFlag; i >= 0; i -= stepa[istep])
					{
						TLotter3Stru_& ls_ = vGenDataLsStru[i];

						{
							for (int i1 = 0; i1 < 10; ++i1)
							{
								vLsFlagStru[iloop].flaga2[i1][lsflagidx] = ((ls_.a[i1] >= 6) ? 1 : 0); ///
							}
						}

						if (++lsflagcnt > LS_CHK_SIZE)
						{
							lsflagcnt = 0;
							break;
						}
					}
				}
			}
		}

		/*
		if (inAddEdtFlag)
		{
		for (int i = 0; i < LS_CHK_SIZE; ++i)
		{
		TLsFlagStru_& lsflag_ = vLsFlagStru[i];
		char buf_[1024] = { 0 }; int idx_ = 0;
		for (int i1 = 0; i1 < (10); ++i1)
		{
		for (int i2 = 0; i2 < lsflagidx; ++i2)
		{
		//std::sprintf(&(buf_[idx_]), "%1d ", lsflag_.flaga2[i1][i2]);
		idx_ += 2; ///
		}
		buf_[idx_++] = ',';
		buf_[idx_++] = ' ';
		}
		//redt2->Lines->Append(buf_);
		}
		}
		*/
		return true;
	}
	return 0;
}

bool GetLsDataStruOE(int inAddEdtFlag)
{
	std::cout << inAddEdtFlag << std::endl;
	if (vGenDataLsStru.size() > LS_RECCHK_MAX_SIZE)
	{
		int lsflagcnt = 0;
		const int stepa[9] = { 0, 1, 1, 2, 3, 5, 8, 13, 21 }; ///
		const int isize_ = vGenDataLsStru.size();
		lsflagidx = 0;
		for (int iloop = 0; iloop < LS_CHK_SIZE; ++iloop)
		{
			///for (int irow = 0; irow < 10; ++irow)
			{
				lsflagidx = 0;
				for (int istep = 2; istep < 9; ++istep, ++lsflagidx)
				{
					for (int i = isize_ - 1 - iloop - inAddEdtFlag; i >= 0; i -= stepa[istep])
					{
						TLotter3Stru_& ls_ = vGenDataLsStru[i];

						{
							for (int i1 = 0; i1 < 10; ++i1)
							{
								vLsFlagStru[iloop].flaga[i1][lsflagidx] = ((ls_.a[i1] & 1) ? 1 : 0); ///
							}
						}

						if (++lsflagcnt > LS_CHK_SIZE)
						{
							lsflagcnt = 0;
							break;
						}
					}
				}
			}
		}
		/*
		if (inAddEdtFlag)
		{
		for (int i = 0; i < LS_CHK_SIZE; ++i)
		{
		TLsFlagStru_& lsflag_ = vLsFlagStru[i];
		char buf_[1024] = { 0 }; int idx_ = 0;
		for (int i1 = 0; i1 < 10; ++i1)
		{
		for (int i2 = 0; i2 < lsflagidx; ++i2)
		{
		//std::sprintf(&(buf_[idx_]), "%1d ", lsflag_.flaga[i1][i2]);
		idx_ += 2; ///
		}

		buf_[idx_++] = ',';
		buf_[idx_++] = ' ';

		}
		//redt2->Lines->Append(buf_);
		}

		*/

		return true;
	}

	return 0;
}

bool GenDataLsStru(const DWORD inbegin)
{
	vGenDataLsStru.clear();
	vGenDataLsStru.reserve(mapI2LsStru.size());
	vLsFlagStru.clear();  lsflagidx = 0;
	vLsFlagStru.resize(LS_CHK_SIZE * 2);
	for (std::map<unsigned int, TLotter3Stru_>::iterator it = (inbegin ? mapI2LsStru.find(inbegin) : mapI2LsStru.begin()); it != mapI2LsStru.end(); ++it)
	{
		TLotter3Stru_& ls = it->second;
		vGenDataLsStru.push_back(ls); ///(it->second);
	}
	return true;
}

static int P_lastNumA[11][11] = { { 0 } };
bool GenParseLsData(int sum)
{
	std::ofstream outfile(".\\temp.txt", std::ios::trunc);
	for (int x = 1; x < sum + 1; x++)
	{
		GetLsDataStruOE(x);

		GetLsDataStruNum(x);

		std::memset(P_lastNumA, 0, sizeof(P_lastNumA));

		int flaga[10][10] = { 0 }; ///

		int oea[10][20] = { 0 };
		int numa[10][20] = { 0 };
		for (int i = 0; i < LS_CHK_SIZE; ++i)
		{
			TLsFlagStru_& lsflag_ = vLsFlagStru[i];

			for (int i1 = 0; i1 < 10; ++i1)
			{
				for (int i2 = 0; i2 < lsflagidx; ++i2)
				{
					oea[i1][i2] += (lsflag_.flaga[i1][i2] ? 1 : 0);

					numa[i1][i2] += (lsflag_.flaga2[i1][i2] ? 1 : 0);
				}
			}
		}

		///
		const DWORD max1 = mapI2LsStru.rbegin()->first;
		//redt2->Lines->Add(IntToStr(max1 + 1) + " : ");

		for (int i1 = 0; i1 < 10; ++i1)
		{
			int  c1_ = 0, c2_ = 0;
			for (int i2 = 0; i2 < lsflagidx; ++i2)
			{
				c1_ += ((oea[i1][i2] > LS_CHK_SIZE / 2) ? 1 : 0);

				c2_ += ((numa[i1][i2] > LS_CHK_SIZE / 2) ? 1 : 0);
			}
			flaga[i1][0] = (c1_ * 2 > lsflagidx);
			flaga[i1][1] = (c2_ * 2 > lsflagidx);

			char buf_[256] = { 0 };
			if (flaga[i1][0])
			{
				//std::cout << 0;
				outfile << 0;
			}
			else
			{
				//std::cout << 1;
				outfile << 1;
			}

		}
		//redt2->Lines->Add("\n");

		outfile << std::endl;
	}
	outfile.close();
	return true;

	/*
	for (int i1 = 0; i1 < 10; ++i1)
	{
	char buf_[1024] = { 0 };
	int  idx_ = 0;
	int  c_ = 0;
	for (int i2 = 0; i2 < lsflagidx; ++i2)
	{
	//			std::sprintf(buf_ + idx_, "(%02d %02d), ", oea[i1][i2], (LS_CHK_SIZE - oea[i1][i2]));
	idx_ += 9;
	c_ += ((oea[i1][i2] >= LS_CHK_SIZE / 2) ? 1 : 0);
	}
	//		std::sprintf(buf_ + idx_, "    (%02d %02d)  - %d", c_, (lsflagidx - c_), c_ * 2>lsflagidx);
	//redt2->Lines->Add(buf_);
	}

	//redt2->Lines->Add("\n\n");

	for (int i1 = 0; i1 < 10; ++i1)
	{
	char buf_[1024] = { 0 };
	int  idx_ = 0;
	int  c_ = 0;
	for (int i2 = 0; i2 < lsflagidx; ++i2)
	{
	//			std::sprintf(buf_ + idx_, "(%02d %02d), ", numa[i1][i2], (LS_CHK_SIZE - numa[i1][i2]));
	idx_ += 9;
	c_ += ((numa[i1][i2] >= LS_CHK_SIZE / 2) ? 1 : 0);
	}
	//		std::sprintf(buf_ + idx_, "    (%02d %02d)  - %d", c_, (lsflagidx - c_), c_ * 2>lsflagidx);
	//redt2->Lines->Add(buf_);
	}

	return true;
	*/
}

void SaveHisDate()
{
	std::ofstream ofs("hisdata.dat");

	if (ofs)
	{
		for (std::map<unsigned int, TLotter3Stru_>::iterator it = mapI2LsStru.begin(); it != mapI2LsStru.end(); ++it)
		{
			TLotter3Stru_& ls = it->second;

			char buf_[1024] = { 0 };
			//			std::sprintf(buf_, "%u  %02d %02d %02d %02d %02d %02d %02d %02d %02d %02d\n",
			//			ls.seqno, ls.a[0], ls.a[1], ls.a[2], ls.a[3], ls.a[4], ls.a[5], ls.a[6], ls.a[7], ls.a[8], ls.a[9]);

			ofs << buf_;
		}

		ofs.flush();
		ofs.close();
	}
}

bool btnLoadDate(const std::string& strHisDataFile)
{
	///const static AnsiString strHisDataFile = curPath + "hisData.dat";
	std::ifstream ifs(strHisDataFile.c_str());
	if (ifs)
	{
		int cnt = 0;
		char cmd[1024] = { 0 };
		while (ifs.getline(cmd, sizeof(cmd), '\n'))
		{
			TLotter3Stru_ ls;
			///if (cmd[0] == 'B' && cmd[1] == 'E' && cmd[2] == 'G' && cmd[3] == 'I' && cmd[4] == 'N') continue;
			///if (cmd[0] == 'E' && cmd[1] == 'N' && cmd[2] == 'D') break;

			if (ParseLotter3Txt(cmd, ls))
			{
				mapI2LsStru[ls.seqno] = ls;
			}
		}

		DWORD min1 = mapI2LsStru.begin()->first;
		DWORD max1 = mapI2LsStru.rbegin()->first;
		DWORD last_ = min1;
		if (mapI2LsStru.size() != (max1 - min1 + 1))
		{
			std::string s;
			DWORD t_ = 0;
			for (std::map<unsigned int, TLotter3Stru_>::iterator it = mapI2LsStru.begin(); it != mapI2LsStru.end(); ++it)
			{
				if (t_ == 0) t_ = it->first;
				else
				{
					if ((t_ + 1) != it->first)
					{
						for (int j = (t_ + 1), cnt = 0; j < it->first; ++j, ++cnt)
						{
							s += std::to_string(j).c_str();
							s += ","; ///"\t";
							if (cnt >= 16) { s += "\n"; cnt = 0; }
						}
						last_ = it->first;
					}
					t_ = it->first;
				}
			}

			char buf_[1024] = { 0 };
			///std::sprintf(buf_, "共 %u 条记录，最大值 %u ,最小值 %u, 中间缺小了 %d 个: %s", mapI2LsStru.size(), max1, min1, (max1 - min1 + 1) - mapI2LsStru.size(), s.c_str());
			//			std::sprintf(buf_, "共 %u 条记录，最大值 %u ,最小值 %u, 中间缺小了 %d 个: ", mapI2LsStru.size(), max1, min1, (max1 - min1 + 1) - mapI2LsStru.size());
			std::string s1 = buf_;
			s1 += s;
			//StatusBar1->SimpleText = s1.c_str(); ///String(buf_) + String(s.c_str()); ///
			//ShowMessage(s1.c_str());
		}
		else
		{
			char buf_[1024] = { 0 };
			//			std::sprintf(buf_, "共 %u 条记录，最大值 %u ,最小值 %u", mapI2LsStru.size(), max1, min1);
			//StatusBar1->SimpleText = String(buf_); ///ShowMessage(buf_);
		}

		GenDataLsStru(last_);

	}
	return FALSE;
}

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


int dataProcess(string filename, int temp)
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
	return 0;
}