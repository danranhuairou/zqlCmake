#ifndef _SQL_H_
#define _SQL_H_
#include "RCD.h"

namespace RCD
{
	// time recorder sql
	class SQL
	{
	public:
		virtual void addApplication(_string name, _string exeName, int mode) = 0;
		virtual void addRecord(_size id, _time begin, _time end, _time total) = 0;
		virtual void getApplication(_listSQL &m_appData) = 0;
	};
}
#endif // !_SQL_H_
