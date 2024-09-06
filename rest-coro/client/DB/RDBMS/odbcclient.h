#pragma once

#include <QString>
#include <QtSql>
#include <QDebug>

#include <memory>

namespace db {  namespace rdbms {

/**
 * @brief The IODBCAdapter class
 * ODBC adapter interface
 */
struct IODBCAdapter {
    struct ConnectionStringBuilder {
        QString connectionString;

        ConnectionStringBuilder &setServer(const QString &val);
        ConnectionStringBuilder &setPort(const QString &val);
        ConnectionStringBuilder &setDatabase(const QString &val);
        ConnectionStringBuilder &setUID(const QString &val);
        ConnectionStringBuilder &setPWD(const QString &val);
        ConnectionStringBuilder &setDriverVersion(const QString &val);
        ConnectionStringBuilder &setTrustServer(bool val);

        QString buildDefaultDSN();
    };

    // different ODBC drivers work differently with respect to binding arguments
    virtual void bindTextOutValue(QSqlQuery &query, const QString& placeholder, int size) = 0;
    // different ODBC drivers work differently with respect to extracting bound values returned from a DB server
    virtual QString boundTextOutValue(const QSqlQuery &query, const QString& placeholder) const = 0;
    // Controls syntax of SQL procedure calls. F.i. MySql does not understand the "{}" in SP call syntax "{CALL ..."
    virtual QString q(const QString &sp) const = 0;
    // SQL server type defined in the .cfg file Must match one of the supported db types. Currently mssql only
    virtual QString getType() = 0;
    // ODBC specific connection string builder
    virtual ConnectionStringBuilder connectionStringBuilder() = 0;
};

class MSODBCAdapter : public IODBCAdapter {
public:
    constexpr static auto type = "mssql";    
    static QString dsn();

protected:
    virtual QString q(const QString &sp) const override {
        return sp;
    }   
    virtual void bindTextOutValue(QSqlQuery &query, const QString &placeholder, int size) override;
    virtual QString boundTextOutValue(const QSqlQuery &query, const QString &placeholder) const override;
    virtual ConnectionStringBuilder connectionStringBuilder() override;
    virtual QString getType() override { return type; }

    static ConnectionStringBuilder conStringBuilder();
};

}}

