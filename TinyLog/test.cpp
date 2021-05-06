#if 1

#include "tinyLog.h"
#include <vector>
#include <iostream>
using namespace std;

//#define DIRECTION "C:/Home/GitProject/zqlCmake/myLog/log"
#define DIRECTION "log"

void TestMain();
inline std::string TwoIntToStr(int target);
inline string RandomName();

int main()
{
	TinyLog log(DIRECTION);
	log << "123123123\n" << "asd" << "���ɴ�\n";
}

// ���� n ������ļ���ʵ�ʲ���������100���ļ���������־�������ɾ������
void TestMain() {
	const int n = 100;
	vector<string>fileName;
	for (int i = 0; i < n; i++) {
		string str = DIRECTION "/" + RandomName();
		fileName.push_back(str);
		// �½��ļ�
		ofstream file(str);
		file.close();
	}
	//cout << "�ѳɹ����� " << n << " ������ļ�\n";
	//sort(fileName.begin(), fileName.end());
	//Log log(DIRECTION);
}

inline std::string TwoIntToStr(int target) {
	if (target < 10) return '0' + std::to_string(target);
	else return std::to_string(target);
}

// ����2021���ʮ�����£�ÿ������ʮһ�죬������ļ���
inline string RandomName() {
	return "2021_" +
		TwoIntToStr(rand() % 12 + 1) + "_"
		+ TwoIntToStr(rand() % 31 + 1) + ".txt";
}

#endif // 0