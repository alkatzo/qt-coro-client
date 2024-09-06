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
    /**
     * @brief onSuccess - Q_EMIT success signal with the http response
     */
    template<class T, class Source, class... Args>
    void onSuccess(const Source &source, void (T::*signal)(Args...), Args... args) {
        LOG << source << OK;
        Q_EMIT (static_cast<T*>(this)->*signal)(args...);
    }
    /**
     * @brief onError - if needed re-authorise and re-run, otherwise Q_EMIT error signal
     */
    template<class T, class Source, class Call>
    void onError(const Source &source, Call call, int authAttempt, int allowedAttempts,
                 void (T::*signalErr)(ER_HttpRequestWorker*, QNetworkReply::NetworkError, QString ),
                 ER_HttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str) {
        LOG << source << Error << "ErrorType:" << error_type << "ErrorText:" << error_str <<
            "HttpResponseCode:" << worker->getHttpResponseCode() << "ResponseHeaders:" << worker->getResponseHeaders();

        Q_EMIT (static_cast<T*>(this)->*signalErr)(worker, error_type, error_str);
    }
    /**
     * @brief onCompleted - tell the dispatcher the request is finished and mark the api object for deletion
     */
    template<class T, class Source, class API>
    void onCompleted(const Source &source, void (T::*signalCompleted)(), API *api) {
        LOG << source << Completed;
        Q_EMIT (static_cast<T*>(this)->*signalCompleted)();
        requestCompleted();
        api->deleteLater();
    }

protected:
    static constexpr char OK[] = "-> OK";
    static constexpr char Error[] = "-> Error";
    static constexpr char Completed[] = "-> Completed";
};

class RAPI : public Client {

protected:
    void wire(auto source, auto s_success, auto s_error, auto s_completed, auto s_api_success, auto s_api_error, auto s_api_completed, auto auth_rerun, auto authAttempt, auto api, auto f) {
        // connect successful response
        QObject::connect(api, s_api_success, api, [=, this](auto summary) {
            this->onSuccess(source, s_success, summary);
            f.promise->setResult(summary);
        });
        // connect full error response
        QObject::connect(api, s_api_error, api, [=, this](auto worker, auto error_type, auto error_str) {
            this->onError(source, auth_rerun, authAttempt, 1, s_error, worker, error_type, error_str);
        });
        // connect request completed
        QObject::connect(api, s_api_completed, api, [=, this]() {
            this->onCompleted(source, s_completed, api);
            f.promise->resume();
        });
    }
};

#define ER_DECLARE_SHIM_SIGNALS(SHIM_METHOD, RESPONSE_TYPE) \
using SHIM_METHOD##ResponseType=RESPONSE_TYPE; \
Q_SIGNALS: \
    void SHIM_METHOD##CompletedOK(RESPONSE_TYPE summary); \
    void SHIM_METHOD##CompletedE(ER_HttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);

#define _ER_DEFINE_SIGNALS(SHIM_API, SHIM_METHOD, API, API_METHOD) \
    auto source = __FUNCTION__; \
    LOG << source; \
    auto s_success = &SHIM_API::SHIM_METHOD##CompletedOK; \
    auto s_error = &SHIM_API::SHIM_METHOD##CompletedE; \
    auto s_completed = &SHIM_API::requestCompleted; \
    auto s_api_success = &API::API_METHOD##Signal; \
    auto s_api_error = &API::API_METHOD##SignalErrorFull; \
    auto s_api_completed = &API::allPendingRequestsCompleted; \
    API *api = IntegrationManager::api<API>().release(); \

#define ER_DEFINE_SIGNALS_0(SHIM_API, SHIM_METHOD, API, API_METHOD) \
    _ER_DEFINE_SIGNALS(SHIM_API, SHIM_METHOD, API, API_METHOD) \
    auto auth_rerun = [this, authAttempt](){SHIM_METHOD(authAttempt + 1);}; \
    wire(source, s_success, s_error, s_completed, s_api_success, s_api_error, s_api_completed, auth_rerun, 1, api);

#define ER_DEFINE_SIGNALS(SHIM_API, SHIM_METHOD, API, API_METHOD, ...) \
    _ER_DEFINE_SIGNALS(SHIM_API, SHIM_METHOD, API, API_METHOD) \
    auto auth_rerun = [this, __VA_ARGS__, authAttempt](){SHIM_METHOD(__VA_ARGS__, authAttempt + 1);}; \
    RAPIFuture<SHIM_METHOD##ResponseType> f; \
    wire(source, s_success, s_error, s_completed, s_api_success, s_api_error, s_api_completed, auth_rerun, 1, api, f);
}

