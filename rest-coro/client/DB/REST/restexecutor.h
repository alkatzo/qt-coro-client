#pragma once

#include <QtConcurrent>

#include "DB/helper.h"
#include "er_base.h"
#include "DB/stream.h"

namespace db { namespace rest {

/**
 * @brief The RestExecutor class
 * Uses QThreadPool class for async calls
 * sync calls are not supported
 * async calls execute on the thread pool and return QFuture
 */
class RestExecutor
{
private:
    static inline QString asyncLog = QString(" -> %1").arg("RestExecutor::async");

public:
    RestExecutor() {
    }

    template<typename O, typename R, typename... Ps, typename... As>
    auto sync_paged(QString s, O *o, R (O::*method)(Ps...), As... args) {
        return (o->*method)(args...);
    }
};

}}
