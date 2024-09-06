#pragma once

#include <QObject>
#include <QDateTime>

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

    QList<QString> peopleGet(QDateTime dbTime, SqlQuery &query);
    QList<QString> peopleGetAll(QDateTime dbTime, SqlQuery &query);
};

}}
