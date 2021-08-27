#include "zqlTools.h"

#include "log.h"
#include "table.h"
#include <iostream>

using namespace zql;
using namespace std;

void TestLog(std::string path) {
    vector<string> files = {"info", "error"};
    Log log(path.c_str(), files);

    log(0) << "this is an info.";

    log(1) << "this is an error."
           << "this is second error.";
    log(1) << 1 << string("我是\nhello");
    log(1) << 1.234 << ' ' << 0.123;
    log(1) << L"宽字节" << L"wide char" << L'字' << L'j';
}

void TestCoding() {
    const char b[] = "你好";

    cout << "\nstr: " << b
         << " is utf8: " << (zql::GetCoding(b) == zql::UTF8 ? "true" : "false");

    cout << "\nstr: " << b << " is utf8: "
         << (zql::GetCoding(zql::ANSIToUTF8(b)) == zql::UTF8 ? "true"
                                                             : "false");

    cout << "\nstr: " << b
         << " is GBK: " << (zql::GetCoding(b) == zql::GBK ? "true" : "false");
    cout << endl;
}

void TestTable() {
#ifdef WIN32
    std::wcout.imbue(std::locale("chs"));
#else
    std::locale::global(std::locale(""));
#endif

    zql::Table table(2, 2);
    table.Set(0, 0, L"1*1=1");
    table.Set(0, 0, L"a");
    table.Set(0, 1, L"我是你");
    table.Set(1, 0, L"asd");
    table.Set(1, 1, L"aaaaaa");

    wcout << table.GetUnicode();

    zql::Table table_b(0, 2);
    table_b.AddLine({L"hello", L"world"});
    table_b.AddLine({L"hel", L"worlda"});
    wcout << table_b.GetUnicode();
}

void TestAll() {
    TestLog("testlog");
    TestCoding();
    TestTable();
}

int main() { TestTable(); }