#pragma once

#include <QCoroTask>
#include <QCoroAsyncGenerator>
#include <QDateTime>

namespace db { namespace rest {

inline bool isRestApi(const QString &name) {
    return name.toLower() == "restapi";
}

class RestApiImpl
{
public:
    RestApiImpl();

    QCoro::AsyncGenerator<QList<QString>> peopleGet(QDateTime dt);
    QCoro::Task<QList<QString>> peopleGetAll(QDateTime dt);
};

}}
