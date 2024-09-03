#pragma once

#include "rdbmsimpl.h"

namespace db { namespace rdbms {

struct MSSQL
{
    using ODBCAdapterT  = MSODBCAdapter;
};

}}
