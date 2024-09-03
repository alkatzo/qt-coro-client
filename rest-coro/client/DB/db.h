#pragma once

#include <QObject>

#include "stream.h"

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
    virtual Stream<QList<QString>> peopleGet(QDateTime dt) = 0;
};

}
