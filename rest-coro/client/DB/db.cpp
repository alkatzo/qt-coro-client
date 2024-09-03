#include "db.h"
#include "DB/backend.h"
#include "DB/REST/restapiimpl.h"
#include "DB/REST/restexecutor.h"

#include "DB/coroexecutor.h"
#include "DB/RDBMS/dbpoolexecutor.h"
#include "DB/RDBMS/rdbmsimpl.h"
#include "DB/RDBMS/mssql.h"

template<typename T> using SimpleExecutorT = db::SimpleExecutor<T>;
template<typename T> using ExecutorT = db::CoroExecutor<T>;
template<typename T> using DBExecutorT = db::rdbms::DBPoolExecutor<T>;

namespace db {

Db *Db::makeDB() {
    if (the != nullptr) {
        delete the;
        the = nullptr;
    }

    QString driver = "mssql";

    if (db::rdbms::isMssql(driver)) {
        using DBTraitsT = db::rdbms::DBTraits<db::rdbms::MSSQL>;
        DBTraitsT::ODBCT::refresh();
        the = new db::Backend<db::rdbms::RdbmsImpl, db::rest::RestExecutor>;
    }
    else if (db::rest::isRestApi(driver)) {
        the = new db::Backend<db::rest::RestApiImpl, db::rest::RestExecutor>;
    }

    if (!the) {
        qCritical() << "Could not create backend connector for" << driver << ". Supported values are \"mssql\" and \"restapi\"";
        throw std::exception();
    }

    return the;
}

}
