#pragma once

#include <QSqlDatabase>
#include <QHash>

#include <memory>

#include "DB/RDBMS/odbcclient.h"
#include "DB/RDBMS/sqlquery.h"

namespace db { namespace rdbms {

/**
 * @brief The ODBCConnectionManager class
 * ODBC connection manager to be used with DBPoolExecutor f.i.
 */
template<typename ODBC>
class ODBCConnectionManager
{
public:
    using QueryT = SqlQuery;

public:
    ODBCConnectionManager() = default;
    ~ODBCConnectionManager() {
        for (const QString &conName : connections.keys()) {
            QSqlDatabase::removeDatabase(conName);
        }
    }

    SqlQuery makeQuery() {
        // Construct connection name
        QString conName = QString("%1 (%2)").arg(ODBC::dsn).arg((long long)QThread::currentThreadId());

        QSqlDatabase con;
        if(QSqlDatabase::contains(conName)) { // if conName exists use it            
            con = QSqlDatabase::database(conName);
            if (connections.contains(conName) && connections.value(conName) == 0) {
                connections.insert(conName, 1);
                QString options = QString("SQL_ATTR_LOGIN_TIMEOUT=%1;SQL_ATTR_CONNECTION_TIMEOUT=%1").arg(normalConnectionTimeout);
                con.setConnectOptions(options);
            }
        }
        else { // esle create a new connection
            con = QSqlDatabase::addDatabase("QODBC", conName);
            QString options = QString("SQL_ATTR_LOGIN_TIMEOUT=%1;SQL_ATTR_CONNECTION_TIMEOUT=%1").arg(initialConnectionTimeout);
            con.setConnectOptions(options);
            // Set connection's DSN
            con.setDatabaseName(ODBC::dsn);
            connections.insert(conName, 0);
            qDebug() << __FUNCTION__ << "New ODBC connection added:" << conName;
        }

        // get ODBCAdapter from the traits
        auto odbc = ODBC::adapter;
        // construct sql query object
        SqlQuery query(con, odbc);

        if (!con.isOpen() && !con.open()) {
            qDebug() << __FUNCTION__ << "Error:" << con.lastError().text();
        }
        else {
            query.setForwardOnly(true);
        }

        return query;
    }

    void releaseQuery(SqlQuery query) {
        query.close();
    }

private:
    static constexpr int initialConnectionTimeout = 1;
    static constexpr int normalConnectionTimeout = 10;

    QHash<QString, int> connections;
};


}}
