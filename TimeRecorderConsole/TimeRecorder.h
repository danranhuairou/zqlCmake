#include <list>
#include <string>
#include <Windows.h>
#include <set>
#include <ctime>

struct _data {
	size_t id;
	std::string name;
	time_t begin;
	time_t front;
	time_t total;

	_data() {
		id = 0;
		begin = 0;
		front = 0;
		total = 0;
	}
};

class TimeRecorder {
private:
	std::list<_data>m_runPro;
	std::set<std::string>m_allPro;
	DWORD m_frontPID, m_nowPID;

	void GetAllProcess();

	inline bool IsProcessRunning(const std::string& name);
	inline bool GetExeName(const DWORD pid, std::string& ret);
	inline DWORD ForeExePid();
	inline time_t GetTime();
	inline std::string TimeToString(time_t target);

public:
	TimeRecorder();
	void Update();
	std::list<_data>& GetProList();
	std::string RunTime(const _data& target);
	std::string TotalTime();
};