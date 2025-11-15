#pragma once

#include <iostream>

namespace AreaSamsara::config
{
    class Config
    {
    public:
        // 服务器配置
        struct ServerConfig
        {
            int port = 0;
            std::string host;
        };
        // 数据库配置
        struct DatabaseConfig
        {
            std::string user;
            std::string pwd;
            std::string host;
            int port = 0;
        };
        // 日志配置
        struct LoggerConfig
        {
            std::string file;
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
        ServerConfig server;
        DatabaseConfig database;
        LoggerConfig logger;

    private:
        bool initialized = false;
    };
}