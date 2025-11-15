#include "server/server.hpp"

#include <string>
#include <format>

#include <httplib.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "server/api.hpp"

namespace AreaSamsara::server
{
    void run_server(const std::string &ip_addr, const int port)
    {
        httplib::Server server;

        // 注册所有 GET 方法
        for (auto it : get_handlers)
        {
            std::string name = it.first;
            HttpHandler handler = it.second;
            server.Get(name, handler);
        }

        // 注册所有 POST 方法
        for (auto it : post_handlers)
        {
            std::string name = it.first;
            HttpHandler handler = it.second;
            server.Post(name, handler);
        }

        spdlog::info("Server listens at {}:{}", ip_addr, port);

        // 监听端口
        if (!server.listen(ip_addr, port))
        {
            throw std::runtime_error(std::format("fail to listen at {}:{}", ip_addr, port));
        }
    }
}
