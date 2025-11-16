#pragma once

#include <string>
#include <vector>
#include <chrono>

#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <nlohmann/json.hpp>

namespace AreaSamsara::database
{
    // 用户信息数据表
    class TbUserInfo
    {
    public:
        TbUserInfo() {}
        TbUserInfo(const std::string &user_name, const std::string &email, const std::string &phone, const std::string &password_hash,
                   const std::chrono::system_clock::time_point &created_at = std::chrono::system_clock::now(),
                   const std::chrono::system_clock::time_point &updated_at = std::chrono::system_clock::now())
            : user_name_(user_name), email_(email), phone_(phone), password_hash_(password_hash),
              created_at_(created_at), updated_at_(updated_at)
        {
        }

    public:
        // 插入一行数据
        static void insert(soci::session &sql, const TbUserInfo &user_info);
        // 查找数据
        static std::vector<TbUserInfo> select(soci::session &sql, const std::string &where_condition = "");
        // 验证数据是否存在
        static bool exists(soci::session &sql, const std::string &where_condition = "");

    public:
        // 数据库名称
        static inline const std::string db_name = "db_ai_chat_info";
        // 数据表名称
        static inline const std::string table_name = "tb_user_info";

    private:
        uint64_t id_ = 0;                                  // 自增ID
        std::string user_name_;                            // 用户名称
        std::string email_;                                // 邮件地址
        std::string phone_;                                // 电话号码
        std::string password_hash_;                        // 密码哈希值
        std::chrono::system_clock::time_point created_at_; // 创建时间
        std::chrono::system_clock::time_point updated_at_; // 更新时间

    public:
        // 自增ID
        uint64_t id() const { return id_; }
        // 用户名称
        std::string user_name() const noexcept { return user_name_; }
        // 邮件地址
        std::string email() const noexcept { return email_; }
        // 电话号码
        std::string phone() const noexcept { return phone_; }
        // 密码哈希值
        std::string password_hash() const noexcept { return password_hash_; }
        // 创建时间
        std::chrono::system_clock::time_point created_at() const noexcept { return created_at_; }
        // 更新时间
        std::chrono::system_clock::time_point updated_at() const noexcept { return updated_at_; }

        nlohmann::ordered_json to_json() const
        {
            return {
                {"id", id_},
                {"user_name", user_name_},
                {"email", email_},
                {"phone", phone_},
                {"password_hash", password_hash_},
                {"created_at", std::format("{:%Y-%m-%d %H:%M:%S}", created_at_)},
                {"updated_at", std::format("{:%Y-%m-%d %H:%M:%S}", updated_at_)}};
        }

        static TbUserInfo from_json(const nlohmann::ordered_json &json_data)
        {
            return TbUserInfo(json_data.value("user_name", ""),
                              json_data.value("email", ""),
                              json_data.value("phone", ""),
                              json_data.value("password_hash", ""));
        }
    };
}