#include "database/database.hpp"

namespace AreaSamsara::database
{
    void test_soci()
    {
        try
        {
            std::string user = "root";
            std::string pwd = "A1351989001";
            std::string host = "localhost";
            std::string db_name = "db_test";

            // 1. 创建MySQL会话
            soci::session sql(soci::mysql, std::format("host={} user={} password='{}' db={}", host, user, pwd, db_name));

            class ServantInfo
            {
            public:
                ServantInfo(const std::string &servant_name, const std::string &servant_class, const std::string &servant_gender)
                    : servant_name(servant_name), servant_class(servant_class), servant_gender(servant_gender)
                {
                }

                std::string servant_name;
                std::string servant_class;
                std::string servant_gender;
            };

            ServantInfo servant_info("赫拉克勒斯", "Berserker", "男");

            // 2. 执行插入操作
            sql << "INSERT INTO tb_test(servant_name,servant_class,servant_gender)"
                   "VALUES(:name, :class, :gender)",
                soci::use(servant_info.servant_name, "name"),
                soci::use(servant_info.servant_class, "class"),
                soci::use(servant_info.servant_gender, "gender");

            std::cout << "Data inserted successfully" << std::endl;
        }
        catch (const soci::soci_error &e)
        {
            throw std::runtime_error(std::format("fail to insert data: {}", e.what()));
        }
        catch (const std::out_of_range &e)
        {
            // 特别捕获vector越界错误
            throw std::runtime_error(std::format("Soci内部错误(向量越界): {}", e.what()));
        }
    }
}
