#pragma once

#include <QCoroTask>
#include <QCoroAsyncGenerator>

#include "DB/concepts.h"

namespace db {

template<QContainer T>
class Stream
{
public:
    Stream(const Stream &) = delete;
    Stream &operator=(const Stream &) = delete;
    Stream(Stream &&) noexcept = default;
    Stream &operator=(Stream &&) noexcept = default;

    Stream(QCoro::AsyncGenerator<T> &&g, QObject *ctx) : generator(std::move(g)), ctx(ctx) {}

    QCoro::Task<bool> hasNext() {
        if (eos) {
            co_return false;
        }

        if (it == QCoro::AsyncGenerator<T>::iterator(nullptr)) {
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

    template<typename DeleterT = Deleter>
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

    template<typename DeleterT = Deleter, typename CB>
    requires (std::is_invocable_v<CB, T>)
    QCoro::Task<> result(CB &&cb) {
        // Move the callback into the coroutine, otherwise the temp cb is destroyed after the coro is suspended
        auto callback = std::forward<CB>(cb);

        const auto &res = co_await result();

        callback(res);
    }

private:
    QCoro::AsyncGenerator<T> generator;
    QPointer<QObject> ctx;
    QCoro::AsyncGenerator<T>::iterator it{nullptr};
    bool eos{false};
};

}
