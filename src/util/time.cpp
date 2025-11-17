#include "util/timee.hpp"

namespace AreaSamsara::util
{
    std::string time_point_to_string(const std::chrono::system_clock::time_point &time_point)
    {
        return std::format("{:%Y-%m-%d %H:%M:%S}", time_point);
    }

    std::chrono::system_clock::time_point string_to_time_point(const std::string &time_str)
    {
        std::tm tm = {};
        std::istringstream ss(time_str);
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        return std::chrono::system_clock::from_time_t(std::mktime(&tm));
    }

    std::tm time_point_to_tm(const std::chrono::system_clock::time_point &time_point)
    {
        time_t time_t_value = std::chrono::system_clock::to_time_t(time_point);
        return *std::localtime(&time_t_value);
    }

    std::chrono::system_clock::time_point tm_to_time_point(const std::tm &tm)
    {
        std::tm tm_value = tm;
        return std::chrono::system_clock::from_time_t(std::mktime(&tm_value));
    }

}