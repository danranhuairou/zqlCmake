#ifndef _SQLITE_H_
#define _SQLITE_H_
#include "sqlite3.h"
#include "tinyLog.h"
#include "SQL.h"
#include <vector>
#include <map>

#define LOGFILE "C:/Home/GitProject/zqlCmake/TimeRecorderConsole/log"
#define SQLFILE "C:/Home/GitProject/zqlCmake/TimeRecorderConsole/resources/Recorder.db"

class SQLite : public RCD::SQL
{
private:
    sqlite3 *m_sqlite;
    TinyLog m_log;

    std::map<std::string, std::vector<std::string>> m_sqlData; // 存储着从数据库获取的数据，第一个string是字段名，第二个为其数据

    static int callback_app(void *, int, char **, char **);
    static int callback_sumTotalTime(void *, int, char **, char **);

    struct _temp_callback
    {
        SQLite *sqlite;
        RCD::_listSQL *list;

        _temp_callback(SQLite *sqlite, RCD::_listSQL *list)
        {
            this->sqlite = sqlite;
            this->list = list;
        }
    };

public:
    SQLite();
    ~SQLite();

    void addApplication(RCD::_string name, RCD::_string exeName, int mode);
    void addRecord(RCD::_size id, RCD::_time begin, RCD::_time end, RCD::_time total);
    void getApplication(RCD::_listSQL &appData);
};
#endif // !_SQLITE_H_
