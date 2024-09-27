#pragma once

#include <QCoroTask>
#include <QCoroQmlTask>

#include "DB/concepts.h"

namespace db {

template<typename T>
class Task
{
public:
    Task(const Task &) = delete;
    Task &operator=(const Task &) = delete;
    Task(Task &&) noexcept = default;
    Task &operator=(Task &&) noexcept = default;

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
    auto result(CB cb) -> QCoro::Task<std::invoke_result_t<CB, T>> { // by value, its a coro
        const auto &res = co_await task;
        co_return cb(res);
    }

    operator QCoro::QmlTask() {
        return std::move(task);
    }

private:
    QCoro::Task<T> task;
};

}
