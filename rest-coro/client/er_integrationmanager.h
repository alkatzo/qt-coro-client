#pragma once

#include <QObject>
#include <QString>
#include <queue>

#include "er_apidefault.h"

namespace er {

class IntegrationManager {

public:
    static void initialise();

    // ER APIs getter
    template<typename Api>
    static std::unique_ptr<Api> erApi() {
        return std::make_unique<Api>();
    }

    // Generated APIs getter
    template<typename Api>
    static std::unique_ptr<Api> api() {
        auto api = std::make_unique<Api>(_timeOut);
        api->setNewServerForAllOperations(_server, _serverDesc);
        return api;
    }

    static uint pageSize() {
        return _pageSize;
    }

private:
    static inline QString _server;
    static inline QString _serverDesc;
    static inline QString _version;
    static inline uint _timeOut;
    static inline uint _pageSize;
};

}
