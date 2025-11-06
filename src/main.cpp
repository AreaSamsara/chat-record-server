#include <string>
#include <spdlog/spdlog.h>
#include "server/server.hpp"

// 初始化日志管理器
void init_logger()
{
    // 设置日志等级
    spdlog::set_level(spdlog::level::debug);
    // 设置日志格式
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

    spdlog::info("Logger initialized as level {}",
                 spdlog::level::to_string_view(spdlog::get_level()));
}

int main()
{
    std::string name = "AS";
    std::cout << std::format("Hello, {}!", name) << std::endl;

    init_logger();

    AreaSamsara::server::run_server();

    return 0;
}