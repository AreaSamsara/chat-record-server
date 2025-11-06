#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace AreaSamsara::log
{
    // 初始化日志管理器
    void init_logger(const std::string &log_filename);
}