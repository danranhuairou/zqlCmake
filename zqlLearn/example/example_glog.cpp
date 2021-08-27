// 路径
#if 0
#define filePath "C:/Home/project/ConsoleApp/Debug/"
#else
#define filePath "./"
#endif

#include <glog/logging.h>

int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::GLOG_INFO);       // 设置glog的输出级别，这里的含义是输出INFO级别以上的信息

    // 设置INFO级别以上的信息log文件的路径和前缀名
    google::SetLogDestination(google::GLOG_INFO, filePath "INFO_");

    // 设置WARNING级别以上的信息log文件的路径和前缀名
    google::SetLogDestination(google::GLOG_WARNING, filePath "WARNING_");

    // 设置ERROR级别以上的信息log文件的路径和前缀名
    google::SetLogDestination(google::GLOG_ERROR, filePath "ERROR_");

    FLAGS_colorlogtostderr = true;                     // 开启终端颜色区分

    LOG(INFO) << "This is my first glog INFO ";        // 像C++标准流一样去使用就可以了，把这条信息定义为INFO级别
    LOG(WARNING) << "This is my first glog WARNING";   // 像C++标准流一样去使用就可以了，把这条信息定义为WARNING级别
    LOG(ERROR) << "This is my first glog ERROR";       // 像C++标准流一样去使用就可以了，把这条信息定义为ERROR级别


    //条件输出
    int num_cookies = 100;
    LOG_IF(INFO, num_cookies > 10) << "Got lots of cookies";

    google::ShutdownGoogleLogging();                   // 全局关闭glog
    return 0;
}