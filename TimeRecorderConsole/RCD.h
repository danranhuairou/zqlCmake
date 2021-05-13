#ifndef _REC_H_
#define _REC_H_
#include <string>
#include <ctime>
#include <vector>
#include <codecvt>

namespace RCD
{
	using _string = std::wstring;
	using _time = time_t;
	using _size = unsigned long long;

	struct _sqlData
	{
		_size sqlId;
		_string name;
		_string exeName;
		_time totalTime;
	};

	struct _appData
	{
		_size pid;
		_time begin, front, total;
		_sqlData sqlData;
		bool inSql;
		// if insql is running?
		bool running;
	};
	using _listSQL = std::vector<_sqlData>;
	using _listApp = std::vector<_appData>;



	inline std::string UnicodeToUTF8(const std::wstring& wstr)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> wcv;
		return wcv.to_bytes(wstr);
	}

	inline std::wstring UTF8ToUnicode(const std::string& str)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> wcv;
		return wcv.from_bytes(str);
	}
}
#endif // !_REC_H_
