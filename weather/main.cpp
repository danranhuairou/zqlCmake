#include "web_of_amap.h"
#include <locale>
#include <iostream>
using namespace std;

void PrintWeather(string city);

int main()
{
	PrintWeather("双鸭山市");

	return 0;
	cout << "请输入一个城市，以获取它的天气：";
	string city;
	cin >> city;
}

string GetWeek(int day);

void PrintWeather(string city) {
	vector<string> info = zql::GetWeather(city);
	setlocale(LC_ALL, "C");
	cout << "-----" << info[1] << "省" << info[0] << "-----\n";
	cout << "更新日期：" << info[2] << endl;

	string str;
	//共显示四组天气信息
	for (int i = 0; i < 4; i++) {
		cout << endl;
		str = info[3 + 0 + i * 10] + '\n';
		str += GetWeek(info[3 + 1 + i * 10][0]) + '\n';

		str += "白天天气：" + info[3 + 2 + i * 10] + '\n';
		str += "晚间天气：" + info[3 + 3 + i * 10] + '\n';

		str += "白天温度：" + info[3 + 4 + i * 10] + '\n';
		str += "晚间温度：" + info[3 + 5 + i * 10] + '\n';

		str += "白天风力：" + info[3 + 6 + i * 10] + '\n';
		str += "晚间风力：" + info[3 + 7 + i * 10] + '\n';

		str += "白天风向：" + info[3 + 8 + i * 10] + '\n';
		str += "晚间风向：" + info[3 + 9 + i * 10] + '\n';

		cout << str;
	}

	cout << "\n\n-----地图信息来自高德地图-----\n";
}

string GetWeek(int day) {
	switch (day)
	{
	case '1':
		return "星期一";
	case '2':
		return "星期二";
	case '3':
		return "星期三";
	case '4':
		return "星期四";
	case '5':
		return "星期五";
	case '6':
		return "星期六";
	case '7':
	default:
		return "星期日";
	}
}