#pragma once
#include "zeus/expected.hpp"

namespace er {

class Client;

struct RAPIError {
    int code{0};
    QString text;
};

struct PromiseBase {
    void setError(int code, const QString& text) {
        error = {code, text};
    }

    void resume() {
        if (handle && !handle.done()) {
            handle.resume();
        }
    }

    RAPIError error;
    std::coroutine_handle<> handle = nullptr;
};

template<typename T>
struct Promise : public PromiseBase {
    void setResult(const T& r) {
        result = r;
    }

    T result{};
};

template<>
struct Promise<void> : public PromiseBase {};

template<typename T>
class RAPIFuture
{
    friend class Client;

public:
    RAPIFuture() : promise(new Promise<T>) {
    }

    bool await_ready() const noexcept {
        return false;
    }

    void await_suspend(std::coroutine_handle<> h) {
        promise->handle = h;
    }

    zeus::expected<T, RAPIError> await_resume() const {
        if (promise->error.code == 0) {
            return promise->result;
        }
        return zeus::unexpected{promise->error};
    }

private:
    std::shared_ptr<Promise<T>> promise;
};

template<>
inline zeus::expected<void, RAPIError> RAPIFuture<void>::await_resume() const {
    if (promise->error.code == 0) {
        return zeus::expected<void, RAPIError>{};
    }
    return zeus::unexpected{promise->error};
}

}
