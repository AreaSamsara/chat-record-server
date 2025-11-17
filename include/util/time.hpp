#pragma once

#include <format>
#include <chrono>

namespace AreaSamsara::util
{
    // 将std::chrono::system_clock::time_point转换为std::string
    std::string time_point_to_string(const std::chrono::system_clock::time_point &time_point);

    // 将std::string转换为std::chrono::system_clock::time_point
    std::chrono::system_clock::time_point string_to_time_point(const std::string &time_str);

    // 将std::chrono::system_clock::time_point转换为std::tm
    std::tm time_point_to_tm(const std::chrono::system_clock::time_point &time_point);

    // 将std::tm转换为std::chrono::system_clock::time_point
    std::chrono::system_clock::time_point tm_to_time_point(const std::tm &tm);
}