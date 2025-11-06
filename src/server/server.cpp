#include "server/server.hpp"
#include "server/api.hpp"

namespace AreaSamsara::server
{
    void run_server()
    {
        httplib::Server server;

        // 注册所有 GET 方法
        for (auto it : get_handlers)
        {
            std::string name = it.first;
            HttpHandler handler = it.second;
            server.Get(name, handler);
        }

        // 注册所有 POST 方法
        for (auto it : post_handlers)
        {
            std::string name = it.first;
            HttpHandler handler = it.second;
            server.Post(name, handler);
        }

        std::cout << "server start running..." << std::endl;
        server.listen("0.0.0.0", 8080);
    }
}
