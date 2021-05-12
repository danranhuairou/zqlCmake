#include "Monitor.h"
#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <algorithm>
#include <iostream>
using namespace Monitor;

std::map<std::string, Monitor::_data> Monitor::m_appData;		// ��¼���еĵĽ�����Ϣ

using _setStr = std::set<std::string>;
static _setStr excludePro;	// �ų��ģ�����¼�Ľ��̣��磺ϵͳ���̣�
static _setStr backPro;		// ���к�̨���̣����ų���Ҫ�ų���
static _setStr forePro;		// ����ǰ̨���̣����ų���Ҫ�ų���
static TinyLog tiny_log("C:/Home/GitProject/zqlCmake/TimeRecorderConsole/log");


static bool GetExeName(HWND hwnd, std::string& ret) {
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
	//��ȡ����·��
	char* path = new char[MAX_PATH];
	bool sign = K32GetModuleFileNameExA(hProcess, NULL, path, MAX_PATH);

	if (sign) {
		std::string exeName = path;
		ret = exeName.substr(exeName.find_last_of('\\') + 1);
	}
	delete[] path;
	return sign;
}

// ��ȡǰ�������õ��Ļص�����
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

// Update�������¼�������
// ��ʼ�� backPro �� forePro
// �û�����ʹ�õ�ǰ̨����ļ�¼
// ��� m_appData �����г��������״̬����ֹͣ��ֱ�Ӽ������ݿ�
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
		tiny_log << "��ȡϵͳ���̿���ʧ��";
		return;
	}
	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);
	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);          // clean the snapshot object
		tiny_log << "��ȡ������Ϣʧ��";
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

	// ��ǰǰ̨�����Ƿ��� m_appData �У�������������ʱ�䣬�������m_appData
	std::string exeName;
	if (GetExeName(GetForegroundWindow(), exeName)) {
		auto pos = m_appData.find(exeName);

		if (pos != m_appData.end()) {
			pos->second.totalTime += timeOut / 1000;
		}
		else m_appData.insert({ exeName, _data(exeName, true, _mode::none) });
	}

	// �Բ�ͬ mode �Ľ��̽��в�ͬ�ļ�飬��ȷ�����Ƿ�������
	for (auto i = m_appData.begin(); i != m_appData.end();) {

		// ����� ǰ̨ / ��̨ �����б� ��û�д˽��̣�˵���˽����Ѿ�����
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
				// �����ݿ��в�������
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
				// �����ݿ��в�������
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
	// �ٶ����г����Ѿ�����
	for (auto i = m_appData.begin(); i != m_appData.end(); i++) {

		switch (i->second.data.mode) {

		case _mode::none:
			break;

		case _mode::fore:
				// �����ݿ��в�������
				// i->second.data.id i->second.begin i->second.end
			break;

		case _mode::back:
				// �����ݿ��в�������
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