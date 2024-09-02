#include "er_base.h"
#include "er_dispatcher.h"
#include "er_integrationmanager.h"

namespace er {

void Client::post(const std::function<void ()> &request) {
    Dispatcher::instance().post(request);
}

void Client::send(const std::function<void ()> &request) {
    Dispatcher::instance().send(request);
}

void Client::requestCompleted() {
    Dispatcher::instance().requestCompleted();
}

}
