#include "TimeRecorder.h"
#include <algorithm>

using namespace RCD;

_time TimeRecorder::GetTime()
{
    return time(NULL);
}

_string TimeRecorder::TotalTime()
{
    _time sum = 0;
    for (auto i : m_runPro)
        if (i.running)
            sum += RunTime(i);
    return TimeToString(sum);
}

_string TimeRecorder::TimeToString(time_t target)
{
    int sec = target % 60;
    target /= 60;
    int min = target % 60;
    target /= 60;
    return std::to_wstring(target) + L"时" +
           std::to_wstring(min) + L"分" +
           std::to_wstring(sec) + L"秒";
}

void TimeRecorder::AddApplication(_string name, _string exeName)
{
    m_sql->addApplication(name, exeName, 1);
}

void TimeRecorder::AddRecord(_size id, _time begin, _time end, _time total)
{
    m_sql->addRecord(id, begin, end, total);
}

bool TimeRecorder::cmp(const _appData& a, const _appData& b)
{
    // 第一优先级 running
    if (a.running != b.running)
        return a.running == true;
    // 第二优先级 in sql
    if (a.inSql != b.inSql)
        return a.inSql == true;
    // 第三优先级 totalTime

    // 1. a b 都 running == true
    if (a.running) {
        if (a.total > b.total)
            return true;
        else if (a.total < b.total)
            return false;
    }
    // else
    if (a.inSql) return a.sqlData.name >= b.sqlData.name;
    else return a.sqlData.exeName >= b.sqlData.exeName;
}

void TimeRecorder::Sort()
{
    sort(m_runPro.begin(), m_runPro.end(), cmp);
}