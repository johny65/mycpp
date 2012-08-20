#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <sstream>

using namespace std;

namespace pdfboo_utils {

	string* split(const string &s);
	int str2int(const string &s);
	string escape(const string &utf8);

}

#endif