#pragma once

#include <string>

namespace AreaSamsara::server
{
    // 启动服务器
    void run_server(const std::string &ip_addr = "0.0.0.0", const int port = 8080);
}
