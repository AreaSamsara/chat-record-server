#include "server/api.hpp"

#include <format>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "database/database.hpp"
#include "database/tb_user_info.hpp"
#include "database/tb_conversation_info.hpp"
#include "server/http_status.hpp"

namespace AreaSamsara::server
{
    // GET UserList 获取用户列表
    void user_list_handler(const httplib::Request &req, httplib::Response &rsp)
    {
        const std::string handler_name = "GET /UserList";
        spdlog::info(handler_name);

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
        auto handle_error = [handler_name](httplib::Response &rsp, const std::string &error_msg, const int status_code)
        {
            // 打印错误日志
            spdlog::error(error_msg);

            Response response;
            response.error = error_msg;

            // 设置响应体
            rsp.status = status_code;
            rsp.set_content(response.to_json().dump(-1, ' ', false), "application/json");

            // 打印响应发送日志
            spdlog::info("{} -> send response data: {}", handler_name, rsp.body);
        };

        // 创建MySQL会话
        soci::session sql = database::new_sql_session(database::TbUserInfo::db_name);

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

        spdlog::info("{} -> send response data: {}", handler_name, rsp.body);
    }

    // GET ConversationList 获取指定用户聊天会话列表
    void conversation_list_handler(const httplib::Request &req, httplib::Response &rsp)
    {
        const std::string handler_name = "GET /ConversationList";
        auto user_name = req.get_param_value("user_name");
        spdlog::info("{} -> user_name: {}", handler_name, user_name);

        // 响应结构体
        struct Response
        {
            std::vector<database::TbConversationInfo> conversation_infos; // 聊天会话信息列表
            std::string error;                                            // 错误

            nlohmann::ordered_json to_json() const
            {
                nlohmann::ordered_json json_data;

                json_data["conversation_infos"] = nlohmann::ordered_json::array();
                for (const auto &conversation_info : conversation_infos)
                {
                    json_data["conversation_infos"].emplace_back(conversation_info.to_json());
                }

                json_data["error"] = error;

                return json_data;
            }

            static Response from_json(const nlohmann::ordered_json &json_data)
            {
                Response response;

                for (const auto &item : json_data["conversation_infos"])
                {
                    response.conversation_infos.emplace_back(database::TbConversationInfo::from_json(item));
                }

                response.error = json_data.value("error", "");

                return response;
            }
        };

        // 错误处理函数
        auto handle_error = [handler_name](httplib::Response &rsp, const std::string &error_msg, const int status_code)
        {
            // 打印错误日志
            spdlog::error(error_msg);

            Response response;
            response.error = error_msg;

            // 设置响应体
            rsp.status = status_code;
            rsp.set_content(response.to_json().dump(-1, ' ', false), "application/json");

            // 打印响应发送日志
            spdlog::info("{} -> send response data: {}", handler_name, rsp.body);
        };

        // 创建MySQL会话
        soci::session sql = database::new_sql_session(database::TbConversationInfo::db_name);

        // 查询聊天会话信息
        Response response;
        try
        {
            // 将查找结果按最后一条消息的时间降序排列
            std::string where_condition = std::format("user_name = '{}' ORDER BY last_message_time DESC", user_name);
            response.conversation_infos = database::TbConversationInfo::select(sql, where_condition);
        }
        catch (const std::exception &e)
        {
            std::string error_msg = std::format("error mysql select: {}", e.what());
            handle_error(rsp, error_msg, http_status::InternalServerError);
            return;
        }

        rsp.status = http_status::OK;
        rsp.set_content(response.to_json().dump(-1, ' ', false), "application/json");

        spdlog::info("{} -> send response data: {}", handler_name, rsp.body);
    }

    std::map<std::string, HttpHandler> get_handlers = {
        {"/UserList", user_list_handler},
        {"/ConversationList", conversation_list_handler}};

    // POST SignUp 用户注册
    void sign_up_handler(const httplib::Request &req, httplib::Response &rsp)
    {
        const std::string handler_name = "POST /SignUp";
        spdlog::info("{} -> receive post data: {}", handler_name, req.body);

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
        auto handle_error = [handler_name](httplib::Response &rsp, const std::string &error_msg, const int status_code)
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
            spdlog::info("{} -> send response data: {}", handler_name, rsp.body);
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
        soci::session sql = database::new_sql_session(database::TbUserInfo::db_name);

        // 如果用户已经注册，则不允许重复注册
        try
        {
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

        spdlog::info("{} -> send response data: {}", handler_name, rsp.body);
    }

    // POST NewConversation 新建聊天会话
    void new_conversation_handler(const httplib::Request &req, httplib::Response &rsp)
    {
        const std::string handler_name = "POST /NewConversation";
        spdlog::info("{} -> receive post data: {}", handler_name, req.body);

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
        auto handle_error = [handler_name](httplib::Response &rsp, const std::string &error_msg, const int status_code)
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
            spdlog::info("{} -> send response data: {}", handler_name, rsp.body);
        };

        // 解析请求体
        database::TbConversationInfo conversation_info;
        try
        {
            conversation_info = database::TbConversationInfo::from_json(nlohmann::ordered_json::parse(req.body));
        }
        catch (const std::exception &e)
        {
            std::string error_msg = std::format("error parsing request data: {}", e.what());
            handle_error(rsp, error_msg, http_status::BadRequest);
            return;
        }

        // 创建MySQL会话
        soci::session sql = database::new_sql_session(database::TbConversationInfo::db_name);

        // 如果用户尚未注册，禁止创建聊天会话
        try
        {
            if (!database::TbUserInfo::exists(sql, std::format("user_name = '{}'", conversation_info.user_name())))
            {
                std::string error_msg = std::format("User {} hasn't signed up yet!", conversation_info.user_name());
                handle_error(rsp, error_msg, http_status::UnprocessableEntity);
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
            database::TbConversationInfo::insert(sql, conversation_info);
        }
        catch (const std::exception &e)
        {
            std::string error_msg = std::format("error mysql insert: {}", e.what());
            handle_error(rsp, error_msg, http_status::InternalServerError);
            return;
        }

        Response response;
        response.message = std::format("Succeed to create new conversation {} for user {}",
                                       conversation_info.conversation_name(), conversation_info.user_name());
        response.error = "";

        rsp.status = http_status::OK;
        rsp.set_content(response.to_json().dump(-1, ' ', false), "application/json");

        spdlog::info("{} -> send response data: {}", handler_name, rsp.body);
    }

    std::map<std::string, HttpHandler> post_handlers = {
        {"/SignUp", sign_up_handler},
        {"/NewConversation", new_conversation_handler}};
}