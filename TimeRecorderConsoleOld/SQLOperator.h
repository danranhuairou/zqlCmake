#ifndef SQLOPERATOR_H_
#define SQLOPERATOR_H_

#define FILENAME "C:\\Home\\Recorder.db"

#include "sql/sqlite3.h"
#include <ctime>
#include <string>

class SQLOperator
{
public:
	static SQLOperator& get_instance();
	void AddRecord(int id, time_t begin);
private:
	SQLOperator();
	~SQLOperator();

	sqlite3* m_conn;
	using _errorFun = void(*)(std::string);

};

#endif // !SQLOPERATOR_H_
