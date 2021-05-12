#include "TimeRecorder.h"
#include <Psapi.h>
#include <TlHelp32.h>
using namespace std;

TimeRecorder::TimeRecorder() {
	m_frontPID = 0;
	m_nowPID = 0;
}

list<_data>& TimeRecorder::GetProList() {
	return m_runPro;
}

std::string TimeRecorder::RunTime(const _data& target) {
	time_t runTime;
	if (m_nowPID == target.id) {
		runTime = time(NULL) - target.front + target.total;
	}
	else {
		runTime = target.total;
	}
	return TimeToString(runTime);
}

void TimeRecorder::Update() {
	m_nowPID = ForeExePid();
	if (m_nowPID == m_frontPID) {
		return;
	}
	else {
		GetAllProcess();
		// frontPID != nowPID 的情况下，是否在 proList 中找到 nowPID
		int sign = false;
		for (auto i = m_runPro.begin(); i != m_runPro.end();)
		{
			if (i->id == m_frontPID) {
				i->total += GetTime() - i->front;
			}
			else if (i->id == m_nowPID) {
				i->front = GetTime();
				sign = true;
			}
			else if (!IsProcessRunning(i->name)) {
				i = m_runPro.erase(i);
				continue;
			}
			i++;
		}

		if (!sign) {
			_data t;
			if (GetExeName(m_nowPID, t.name)) {
				t.id = m_nowPID;
				t.front = t.begin = GetTime();
				m_runPro.push_back(t);
			}
		}
		m_frontPID = m_nowPID;
	}
}

std::string TimeRecorder::TotalTime() {
	size_t sum = 0;
	for (auto i : m_runPro)
		sum += m_nowPID == i.id ? time(NULL) - i.front + i.total : i.total;
	return TimeToString(sum);
}

void TimeRecorder::GetAllProcess() {
	m_allPro.clear();
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		// error
		return;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32)) {
		// error
		CloseHandle(hProcessSnap);
		return;
	}

	do
	{
		m_allPro.insert(pe32.szExeFile);
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return;
}

bool TimeRecorder::IsProcessRunning(const std::string& name) {
	return m_allPro.find(name) != m_allPro.end();
}

bool TimeRecorder::GetExeName(const DWORD pid, std::string& ret) {
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
	//获取进程路径
	char* path = new char[MAX_PATH];
	bool sign = K32GetModuleFileNameExA(hProcess, NULL, path, MAX_PATH);

	if (sign) {
		std::string exeName = path;
		ret = exeName.substr(exeName.find_last_of('\\') + 1);
	}
	delete[] path;
	return sign;
}

DWORD TimeRecorder::ForeExePid() {
	DWORD pid;
	GetWindowThreadProcessId(GetForegroundWindow(), &pid);
	return pid;
}

time_t TimeRecorder::GetTime() {
	return time(NULL);
}

string TimeRecorder::TimeToString(time_t target) {
	int sec = target % 60;
	target /= 60;
	int min = target % 60;
	target /= 60;
	return to_string(target) + "时" +
		to_string(min) + "分" +
		to_string(sec) + "秒";
}