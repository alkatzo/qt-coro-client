#pragma once

#include <utility>
#include <memory>

namespace db {

class stop_token
{
public:
    stop_token() noexcept : _State{new bool{false}} { }
    ~stop_token() = default;

    bool request_stop() noexcept {
        *_State = true;
        return *_State;
    }

    bool stop_requested() const noexcept {
        return *_State;
    }

private:
    std::shared_ptr<volatile bool> _State;
};

}
