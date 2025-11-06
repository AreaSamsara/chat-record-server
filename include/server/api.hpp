#pragma once

#include <iostream>
#include <map>

#include <format>
#include <httplib.h>

#include "log/log.hpp"

namespace AreaSamsara::server
{
    // HTTP 路由方法
    using HttpHandler = std::function<void(const httplib::Request &, httplib::Response &)>;

    // HTTP GET 方法集合
    extern std::map<std::string, HttpHandler> get_handlers;
    // HTTP POST 方法集合
    extern std::map<std::string, HttpHandler> post_handlers;
}