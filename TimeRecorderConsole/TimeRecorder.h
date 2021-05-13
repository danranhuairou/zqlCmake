#ifndef _TIMERECORDER_H_
#define _TIMERECORDER_H_
#include "RCD.h"
#include "SQL.h"

namespace RCD
{
    class TimeRecorder
    {
    protected:
        _listApp m_runPro;
        SQL *m_sql;

        virtual void Update() = 0;
        virtual inline time_t RunTime(const _appData &target) = 0;

        _time GetTime();
        _string TotalTime();
        void AddApplication(_string name, _string exeName);
        void AddRecord(_size id, _time begin, _time end, _time total);
        void Sort();

        static _string TimeToString(time_t target);
        static bool cmp(const _appData& a, const _appData& b);
    public:
        virtual void exec() = 0;
    };
}
#endif // !_TIMERECORDER_H_