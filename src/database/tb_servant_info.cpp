#include "database/tb_servant_info.hpp"

#include <format>

namespace AreaSamsara::database
{
    void TbServantInfo::insert(soci::session &sql, const TbServantInfo &servant_info)
    {
        sql << std::format("INSERT INTO {}(servant_name,servant_class,servant_gender) "
                           "VALUES(:servant_name, :servant_class, :servant_gender)",
                           table_name),
            soci::use(servant_info.servant_name_, "servant_name"),
            soci::use(servant_info.servant_class_, "servant_class"),
            soci::use(servant_info.servant_gender_, "servant_gender");
    }

    std::vector<TbServantInfo> TbServantInfo::select(soci::session &sql, const std::string &where_condition)
    {
        // 构建select语句
        std::string select_str = std::format(
            "SELECT * FROM {}",
            table_name);

        // 如果有where条件，添加之
        if (!where_condition.empty())
        {
            select_str += " WHERE " + where_condition;
        }

        // 提取选中的数据
        std::vector<TbServantInfo> servants;
        soci::rowset<soci::row> rows = (sql.prepare << select_str);
        for (auto it = rows.begin(); it != rows.end(); ++it)
        {
            TbServantInfo servant_info(it->get<std::string>("servant_name"),
                                       it->get<std::string>("servant_class"),
                                       it->get<std::string>("servant_gender"));
            servant_info.id_ = it->get<uint32_t>("id");

            servants.push_back(std::move(servant_info));
        }

        return servants;
    }
}
