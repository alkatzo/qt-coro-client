#pragma once

#include <QObject>
#include <QCoroTask>

namespace db {
/**
 *@brief The Db class
 *Pure virtual interface for template db::Backend classes
 *Serves as a runtime switch for possible Backend class types
 */
class Db : public QObject
{
    Q_OBJECT
public:
    inline static Db *the = nullptr;

public:
    Db() = default;

    static Db *makeDB();

public:
    virtual QCoro::Task<QList<QString>> peopleGet(QDateTime dt) = 0;
    virtual void peopleGet(QDateTime dt, std::function <void(QList<QString>)> cb) = 0;
    virtual void peopleGet(QDateTime dt, QObject *ctx, std::function <void(QList<QString>)> cb) = 0;
};

}
