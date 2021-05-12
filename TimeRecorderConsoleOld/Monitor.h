#ifndef MONITOR_H
#define MONITOR_H
#include <map>
#include <tuple>
#include <string>
#include <ctime>
#include <set>
#include <Windows.h>
#include <vector>
#include "sql/tinyLog.h"

namespace Monitor {

	enum class _mode {
		none = 0,		// �������ݿ��е�ǰ̨����ҲҪ��¼��Ϣ
		fore = 100,
		back = 200
	};

	// �б�ʾa���б�ʾb
	// a > b return 1
	// a < b return -1
	// a == b return 0
	//         none    fore    back
	// none	    0	    1	    -1
	// fore	    -1	    0	    -1
	// back	    1	    1	    0
	int compareMode(_mode& a, _mode& b);

	// �������ṹ������ù��캯�����죬���ⲻ��ȫ��ʹ��
	struct _sqlData {
		_sqlData(_mode a_mode) {
			mode = a_mode;
			id = 0;
		}

		_mode mode;
		std::string name;	// �û��Զ�������֣�������exeName
		unsigned long long id;
		//time_t total;		// ������Ϣ
	};

	struct _data {

		_data(std::string a_exeName,bool a_isRun, _mode a_mode)
			:data(a_mode) {
			exeName = a_exeName;
			isRun = a_isRun;

			begin = time(nullptr);
			totalTime = 0;
		}
		std::string exeName;

		bool isRun;			//�Ƿ���������
		time_t begin;		//����������У��俪ʼʱ��
		// ǰ̨���� ��������
		time_t totalTime;	//ǰ̨�����ʵ������ʱ�䣬����ʱ��Ϊ��begin + totalTime * n, nΪ����������һ��update����

		_sqlData data;
	};

	extern std::map<std::string, _data> m_appData;
	// �޸�mapΪlist���������������������ö�٣������Զ���Ĳ��Һ���
	// �����ų���������
	// �����ݿ��ȡ��Ϣ��m_appData
	void SetExclude(std::set<std::string>& exclude);
	// Ӧ��ʱ���ô˳����Լ�¼����ʹ��ʱ�䣬����ԽƵ����¼Խ׼ȷ
	void Update(unsigned);
	// �����м�¼¼�����ݿ⣬������ֹ��
	void end();
	// ��ʼ��
	void Init();
}
#endif // !MONITOR_H