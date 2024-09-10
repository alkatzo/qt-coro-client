#pragma once

namespace er {

class Client;

template<typename T>
class RAPIFuture
{
    friend class Client;

public:
    RAPIFuture() : promise(new Promise) {
    }

    bool await_ready() const noexcept {
        return false;
    }

    void await_suspend(std::coroutine_handle<> h) {
        promise->handle = h;
    }

    T await_resume() const {
        return promise->result;
    }

private:
    struct Promise {
        void setResult(const T& r) {
            result = r;
        }

        void resume() {
            if (handle && !handle.done()) {
                handle.resume();
            }
        }

        T result{};
        std::coroutine_handle<> handle = nullptr;
    };

    std::shared_ptr<Promise> promise;
};

}
