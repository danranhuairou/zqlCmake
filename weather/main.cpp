#include "web_of_amap.h"
#include <locale>
#include <iostream>
using namespace std;

void PrintWeather(string city);

int main()
{
	PrintWeather("˫Ѽɽ��");

	return 0;
	cout << "������һ�����У��Ի�ȡ����������";
	string city;
	cin >> city;
}

string GetWeek(int day);

void PrintWeather(string city) {
	vector<string> info = zql::GetWeather(city);
	setlocale(LC_ALL, "C");
	cout << "-----" << info[1] << "ʡ" << info[0] << "-----\n";
	cout << "�������ڣ�" << info[2] << endl;

	string str;
	//����ʾ����������Ϣ
	for (int i = 0; i < 4; i++) {
		cout << endl;
		str = info[3 + 0 + i * 10] + '\n';
		str += GetWeek(info[3 + 1 + i * 10][0]) + '\n';

		str += "����������" + info[3 + 2 + i * 10] + '\n';
		str += "���������" + info[3 + 3 + i * 10] + '\n';

		str += "�����¶ȣ�" + info[3 + 4 + i * 10] + '\n';
		str += "����¶ȣ�" + info[3 + 5 + i * 10] + '\n';

		str += "���������" + info[3 + 6 + i * 10] + '\n';
		str += "��������" + info[3 + 7 + i * 10] + '\n';

		str += "�������" + info[3 + 8 + i * 10] + '\n';
		str += "������" + info[3 + 9 + i * 10] + '\n';

		cout << str;
	}

	cout << "\n\n-----��ͼ��Ϣ���Ըߵµ�ͼ-----\n";
}

string GetWeek(int day) {
	switch (day)
	{
	case '1':
		return "����һ";
	case '2':
		return "���ڶ�";
	case '3':
		return "������";
	case '4':
		return "������";
	case '5':
		return "������";
	case '6':
		return "������";
	case '7':
	default:
		return "������";
	}
}