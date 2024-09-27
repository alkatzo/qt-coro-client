#pragma once

#include <QCoroTask>
#include <QCoroQmlTask>

#include "DB/concepts.h"
#include "DB/macros.h"
#include "DB/loghelper.h"

namespace db {

template<typename T>
class Task
{
public:
    DISABLE_COPY(Task)
    DEFAULT_MOVE(Task)
    using value_type = T;
    Task(QCoro::Task<T> &&t) : task(std::move(t)) {}

    QCoro::Task<T> result() {
        return std::move(task);
    }

    /**
     * @brief result
     * Fetches results via co_await
     * The result is deallocated if the context is deleted while the coro was suspended
     */
    template<typename CB>
    requires (std::is_invocable_v<CB, T>)
    auto result(CB cb) -> std::conditional_t<QCoro::detail::isTask_v<std::invoke_result_t<CB, T>>, std::invoke_result_t<CB, T>, QCoro::Task<std::invoke_result_t<CB, T>>> {
        const auto &res = co_await task;
        if constexpr (QCoro::detail::isTask_v<std::invoke_result_t<CB, T>>) {
            LOG << "Calling co_return co_await cb(res);";
            co_return co_await cb(res);
        }
        else {
            LOG << "Calling co_return cb(res);";
            co_return cb(res);
        }
    }

    /**
     * @brief operator QCoro::QmlTask - use to return db::Task from Q_INVOKABLE QCoro::QmlTask myMethod(..) to qml
     */
    operator QCoro::QmlTask() {
        return std::move(task);
    }

private:
    QCoro::Task<T> task;
};

}
