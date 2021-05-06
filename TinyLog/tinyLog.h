#ifndef TINYLOG_H
#define TINYLOG_H

#include <string>
#include <fstream>

class TinyLog {
private:
	struct HeadLess {
		TinyLog* log;
		HeadLess& operator<<(const std::string& str);
	};

public:
	TinyLog(std::string direction, int maxCount = 30);
	HeadLess& operator<<(const std::string&);

private:
	int m_maxCount;				// log 文件最多的数量
	std::string m_direction;	// 所有 log 文件的存储位置 
	std::string m_fileName;		// log 文件的完整路径名

	void ClearFile();			// 删除多于 m_maxCount 的log文件
};
#endif // !TINYLOG_H
