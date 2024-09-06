#include "db.h"
#include "DB/backend.h"

#include "DB/REST/restapiimpl.h"
#include "DB/REST/restexecutor.h"

#include "DB/RDBMS/dbpoolexecutor.h"
#include "DB/RDBMS/rdbmsimpl.h"
#include "DB/RDBMS/dbtraits.h"
#include "DB/RDBMS/mssql.h"

#include "DB/concepts.h"
#include "DB/deleters.h"

namespace db {

Db *Db::makeDB() {
    if (the != nullptr) {
        delete the;
        the = nullptr;
    }

    // QString driver = "mssql";
    QString driver = "restapi";

    if (db::rdbms::isMssql(driver)) {
        using DBTraitsT = db::rdbms::DBTraits<db::rdbms::MSSQL>;
        DBTraitsT::ODBCT::refresh();
        the = new db::Backend<db::rdbms::RdbmsImpl, db::rdbms::DBPoolExecutor<DBTraitsT::ConMgrT>, Deleter>;
    }
    else if (db::rest::isRestApi(driver)) {
        the = new db::Backend<db::rest::RestApiImpl, db::rest::RestExecutor, Deleter>;
    }

    if (!the) {
        qCritical() << "Could not create backend connector for" << driver << ". Supported values are \"mssql\" and \"restapi\"";
        throw std::exception();
    }

    return the;
}

}
