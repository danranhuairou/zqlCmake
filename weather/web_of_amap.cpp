#include "web_of_amap.h"
#include <string>
#include <vector>
#include <WinSock.h>
#include <codecvt>
#include <locale>
#pragma comment(lib, "ws2_32.lib")

//在str中查找target，并返回target所对应的数据
//每次查找都会删除找到内容之前的内容
std::string FindData(std::string& str, const std::string& target) {
    size_t begin = str.find(target);
    if (begin == std::string::npos)
        return "";
    //找到了target，移到其尾部
    begin += target.length() + 1;
    //找接下来的第一组双引号，其内即为所要数据
    begin = str.find('\"', begin) + 1;
    size_t end = str.find('\"', begin);
    if (begin == std::string::npos || end == std::string::npos)
        return "";

    std::string res = str.substr(begin, end - begin);
    //删除之前的内容
    str = str.substr(end);
    return res;
}

/***********************************************************************
模块名       : 获取天气信息
文件名       : web_of_amap.cpp
相关文件     : web_of_amap.h
文件实现功能 :
作者         : zql
版本         : 1.0
----------------------------------------------------------------------
备注         : 使用高德地图提供的天气服务
               https://lbs.amap.com/api/webservice/guide/api/weatherinfo/#t1
               示例URL
               https://restapi.amap.com/v3/weather/weatherInfo?key=fa1228bc6667a259801d247e44dbb88e&&city=230100&extensions=all&output=json
----------------------------------------------------------------------
修改记录     :
日 期        版本   修改人                 修改内容
2021/4/17    1.0    zql                     创建

*********************************************************************/
#define URL_WEA_FRONT "https://restapi.amap.com/v3/weather/weatherInfo?"
#define URL_WEA_KEY "key=fa1228bc6667a259801d247e44dbb88e"
#define URL_WEA_CITY "&city="
#define URL_WEA_EXTENSION "&extensions=all"
#define URL_WEA_OUTPUT "&output=json"

//根据城市码生成URL
std::string URL_WEA(int city) {
    std::string ret = URL_WEA_FRONT URL_WEA_KEY URL_WEA_CITY;
    ret += std::to_string(city);
    ret += URL_WEA_EXTENSION URL_WEA_OUTPUT;
    return ret;
}

//天气信息，前三个数据是城市，省份，更新日期，后面包含四组，每组10个数据，存储着四天的天气信息
//详见m_keyword常量
std::vector<std::string> zql::GetWeather(std::string city) {
    //获取json并解析
    std::string json = HttpGet(URL_WEA(zql::GetAdcode(city)));
    //Keyword
    const std::vector<std::string>keyword = {
        "city","province","reporttime",
        "date","week","dayweather","nightweather","daytemp","nighttemp","daywind","nightwind","daypower","nightpower"
    };

    std::vector<std::string>ret;
    //共有3 + 10 * 4 个数据
    for (int i = 0; i < 3; i++) {
        ret.push_back(FindData(json, keyword[i]));
    }
    for (int j = 0; j < 4; j++)
        for (int i = 0; i < 10; i++) {
            ret.push_back(FindData(json, keyword[i + 3]));
        }
    return ret;
}

/***********************************************************************
模块名       : 获取城市编码(adcode)
文件名       : web_of_amap.cpp
相关文件     : web_of_amap.h
文件实现功能 :
作者         : zql
版本         : 1.0
----------------------------------------------------------------------
备注         : 使用高德地图提供的行政区域查询API
               https://lbs.amap.com/api/webservice/guide/api/district/#district
               示例URL
               https://restapi.amap.com/v3/config/district?key=fa1228bc6667a259801d247e44dbb88e&keywords=双鸭山&subdistrict=0&extensions=base
----------------------------------------------------------------------
修改记录     :
日 期        版本   修改人                 修改内容
2021/4/19    1.0    zql                     setlocale

*********************************************************************/

#define URL_CITY_FRONT "https://restapi.amap.com/v3/config/district?"
#define URL_CITY_KEY "key=fa1228bc6667a259801d247e44dbb88e"
#define URL_CITY_CITY "&keywords="
#define URL_CITY_SUB "&subdistrict=0"
#define URL_CITY_EXTENSION "&extensions=base"

//生成URL
std::string URL_CITY(std::string city) {
    std::string ret = URL_CITY_FRONT URL_CITY_KEY URL_CITY_CITY;
    ret += city;
    ret += URL_CITY_EXTENSION URL_CITY_SUB;
    return ret;
}

//根据给定参数搜索其对应adcode（城市编码），对于多个搜索结果仅第一个有效
//必须传入一个u8参数
int zql::GetAdcode(std::string city) {
    setlocale(LC_ALL, "");
    city = zql::ANSIToUTF8(city);
    std::string info = zql::HttpGet(URL_CITY(city));
    std::string num = FindData(info, "adcode");
    return atoi(num.c_str());
}

