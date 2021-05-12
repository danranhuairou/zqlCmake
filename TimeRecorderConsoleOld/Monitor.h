#ifndef MONITOR_H
#define MONITOR_H
#include <map>
#include <tuple>
#include <string>
#include <ctime>
#include <set>
#include <Windows.h>
#include <vector>
#include "sql/tinyLog.h"

namespace Monitor {

	enum class _mode {
		none = 0,		// 不在数据库中的前台程序，也要记录信息
		fore = 100,
		back = 200
	};

	// 列表示a，行表示b
	// a > b return 1
	// a < b return -1
	// a == b return 0
	//         none    fore    back
	// none	    0	    1	    -1
	// fore	    -1	    0	    -1
	// back	    1	    1	    0
	int compareMode(_mode& a, _mode& b);

	// 这两个结构体必须用构造函数构造，避免不安全的使用
	struct _sqlData {
		_sqlData(_mode a_mode) {
			mode = a_mode;
			id = 0;
		}

		_mode mode;
		std::string name;	// 用户自定义的名字，而不是exeName
		unsigned long long id;
		//time_t total;		// 其他信息
	};

	struct _data {

		_data(std::string a_exeName,bool a_isRun, _mode a_mode)
			:data(a_mode) {
			exeName = a_exeName;
			isRun = a_isRun;

			begin = time(nullptr);
			totalTime = 0;
		}
		std::string exeName;

		bool isRun;			//是否正在运行
		time_t begin;		//如果正在运行，其开始时间
		// 前台程序 特有属性
		time_t totalTime;	//前台程序的实际运行时间，结束时间为，begin + totalTime * n, n为几毫秒运行一次update函数

		_sqlData data;
	};

	extern std::map<std::string, _data> m_appData;
	// 修改map为list，便于排序，设置排序规则枚举，设置自定义的查找函数
	// 设置排除程序名单
	// 从数据库读取信息到m_appData
	void SetExclude(std::set<std::string>& exclude);
	// 应定时调用此程序，以记录程序使用时间，调用越频繁记录越准确
	void Update(unsigned);
	// 把所有记录录入数据库，程序终止了
	void end();
	// 初始化
	void Init();
}
#endif // !MONITOR_H