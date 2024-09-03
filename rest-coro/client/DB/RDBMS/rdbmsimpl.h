#pragma once

#include <QObject>
#include <QtSql>
#include <QtDebug>
#include <QCoroAsyncGenerator>

#include "DB/RDBMS/connectionmanager.h"
#include "DB/RDBMS/sqlquery.h"

namespace db { namespace rdbms {

inline bool isMssql(const QString &name) {
    return name.toLower() == MSODBCAdapter::type;
}

//type traits for differnt use cases
struct ODBCBuilderDyn {
    static void refresh();
    inline static QString dsn{};
    inline static std::shared_ptr<db::rdbms::IODBCAdapter> adapter{};
};

using ConMgrT = db::rdbms::ODBCConnectionManager<ODBCBuilderDyn>;

template<typename ODBCAdapterT>
struct ODBCBuilder {
    static void refresh() {
        dsn = ODBCAdapterT::dsn();
        adapter = std::make_shared<ODBCAdapterT>();
    }
    inline static QString dsn{};
    inline static std::shared_ptr<db::rdbms::IODBCAdapter> adapter{};
};

template<typename DbT>
struct DBTraits {
    using ODBCT = ODBCBuilder<typename DbT::ODBCAdapterT>;
    using ConMgrT = db::rdbms::ODBCConnectionManager<ODBCT>;
};

/**
 * @brief The RdbmsImpl class
 * General RDBMS Backend implementation for SQL server
 */
class RdbmsImpl : public QObject {
    Q_OBJECT
public:
    RdbmsImpl() = default;
    virtual ~RdbmsImpl() = default;

    QCoro::AsyncGenerator<QList<QString>> peopleGet(QDateTime &dbTime, SqlQuery query);
};

}}
