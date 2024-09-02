#pragma once

#include <QCoroTask>
#include <QCoroAsyncGenerator>

namespace db {

template<typename T>
class Stream
{
    Stream(const Stream &) = delete;
    Stream &operator=(const Stream &) = delete;
    Stream(Stream &&) noexcept = default;                                                            \
    Stream &operator=(Stream &&) noexcept = default;

public:
    Stream(QCoro::AsyncGenerator<T> &&g) : generator(std::move(g)) {}

    auto begin() noexcept {
        return generator.begin();
    }

    auto end() noexcept {
        return generator.end();
    }

    QCoro::Task<bool> hasNext() {
        if (it == QCoro::AsyncGenerator<T>::iterator(nullptr)) {
            it = co_await generator.begin();
        }
        else if (it != generator.end()) {
            co_await ++(it);
        }
        co_return it != generator.end();
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

private:
    QCoro::AsyncGenerator<T> generator;
    QCoro::AsyncGenerator<T>::iterator it{nullptr};
};

}
