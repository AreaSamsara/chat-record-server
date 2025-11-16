#include "database/tb_conversation_info.hpp"

#include <format>

namespace AreaSamsara::database
{
    void TbConversationInfo::insert(soci::session &sql, const TbConversationInfo &conversation_info)
    {
        // 将 time_point 转换为 std::tm
        time_t last_message_time_time_t = std::chrono::system_clock::to_time_t(conversation_info.last_message_time_);
        std::tm last_message_time_tm = *std::localtime(&last_message_time_time_t);

        sql << std::format("INSERT INTO {}(user_name, conversation_name, last_message_time) "
                           "VALUES(:user_name, :conversation_name, :last_message_time)",
                           table_name),
            soci::use(conversation_info.user_name_, "user_name"),
            soci::use(conversation_info.conversation_name_, "conversation_name"),
            soci::use(last_message_time_tm, "last_message_time");
    }

    std::vector<TbConversationInfo> TbConversationInfo::select(soci::session &sql, const std::string &where_condition)
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
        std::vector<TbConversationInfo> conversation_infos;
        soci::rowset<soci::row> rows = (sql.prepare << select_sql);
        for (auto it = rows.begin(); it != rows.end(); ++it)
        {
            // 使用std::tm接收数据库时间
            std::tm created_at_tm = it->get<std::tm>("created_at");
            std::tm updated_at_tm = it->get<std::tm>("updated_at");
            std::tm last_message_time_tm = it->get<std::tm>("last_message_time");

            TbConversationInfo conversation_info(it->get<std::string>("user_name"),
                                                 it->get<std::string>("conversation_name"),
                                                 std::chrono::system_clock::from_time_t(std::mktime(&created_at_tm)),
                                                 std::chrono::system_clock::from_time_t(std::mktime(&updated_at_tm)));

            conversation_info.id_ = it->get<uint64_t>("id");
            conversation_info.last_message_time_ = std::chrono::system_clock::from_time_t(std::mktime(&last_message_time_tm));

            conversation_infos.push_back(std::move(conversation_info));
        };
        return conversation_infos;
    }

    bool TbConversationInfo::exists(soci::session &sql, const std::string &where_condition)
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
