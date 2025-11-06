#include <string>

#include "server/server.hpp"
#include "log/log.hpp"

int main()
{
    using namespace AreaSamsara;

    std::string name = "AS";
    std::cout << std::format("Hello, {}!", name) << std::endl;

    // 初始化日志管理器
    const std::string log_filename = "./bin/log.txt";
    log::init_logger(log_filename);

    // 启动服务器
    try
    {
        server::run_server();
    }
    catch (const std::exception &e)
    {
        spdlog::error("Fail to start server: {}", e.what());
        return 1;
    }

    return 0;
}