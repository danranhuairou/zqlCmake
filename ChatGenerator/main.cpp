#include "chat.h"
#include <iostream>

#define EXENAME "QQ.exe"

void Import() {
    _vec content = ReadSQL();

    std::string str;
    while (std::cin >> str) {
        if (str == "1")
            break;
        // 格式处理
        str = str.substr(str.find("、") + 2);
        if (str.size() != 0)
            content.push_back(str);
    }

    WriteSQL(content);
    std::cout << "录入完成\n";
}

int main() {
    std::string exeName = EXENAME;

    while (true) {
        int choose;
        std::cout << "0.导入数据：\n";
        std::cout << "1.数据库：\n";
        std::cout << "2.单条消息：\n";
        std::cout << "3.自定义消息：\n";
        std::cout << "4.自定义应用：\n";
        std::cout << "5.退出：\n";
        std::cout << "choose(0-5)：";
        std::cin >> choose;

        switch (choose) {
        case 0:
            std::cout << "请输入语句，输入1结束输入：\n";
            Import();
            break;
        case 1:
            std::cout << "请输入语句数量：";
            std::cin >> choose;
            SendChatContent(exeName, ReadSQL(), choose);
            break;
        case 2:
            std::cout << "请输入语句数量：";
            std::cin >> choose;
            SendChatContent(exeName, {"我是你爹"}, choose);
            break;
        case 3: {
            std::cout << "请输入语句，输入0清空，输入1结束输入：\n";
            static _vec customize;
            std::string str;
            while (std::cin >> str) {
                if (str == "0")
                    customize.clear();
                else if (str == "1")
                    break;
                customize.push_back(str);
            }

            std::cout << "请输入语句数量：";
            std::cin >> choose;
            SendChatContent(exeName, customize, choose);
            break;
        }
        case 4:
            std::cin >> exeName;
            break;
        case 5:
            return 0;
            break;
        }
    }
    return 0;
}
