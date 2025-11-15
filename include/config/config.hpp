#pragma once

#include <string>

namespace AreaSamsara::config
{
    class Config
    {
    public:
        struct ServerConfig
        {
            std::string host; // 主机
            int port = 0;     // 端口
        };
        struct DatabaseConfig
        {
            std::string user; // 用户
            std::string pwd;  // 密码
            std::string host; // 主机
            int port = 0;     // 端口
        };
        struct LoggerConfig
        {
            std::string file_name; // 日志文件名称
        };

    public:
        // 获取单例实例
        static const Config &global_config();

    private:
        // 私有构造函数
        Config() = default;
        // 禁用拷贝和赋值
        Config(const Config &) = delete;
        Config &operator=(const Config &) = delete;

        // 配置初始化
        void initialize();

    private:
        // 配置文件名称
        static inline std::string config_file_name = "./bin/config.yaml";

    public:
        ServerConfig server;     // 服务器配置
        DatabaseConfig database; // 数据库配置
        LoggerConfig logger;     // 日志配置

    private:
        bool initialized = false;
    };
}