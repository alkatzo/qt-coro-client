#pragma once
#include "zeus/expected.hpp"

namespace er {

class Client;

template<typename T>
struct Promise {
    void setResult(const T& r) {
        result = r;
    }

    void setError(uint code, const QString& text) {
        error.first = code;
        error.second = text;
    }

    void resume() {
        if (handle && !handle.done()) {
            handle.resume();
        }
    }

    T result{};
    std::pair<uint, QString> error;
    std::coroutine_handle<> handle = nullptr;
};

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

    zeus::expected<T, std::pair<uint, QString>> await_resume() const {
        if (promise->error.first == 0) {
            return promise->result;
        }
        else {
            return zeus::unexpected{promise->error};
        }
    }

private:
    std::shared_ptr<Promise<T>> promise;
};

}
