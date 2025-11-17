#include "database/tb_chat_message_info.hpp"

namespace AreaSamsara::database
{
    void TbChatMessageInfo::insert(soci::session &sql, const TbChatMessageInfo &conversation_info)
    {
        sql << std::format("INSERT INTO {}(conversation_id, user_name, message_type, message_content, token_count) "
                           "VALUES(:conversation_id, :user_name, :message_type, :message_content, :token_count)",
                           table_name),
            soci::use(conversation_info.conversation_id_, "conversation_id"),
            soci::use(conversation_info.user_name_, "user_name"),
            soci::use(conversation_info.message_type_, "message_type"),
            soci::use(conversation_info.message_content_, "message_content"),
            soci::use(conversation_info.message_type_, "token_count");
    }

    std::vector<TbChatMessageInfo> TbChatMessageInfo::select(soci::session &sql, const std::string &where_condition)
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
        std::vector<TbChatMessageInfo> message_infos;
        soci::rowset<soci::row> rows = (sql.prepare << select_sql);
        for (auto it = rows.begin(); it != rows.end(); ++it)
        {
            TbChatMessageInfo message_info(it->get<std::uint64_t>("conversation_id"),
                                           it->get<std::string>("user_name"),
                                           it->get<std::string>("message_type"),
                                           it->get<std::string>("message_content"),
                                           it->get<std::uint32_t>("token_count"));

            message_info.id_ = it->get<uint64_t>("id");

            message_infos.push_back(std::move(message_info));
        };
        return message_infos;
    }

    bool TbChatMessageInfo::exists(soci::session &sql, const std::string &where_condition)
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