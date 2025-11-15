#include <string>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "server/server.hpp"
#include "log/log.hpp"
#include "database/database.hpp"
#include "config/config.hpp"

int main()
{
    using namespace AreaSamsara;

    try
    {
        log::init_logger(config::Config::global_config().logger.file);
    }
    catch (const std::exception &e)
    {
        spdlog::error("Error log::init_logger(): {}", e.what());
        return 1;
    }

    try
    {
        // database::test_soci_insert();
        database::test_soci_select();
    }
    catch (const std::exception &e)
    {
        spdlog::error("Error database::test_soci(): {}", e.what());
        return 1;
    }

    // 启动服务器
    try
    {
        server::run_server();
    }
    catch (const std::exception &e)
    {
        spdlog::error("Error server::run_server(): {}", e.what());
        return 1;
    }

    return 0;
}