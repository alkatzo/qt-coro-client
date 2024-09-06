#include "odbcclient.h"

#include <memory>
#include <QSettings>

namespace db {  namespace rdbms {

IODBCAdapter::ConnectionStringBuilder &IODBCAdapter::ConnectionStringBuilder::setServer(const QString &val) {
    connectionString.append(QString("Server=%1;").arg(val));
    return *this;
}

IODBCAdapter::ConnectionStringBuilder &IODBCAdapter::ConnectionStringBuilder::setPort(const QString &val) {
    connectionString.append(QString("Port=%1;").arg(val));
    return *this;
}

IODBCAdapter::ConnectionStringBuilder &IODBCAdapter::ConnectionStringBuilder::setDatabase(const QString &val) {
    connectionString.append(QString("Database=%1;").arg(val));
    return *this;
}

IODBCAdapter::ConnectionStringBuilder &IODBCAdapter::ConnectionStringBuilder::setUID(const QString &val) {
    connectionString.append(QString("UID=%1;").arg(val));
    return *this;
}

IODBCAdapter::ConnectionStringBuilder &IODBCAdapter::ConnectionStringBuilder::setPWD(const QString &val) {
    connectionString.append(QString("PWD=%1;").arg(val));
    return *this;
}

IODBCAdapter::ConnectionStringBuilder &IODBCAdapter::ConnectionStringBuilder::setDriverVersion(const QString &val) {
    connectionString.append(QString("DRIVER={%1};").arg(val));
    return *this;
}

IODBCAdapter::ConnectionStringBuilder &IODBCAdapter::ConnectionStringBuilder::setTrustServer(bool val) {
    QString str = val ? "yes" : "no";
    connectionString.append(QString("TrustServerCertificate=%1;SQL_ATTR_ODBC_VERSION=SQL_OV_ODBC3;").arg(str));
    return *this;
}

QString IODBCAdapter::ConnectionStringBuilder::buildDefaultDSN() {
    QString ip = "localhost";
    QString dbName = "testdb";
    QString port = "1431";
    QString uid = "pharmusr";
    QString psswd = "$RPh4rM8l0g!N";

    return this->setServer(ip).
        setPort(port).
        setDatabase(dbName).
        setUID(uid).
        setPWD(psswd).
        connectionString;
}

IODBCAdapter::ConnectionStringBuilder MSODBCAdapter::conStringBuilder() {
    ConnectionStringBuilder builder;
    QString sqlDriver = "SQL Server";

    QSettings regKey("HKEY_LOCAL_MACHINE\\SOFTWARE\\ODBC\\ODBCINST.INI\\ODBC Drivers", QSettings::NativeFormat);

    const QString installed("Installed");
    for (int i = 11; i < 30; i++) {
        QString driverName = QString("ODBC Driver %1 for SQL Server").arg(i);
        if (regKey.value(driverName).toString() == installed) {
            sqlDriver = driverName;
        }
    }

    bool trustServerCertificate = true;

    builder.setDriverVersion(sqlDriver).
        setTrustServer(trustServerCertificate);

    return builder;
}

QString MSODBCAdapter::dsn() {
    return conStringBuilder().buildDefaultDSN();
}

void MSODBCAdapter::bindTextOutValue(QSqlQuery &query, const QString &placeholder, int size) {
    query.bindValue(placeholder, QString(size, static_cast<QChar>(0)), QSql::Out);
}

QString MSODBCAdapter::boundTextOutValue(const QSqlQuery &query, const QString &placeholder) const {
    return query.boundValue(placeholder).toString();
}

IODBCAdapter::ConnectionStringBuilder MSODBCAdapter::connectionStringBuilder() {
    return MSODBCAdapter::conStringBuilder();
}

}}
