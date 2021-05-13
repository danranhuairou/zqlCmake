#include "tinyLog.h"
#include <ctime>
#include <direct.h>
#include <io.h>
#include <set>

TinyLog::TinyLog(std::string direction, int maxCount)
	:m_direction(direction), m_maxCount(maxCount)
{
	(void)_mkdir(m_direction.c_str());
	ClearFile();

	// fileName
	time_t t = time(nullptr);
	tm date;
	localtime_s(&date, &t);
	m_fileName = m_direction + '/' + std::to_string(date.tm_year + 1900) + '_' +
		TwoIntToStr(date.tm_mon + 1) + '_' + TwoIntToStr(date.tm_mday) + ".txt";
}

void TinyLog::ClearFile() {
	// 把所有文件加入 fileSet
	std::set<std::string> fileSet;

    struct _finddata_t c_file;
    intptr_t hFile;
    // Find first .txt file in log
    if ((hFile = _findfirst((m_direction + "/*.txt").c_str(), &c_file)) != -1L)
    {
        do {
			if (FileIsLog(c_file.name))
				fileSet.insert(c_file.name);
		} while (_findnext(hFile, &c_file) == 0);
        _findclose(hFile);
    }

	// 删除 fileSet 中多余的日志文件
	// 因为set是升序排列，故第m_maxCount个后面即为要删除的
	if (fileSet.size() > m_maxCount) {
		auto it = fileSet.begin();
		for (int i = 0; it != fileSet.end() && i < m_maxCount; i++)
			it++;
		for (; it != fileSet.end(); it++)
			remove((m_direction + '/' + *it).c_str());
	}
}

TinyLog::HeadLess& TinyLog::operator<<(const std::string str) {
	bool sign = false;

	//判断文件最后一个字符是否是\n 不是则设置sign = true 以添加\n
	std::ifstream ifile(m_fileName.c_str());
	ifile.seekg(-1, std::ios::end);
	char c = -1;
	ifile >> c;
	// c == -1表示是文件为空
	if (c != -1 && c != '\n')
		sign = true;
	ifile.close();

	time_t t = time(nullptr);
	tm date;
	localtime_s(&date, &t);
	std::string dateString = 
		std::to_string(date.tm_year + 1900) + "年" +
		TwoIntToStr(date.tm_mon + 1) + "月" +
		TwoIntToStr(date.tm_mday) + "日" +
		TwoIntToStr(date.tm_hour) + "分" +
		TwoIntToStr(date.tm_sec) + "秒";
	
	std::string put = std::string(sign ? std::string("\n") : std::string("")) + "[ " + dateString + " ]\t" + str;
	static HeadLess headLess({ this });
	return headLess << put;
}

TinyLog::HeadLess& TinyLog::HeadLess::operator<<(const std::string str) {
	std::ofstream file(log->m_fileName.c_str(), std::ios::app);
	file << str;
	file.close();
	return *this;
}

bool TinyLog::FileIsLog(const char* name) {
	// name 规则
	// 数字 * 4 _ 数字 * 2 _ 数字 * 2 .txt \0
	// where is digit
	int a[] = { 0,1,2,3,5,6,8,9 };
	for (int i = 0; i < 8; i++)
		if (!isdigit(name[a[i]]))
			return false;
	if (name[4] != '_' || name[7] != '_')
		return false;
	if (name[10] != '.' || name[11] != 't' || name[12] != 'x' || name[13] != 't' || name[14] != '\0')
		return false;
	return true;
}

std::string TinyLog::TwoIntToStr(int target) {
	if (target < 10) return '0' + std::to_string(target);
	else return std::to_string(target);
}

//std::string TinyLog::FileName(std::string dir) {
//	time_t t = time(nullptr);
//	tm date;
//	localtime_s(&date, &t);
//	return dir + '/' + std::to_string(date.tm_year + 1900) + '_' +
//		TwoIntToStr(date.tm_mon + 1) + '_' + TwoIntToStr(date.tm_mday) + ".txt";
//}