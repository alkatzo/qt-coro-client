#pragma once

#include <QObject>
#include <QtSql>
#include <QtDebug>
#include <QFuture>
#include <QCoroTask>
#include <QCoroAsyncGenerator>
#include <QThread>

#include "DB/RDBMS/dbtraits.h"
#include "DB/RDBMS/sqlquery.h"

namespace db { namespace rdbms {

/**
 * @brief The RdbmsImpl class
 * General RDBMS Backend implementation for SQL server
 */
class RdbmsImpl : public QObject {
    Q_OBJECT
public:
    RdbmsImpl() = default;
    virtual ~RdbmsImpl() = default;

    QList<QString> peopleGet(QDateTime dbTime, SqlQuery query);
    QList<QString> peopleGetAll(QDateTime dbTime, SqlQuery query);
};

}}
