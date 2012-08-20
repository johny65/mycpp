#include "utils.h"

int pdfboo_utils::str2int(const string &s)
{
	stringstream ss(s);
	int res;
	ss>>res;
	return res;
}

string* pdfboo_utils::split(const string &s)
{
	#define NUM 4
	string *res = new string[NUM];
	int pos = 0, ind = 0;

	for (int i=0; i<s.length(); ++i){
		if (s[i] == ';'){
			res[ind] = s.substr(pos, i-pos);
			pos = i + 1;
			ind++;
		}
	}
	res[ind] = s.substr(pos);

	return res;
}

string pdfboo_utils::escape(const string &utf8)
{
	string res;
	for (int i=0; i<utf8.size(); ++i){
		if (utf8.at(i) == '\\')
			res += "\\5C";
		else if (utf8.at(i) == ';')
			res += "\\3B";
		else
			res += utf8.at(i);
	}
	return res;
}
