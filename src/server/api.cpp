#include "server/api.hpp"

#include <format>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "database/tb_user_info.hpp"
#include "config/config.hpp"
#include "server/http_status.hpp"

namespace AreaSamsara::server
{
    // GET Hello
    void hello_handler(const httplib::Request &req, httplib::Response &rsp)
    {
        auto name = req.get_param_value("name");
        spdlog::info("GET /Hello -> name: {}", name);

        std::string response_content = std::format("Hello, {}", name);
        rsp.set_content(response_content, "text/plain");
    }

    // GET UserList
    void user_list_handler(const httplib::Request &req, httplib::Response &rsp)
    {
        spdlog::info("GET /UserList");

        // 响应结构体
        struct Response
        {
            std::vector<database::TbUserInfo> user_infos; // 用户信息列表
            std::string error;                            // 错误

            nlohmann::ordered_json to_json() const
            {
                nlohmann::ordered_json json_data;

                json_data["user_infos"] = nlohmann::ordered_json::array();
                for (const auto &user_info : user_infos)
                {
                    json_data["user_infos"].emplace_back(user_info.to_json());
                }

                json_data["error"] = error;

                return json_data;
            }

            static Response from_json(const nlohmann::ordered_json &json_data)
            {
                Response response;

                for (const auto &item : json_data["user_infos"])
                {
                    response.user_infos.emplace_back(database::TbUserInfo::from_json(item));
                }

                response.error = json_data.value("error", "");

                return response;
            }
        };

        // 错误处理函数
        auto handle_error = [](httplib::Response &rsp, const std::string &error_msg, const int status_code)
        {
            // 打印错误日志
            spdlog::error(error_msg);

            Response response;
            response.error = error_msg;

            // 设置响应体
            rsp.status = status_code;
            rsp.set_content(response.to_json().dump(-1, ' ', false), "application/json");

            // 打印响应发送日志
            spdlog::info("POST /SignUp -> send response data: {}", rsp.body);
        };

        // 创建MySQL会话
        auto &database_config = config::Config::global_config().database;
        soci::session sql(
            soci::mysql,
            std::format("host={} user={} password='{}' db={}", database_config.host,
                        database_config.user, database_config.pwd, database::TbUserInfo::db_name));

        // 查询角色信息
        Response response;
        try
        {
            response.user_infos = database::TbUserInfo::select(sql, "");
        }
        catch (const std::exception &e)
        {
            std::string error_msg = std::format("error mysql select: {}", e.what());
            handle_error(rsp, error_msg, http_status::InternalServerError);
            return;
        }

        rsp.status = http_status::OK;
        rsp.set_content(response.to_json().dump(-1, ' ', false), "application/json");

        spdlog::info("GET /UserList -> send response data: {}", rsp.body);
    }

    std::map<std::string, HttpHandler> get_handlers = {
        {"/Hello", hello_handler},
        {"/UserList", user_list_handler}};

    // POST Echo
    void echo_handler(const httplib::Request &req, httplib::Response &rsp)
    {
        spdlog::info("POST /Echo -> receive post data: {}", req.body);

        rsp.set_content("{}", "application/json");
    }

    // POST SignUp
    void sign_up_handler(const httplib::Request &req, httplib::Response &rsp)
    {
        spdlog::info("POST /SignUp -> receive post data: {}", req.body);

        // 响应结构体
        struct Response
        {
            std::string message; // 消息
            std::string error;   // 错误

            nlohmann::ordered_json to_json() const
            {
                return {
                    {"message", message},
                    {"error", error}};
            }

            static Response from_json(const nlohmann::ordered_json &json_data)
            {
                return Response(json_data.value("message", ""),
                                json_data.value("error", ""));
            }
        };

        // 错误处理函数
        auto handle_error = [](httplib::Response &rsp, const std::string &error_msg, const int status_code)
        {
            // 打印错误日志
            spdlog::error(error_msg);

            Response response;
            response.message = "";
            response.error = error_msg;

            // 设置响应体
            rsp.status = status_code;
            rsp.set_content(response.to_json().dump(-1, ' ', false), "application/json");

            // 打印响应发送日志
            spdlog::info("POST /SignUp -> send response data: {}", rsp.body);
        };

        // 解析请求体
        database::TbUserInfo user_info;
        try
        {
            user_info = database::TbUserInfo::from_json(nlohmann::ordered_json::parse(req.body));
        }
        catch (const std::exception &e)
        {
            std::string error_msg = std::format("error parsing request data: {}", e.what());
            handle_error(rsp, error_msg, http_status::BadRequest);
            return;
        }

        // 创建MySQL会话
        auto &database_config = config::Config::global_config().database;
        soci::session sql(
            soci::mysql,
            std::format("host={} user={} password='{}' db={}", database_config.host,
                        database_config.user, database_config.pwd, database::TbUserInfo::db_name));

        // 检查用户是否已注册
        try
        {
            // 如果已经注册过了，则不允许重复注册
            if (database::TbUserInfo::exists(sql, std::format("user_name = '{}'", user_info.user_name())))
            {
                std::string error_msg = std::format("User {} has already signed up!", user_info.user_name());
                handle_error(rsp, error_msg, http_status::Conflict);
                return;
            }
        }
        catch (const std::exception &e)
        {
            std::string error_msg = std::format("error mysql exists: {}", e.what());
            handle_error(rsp, error_msg, http_status::InternalServerError);
            return;
        }

        // 插入数据
        try
        {
            database::TbUserInfo::insert(sql, user_info);
        }
        catch (const std::exception &e)
        {
            std::string error_msg = std::format("error mysql insert: {}", e.what());
            handle_error(rsp, error_msg, http_status::InternalServerError);
            return;
        }

        Response response;
        response.message = std::format("Succeed to sign up for user {}", user_info.user_name());
        response.error = "";

        rsp.status = http_status::OK;
        rsp.set_content(response.to_json().dump(-1, ' ', false), "application/json");

        spdlog::info("POST /SignUp -> send response data: {}", rsp.body);
    }

    std::map<std::string, HttpHandler> post_handlers = {
        {"/Echo", echo_handler},
        {"/SignUp", sign_up_handler}};
}