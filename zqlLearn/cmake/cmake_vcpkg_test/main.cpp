// main.cpp
#include <iostream>
#include <glog/logging.h>
#include <sqlite3.h>

int main()
{
    std::cout << "GLOG_ERROR: "
        << google::GLOG_ERROR << std::endl;
    std::cout << "SQLITE_OK: "
        << SQLITE_OK << std::endl;
    return 0;
}