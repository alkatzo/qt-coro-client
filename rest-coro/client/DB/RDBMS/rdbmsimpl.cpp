#include "DB/RDBMS/rdbmsimpl.h"

#include <QCoroTask>
#include <QCoroFuture>
#include <QtConcurrent>
#include <QDateTime>

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

QList<QString> RdbmsImpl::peopleGet(QDateTime &dbTime, SqlQuery query) {
    QThread::sleep(100);
    return QList<QString>{"Hello", "Bonjour", "Pryvit"};
}

}}
