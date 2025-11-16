#include "database/tb_user_info.hpp"

#include <format>

namespace AreaSamsara::database
{
    void TbUserInfo::insert(soci::session &sql, const TbUserInfo &user_info)
    {
        sql << std::format("INSERT INTO {}(user_name, email, phone, password_hash) "
                           "VALUES(:user_name, :email, :phone, :password_hash)",
                           table_name),
            soci::use(user_info.user_name_, "user_name"),
            soci::use(user_info.email_, "email"),
            soci::use(user_info.phone_, "phone"),
            soci::use(user_info.password_hash_, "password_hash");
    }

    std::vector<TbUserInfo> TbUserInfo::select(soci::session &sql, const std::string &where_condition)
    {
        // 构建select语句
        std::string select_sql = std::format(
            "SELECT * FROM {}",
            table_name);

        // 如果有where条件，添加之
        if (!where_condition.empty())
        {
            select_sql += " WHERE " + where_condition;
        }

        // 提取选中的数据
        std::vector<TbUserInfo> user_infos;
        soci::rowset<soci::row> rows = (sql.prepare << select_sql);
        for (auto it = rows.begin(); it != rows.end(); ++it)
        {
            // 使用std::tm接收数据库时间
            std::tm created_at_tm = it->get<std::tm>("created_at");
            std::tm updated_at_tm = it->get<std::tm>("updated_at");

            TbUserInfo user_info(it->get<std::string>("user_name"),
                                 it->get<std::string>("email"),
                                 it->get<std::string>("phone"),
                                 it->get<std::string>("password_hash"),
                                 std::chrono::system_clock::from_time_t(std::mktime(&created_at_tm)),
                                 std::chrono::system_clock::from_time_t(std::mktime(&updated_at_tm)));
            user_info.id_ = it->get<uint64_t>("id");

            user_infos.push_back(std::move(user_info));
        };
        return user_infos;
    }

    bool TbUserInfo::exists(soci::session &sql, const std::string &where_condition)
    {
        // 构建select语句
        std::string exists_sql = std::format(
            "SELECT EXISTS(SELECT 1 FROM {}",
            table_name);

        // 如果有where条件，添加之
        if (!where_condition.empty())
        {
            exists_sql += " WHERE " + where_condition;
        }
        exists_sql += ")";

        int exists = 0;
        sql << exists_sql, soci::into(exists);

        return exists != 0;
    }
}
