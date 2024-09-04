#pragma once

#include <QObject>

#include "stream.h"
#include "task.h"

namespace db {
/**
 *@brief The Db class
 *Pure virtual interface for template db::Backend classes
 *Serves as a runtime switch for possible Backend class types
 */
class Db : public QObject
{
    Q_OBJECT

private:
    static inline QScopedPointer<QObject> alive{new QObject};

public:
    inline static Db *the = nullptr;

public:
    Db() = default;

    static Db *makeDB();

public:
    virtual Stream<QList<QString>> peopleGet(QDateTime dt, QObject *ctx = alive.get()) = 0;
    virtual Task<QList<QString>> peopleGetAll(QDateTime dt, QObject *ctx = alive.get()) = 0;
};

}
