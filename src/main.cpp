#include <string>

#include "server/server.hpp"

int main()
{
    std::string name = "AS";
    std::cout << std::format("Hello world, {}!", name) << std::endl;
    AreaSamsara::server::run_server();

    return 0;
}