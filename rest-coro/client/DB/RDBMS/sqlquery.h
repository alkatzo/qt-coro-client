#pragma once

#include <QSqlQuery>

#include "odbcclient.h"

namespace db { namespace rdbms {

/**
 * @brief The SqlQuery class
 * Helper class to work with ODBC drivers
 */
struct SqlQuery : public QSqlQuery
{
    SqlQuery();
    SqlQuery(QSqlDatabase db, std::shared_ptr<IODBCAdapter> odbc) :
        QSqlQuery(db),
        db(db),
        odbc(odbc) {
    }
    void bindTextOutValue(const QString& placeholder, int size) {
        odbc->bindTextOutValue(*this, placeholder, size);
    }
    QString boundTextOutValue(const QString& placeholder) const {
        return odbc->boundTextOutValue(*this, placeholder);
    }
    bool transaction() {
        return db.transaction();
    }
    bool commit() {
        // OUT Bound params are returned as a result set and an empty result set is returned too (happens on Mariadb, but for now comon for all supported DBs)
        // Without consuming all result sets commit() won't work
        while (nextResult()) {};
        return db.commit();
    }
    bool rollback() {
        while (nextResult()) {};
        return db.rollback();
    }
    void close() {
        db.close();
    }
    bool prepare(const QString& query) {
        return QSqlQuery::prepare(odbc->q(query));
    }
private:
    QSqlDatabase db;
    std::shared_ptr<IODBCAdapter> odbc;
};

}}