/***********************************************************************
模块名       : Http 请求
文件名       : web_of_amap.cpp
相关文件     : web_of_amap.h
文件实现功能 :
作者         : zql
版本         : 1.0
----------------------------------------------------------------------
备注         :
----------------------------------------------------------------------
修改记录     :
日 期        版本   修改人                 修改内容
2021/4/17    1.0    zql                     创建

*********************************************************************/

std::string HttpGetUTF8(std::string url, int port) {
    // 开始进行socket初始化
    WSADATA wData;
    WSAStartup(MAKEWORD(2, 2), &wData);

    //根据URL获取host主机和IP地址
    //URL可以以Http://开头，也可以直接以host主机开头
    //先去掉Http://
    int begin = 0;
    if (tolower(url[0]) == 'h')
        begin = 8;
    //截取host
    size_t pos = url.find('/', begin);
    std::string host = url.substr(begin, pos - begin);
    HOSTENT* getHost = gethostbyname(host.c_str());
    std::string ip = inet_ntoa(*(in_addr*)*getHost->h_addr_list);

    //创建套接字, 三个参数分别为：ipv4（协议域）, SOCK_STREAM（SOCKET类型），TCP（协议）
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    //服务器地址
    sockaddr_in serverAddr = { 0 };
    //使用ipv4地址
    serverAddr.sin_family = AF_INET;
    //具体的ip地址
    serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    //端口号
    serverAddr.sin_port = htons(port);

    //连接服务器
    int errNo = connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (errNo == SOCKET_ERROR) {
        //Error();
        return "";
    }

    std::string strSend = "GET " + url + " HTTP/1.1\r\n"
        "Host: " + host + "\r\n\r\n";

    // 发送
    errNo = send(sock, strSend.c_str(), strSend.length(), 0);
    if (errNo <= 0)
    {
        //发送失败
        //std::cout << "errNo:" << errNo << std::endl;
        return "";
    }

    // 接收
    char bufRecv[4096] = { 0 };
    errNo = recv(sock, bufRecv, 4096, 0);
    if (errNo <= 0)
    {
        //接受失败
        //std::cout << "errNo:" << errNo << std::endl;
        return "";
    }

    // socket环境清理
    WSACleanup();
    return bufRecv;
}

//对url发送http get请求，并返回一个ANSI编码的字符串
std::string zql::HttpGet(std::string url, int port) {
    //为了转utf8为ANSI
    setlocale(LC_ALL, "");
    //char*转string再转ANSI
    return zql::UTF8ToANSI(HttpGetUTF8(url, port));
}

/***********************************************************************
模块名       : 编码转换
文件名       : web_of_amap.cpp
相关文件     : web_of_amap.h
文件实现功能 :
作者         : zql
版本         : 1.0
----------------------------------------------------------------------
备注         : 详见 https://www.cnblogs.com/lidabo/p/10482864.html
                    https://blog.csdn.net/flushhip/article/details/82836867
----------------------------------------------------------------------
修改记录     :
日 期        版本   修改人                 修改内容
2021/4/17    1.0    zql                     创建

*********************************************************************/

std::string zql::UnicodeToUTF8(const std::wstring& wstr)
{
    std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
    return wcv.to_bytes(wstr);
}

std::wstring zql::UTF8ToUnicode(const std::string& str)
{
    std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
    return wcv.from_bytes(str);
}

//使用前请加入代码setlocale(LC_ALL, "");
//为防止cout错误，代码后输入setlocale(LC_ALL, "C");
std::string zql::UnicodeToANSI(const std::wstring& wstr)
{
    std::string ret;
    std::mbstate_t state = {};
    const wchar_t* src = wstr.data();
    size_t len = std::wcsrtombs(nullptr, &src, 0, &state);
    if (static_cast<size_t>(-1) != len) {
        std::unique_ptr< char[] > buff(new char[len + 1]);
        len = std::wcsrtombs(buff.get(), &src, len, &state);
        if (static_cast<size_t>(-1) != len) {
            ret.assign(buff.get(), len);
        }
    }
    return ret;
}

//使用前请加入代码setlocale(LC_ALL, "");
std::wstring zql::ANSIToUnicode(const std::string& str)
{
    std::wstring ret;
    std::mbstate_t state = {};
    const char* src = str.data();
    size_t len = std::mbsrtowcs(nullptr, &src, 0, &state);
    if (static_cast<size_t>(-1) != len) {
        std::unique_ptr< wchar_t[] > buff(new wchar_t[len + 1]);
        len = std::mbsrtowcs(buff.get(), &src, len, &state);
        if (static_cast<size_t>(-1) != len) {
            ret.assign(buff.get(), len);
        }
    }
    return ret;
}

//使用前请加入代码setlocale(LC_ALL, "");
std::string zql::UTF8ToANSI(const std::string& str)
{
    return UnicodeToANSI(UTF8ToUnicode(str));
}

//使用前请加入代码setlocale(LC_ALL, "");
std::string zql::ANSIToUTF8(const std::string& str)
{
    return UnicodeToUTF8(ANSIToUnicode(str));
}