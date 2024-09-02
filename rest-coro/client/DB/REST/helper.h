#pragma once

#define DECLARE_API(API) \
    auto source = QString(__FUNCTION__); \
    QEventLoop loop; \
    API *api = IntegrationManager::erApi<API>().release();

#define CONNECT_AND_RUN_0(ON_SUCCESS, API, API_METHOD) \
    connect(api, &API::API_METHOD##CompletedOK, api, ON_SUCCESS); \
    \
    connect(api, &API::API_METHOD##CompletedE, api, [&](auto worker, auto error_type, auto error_str) { \
        qCritical() << source << error_type << error_str; \
    }); \
    \
    connect(api, &API::requestCompleted, api, [&](){ \
        delete api; \
        loop.quit(); \
    }); \
    \
    api->API_METHOD(); \
    \
    loop.exec();

#define CONNECT_AND_RUN(ON_SUCCESS, API, API_METHOD, ...) \
    connect(api, &API::API_METHOD##CompletedOK, api, ON_SUCCESS); \
    \
    connect(api, &API::API_METHOD##CompletedE, api, [&](auto worker, auto error_type, auto error_str) { \
        qCritical() << source << error_type << error_str; \
    }); \
    \
    connect(api, &API::requestCompleted, api, [&](){ \
        delete api; \
        loop.quit(); \
    }); \
    \
    api->API_METHOD(__VA_ARGS__); \
    \
    loop.exec();
