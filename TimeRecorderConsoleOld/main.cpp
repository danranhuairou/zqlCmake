//#include <iostream>
//#include <ctime>
//#include "Monitor.h"
//#include "../TinyLog/tinyLog.h"
//
//using namespace std;
//
//std::string TimeToString(time_t time);
//void Print();
//std::string Space(std::string);
//
////int main()
////{
////   
////}
//
//void Print() {
//    Monitor& monitor = Monitor::get_instance();
//    system("cls");
//    monitor.Upadate();
//    cout << "m_endingMap:\n";
//    for (auto i : monitor.m_ending) {
//        cout << Space(i.first) << '\t' << TimeToString(i.second) << endl;
//    }
//    cout << endl << endl << endl;
//    cout << "m_runningMap:\n";
//    for (auto i : monitor.m_runningBack) {
//        cout << Space(i.first) << '\t' << TimeToString(i.second) << endl;
//    }
//}
//
//std::string TimeToString(time_t time) {
//    tm* date = localtime(&time);
//    return std::to_string(date->tm_year + 1900) + "��" +
//        std::to_string(date->tm_mon + 1) + "��" +
//        std::to_string(date->tm_mday) + "��" +
//        std::to_string(date->tm_hour) + "ʱ" +
//        std::to_string(date->tm_min) + "��" +
//        std::to_string(date->tm_sec) + "��";
//}
//
//std::string Space(std::string str) {
//    while (str.size() < 50) {
//        str.push_back(' ');
//    }
//    return str;
//}