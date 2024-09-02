#pragma once

#include <QObject>

#include <queue>
#include <functional>

namespace er {

class Dispatcher : public QObject {
    Q_OBJECT

public:

    static Dispatcher &instance();

    void post(const std::function<void()> &request);
    void send(const std::function<void()> &request);
    void requestCompleted();

private:
    Dispatcher();
    ~Dispatcher();
    void _process();

Q_SIGNALS:
    void _requestCompleted();

private:
    static Dispatcher *_instance;
    std::deque<std::function<void()>> _queue;
    bool _processing = false;
};

}
