#include "database/database.hpp"

#include <format>
#include <iostream>

#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <json/json.h>

#include "database/servant_info.hpp"

namespace AreaSamsara::database
{
    // 用户名
    const std::string user = "root";
    // 密码
    const std::string pwd = "A1351989001";
    // 主机
    const std::string host = "localhost";
    // 数据库名称
    const std::string db_name = "db_test";

    void test_soci_insert()
    {
        try
        {
            // 创建MySQL会话
            soci::session sql(soci::mysql, std::format("host={} user={} password='{}' db={}", host, user, pwd, db_name));

            // 执行插入操作
            ServantInfo servant_info("卫宫士郎", "Archer", "男");
            ServantInfo::insert(sql, servant_info);

            spdlog::info("Data inserted successfully");
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::format("fail to insert data: {}", e.what()));
        }
    }

    void test_soci_select()
    {
        try
        {
            // 创建MySQL会话
            soci::session sql(soci::mysql, std::format("host={} user={} password='{}' db={}", host, user, pwd, db_name));

            // 执行查询操作
            std::vector<ServantInfo> servants = ServantInfo::select(sql, "servant_class like '%er'");

            // 进行序列化
            Json::Value servant_datas(Json::arrayValue);
            for (auto &servant : servants)
            {
                servant_datas.append(servant.to_json());
            }

            Json::StreamWriterBuilder writer;
            writer.settings_["emitUTF8"] = true;
            spdlog::info(Json::writeString(writer, servant_datas));
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::format("fail to select data: {}", e.what()));
        }
    }
}
