#pragma once

#include <QCoroTask>

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

    template<typename CB>
    QCoro::Task<> result(CB cb) {
        const auto &res = co_await result();
        cb(res);
    }

private:
    QCoro::Task<T> task;
};

}
