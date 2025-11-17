#include "database/database.hpp"

#include <format>
#include <string>

#include "config/config.hpp"

namespace AreaSamsara::database
{
    soci::session new_sql_session(const std::string db_name)
    {
        auto &database_config = config::Config::global_config().database;
        return soci::session(
            soci::mysql,
            std::format("host={} user={} password='{}' db={}", database_config.host,
                        database_config.user, database_config.pwd, db_name));
    }
}
