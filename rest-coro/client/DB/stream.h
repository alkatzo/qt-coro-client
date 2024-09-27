#pragma once

#include <QCoroTask>
#include <QCoroAsyncGenerator>

#include "DB/concepts.h"
#include "DB/macros.h"

namespace db {

template<Container T>
class Stream
{
public:
    DISABLE_COPY(Stream)
    DEFAULT_MOVE(Stream)
    Stream() = default;
    Stream(QCoro::AsyncGenerator<T> &&g) : generator(std::move(g)) {}
    ~Stream() = default;

    QCoro::Task<bool> hasNext() {
        if (eos) {
            co_return false;
        }

        if (it == typename QCoro::AsyncGenerator<T>::iterator(nullptr)) {
            it = co_await generator.begin();
        }
        else if (it != generator.end()) {
            co_await ++(it);
        }
        eos = (it == generator.end());
        co_return !eos;
    }

    const T& next() {
        return *it;
    }

    QCoro::Task<T> result() {
        it = co_await generator.begin();

        T res;
        while (it != generator.end()) {
            T page = *it;
            for (const auto &s : page) {
                res << s;
            }
            // And we must co_await increment
            co_await ++(it);
        }

        co_return res;
    }

    template<typename CB>
    requires (std::is_invocable_v<CB, T>)
    auto result(CB cb) -> std::conditional_t<QCoro::detail::isTask_v<std::invoke_result_t<CB, T>>, std::invoke_result_t<CB, T>, QCoro::Task<std::invoke_result_t<CB, T>>> {
        const auto &res = co_await result();
        if constexpr (QCoro::detail::isTask_v<std::invoke_result_t<CB, T>>) {
            co_return co_await cb(res);
        }
        else {
            co_return cb(res);
        }
    }

private:
    QCoro::AsyncGenerator<T> generator;
    QCoro::AsyncGenerator<T>::iterator it{nullptr};
    bool eos{false};
};

}
