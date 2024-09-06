#pragma once

#include <QObject>

#include "DB/stream.h"
#include "DB/task.h"
#include "DB/deleters.h"

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
    inline static QScopedPointer<Db> the;

public:
    Db() = default;

    static Db *makeDB();

public:
    virtual Stream<QList<QString>> peopleGet(QDateTime dt, Cancel c = { alive.get(), {} }) = 0;
    virtual Task<QList<QString>> peopleGetAll(QDateTime dt, Cancel c = { alive.get(), {} }) = 0;
};

}
