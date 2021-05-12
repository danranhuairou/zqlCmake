#include "SQLOperator.h"

SQLOperator& SQLOperator::get_instance() {
	static SQLOperator instance;
	return instance;
}

SQLOperator::SQLOperator() {
	if (sqlite3_open(FILENAME, &m_conn) != SQLITE_OK) {
		//Error("数据库连接失败");
		//return false;
	}
}

SQLOperator::~SQLOperator() {

}

void SQLOperator::AddRecord(int id, time_t begin) {

}