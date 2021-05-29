#ifndef _LOG_H__
#define _LOG_H__
#include <ctime>
#include <string>

namespace zql {

// 先调用 Head()，再通过其返回的引用使用 << 运算符进行输入
// [2021-05-28 13:54:10] [error   ] This is a log.
class LogStream {
private:
    void Write(const std::string&);
    std::string GetFileName();

    // 下级日志流，上级日志流会在写入时同时写入下级
    // 如：等级 ERROR > INFO ，故写入 ERROR 的同时也要写入 info
    LogStream* m_subordinate;
    // 包含路径的完整文件名
    std::string m_fileName;
    // 头部 [2021-05-28 13:54:10] [m_extraHead] 中的 m_extraHead
    std::string m_extraHead;

public:
    LogStream() { m_subordinate = nullptr; }
    // 默认参数使用 m_extraHead，不然使用参数 extraHead
    LogStream& Head(std::string extraHead = "");
    LogStream& operator<<(const std::string&);
    void SetFileName(std::string fileName) { m_fileName = fileName; }
    void SetExtraHead(std::string extraHead) { m_extraHead = extraHead; }
    void SetSubordinate(LogStream* subordinate) { m_subordinate = subordinate; }
};

// 您可以通过这个类了解到如何使用 LogStream 进行多文件的日志记录
class Log {
public:
    // 日志文件夹
    std::string m_path;
    // 所有日志的最大数量 默认为 60 即每个类型的日志最少 30 个，即一个月的日志
    size_t m_maxCount;
    // log stream
    LogStream m_s_error;
    LogStream m_s_info;

    // 根据文件创建时间进行清理
    void ClearFiles();
    // 辅助函数及结构体
    struct FileInfo {
        bool isLog;
        std::string prefix;
        unsigned short year;
        unsigned short month;
        unsigned short day;
        std::string fileName;

        const Log::FileInfo& operator=(const Log::FileInfo& copy) {
            isLog = copy.isLog;
            prefix = copy.prefix;
            year = copy.year;
            month = copy.month;
            day = copy.day;
            fileName = copy.fileName;
            return *this;
        }
    };
    // 用于 ClearFiles() 的比较函数，升序排列，小的将会被删除
    static bool fileCMP(const FileInfo&, const FileInfo&);
    static FileInfo FileNameParse(std::string);

public:
    enum LOG_TYPE { LOG_ERROR, LOG_INFO };

    Log(std::string path);
    LogStream& operator()(LOG_TYPE type);
    void SetMaxCount(size_t maxCount) { m_maxCount = maxCount; }
};

} // namespace zql
#endif // !_LOG_H__
