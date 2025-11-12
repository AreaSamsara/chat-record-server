#include "log/log.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace AreaSamsara::log
{
    void init_logger(const std::string &log_filename)
    {
        // 控制台日志槽
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v"); // 彩色

        // 文件日志槽
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_filename);
        file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v"); // 无颜色

        // 创建日志管理器
        auto logger = std::make_shared<spdlog::logger>(
            "default_logger",
            spdlog::sinks_init_list{console_sink, file_sink});
        // 设置日志等级
        logger->set_level(spdlog::level::info);
        // 设置刷新策略
        logger->flush_on(spdlog::level::warn);        // 遇到警告时立即刷新
        spdlog::flush_every(std::chrono::seconds(3)); // 每3s刷新一次

        // 设置默认日志管理器
        spdlog::set_default_logger(logger);

        spdlog::info("Logger {} initialized as level [{}]",
                     logger->name(),
                     spdlog::level::to_string_view(logger->level()));
    }
}
