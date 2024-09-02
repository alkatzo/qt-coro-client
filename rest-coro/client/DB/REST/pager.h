#pragma once

#include <QCoroAsyncGenerator>
#include <QDateTime>

namespace db { namespace rest {

class Pager
{
public:
    Pager();

    QCoro::AsyncGenerator<QList<QString>> peopleGet(QDateTime dt);
};

}}
