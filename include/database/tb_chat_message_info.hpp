#pragma once

#include <string>
#include <vector>
#include <chrono>

#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <nlohmann/json.hpp>

#include "util/time.hpp"

namespace AreaSamsara::database
{
    // 聊天消息信息数据表
    class TbChatMessageInfo
    {
    public:
        TbChatMessageInfo() {}
        TbChatMessageInfo(const uint64_t conversation_id, const std::string &user_name, const std::string &message_type,
                          const std::string &message_content, const uint32_t token_count,
                          const std::chrono::system_clock::time_point &created_at = std::chrono::system_clock::now())
            : conversation_id_(conversation_id), user_name_(user_name), message_type_(message_type),
              message_content_(message_content), token_count_(token_count), created_at_(created_at)
        {
        }

    public:
        // 插入一行数据
        static void insert(soci::session &sql, const TbChatMessageInfo &conversation_info);
        // 查找数据
        static std::vector<TbChatMessageInfo> select(soci::session &sql, const std::string &where_condition = "");
        // 验证数据是否存在
        static bool exists(soci::session &sql, const std::string &where_condition = "");

    public:
        // 数据库名称
        static inline const std::string db_name = "db_ai_chat_info";
        // 数据表名称
        static inline const std::string table_name = "tb_chat_message_info";

    private:
        uint64_t id_ = 0;                                  // 自增ID
        uint64_t conversation_id_ = 0;                     // 会话ID
        std::string user_name_;                            // 用户名称
        std::string message_type_;                         // 消息类型：user/assistant/system
        std::string message_content_;                      // 消息内容
        uint32_t token_count_;                             // 消息的token数量
        std::chrono::system_clock::time_point created_at_; // 创建时间

    public:
        // 自增ID
        uint64_t id() const noexcept { return id_; }
        // 会话ID
        uint64_t conversation_id() const noexcept { return conversation_id_; }
        // 用户名称
        std::string user_name() const noexcept { return user_name_; }
        // 消息类型
        std::string message_type() const noexcept { return message_type_; }
        // 消息内容
        std::string message_content() const noexcept { return message_content_; }
        // 消息的token数量
        uint32_t token_count() const noexcept { return token_count_; }
        // 创建时间
        std::chrono::system_clock::time_point created_at() const noexcept { return created_at_; }

        nlohmann::ordered_json to_json() const
        {
            return {
                {"id", id_},
                {"conversation_id", conversation_id_},
                {"user_name", user_name_},
                {"message_type", message_type_},
                {"message_content", message_content_},
                {"token_count", token_count_},
                {"created_at", util::time_point_to_string(created_at_)}};
        }

        static TbChatMessageInfo from_json(const nlohmann::ordered_json &json_data)
        {
            return TbChatMessageInfo(
                json_data.value("conversation_id", 0),
                json_data.value("user_name", ""),
                json_data.value("message_type", ""),
                json_data.value("message_content", ""),
                json_data.value("token_count", 0));
        }
    };
}