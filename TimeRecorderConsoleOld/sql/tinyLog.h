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
	int m_maxCount;				// log �ļ���������
	std::string m_direction;	// ���� log �ļ��Ĵ洢λ�� 
	std::string m_fileName;		// log �ļ�������·����

	void ClearFile();			// ɾ������ m_maxCount ��log�ļ�
};
#endif // !TINYLOG_H
