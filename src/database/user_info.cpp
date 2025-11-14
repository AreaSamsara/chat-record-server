#include "database/user_info.hpp"

#include <format>

namespace AreaSamsara::database
{
    void UserInfo::insert(soci::session &sql, const UserInfo &user_info)
    {
        sql << std::format("INSERT INTO {}(user_name,email,phone,password_hash) "
                           "VALUES(:user_name, :email, :phone, :password_hash)",
                           table_name),
            soci::use(user_info.user_name_, "user_name"),
            soci::use(user_info.email_, "email"),
            soci::use(user_info.phone_, "phone"),
            soci::use(user_info.password_hash_, "password_hash");
    }

    std::vector<UserInfo> UserInfo::select(soci::session &sql, const std::string &where_condition)
    {
        // 构建select语句
        std::string select_str = std::format(
            "SELECT user_name,email,phone,password_hash,created_at,updated_at FROM {}",
            table_name);

        // 如果有where条件，添加之
        if (!where_condition.empty())
        {
            select_str += " WHERE " + where_condition;
        }

        // 提取选中的数据
        std::vector<UserInfo> user_infos;
        soci::rowset<soci::row> rows = (sql.prepare << select_str);
        for (auto it = rows.begin(); it != rows.end(); ++it)
        {
            user_infos.push_back(UserInfo(
                it->get<std::string>("user_name"),
                it->get<std::string>("email"),
                it->get<std::string>("phone"),
                it->get<std::string>("password_hash"),
                it->get<std::chrono::system_clock::time_point>("created_at"),
                it->get<std::chrono::system_clock::time_point>("updated_at")));
        }

        return user_infos;
    }
}
