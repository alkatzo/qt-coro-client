#pragma once

#include "DB/deleters.h"

namespace db { namespace rest {

/**
 * @brief The RestExecutor class
 * This class is required to conform with the rdbms way of executing
 */
class RestExecutor
{
private:
    static inline QString asyncLog = QString(" -> %1").arg("RestExecutor::async");

public:
    RestExecutor() {
    }

    template<typename O, typename R, typename... Ps, typename... As>
    R sync(QString s, Cancel c, O *o, R (O::*method)(Ps...), As... args) {
        QList<QString> res = co_await (o->*method)(args...);

        if (!c.ctx) {
            free(res);
            co_return {};
        }

        co_return res;
    }

    template<typename O, typename R, typename... Ps, typename... As>
    auto sync_paged(QString s, Cancel c, O *o, R (O::*method)(Ps...), As... args) {
        return (o->*method)(args..., c);
    }
};

}}
