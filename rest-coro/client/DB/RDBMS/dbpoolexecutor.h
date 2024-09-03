#pragma once
#include <QtConcurrent>

#include "DB/RDBMS/connectionmanager.h"
#include "DB/RDBMS/sqlquery.h"
#include "DB/helper.h"

namespace db { namespace rdbms {

/**
 * @brief The DBPoolExecutor class
 * Executor class to execute SQL queries
 * Uses QThreadPool class for async calls
 * ConMgrT class is an SQL DB connection manager
 * sync call executes on the calling thread and returns results from the backend
 * async calls execute on the thread pool and return QFuture
 */
template<typename ConMgrT>
class DBPoolExecutor {
private:
    static inline QString syncLog = QString(" -> %1").arg("DBPoolExecutor::sync");
    static inline QString asyncLog = QString(" -> %1").arg("DBPoolExecutor::async");

public:
    using QueryT = typename ConMgrT::QueryT;
    template <class _Callable, class... _Args>
    using callable_result_t = typename std::invoke_result_t<_Callable, _Args..., QueryT>;

public:
    DBPoolExecutor() {
        threadpool.setMaxThreadCount(1);
        threadpool.setExpiryTimeout(-1);
    }

    ~DBPoolExecutor() {
    }

    template<typename  Binding, typename... As>
    auto sync(QString s, Binding&& b, As... args) {
        db::log_start(s + syncLog);
        auto query = cm.makeQuery();
        auto r = b(args..., query);
        cm.releaseQuery(query);
        db::log_finish(s, r);
        return r;
    }

    template<typename O, typename R, typename... Ps, typename... As>
    R sync(QString s, O *o, R (O::*method)(Ps...), As... args) {
        db::log_start(s + syncLog);
        auto query = cm.makeQuery();
        R r = (o->*method)(args..., query);
        cm.releaseQuery(query);
        db::log_finish(s, r);
        return r;
    }

    template<typename  Binding, typename... As>
    auto async(QString s, Binding&& b, As... args) {
        using R = std::invoke_result_t<Binding, As..., QueryT>;
        std::function<R()> fun = [=, this]() {
            return sync(s + asyncLog, b, args...);
        };
        log_start(QString("%1 threadpool #activeThreads: %2").arg(s).arg(threadpool.activeThreadCount()));
        return QtConcurrent::run(&threadpool, fun);
    }

    template<typename O, typename R, typename... Ps, typename... As>
    QFuture<R> async(QString s, O *o, R (O::*method)(Ps...), As... args) {
        std::function<R()> fun = [=, this]() {
            return sync(s + asyncLog, o, method, args...);
        };
        log_start(QString("%1 threadpool #activeThreads: %2").arg(s).arg(threadpool.activeThreadCount()));
        return QtConcurrent::run(&threadpool, fun);
    }

protected:
    QThreadPool threadpool;
    ConMgrT cm;
};

}}
