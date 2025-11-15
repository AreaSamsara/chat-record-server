#include "server/api.hpp"

#include <format>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "database/user_info.hpp"

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

        // try
        // {
        //     database::UserInfo::select();
        //     database::UserInfo user_info = database::UserInfo::from_json(nlohmann::ordered_json::parse(req.body));
        //     nlohmann::ordered_json response_json = user_info.to_json();
        //     rsp.set_content(response_json.dump(-1, ' ', false), "application/json");
        // }
        // catch (const std::exception &e)
        // {
        //     spdlog::error("error from_json(): {}", e.what());
        //     return;
        // }

        // spdlog::info("POST /SignUp -> send response data: {}", rsp.body);
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

        try
        {
            database::UserInfo user_info = database::UserInfo::from_json(nlohmann::ordered_json::parse(req.body));
            nlohmann::ordered_json response_json = user_info.to_json();
            rsp.set_content(response_json.dump(-1, ' ', false), "application/json");
        }
        catch (const std::exception &e)
        {
            spdlog::error("error from_json(): {}", e.what());
            return;
        }

        spdlog::info("POST /SignUp -> send response data: {}", rsp.body);
    }

    std::map<std::string, HttpHandler> post_handlers = {
        {"/Echo", echo_handler},
        {"/SignUp", sign_up_handler}};
}