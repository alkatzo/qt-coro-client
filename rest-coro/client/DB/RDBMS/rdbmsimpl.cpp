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

QCoro::AsyncGenerator<QList<QString> > RdbmsImpl::peopleGet(QDateTime &dbTime) {
    QList<QString> ret = co_await _peopleGet(dbTime);
    co_yield ret;
}

QCoro::Task<QList<QString> > RdbmsImpl::_peopleGet(QDateTime &dbTime) {
    co_return QtConcurrent::run(&threadpool, []() {
        QThread::sleep(3);
        return QList<QString>{"Hello", "Bonjour", "Pryvit"};
    });
}

}}
