#pragma once

namespace er {

class RAPI;

template<typename T>
class RAPIFuture
{
    friend class RAPI;

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
            if (handle && !handle.done()) {
                result = r;
                handle.resume();
            }
        }

        T result;
        std::coroutine_handle<> handle = nullptr;
    };

    std::shared_ptr<Promise> promise;
};

}
