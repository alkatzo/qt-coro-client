#pragma once

#include <QtConcurrent>

#include "DB/helper.h"
#include "er_base.h"
#include "DB/stream.h"

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
    auto sync(QString s, O *o, R (O::*method)(Ps...), As... args) {
        return (o->*method)(args...);
    }

    template<typename O, typename R, typename... Ps, typename... As>
    auto sync_paged(QString s, O *o, R (O::*method)(Ps...), As... args) {
        return sync(s, o, method, args...);
    }
};

}}
