#include "er_dispatcher.h"

#include "DB/loghelper.h"

namespace er {

Dispatcher * Dispatcher::_instance = nullptr;

Dispatcher::Dispatcher() {
    connect(this, &Dispatcher::_requestCompleted, this, &Dispatcher::_process, Qt::ConnectionType::QueuedConnection);
}

Dispatcher::~Dispatcher() {
}

Dispatcher &Dispatcher::instance() {
    if (!_instance) {
        _instance = new Dispatcher;
    }
    return *_instance;
}

void Dispatcher::post(const std::function<void ()> &request) {
    _queue.push_back(request);
    _process();
}

void Dispatcher::send(const std::function<void ()> &request) {
    _queue.push_front(request);
    _process();
}

void Dispatcher::requestCompleted() {
    LOG << "queue size =" << _queue.size();
    _processing = false;
    Q_EMIT _requestCompleted();
}

void Dispatcher::_process() {
    LOG << "_processing =" << _processing << "size =" << _queue.size();
    if (!_processing && !_queue.empty()) {
        _processing = true;
        std::function<void ()> request = _queue.front();
        _queue.pop_front();
        request();
    }
}

}
