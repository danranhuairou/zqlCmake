#include "Monitor.h"
#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <algorithm>
#include <iostream>
using namespace Monitor;

std::map<std::string, Monitor::_data> Monitor::m_appData;		// 记录所有的的进程信息

using _setStr = std::set<std::string>;
static _setStr excludePro;	// 排除的，不记录的进程（如：系统进程）
static _setStr backPro;		// 所有后台进程，已排除需要排除的
static _setStr forePro;		// 所有前台进程，已排除需要排除的
static TinyLog tiny_log("C:/Home/GitProject/zqlCmake/TimeRecorderConsole/log");


static bool GetExeName(HWND hwnd, std::string& ret) {
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
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

// 获取前景窗口用到的回调函数
static BOOL callbackForeWin(HWND hwnd, LPARAM lparam) {
	std::string exeName;
	bool sign = GetExeName(hwnd, exeName);
	if (sign && excludePro.find(exeName) == excludePro.end())
		forePro.insert(exeName);
	return sign;
}

void Monitor::SetExclude(std::set<std::string>& exclude) {
	excludePro.clear();
	for (auto i : exclude) {
		excludePro.insert(i);
	}
}

// Update函数如下几个步骤
// 初始化 backPro 和 forePro
// 用户正在使用的前台程序的记录
// 检测 m_appData 中所有程序的运行状态，已停止的直接加入数据库
void Monitor::Update(unsigned timeOut) {
	forePro.clear();
	while (EnumDesktopWindows(NULL, callbackForeWin, NULL))
		;

	backPro.clear();
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		tiny_log << "获取系统进程快照失败";
		return;
	}
	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);
	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);          // clean the snapshot object
		tiny_log << "获取进程信息失败";
		return;
	}
	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do
	{
		std::string str = pe32.szExeFile;
		if (excludePro.find(str) == excludePro.end())
			backPro.insert(str);
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);

	// 当前前台程序是否在 m_appData 中，是则增加运行时间，否则加入m_appData
	std::string exeName;
	if (GetExeName(GetForegroundWindow(), exeName)) {
		auto pos = m_appData.find(exeName);

		if (pos != m_appData.end()) {
			pos->second.totalTime += timeOut / 1000;
		}
		else m_appData.insert({ exeName, _data(exeName, true, _mode::none) });
	}

	// 对不同 mode 的进程进行不同的检查，以确定其是否在运行
	for (auto i = m_appData.begin(); i != m_appData.end();) {

		// 如果在 前台 / 后台 程序列表 中没有此进程，说明此进程已经结束
		switch (i->second.data.mode) {

		case _mode::none:
			if (forePro.find(i->first) == forePro.end()) {
				i = m_appData.erase(i);
			}
			else i++;
			break;

		case _mode::fore:
			if (forePro.find(i->first) != forePro.end()) {
				i->second.isRun = false;
				// 向数据库中插入数据
				// i->second.data.id i->second.begin i->second.end
			}
			else if (i->second.isRun == false) {
				i->second.isRun = true;
				i->second.totalTime = 0;
			}
			i++;
			break;

		case _mode::back:
			if (backPro.find(i->first) != backPro.end()) {
				i->second.isRun = false;
				// 向数据库中插入数据
				// i->second.data.id i->second.begin i->second.end
			}
			else if (i->second.isRun == false) {
				i->second.isRun = true;
				i->second.begin = time(nullptr);
				i->second.totalTime = 0;
			}
			i++;
			break;
		}
	}
}

void Monitor::end() {
	// 假定所有程序都已经结束
	for (auto i = m_appData.begin(); i != m_appData.end(); i++) {

		switch (i->second.data.mode) {

		case _mode::none:
			break;

		case _mode::fore:
				// 向数据库中插入数据
				// i->second.data.id i->second.begin i->second.end
			break;

		case _mode::back:
				// 向数据库中插入数据
				// i->second.data.id i->second.begin i->second.end
			break;
		}
	}
	m_appData.clear();
}

void Monitor::Init() {
	//excludePro.insert("SynTPEnh.exe");

}

int Monitor::compareMode(_mode& a, _mode& b) {
	static std::vector<std::vector<int>>table = {
		{ 0, 1, -1 },
		{ -1, 0, -1 },
		{ 1, 1, 0 } };
	static std::map<_mode, int>ModeMap = {
		{ _mode::none, 0 },
		{ _mode::fore, 1 },
		{ _mode::back, 2 } };
	return table[ModeMap[a]][ModeMap[b]];
}

#include <iostream>
using namespace std;

int main() {
	Monitor::Init();
	Monitor::Update(1000);
	for (auto i : forePro) {
		cout << i << endl;
	}
}