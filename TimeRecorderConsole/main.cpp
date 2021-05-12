#include "TimeRecorder.h"
#include <iostream>
using namespace std;

string TimeToString(time_t target) {
	tm* date = localtime(&target);
	return to_string(date->tm_hour) + "ʱ" +
		to_string(date->tm_min) + "��" +
		to_string(date->tm_sec) + "��";
}

int main() {
	TimeRecorder timeRecorder;
	time_t begin = time(NULL);
	time_t end = begin + 60 * 60;

	while (time(NULL) <= end) {
		timeRecorder.Update();

		system("cls");
		for (auto i : timeRecorder.GetProList()) {
			string name = i.name;
			name = name.size() >= 20 ? name : name + string(20 - name.size(), ' ');

			cout << name << timeRecorder.RunTime(i) << endl;
		}
		cout << "\n\ntotal:\t" << timeRecorder.TotalTime() << endl;

		Sleep(1000);
	}

	cout << "\n��ʼʱ�䣺" << TimeToString(begin) << endl;
	cout << "����ʱ�䣺" << TimeToString(time(NULL)) << endl;
	system("pause");
}