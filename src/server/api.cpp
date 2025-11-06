#include "server/api.hpp"

namespace AreaSamsara::server
{
    void hello_handler(const httplib::Request &req, httplib::Response &rsp)
    {
        auto name = req.get_param_value("name");
        spdlog::info("GET /Hello -> name: {}", name);

        std::string response_content = std::format("Hello, {}", name);
        rsp.set_content(response_content, "text/plain");
    }

    void echo_handler(const httplib::Request &req, httplib::Response &rsp)
    {
        spdlog::info("POST /Echo -> receive post data: {}", req.body);

        std::string response_content =
            "{\"status\": \"success\", \"message\": \"Data received: " + req.body + "\"}";
        rsp.set_content(response_content, "application/json");
    }

    std::map<std::string, HttpHandler> get_handlers = {
        {"/Hello", hello_handler}};

    std::map<std::string, HttpHandler> post_handlers = {
        {"/Echo", echo_handler}};
}