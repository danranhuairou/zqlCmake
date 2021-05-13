#include "SQLite.h"

SQLite::SQLite() : m_log(LOGFILE)
{
    if (sqlite3_open(SQLFILE, &m_sqlite) != SQLITE_OK)
        m_log << "数据库初始化失败!";
}

SQLite::~SQLite()
{
    if (sqlite3_close(m_sqlite) == SQLITE_BUSY)
        m_log << "数据库关闭失败";
}

void SQLite::addApplication(RCD::_string name, RCD::_string exeName, int mode)
{
    std::string sql =
        "INSERT INTO record (app_id, app_name, app_exeName, app_mode) VALUES (" +
        RCD::UnicodeToUTF8(name) + ", " + RCD::UnicodeToUTF8(exeName) + ", " + std::to_string(mode) + ")";
    char *errmsg = NULL;
    if (sqlite3_exec(m_sqlite, sql.c_str(), NULL, NULL, &errmsg) != SQLITE_OK)
    {
        m_log << "向数据库添加应用失败，错误信息：" << std::string(errmsg) << "\n"
            << "数据, name: " << RCD::UnicodeToUTF8(name)
            << "\texeName: " << RCD::UnicodeToUTF8(exeName);
        delete[] errmsg;
    }
}

void SQLite::addRecord(RCD::_size id, RCD::_time begin, RCD::_time end, RCD::_time total)
{
    std::string sql =
        "INSERT INTO record (app_id, rec_begin, rec_end, rec_total) VALUES (" +
        std::to_string(id) + ", " + std::to_string(begin) + ", " + std::to_string(end) + ", " + std::to_string(total) + ")";
    char *errmsg = NULL;
    if (sqlite3_exec(m_sqlite, sql.c_str(), NULL, NULL, &errmsg) != SQLITE_OK)
    {
        m_log << "向数据库中添加记录失败：" << std::string(errmsg) << "\n"
            << "数据, id: " << std::to_string(id)
            << "\tbegin: " << std::to_string(begin)
            << "\tend: " << std::to_string(end)
            << "\ttotal: " << std::to_string(total);
        delete[] errmsg;
    }
}

void SQLite::getApplication(RCD::_listSQL &appData)
{
    appData.clear();
    std::string sql = "SELECT app_id, app_name, app_exeName, app_type_id, app_mode FROM application";
    char *errmsg = NULL;
    _temp_callback temp_call(this, &appData);
    if (sqlite3_exec(m_sqlite, sql.c_str(), callback_app, (void*)&temp_call, &errmsg) != SQLITE_OK) {
        m_log << "从数据库中获取应用基本信息失败，错误信息：" << std::string(errmsg);
        delete[] errmsg;
    }
}

int SQLite::callback_sumTotalTime(void *lp, int nColumn, char **colValues, char **colNames)
{
    RCD::_time *sum = (RCD::_time *)lp;
    if (strcmp(colNames[0], "SUM(rec_total)") == 0 && colValues[0] != NULL)
        *sum = std::atoi(colValues[0]);
    else
    {
        *sum = 0;
    }
    return 0;
}

// 此函数每次返回查询的一行
// 第一个参数是通过 sqlite3_exec 传递过来的
// 第二个参数 nColumn 列数，及后两个参数的字符串的个数
int SQLite::callback_app(void *lp, int nColumn, char **colValues, char **colNames)
{
    _temp_callback *temp_call = (_temp_callback *)lp;
    SQLite *sqlite = temp_call->sqlite;

    if (sqlite->m_sqlData.size() == 0)
    {
        for (auto i = 0; i < nColumn; i++)
            sqlite->m_sqlData.insert({colNames[i], {}});
    }
    RCD::_sqlData temp;
    for (auto i = 0; i < nColumn; i++)
    {
        if (strcmp(colNames[i], "app_id") == 0)
            temp.sqlId = std::atoi(colValues[i]);
        else if (strcmp(colNames[i], "app_name") == 0)
            temp.name = RCD::UTF8ToUnicode(colValues[i]);
        else if (strcmp(colNames[i], "app_exeName") == 0)
            temp.exeName = RCD::UTF8ToUnicode(colValues[i]);
    }
    std::string sql = "SELECT SUM(rec_total) FROM record WHERE app_id == " + std::to_string(temp.sqlId);
    char *errmsg = NULL;
    if (sqlite3_exec(sqlite->m_sqlite, sql.c_str(), callback_sumTotalTime, (void *)(&temp.totalTime), &errmsg) != SQLITE_OK)
    {
        sqlite->m_log << "从数据库中获取总计使用时间失败，错误信息：" << std::string(errmsg);
        delete[] errmsg;
        return 0;
    }

    temp_call->list->push_back(temp);
    return 0;
}