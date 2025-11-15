#pragma once

#include <string>

namespace AreaSamsara::database
{
    void test_soci_insert(const std::string &user, const std::string &pwd, const std::string &host);
    void test_soci_select(const std::string &user, const std::string &pwd, const std::string &host);
}
