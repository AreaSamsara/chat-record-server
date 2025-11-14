#pragma once

#include <iostream>
#include <vector>
#include <chrono>

#include <json/json.h>
#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>

namespace AreaSamsara::database
{
    // 用户信息
    class UserInfo
    {
    public:
        UserInfo(const std::string &user_name, const std::string &email, const std::string &phone, const std::string &password_hash,
                 const std::chrono::system_clock::time_point &created_at = std::chrono::system_clock::now(),
                 const std::chrono::system_clock::time_point &updated_at = std::chrono::system_clock::now())
            : user_name_(user_name), email_(email), phone_(phone), password_hash_(password_hash),
              created_at_(created_at), updated_at_(updated_at)
        {
        }

    public:
        // 插入一行数据
        static void insert(soci::session &sql, const UserInfo &user_info);
        // 查找数据
        static std::vector<UserInfo> select(soci::session &sql, const std::string &where_condition = "");

    public:
        Json::Value to_json() const
        {
            Json::Value json_data;
            json_data["id"] = id_;
            json_data["user_name"] = user_name_;
            json_data["email"] = email_;
            json_data["phone"] = phone_;
            json_data["password_hash"] = password_hash_;
            json_data["created_at"] = std::format("{:%Y-%m-%d %H:%M:%S}", created_at_);
            json_data["updated_at"] = std::format("{:%Y-%m-%d %H:%M:%S}", updated_at_);

            return json_data;
        }

    public:
        // 数据库名称
        const static inline std::string db_name = "db_ai_chat_info";
        // 数据表名称
        const static inline std::string table_name = "tb_user_info";

    private:
        uint64_t id_ = 0; // 自增ID
        std::string user_name_;
        std::string email_;
        std::string phone_;
        std::string password_hash_;
        std::chrono::system_clock::time_point created_at_;
        std::chrono::system_clock::time_point updated_at_;
    };
}