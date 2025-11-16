#include "database/database.hpp"

#include <format>
#include <string>

#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "database/tb_servant_info.hpp"

namespace AreaSamsara::database
{
    void test_soci_insert(const std::string &user, const std::string &pwd, const std::string &host)
    {
        try
        {
            // 创建MySQL会话
            soci::session sql(soci::mysql, std::format("host={} user={} password='{}' db={}", host, user, pwd, TbServantInfo::db_name));

            // 执行插入操作
            TbServantInfo servant_info("卫宫士郎", "Archer", "男");
            TbServantInfo::insert(sql, servant_info);

            spdlog::info("Data inserted successfully");
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::format("fail to insert data: {}", e.what()));
        }
    }

    void test_soci_select(const std::string &user, const std::string &pwd, const std::string &host)
    {
        try
        {
            // 创建MySQL会话
            soci::session sql(soci::mysql, std::format("host={} user={} password='{}' db={}", host, user, pwd, TbServantInfo::db_name));

            // 执行查询操作
            std::vector<TbServantInfo> servants = TbServantInfo::select(sql, "servant_class like '%er'");

            nlohmann::ordered_json json_data = nlohmann::ordered_json::array();
            for (const auto &servant : servants)
            {
                json_data.push_back(servant.to_json());
            }
            spdlog::info(json_data.dump(-1, ' ', false));
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::format("fail to select data: {}", e.what()));
        }
    }
}
