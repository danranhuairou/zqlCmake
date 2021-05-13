#include "Console.h"
#include "SQLite.h"
#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <iostream>
#include <locale>
using namespace std;

Console::Console()
{
    m_frontPID = 0;
    m_nowPID = 0;
    m_sql = new SQLite;

    RCD::_listSQL list;
    m_sql->getApplication(list);
    RCD::_appData temp;
    temp.inSql = true;
    temp.running = false;
    for (auto &i : list)
    {
        temp.sqlData = i;
        //temp.begin = temp.front = temp.total = 0;
        m_runPro.push_back(temp);
    }
    return;
}


void Console::exec()
{
    wcout.imbue(locale("chs"));

    while (true)
    {
        Update();
        Sort();

        system("cls");

        wcout << L"正在运行的程序: \n";
        auto i = m_runPro.begin();
        for (; i != m_runPro.end(); i++) {
            if (!i->running)
                break;

            RCD::_string name;
            if (i->inSql)  name = i->sqlData.name;
            else name = i->sqlData.exeName;
            name = name.size() >= 20 ? name : name + RCD::_string(20 - name.size(), ' ');

            wcout << name << TimeToString(RunTime(*i)) << endl;
        }

        wcout << L"\n\n不在运行的程序: \n";
        for (; i != m_runPro.end(); i++) {
            RCD::_string name = i->sqlData.name;
            name = name.size() >= 20 ? name : name + RCD::_string(20 - name.size(), ' ');

            wcout << name << TimeToString(i->sqlData.totalTime) << endl;
        }

        wcout << L"\n\n总计使用时间:\t" << TotalTime() << endl;

        Sleep(1000);
    }
}

void Console::Update()
{
    m_nowPID = ForeExePid();
    if (m_nowPID == m_frontPID)
    {
        return;
    }
    else
    {
        RCD::_time nowTime = GetTime();
        GetAllProcess();
        // frontPID != nowPID 的情况下，是否在 proList 中找到 nowPID
        int sign = false;
        for (auto i = m_runPro.begin(); i != m_runPro.end();)
        {
            if (i->inSql)
            {
                if (i->running)
                {
                    if (i->pid == m_frontPID)
                    {
                        i->total += nowTime - i->front;
                    }
                    else if (i->pid == m_nowPID)
                    {
                        i->front = nowTime;
                        sign = true;
                    }
                    else if (!IsProcessRunning(i->sqlData.exeName))
                    {
                        i->total += nowTime - i->front;
                        m_sql->addRecord(i->sqlData.sqlId, i->begin, nowTime, i->total);
                        i->running = false;
                    }
                }
                else
                {
                    RCD::_string str;
                    GetExeName(m_nowPID, str);

                    if (i->sqlData.exeName == str)
                    {
                        i->running = true;
                        i->pid = m_nowPID;
                        i->front = i->begin = nowTime;
                        i->total = 0;
                        sign = true;
                    }
                }
            }
            else
            {
                if (i->pid == m_frontPID)
                {
                    i->total += nowTime - i->front;
                }
                else if (i->pid == m_nowPID)
                {
                    i->front = nowTime;
                    sign = true;
                }
                else if (!IsProcessRunning(i->sqlData.exeName))
                {
                    i = m_runPro.erase(i);
                    continue;
                }
            }
            i++;
        }

        if (!sign)
        {
            RCD::_appData t;
            if (GetExeName(m_nowPID, t.sqlData.exeName))
            {
                t.inSql = false;
                t.pid = m_nowPID;
                t.front = t.begin = nowTime;
                t.total = 0;
                t.running = true;
                m_runPro.push_back(t);
            }
        }
        m_frontPID = m_nowPID;
    }
}

void Console::GetAllProcess()
{
    m_allPro.clear();
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        // error
        return;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32))
    {
        // error
        CloseHandle(hProcessSnap);
        return;
    }

    do
    {
        m_allPro.insert(RCD::_string(pe32.szExeFile));
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return;
}

bool Console::IsProcessRunning(const RCD::_string &name)
{
    return m_allPro.find(name) != m_allPro.end();
}

bool Console::GetExeName(const RCD::_size pid, RCD::_string &ret)
{
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, (DWORD)pid);
    //获取进程路径
    LPWSTR path = new WCHAR[MAX_PATH];
    bool sign = K32GetModuleFileNameExW(hProcess, NULL, path, MAX_PATH);

    if (sign)
    {
        RCD::_string exeName = path;
        ret = exeName.substr(exeName.find_last_of('\\') + 1);
    }
    delete[] path;
    return sign;
}

RCD::_size Console::ForeExePid()
{
    DWORD pid;
    GetWindowThreadProcessId(GetForegroundWindow(), &pid);
    return pid;
}

time_t Console::RunTime(const RCD::_appData &target)
{
    return m_nowPID == target.pid ? time(NULL) - target.front + target.total : target.total;
}