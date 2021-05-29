#include "log.h"
#include <algorithm>
#include <fstream>
#include <io.h>
#include <vector>

// class log stream
void zql::LogStream::Write(const std::string& ss) {
    // 写入文件
    std::ofstream file(GetFileName().c_str(), std::ios::app);
    file << ss;
    file.close();
}

zql::LogStream& zql::LogStream::operator<<(const std::string& target) {
    Write(target);
    if (m_subordinate != nullptr)
        m_subordinate->Head() << target;
    return *this;
}

zql::LogStream& zql::LogStream::Head(std::string extraHead) {
    //判断文件最后一个字符是否是\n 不是则设置sign = true 以添加\n
    bool sign = false;
    std::ifstream file(GetFileName().c_str());
    file.seekg(-1, std::ios::end);
    char c = -1;
    file >> c;
    // c == -1表示是文件为空
    if (c != -1 && c != '\n')
        sign = true;
    file.close();

    // 获取日期信息
    time_t t = time(nullptr);
    tm date;
    localtime_s(&date, &t);

    // 格式化日期
    // [2021-05-27 16:45:52]
    char* buffer = new char[30];
    sprintf_s(buffer, 30, "[%04d-%02d-%02d %02d:%02d:%02d] ",
              date.tm_year + 1900, date.tm_mon + 1, date.tm_mday, date.tm_hour,
              date.tm_min, date.tm_sec);

    // ret
    std::string ret;
    if (sign)
        ret += '\n';
    ret += buffer;
    // add m_extraHead
    if (extraHead.size() == 0)
        extraHead = m_extraHead;
    sprintf_s(buffer, 30, "[%-8s] ", m_extraHead.c_str());
    ret += buffer;

    delete[] buffer;
    Write(ret);
    return *this;
}

std::string zql::LogStream::GetFileName() {
    // 获取日期信息
    time_t t = time(nullptr);
    tm date;
    localtime_s(&date, &t);

    // 格式化日期
    // 2021_05_27
    char* buffer = new char[30];
    sprintf_s(buffer, 30, "%04d-%02d-%02d", date.tm_year + 1900,
              date.tm_mon + 1, date.tm_mday);

    std::string ret = m_fileName + buffer + ".txt";
    delete[] buffer;

    return ret;
}

// class log
zql::Log::Log(std::string path) {
    m_path = path;

    // m_s_info
    m_s_info.SetExtraHead("info");
    m_s_info.SetFileName(m_path + "/info_");

    // m_s_error
    m_s_error.SetExtraHead("error");
    m_s_error.SetFileName(m_path + "/error_");
    m_s_error.SetSubordinate(&m_s_info);

    m_maxCount = 60;
    ClearFiles();
}

zql::LogStream& zql::Log::operator()(LOG_TYPE type) {
    switch (type) {
    case LOG_ERROR:
        return m_s_error.Head();
    case LOG_INFO:
    default:
        return m_s_info.Head();
    }
}

void zql::Log::ClearFiles() {
    std::vector<FileInfo> allFiles;

    // 查找所有文件，并加入 allFiles 中
    struct _finddata_t c_file;
    intptr_t hFile;
    // Find first .txt file in log
    if ((hFile = _findfirst((m_path + "/*.txt").c_str(), &c_file)) != -1L) {
        do {
            FileInfo tmp = FileNameParse(c_file.name);
            if (tmp.isLog)
                allFiles.push_back(tmp);
        } while (_findnext(hFile, &c_file) == 0);
        _findclose(hFile);
    }

    sort(allFiles.begin(), allFiles.end(), fileCMP);
    // 删除小的文件夹，使文件夹总数为 m_maxCount
    for (int i = 0; i < allFiles.size() - m_maxCount; i++) {
        remove((m_path + '/' + allFiles[i].fileName).c_str());
    }
}

bool zql::Log::fileCMP(const FileInfo& a, const FileInfo& b) {
    if (a.year != b.year)
        return a.year < b.year;
    if (a.month != b.month)
        return a.month < b.month;
    if (a.day != b.day)
        return a.day < b.day;
    return a.fileName < b.fileName;
}

zql::Log::FileInfo zql::Log::FileNameParse(std::string str) {
    FileInfo ret;
    ret.isLog = false;
    ret.fileName = str;

    // 查找由 '_' 分割开的四个字符串
    std::string part[5];
    size_t begin, end = -1;
    for (int i = 0; i < 3; i++) {
        begin = end + 1;
        end = str.find('_', begin);
        if (end == std::string::npos)
            return ret;
        part[i] = str.substr(begin, end - begin);
    }
    begin = end + 1;
    part[3] = str.substr(begin, 2);
    part[4] = str.substr(begin + 2);
    if (part[4] != ".txt")
        return ret;
    if (part[1].size() != 4 || part[2].size() != 2 || part[3].size() != 2)
        return ret;

    // 对找到的四部分分别赋值
    ret.isLog = true;
    ret.prefix = part[0];
    ret.year = atoi(part[1].c_str());
    ret.month = atoi(part[2].c_str());
    ret.day = atoi(part[3].c_str());
    // check
    if (ret.year > 0 && ret.year < 1900)
        return ret;
    if (ret.month > 0 && ret.month > 12)
        return ret;
    if (ret.day > 0 && ret.day > 31)
        return ret;
    if (ret.prefix != "error" || ret.prefix != "info")
        return ret;
    return ret;
}