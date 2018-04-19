#pragma once

#ifndef ssqIncH
#define ssqIncH

#define MEMSET_ std::memset
#define MEMCPY_ std::memcpy
#define STRCPY_ std::strcpy

#define Lotter_TM_STR_LEN (20)

struct TParseCmd
{
	std::vector<std::string> vS;
	TParseCmd() {}
	TParseCmd(const std::string& s) { parse(s); }

	int size() { return vS.size(); }

	static int StrToInt(const std::string& s)
	{
		if (s.empty())return 0;
		if (s.size() < 1) return 0;

		int len = s.length();

		int i = 0;
		int flag = 10;
		while (s[i] == ' ')++i;
		while (s[i] == '0') { ++i; }
		if (s[i] == 'x' || s[i] == 'X')
		{
			++i;
			flag = 16;
		}
		if (len > i)
		{
			char *p = NULL;
			return std::strtol(&(s[i]), &p, flag);
		}

		return 0;

	}

	int parse(const std::string& s)
	{
		vS.clear();

		int len = s.length();
		int i = 0;
		do
		{
			while ((s[i] == ' ' || s[i] == '\t') && i < len)++i;
			if (i >= len) return 0;

			int j = i;
			while (j < len && (s[j] != ' ' && s[j] != '\t'))++j;
			vS.push_back(s.substr(i, j - i));

			i = j;
		} while (i < len);

		return vS.size();
	}

private:
	std::string s;
};

struct TLotter3Stru_
{
	static int SSQSumAvg;

	char tmstr[Lotter_TM_STR_LEN];        ///
	unsigned int seqno; ///
	int  a[10]; ///   

	int  p_[10];

	int o1; ///
	int e1; ///

	char stroe[16]; ///


	int  iSectiona_[3];  ///
	char secranger[16]; ///

	int  sum1;
	char strSum1[16];

	int  infosame1;
	char infoDir1[8];


	int the1stOE;
	int theLastOe;

	///int  iLinkCnt;
	int  iLinkA[3 + 1];
	char linkStr[6];
	char str1[16];
	char infostr1[64];
	char str2[16];
	char infostr2[64];
	int flag_;
	int itmp_; ///

	std::string c_next_infostr2_;
	std::string c_infostr2_;

	TLotter3Stru_(int inFlag = 0) ///
	{
		reset();
	}

	TLotter3Stru_(TParseCmd& parseCmd)
	{
		reset();

		if (parseCmd.vS.size() > 1)  ///loadFromFile
		{
			std::memcpy(tmstr, parseCmd.vS[1].c_str(), parseCmd.vS[1].size()); ///idtmstr = parseCmd.vS[1];
			const char* p = parseCmd.vS[2].c_str();

		}

		calc_();
	}

	void calc_()
	{
		///std::sprintf(idstr, "%02d%02d%02d%02d%02d%02d%02d%02d%02d%02d\0", i1, i2, i3, i4, i5, i6, i7, i8, i9, i10);
	}


	int chkO_(TLotter3Stru_& rhv, const int inChkNextFlag = 0);



	void reset()
	{
		///i1 = i2 = i3 = i4 = i5 = i6 = j1 = 0;
		///e1 = o1 = sum1 = 0;
		const int isize1_ = offsetof(TLotter3Stru_, itmp_) - offsetof(TLotter3Stru_, tmstr);
		MEMSET_(tmstr, 0, isize1_);

		flag_ = 1; ///

		c_infostr2_ = "";

	}


	const char* get_c_infostr2_()
	{

		return c_infostr2_.c_str();

	}


};

#define LS_CHK_SIZE (101)
#define LS_RECCHK_MAX_SIZE (10000)
struct TLsFlagStru_
{
	int flaga[10][20];
	int flaga2[10][20]; ///

	int flagsum2a[10][20]; ///
	int flagsum2a2[10][20]; ///
	int p_[20];

	int sumOEa[3][5][20]; /// 
	int sumNumA[3][5][20]; ///


};

extern int ParseLotter3Txt(const char s[1024], TLotter3Stru_& inlt);
extern int ParseLotter3XMLRom(const char s[1024], TLotter3Stru_& inlt);
extern int ParseLotter3XMLRom(const std::string& s, TLotter3Stru_& inlt);
extern int ParseLotter3XMLRom2_(const std::string& s, TLotter3Stru_& inlt);



#endif



///std::map<std::string, TLotter3Stru_> mapS2SSQStru;


//unsigned int minLsseqno, maxLsseqno;    ///



bool  GenDataLsStru(const DWORD inbegin);
bool  GetLsDataStruNum(const bool inAddEdtFlag = 0);
bool  GetLsDataStruOE(const bool inAddEdtFlag = 0);
bool  GetLsDataStruSumNum(const int inflag);   ///相邻2个(也可以任意2个)和值的大小和奇偶
bool  GetLsDataStruSumOE(const int inflag);
bool  GenParseLsData(int x);
//bool bDataRefresh; 

//void  SaveHisDate();
bool  btnLoadDate(const std::string& strHisDataFile);


int get_longest();
void get_need_shoot(int sum);
void text_incersion();
void get_shoot();
int dataProcess(std::string filename, int temp);