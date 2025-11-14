#pragma once

#include <iostream>
#include <vector>

#include <json/json.h>
#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>

namespace AreaSamsara::database
{
    // 从者信息
    class ServantInfo
    {
    public:
        ServantInfo(const std::string &servant_name, const std::string &servant_class, const std::string &servant_gender)
            : servant_name_(servant_name), servant_class_(servant_class), servant_gender_(servant_gender)
        {
        }

    public:
        // 插入一行数据
        static void insert(soci::session &sql, const ServantInfo &servant_info);
        // 查找数据
        static std::vector<ServantInfo> select(soci::session &sql, const std::string &where_condition = "");

    public:
        // 数据库名称
        const static inline std::string db_name = "db_test";
        // 数据表名称
        const static inline std::string table_name = "tb_test";

    private:
        uint32_t id_ = 0;            // 自增ID
        std::string servant_name_;   // 从者名称
        std::string servant_class_;  // 从者职阶
        std::string servant_gender_; // 从者性别
    public:
        // 自增ID
        uint32_t id() const { return id_; }
        // 从者名称
        std::string servant_name() const { return servant_name_; }
        // 从者职阶
        std::string servant_class() const { return servant_class_; }
        // 从者性别
        std::string servant_gender() const { return servant_gender_; }

        Json::Value to_json() const
        {
            Json::Value json_data;
            json_data["id"] = id_;
            json_data["servant_name"] = servant_name_;
            json_data["servant_class"] = servant_class_;
            json_data["servant_gender"] = servant_gender_;
            return json_data;
        }
    };
}