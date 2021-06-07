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
	log << "123123123\n" << "asd" << "阿松大\n";
}

// 生成 n 次随机文件，实际并不能生成100个文件，测试日志类的排序删除功能
void TestMain() {
	const int n = 100;
	vector<string>fileName;
	for (int i = 0; i < n; i++) {
		string str = DIRECTION "/" + RandomName();
		fileName.push_back(str);
		// 新建文件
		ofstream file(str);
		file.close();
	}
	//cout << "已成功生成 " << n << " 次随机文件\n";
	//sort(fileName.begin(), fileName.end());
	//Log log(DIRECTION);
}

inline std::string TwoIntToStr(int target) {
	if (target < 10) return '0' + std::to_string(target);
	else return std::to_string(target);
}

// 生成2021年的十二个月，每个月三十一天，的随机文件名
inline string RandomName() {
	return "2021_" +
		TwoIntToStr(rand() % 12 + 1) + "_"
		+ TwoIntToStr(rand() % 31 + 1) + ".txt";
}

#endif // 0