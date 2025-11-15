#include "config/config.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <yaml-cpp/yaml.h>

namespace AreaSamsara::config
{
    const Config &Config::global_config()
    {
        static Config instance;
        instance.initialize();
        return instance;
    }

    namespace
    {
        // 读取必需配置字段
        template <typename T>
        T get_required(const YAML::Node &node, const std::string &key)
        {
            if (!node[key])
            {
                throw std::runtime_error("missing required field: " + key);
            }
            return node[key].as<T>();
        }

        // 读取可选配置字段
        template <typename T>
        T get_optional(const YAML::Node &node, const std::string &key, const T &defaultValue)
        {
            return node[key] ? node[key].as<T>() : defaultValue;
        }
    }

    void Config::initialize()
    {
        try
        {
            YAML::Node root = YAML::LoadFile(config_file_name);

            // 服务器配置
            if (!root["server"])
            {
                throw std::runtime_error("Missing 'server' section");
            }
            auto server_node = root["server"];
            server.host = get_required<std::string>(server_node, "host");
            server.port = get_required<int>(server_node, "port");

            // 数据库配置
            if (!root["database"])
            {
                throw std::runtime_error("Missing 'database' section");
            }
            auto db_node = root["database"];
            database.user = get_required<std::string>(db_node, "user");
            database.pwd = get_required<std::string>(db_node, "pwd");
            database.host = get_required<std::string>(db_node, "host");
            database.port = get_required<int>(db_node, "port");

            // 日志配置
            if (!root["logger"])
            {
                throw std::runtime_error("Missing 'database' section");
            }
            auto log_node = root["logger"];
            logger.file_name = get_required<std::string>(log_node, "file_name");
        }
        catch (const std::exception &e)
        {
            spdlog::critical("Error Config::initialize(): {}", e.what());
            std::exit(EXIT_FAILURE);
        }
    }
}