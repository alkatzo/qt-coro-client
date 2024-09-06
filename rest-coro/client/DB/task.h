#pragma once

#include <QCoroTask>

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
        const auto &res = co_await task;
        co_return res;
    }

    /**
     * @brief result
     * Fetches results via co_await
     * The result is deallocated if the context is deleted while the coro was suspended
     */
    template<typename CB>
    requires (std::is_invocable_v<CB, T>)
    QCoro::Task<> result(CB &&cb) {
        auto callback = std::forward<CB>(cb);
        const auto &res = co_await task;
        callback(res);
    }

private:
    QCoro::Task<T> task;
};

}
