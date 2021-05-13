#ifndef _CONSOLE_H_
#define _CONSOLE_H_
#include "TimeRecorder.h"
#include <set>

class Console : public RCD::TimeRecorder
{
private:
    std::set<RCD::_string> m_allPro;
    RCD::_size m_frontPID, m_nowPID;

    void GetAllProcess();

    inline bool IsProcessRunning(const RCD::_string &name);
    inline bool GetExeName(const RCD::_size pid, RCD::_string &ret);
    inline RCD::_size ForeExePid();

    void Update();
    virtual RCD::_time RunTime(const RCD::_appData &target);

public:
    Console();
    void exec();
};
#endif // !_CONSOLE_H_