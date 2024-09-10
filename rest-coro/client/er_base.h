#pragma once

#include <QObject>

#include <functional>
#include <coroutine>

#include "openapi/ER_HttpRequest.h"
#include "er_rapifuture.h"
#include "DB/loghelper.h"

namespace er {

class Client {

protected:
    void post(const std::function<void()> &request);
    void send(const std::function<void()> &request);
    void requestCompleted();

    void wire(auto source, auto s_api_success, auto s_api_error, auto s_api_completed, auto api, auto f) {
        // connect successful response
        QObject::connect(api, s_api_success, api, [=, this](auto summary) {
            LOG << source << OK;
            f.promise->setResult(summary);
        });
        // connect full error response
        QObject::connect(api, s_api_error, api, [=, this](auto worker, auto error_type, auto error_str) {
            LOG << source << Error << "ErrorType:" << error_type << "ErrorText:" << error_str <<
                "HttpResponseCode:" << worker->getHttpResponseCode() << "ResponseHeaders:" << worker->getResponseHeaders();
        });
        // connect request completed
        QObject::connect(api, s_api_completed, api, [=, this]() {
            LOG << source << Completed;
            requestCompleted();
            api->deleteLater();
            f.promise->resume();
        });
    }

private:
    static constexpr char OK[] = "-> OK";
    static constexpr char Error[] = "-> Error";
    static constexpr char Completed[] = "-> Completed";
};

/**
 * Macros to reduce signal/slot connections typing
*/
#define _WIRE_API_SIGNALS(CLASS, METHOD) \
    auto source = __FUNCTION__; \
    LOG << source; \
    auto s_api_success = &CLASS::METHOD##Signal; \
    auto s_api_error = &CLASS::METHOD##SignalErrorFull; \
    auto s_api_completed = &CLASS::allPendingRequestsCompleted; \
    CLASS *api = IntegrationManager::api<CLASS>().release(); \

#define WIRE_API_SIGNALS(RETURN_TYPE, CLASS, METHOD) \
    _WIRE_API_SIGNALS(CLASS, METHOD) \
    RAPIFuture<RETURN_TYPE> f; \
    wire(source, s_api_success, s_api_error, s_api_completed, api, f);
}

