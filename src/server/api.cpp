#include "server/api.hpp"

#include <format>
#include <json/json.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

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

    std::map<std::string, HttpHandler> get_handlers = {
        {"/Hello", hello_handler}};

    // POST Echo
    void echo_handler(const httplib::Request &req, httplib::Response &rsp)
    {
        spdlog::info("POST /Echo -> receive post data: {}", req.body);

        Json::Value response_json;
        response_json["status"] = "success";
        response_json["message"] = std::format("Data received: {}", req.body);

        Json::StreamWriterBuilder writer;
        writer.settings_["emitUTF8"] = true;
        rsp.set_content(Json::writeString(writer, response_json), "application/json");
    }

    // POST SignUp
    void sign_up_handler(const httplib::Request &req, httplib::Response &rsp)
    {
        // spdlog::info("POST /SignUp -> receive post data: {}", req.body);

        // Json::Value request_json;
        // std::istringstream stream(req.body);
        // std::string errors;
        // Json::parseFromStream(Json::CharReaderBuilder(), stream, &request_json, &errors);

        // std::string user_name = request_json["user_name"].asString();
        // std::string email = request_json["email"].asString();
        // std::string phone = request_json["phone"].asString();

        // Json::Value response_json;
        // response_json["status"] = "success";
        // response_json["message"] = std::format("Data received: {}", req.body);

        // Json::StreamWriterBuilder writer;
        // writer.settings_["emitUTF8"] = true;
        // rsp.set_content(Json::writeString(writer, response_json), "application/json");
    }

    std::map<std::string, HttpHandler> post_handlers = {
        {"/Echo", echo_handler},
        {"/SignUp", sign_up_handler}};
}