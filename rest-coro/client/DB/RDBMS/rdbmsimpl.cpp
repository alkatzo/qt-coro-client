#include "DB/RDBMS/rdbmsimpl.h"

#include <QDateTime>
#include <QRegularExpression>
#include <QStandardItem>

namespace db { namespace rdbms {

void ODBCBuilderDyn::refresh() {
    QString dbType = db::rdbms::MSODBCAdapter::type;

    if (isMssql(dbType)) {
        adapter = std::make_shared<MSODBCAdapter>();
        dsn = db::rdbms::MSODBCAdapter::dsn();
    }
    else {
        qCritical() << "Could not construct ODBC adapter for dbType" << dbType;
    }
}

QCoro::AsyncGenerator<QList<QString> > RdbmsImpl::peopleGet(QDateTime &dbTime, SqlQuery query) {
    return {};
}

}}
