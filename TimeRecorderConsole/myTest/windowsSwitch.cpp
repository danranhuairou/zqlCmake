#include <windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

struct _pro {
	DWORD id;
	HWND hwnd;
	bool sign;

	_pro(DWORD aid) {
		id = aid;
		sign = false;
	}
	_pro(DWORD aid, HWND ahwnd, bool asign) {
		id = aid;
		hwnd = ahwnd;
		sign = asign;
	}
};

map<DWORD, _pro>pros;

string TimeToString(time_t target) {
	tm* date = localtime(&target);
	return to_string(date->tm_hour) + "时" +
		to_string(date->tm_min) + "分" +
		to_string(date->tm_sec) + "秒";
}

bool GetPID(HWND hwnd, DWORD& pid) {
	GetWindowThreadProcessId(hwnd, &pid);
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
	char* path = new char[MAX_PATH];
	bool sign = K32GetModuleFileNameExA(hProcess, NULL, path, MAX_PATH);
	delete[] path;
	return sign;
}

BOOL callbackForeWin(HWND hwnd, LPARAM lparam) {
	DWORD pid;
	bool sign = GetPID(hwnd, pid);
	if (sign) {
		if (pros.find(pid) == pros.end())
			pros.insert({ pid, _pro(pid, hwnd, true) });
	}
	return sign;
}

int main() {
	while (EnumDesktopWindows(NULL, callbackForeWin, NULL)) ;

	srand(time(NULL));
	time_t begin = time(NULL);

	cout << "随机窗口选择已经开始运行\n";
	cout << "开始时间：" << TimeToString(begin) << endl;


	vector<DWORD>pids;
	pids.push_back(12928);
	pids.push_back(13740);
	pids.push_back(13868);
	pids.push_back(9772);

	cout << "所有可获取窗口句柄的程序：\n";
	for (auto &i : pros) {
		if (find(pids.begin(), pids.end(), i.first) == pids.end())
			cout << i.first << "\t";
	}
	cout << endl;

	for (auto i : pids) {
		if (pros.find(i) == pros.end())
			cout << i << " 设置窗口句柄失败\n";
		else cout << i << " 设置窗口句柄成功\n";
	}
	cout << endl << "测试开始\n";

	int front = -1;
	while (true) {

		Sleep(rand() % 10000 + 100);

		size_t pos = rand() % pids.size();
		if (front != pos) {
			auto temp = pros.find(pids[pos]);
			if (!SetForegroundWindow(temp->second.hwnd))
				cout << "设置失败：" << temp->second.id << endl;
			//else cout << "设置成功：" << temp->second.id << endl;
		}
	}
}