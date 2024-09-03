#pragma once
#include <QtConcurrent>
#include <QCoroTask>
#include <QCoroFuture>
#include <QCoroAsyncGenerator>

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
    static inline QString _syncLog = QString(" -> %1").arg("DBPoolExecutor::_sync");
    static inline QString syncLog = QString(" -> %1").arg("DBPoolExecutor::sync");
    static inline QString syncpagedLog = QString(" -> %1").arg("DBPoolExecutor::sync_paged");

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

    /**
     * @brief _sync
     * @param args - variadic args for O::*method
     * @return R - result of O::*method
     * This method runs in a thread allocated in the threadpool
     */
    template<typename O, typename R, typename... Ps, typename... As>
    R _sync(QString s, O *o, R (O::*method)(Ps...), As... args) {
        LSCOPE
        db::log_start(s + _syncLog);
        auto query = cm.makeQuery();
        R r = (o->*method)(args..., query);
        cm.releaseQuery(query);
        db::log_finish(s, r);
        return r;
    }

    /**
     * @brief sync
     * @return QCoro::Task<R>
     * Launches O::*method in a seprate thread, co_awaits on its results via QFuture and co_returns QCoro::Task<R>
     */
    template<typename O, typename R, typename... Ps, typename... As>
    QCoro::Task<R> sync(QString s, O *o, R (O::*method)(Ps...), As... args) {
        LSCOPE
        std::function<R()> fun = [=, this]() {
            return _sync(s + syncLog, o, method, args...);
        };
        log_start(QString("%1 threadpool #activeThreads: %2").arg(s).arg(threadpool.activeThreadCount()));
        co_return co_await QtConcurrent::run(&threadpool, fun); // co_return R
    }

    /**
     * @brief sync_paged
     * @return QCoro::AsyncGenerator<R>
     * Simulate paged extraction from Sql server by getting R from sync() and wrapping it in QCoro::AsyncGenerator<R>
     * QCoro::AsyncGenerator<R> coro cannot co_await directly on QFuture, have to co_await on QCoro::Task<R>
     * Sql paging simulation is required in order to match the one from paged REST APIs
     * It gets converted to Stream<R> up the stack
     */
    template<typename O, typename R, typename... Ps, typename... As>
    QCoro::AsyncGenerator<R> sync_paged(QString s, O *o, R (O::*method)(Ps...), As... args) {
        LSCOPE
        QList<QString> ret = co_await sync(s + syncpagedLog, o, method, args...);
        co_yield ret;
    }

protected:
    QThreadPool threadpool;
    ConMgrT cm;
};

}}
