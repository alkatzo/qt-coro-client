#pragma once

#include <QObject>
#include <QFutureWatcher>

#include "DB/concepts.h"

namespace db {

/**
 * @brief The Deleter class
 * Default deleter class to free memory for the async call with a context object
 * The memory needs to be freed for the QML_ELEMENT use case
 */
class Deleter {
public:
    template<typename T>
    static void free(const T &) {}

    template<QPtrContainer T>
    static void free(const T &arg) {
        qDeleteAll(arg);
    }

    template<PtrType T>
    static void free(const T &arg) {
        delete arg;
    }
};

/**
 * @brief The SimpleExecutor class
 * Execution policy class for regular C++ sync and async calls
 * sync call returns results from a backend db
 * async calls execute calls defined by a member method, a lambda or a std::bind
 * the calls are forwarded to the underlying backend executor DBExecutor ( DBPoolExecutor / RestPoolExecutor / XYZExecutor / ... )
 * the underlying executor returns results for the sync call or a QFuture for the async calls
 */
template<typename DBExecutor>
class SimpleExecutor {
private:
    static inline QString syncLog = QString(" -> %1").arg("SimpleExecutor::sync");
    static inline QString asyncLog = QString(" -> %1").arg("SimpleExecutor::async");

protected:
    DBExecutor ex{};

public:
    template<typename O, typename R, typename... Ps, typename... As>
    R sync(QString s, O* o, R (O::*method)(Ps...), As&&... args) {
        R r = ex.sync(s + syncLog, o, method, std::forward<As>(args)...);
        return r;
    }

    template<typename CB, typename  Binding, typename... As>
    void async(QString s, CB cb, Binding&& b, As&&... args) {
        auto future = ex.async(s + asyncLog, b, std::forward<As>(args)...);
        using R = decltype(future.result());
        connect<R>(future, cb);
    }

    template<typename CB, typename  Binding, typename... As>
    void async(QString s, QObject *ctx, CB cb, Binding&& b, As&&... args) {
        auto future = ex.async(s + asyncLog, b, std::forward<As>(args)...);
        using R = decltype(future.result());
        connect<R>(future, cb, ctx);
    }

    template<typename O, typename R, typename CB, typename... Ps, typename... As>
    void async(QString s, CB cb, O* o, R (O::*method)(Ps...), As&&... args) {
        auto future = ex.async(s + asyncLog, o, method, std::forward<As>(args)...);
        connect<R>(future, cb);
    }

    template<typename O, typename R, typename CB, typename... Ps, typename... As>
    void async(QString s, QObject *ctx, CB cb, O* o, R (O::*method)(Ps...), As&&... args) {
        auto future = ex.async(s + asyncLog, o, method, std::forward<As>(args)...);
        connect<R>(future, cb, ctx);
    }

protected:
    template<typename R, typename DeleterT = Deleter, typename CB>
    void connect(QFuture<R> future, CB cb, QObject *ctx = nullptr) {
        QFutureWatcher<R> *fw = new QFutureWatcher<R>;

        if (!ctx) {
            QObject::connect(fw, &QFutureWatcherBase::finished, [cb, fw]() mutable {
                cb(fw->result());
                fw->deleteLater();
            });
        }
        else {
            QPointer ctxWatcher{ctx};
            QObject::connect(fw, &QFutureWatcherBase::finished, [ctxWatcher, cb, fw]() mutable {
                if (ctxWatcher) {
                    cb(fw->result()); // client code must take ownership on the result
                }
                else {
                    DeleterT::free(fw->result()); // otherwise deallocate the result
                }
                fw->deleteLater();
            });
        }

        fw->setFuture(future);
    }
};

}
