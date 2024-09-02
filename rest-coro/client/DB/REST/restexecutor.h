#pragma once

#include <QtConcurrent>

#include "DB/helper.h"
#include "er_base.h"

namespace db { namespace rest {

/**
 * @brief The RestExecutor class
 * Uses QThreadPool class for async calls
 * sync calls are not supported
 * async calls execute on the thread pool and return QFuture
 */
class RestExecutor
{
public:
    template <class _Callable, class... _Args>
    using callable_result_t = typename std::invoke_result_t<_Callable, _Args...>;

private:
    static inline QString asyncLog = QString(" -> %1").arg("RestExecutor::async");

public:
    RestExecutor() {
    }

    template<typename O, typename R, typename CB, typename... Ps, typename... As>
    void async(QString s, CB cb, O* o, R (O::*method)(Ps...), As&&... args) {
        (o->*method)(args...);
    }

    template<typename O, typename R, typename CB, typename... Ps, typename... As>
    void async(QString s, QObject *ctx, CB cb, O* o, R (O::*method)(Ps...), As&&... args) {
        (o->*method)(args...);
    }

    template<typename O, typename R, typename... Ps, typename... As>
    auto sync(QString s, O *o, R (O::*method)(Ps...), As... args) {
        return (o->*method)(args...);
    }
};

}}
