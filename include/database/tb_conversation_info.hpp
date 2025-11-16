#pragma once

#include <string>
#include <vector>
#include <chrono>

#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <nlohmann/json.hpp>

namespace AreaSamsara::database
{
    // 聊天会话信息数据表
    class TbConversationInfo
    {
    public:
        TbConversationInfo() {}
        TbConversationInfo(const std::string &user_name, const std::string &conversation_name,
                           const std::chrono::system_clock::time_point &created_at = std::chrono::system_clock::now(),
                           const std::chrono::system_clock::time_point &updated_at = std::chrono::system_clock::now())
            : user_name_(user_name), conversation_name_(conversation_name),
              created_at_(created_at), updated_at_(updated_at)
        {
        }

    public:
        // 插入一行数据
        static void insert(soci::session &sql, const TbConversationInfo &conversation_info);
        // 查找数据
        static std::vector<TbConversationInfo> select(soci::session &sql, const std::string &where_condition = "");
        // 验证数据是否存在
        static bool exists(soci::session &sql, const std::string &where_condition = "");

    public:
        // 数据库名称
        static inline const std::string db_name = "db_ai_chat_info";
        // 数据表名称
        static inline const std::string table_name = "tb_conversation_info";

    private:
        uint64_t id_ = 0;                                         // 自增ID
        std::string user_name_;                                   // 用户名称
        std::string conversation_name_;                           // 会话名称
        std::chrono::system_clock::time_point last_message_time_; // 最后消息时间
        std::chrono::system_clock::time_point created_at_;        // 创建时间
        std::chrono::system_clock::time_point updated_at_;        // 更新时间

    public:
        // 自增ID
        uint64_t id() const noexcept { return id_; }
        // 用户名称
        std::string user_name() const noexcept { return user_name_; }
        // 会话名称
        std::string conversation_name() const noexcept { return conversation_name_; }
        // 最后消息时间
        std::chrono::system_clock::time_point last_message_time() const noexcept { return last_message_time_; }
        // 创建时间
        std::chrono::system_clock::time_point created_at() const noexcept { return created_at_; }
        // 更新时间
        std::chrono::system_clock::time_point updated_at() const noexcept { return updated_at_; }

        nlohmann::ordered_json to_json() const
        {
            return {
                {"id", id_},
                {"user_name", user_name_},
                {"conversation_name", conversation_name_},
                {"last_message_time", std::format("{:%Y-%m-%d %H:%M:%S}", last_message_time_)},
                {"created_at", std::format("{:%Y-%m-%d %H:%M:%S}", created_at_)},
                {"updated_at", std::format("{:%Y-%m-%d %H:%M:%S}", updated_at_)}};
        }

        static TbConversationInfo from_json(const nlohmann::ordered_json &json_data)
        {
            return TbConversationInfo(json_data.value("user_name", ""),
                                      json_data.value("conversation_name", ""));
        }
    };
}