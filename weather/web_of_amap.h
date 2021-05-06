#ifndef _WEB_OF_AMAP_H__
#define _WEB_OF_AMAP_H__
#include <vector>
#include <string>
/***********************************************************************
模块名       : web of amap
文件名       : web_of_amap.h
相关文件     : web_of_amap.cpp
文件实现功能 : 高德地图的web服务API
作者         : zql
版本         : 1.0
----------------------------------------------------------------------
备注         : 使用高德地图提供的API
               https://console.amap.com/dev/key/app
----------------------------------------------------------------------
修改记录     :
日 期        版本   修改人                 修改内容
2021/4/17    1.0    zql                     创建

*********************************************************************/

namespace zql {
    //对url发送http get请求，并返回一个ANSI编码的字符串
    std::string HttpGet(std::string url, int port = 80);

    //获取天气信息
    std::vector<std::string> GetWeather(std::string city);

    //根据city搜索其对应城市编码，若结果由多个仅返回第一个
    int GetAdcode(std::string city);

    //编码转换
    //使用前请加入代码setlocale(LC_ALL, "");
    //为防止cout错误，代码后输入setlocale(LC_ALL, "C");
    std::string UnicodeToUTF8(const std::wstring& wstr);
    std::string UnicodeToANSI(const std::wstring& wstr);
    std::wstring UTF8ToUnicode(const std::string& str);
    std::string UTF8ToANSI(const std::string& str);
    std::wstring ANSIToUnicode(const std::string& str);
    std::string ANSIToUTF8(const std::string& str);
}
#endif // !_WEB_OF_AMAP_H__
