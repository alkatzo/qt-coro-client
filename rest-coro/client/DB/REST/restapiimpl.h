#pragma once

#include <QCoroTask>
#include <QCoroAsyncGenerator>
#include <QDateTime>

namespace db { namespace rest {

class RestApiImpl
{
public:
    RestApiImpl();

    QCoro::AsyncGenerator<QList<QString>> peopleGet(QDateTime dt);
};

}}
