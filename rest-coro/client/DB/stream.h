#pragma once

#include <QCoroTask>
#include <QCoroAsyncGenerator>

namespace db {

template<typename T>
class Stream;

/**
 * @brief The StreamIterator class
 * TODO
 */
template<typename T>
class StreamIterator {

public:
    using iterator_category = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = std::remove_reference_t<T>;
    using reference = std::add_lvalue_reference_t<T>;
    using pointer = std::add_pointer_t<value_type>;

    StreamIterator(Stream<T> *s) : stream(s) {}
    StreamIterator(StreamIterator &&other) = default;

    QCoro::Task<StreamIterator> operator++() noexcept {
        co_return *this;
    }

    reference operator *() const noexcept ;

    bool operator==(const StreamIterator &other) const noexcept {
        return stream = other.stream && pos = other.pos;
    }

    bool operator!=(const StreamIterator &other) const noexcept {
        return !(*this == other);
    }

private:
    Stream<T> *stream{nullptr};
    int pos = 0;
};

template<typename T>
class Stream
{
public:
    Stream(const Stream &) = delete;
    Stream &operator=(const Stream &) = delete;
    Stream(Stream &&) noexcept = default;
    Stream &operator=(Stream &&) noexcept = default;

    Stream(QCoro::AsyncGenerator<T> &&g) : generator(std::move(g)) {}

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
    QCoro::Task<> result(CB cb) {
        const auto &res = co_await result();
        cb(res);
    }

private:
    QCoro::AsyncGenerator<T> generator;
    QCoro::AsyncGenerator<T>::iterator it{nullptr};
    bool eos{false};
};

}
