#pragma once

#include <QObject>
#include <QFutureWatcher>
#include <QCoroFuture>

#include "DB/simpleexecutor.h"

namespace db {


/**
 * @brief The CoroExecutor class
 * Coroutine Execution policy class
 * Calls async method of the underlying backend executor DBExecutor and co-awaits on its QFuture
 * defines QCoro::Task<R> as the type of the coro and returns R from co_await
 */
template<typename DBExecutor>
class CoroExecutor : public SimpleExecutor<DBExecutor> {
private:
    static inline QString syncLog = QString(" -> %1").arg("CoroExecutor::sync");

public:
    template<typename Lambda>
    QCoro::Task<typename DBExecutor::template callable_result_t<Lambda>> sync(QString s, Lambda&& l) {
        co_return co_await this->ex.async(s + syncLog, std::forward<Lambda>(l));
    }

    template<typename O, typename R, typename... Ps, typename... As>
    QCoro::Task<R> sync(QString s, O* o, R (O::*method)(Ps...), As&&... args) {
        R r = co_await this->ex.async(s + syncLog, o, method, std::forward<As>(args)...);
        co_return r;
    }
};

}
