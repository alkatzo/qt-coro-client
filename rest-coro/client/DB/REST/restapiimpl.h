#pragma once

#include <QCoroTask>
#include <QDateTime>

namespace db { namespace rest {

class RestApiImpl
{
public:
    RestApiImpl();

    QCoro::Task<QList<QString>> peopleGet(QDateTime dt);
};

}}
