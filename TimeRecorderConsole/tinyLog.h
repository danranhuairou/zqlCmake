#ifndef TINYLOG_H
#define TINYLOG_H

#include <string>
#include <fstream>

class TinyLog {
private:
	struct HeadLess {
		TinyLog* log;
		HeadLess& operator<<(const std::string str);
	};

public:
	TinyLog(std::string direction, int maxCount = 30);
	HeadLess& operator<<(const std::string);

private:
	int m_maxCount;				// log �ļ���������
	std::string m_direction;	// ���� log �ļ��Ĵ洢λ�� 
	std::string m_fileName;		// log �ļ�������·����

	void ClearFile();			// ɾ������ m_maxCount ��log�ļ�

	inline bool FileIsLog(const char* name);		// �ж��ļ��Ƿ����log�ļ���������ʽ
	inline std::string TwoIntToStr(int target);		//��λ����ת�ַ�����0����ǰ������
	//std::string FileName(std::string dir);
};
#endif // !TINYLOG_H
