#include "db.h"
#include "DB/backend.h"

#include "DB/REST/restexecutor.h"
#include "DB/REST/restapiimpl.h"

#include "DB/RDBMS/dbpoolexecutor.h"
#include "DB/RDBMS/rdbmsimpl.h"
#include "DB/RDBMS/dbtraits.h"

namespace db {

Db *Db::makeDB() {

    // QString driver = "mssql";
    QString driver = "restapi";

    if (db::rdbms::isMssql(driver)) {
        using DBTraitsT = db::rdbms::DBTraits<db::rdbms::MSSQL>;
        DBTraitsT::ODBCT::refresh();
        the.reset(new db::Backend<db::rdbms::RdbmsImpl, db::rdbms::DBPoolExecutor<DBTraitsT::ConMgrT>>);
    }
    else if (db::rest::isRestApi(driver)) {
        the.reset(new db::Backend<db::rest::RestApiImpl, db::rest::RestExecutor>);
    }

    if (!the) {
        qCritical() << "Could not create backend connector for" << driver << ". Supported values are \"mssql\" and \"restapi\"";
        throw std::exception();
    }

    return the.get();
}

}
