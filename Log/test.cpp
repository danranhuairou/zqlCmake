#include <iostream>
#include <vector>

#include "log.h"
using namespace zql;
using namespace std;

bool operator==(Log::FileInfo a, Log::FileInfo b) {
    if (a.isLog != b.isLog)
        return false;
    if (!a.isLog || !b.isLog)
        return a.fileName != b.fileName;
    if (a.fileName != b.fileName)
        return false;
    if (a.prefix != b.prefix)
        return false;
    if (a.year != b.year)
        return false;
    if (a.month != b.month)
        return false;
    if (a.day != b.day)
        return false;
    return true;
}

void Test_FileNameParse() {
    Log::FileInfo fileInfo;
    // data
    fileInfo.isLog = true;
    fileInfo.prefix = "error";
    fileInfo.year = 2021;
    fileInfo.month = 11;
    fileInfo.day = 1;
    fileInfo.fileName = "error_2021_11_01.txt";
    Log::FileInfo res = Log::FileNameParse(fileInfo.fileName);
    if (res == fileInfo)
        cout << "is right\n";
    else
        cout << "wrong";
}

int main() {
    Test_FileNameParse();
    // Log log("C:/Home/GitProject/zqlCmake/Log/testlog");
    // log(zql::Log::LOG_INFO) << "this is an info.";
    // log(zql::Log::LOG_ERROR) << "this is an error.";
}