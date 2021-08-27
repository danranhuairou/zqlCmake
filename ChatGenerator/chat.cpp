#include "chat.h"
#include "sqlite3.h"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <windows.h>

#include "C:/Home/GitProject/zqlTools/include/zqlTools.h"
#include <psapi.h>

std::string ForeExeName() {
    DWORD pid;
    GetWindowThreadProcessId(GetForegroundWindow(), &pid);
    HANDLE hProcess =
        OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, (DWORD)pid);
    //获取进程路径
    wchar_t path[MAX_PATH];
    bool sign = K32GetModuleFileNameExW(hProcess, NULL, path, MAX_PATH);

    if (sign) {
        std::string res = zql::UnicodeToANSI(path);
        res = res.substr(res.find_last_of('\\') + 1);
        return res;
    }
    return "";
}

void SendChatContent(std::string exeName, _vec content, size_t count) {
    std::string nowExeName = ForeExeName();
    while (true) {
        Sleep(1000);
        nowExeName = ForeExeName();
        if (nowExeName == exeName) {
            // 生成随机排序的 content ，其大小至少为count
            size_t n = content.size();
            srand((unsigned int)time(NULL));

            for (int i = 0; i < n * 2; i++)
                swap(content[rand() % n], content[0]);
            while (content.size() < count)
                content.push_back(content[rand() % n]);

            // 输出
            for (int i = 0; i < count; i++) {
                Sleep(100);
                std::string& str = content[i];
                // 复制 str 到剪切板
                size_t n = str.size() + 1;
                if (OpenClipboard(NULL)) {
                    HGLOBAL hmem = GlobalAlloc(GHND, n);
                    char* pmem = (char*)GlobalLock(hmem);
                    EmptyClipboard();
                    memcpy(pmem, str.c_str(), n);
                    SetClipboardData(CF_TEXT, hmem);
                    CloseClipboard();
                    GlobalFree(hmem);
                }
                // 按下回车并放开
                keybd_event(VK_RETURN, 0, 0, 0);
                keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
                // 按下 CRTL+V 并放开
                keybd_event(VK_CONTROL, 0, 0, 0);
                keybd_event(0x56, 0, 0, 0);
                keybd_event(0x56, 0, KEYEVENTF_KEYUP, 0);
                keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
                // 按下回车并放开
                keybd_event(VK_RETURN, 0, 0, 0);
                keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
            }
            std::cout << "输出完成\n";
            return;
        }
    }
}

int callback(void* data, int argc, char** argv, char** colName) {
    _vec* res = (_vec*)data;
    res->push_back(zql::UTF8ToANSI(argv[0]));
    return false;
}

_vec ReadSQL() {
    _vec res;

    sqlite3* sql;
    sqlite3_open(SQLFILENAME, &sql);
    sqlite3_exec(sql, "select * from content", callback, (void*)&res, NULL);
    sqlite3_close(sql);

    return res;
}

void WriteSQL(_vec content) {
    // 排序
    sqlite3* sql;
    sqlite3_open(SQLFILENAME, &sql);
    sqlite3_exec(sql, "delete from content", NULL, NULL, NULL);
    sort(content.begin(), content.end());

    std::string front; // 保证不会加入重复的字符串
    for (auto str : content) {
        str = zql::ANSIToUTF8(str);
        if (front != str) {
            sqlite3_exec(
                sql,
                ("INSERT INTO content(chat) VALUES ('" + str + "')").c_str(),
                NULL, NULL, NULL);
        }
        front = str;
    }
    sqlite3_close(sql);
}