#include "Monitor.h"
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <iostream>
#include <windows.h>
#include <Psapi.h>
#include <vector>
#include <algorithm>
using namespace std;

void Print();

//int main() {
//	Monitor::Init();
//
//	while (1) {
//		Sleep(1000);
//		Monitor::Update(1000);
//		Print();
//	}
//
//	Monitor::end();
//}

bool cmp(Monitor::_data a, Monitor::_data b) {
	return true;
	//int compare = Monitor::compareMode(a->data.mode, b->data.mode);
	//if (compare != 0)
	//	return compare == 1 ? true : false;
	//switch (a->data.mode) {
	//case Monitor::_mode::none:
	//case Monitor::_mode::fore:
	//	if (a->totalTime == b->totalTime)
	//		return a->data.name <= b->data.name;
	//	else return a->totalTime > b->totalTime;

	//case Monitor::_mode::back:
	//	if (a->begin == b->begin)
	//		return a->data.name <= b->data.name;
	//	else return a->begin < b->begin;
	//}
}

std::string Space(std::string str) {
	return str + std::string(20 - str.size(), ' ');
}

std::string ToString(time_t time) {
	tm* timeString = localtime(&time);
	return to_string(timeString->tm_hour) + "时" +
		to_string(timeString->tm_min) + "分" +
		to_string(timeString->tm_sec) + "秒";
}

std::string TimeString(Monitor::_data data) {
	switch (data.data.mode) {
	case Monitor::_mode::none:
	case Monitor::_mode::fore:
		return std::to_string(data.totalTime);
	//case Monitor::_mode::back:
	//	return ToString(time(NULL) - data.begin);
	}
	return "";
}

void Print() {
	system("cls");
	vector<Monitor::_data>appList;
	for (auto i : Monitor::m_appData) {
		appList.push_back(i.second);
	}
	sort(appList.begin(), appList.end(), cmp);

	cout << Space("名称") << Space("运行时间") << endl;
	for (auto i : appList) {
		cout << Space(i.exeName) << TimeString(i) << endl;
	}
}