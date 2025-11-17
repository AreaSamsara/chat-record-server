#pragma once

#include <string>

#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>

namespace AreaSamsara::database
{
    // 创建MySQL会话
    soci::session new_sql_session(const std::string db_name);
}
