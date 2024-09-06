#pragma once

#include <stop_token>

#include "DB/concepts.h"
#include "DB/helper.h"

#include "er_integrationmanager.h"

namespace db {

struct Cancel {
    QPointer<QObject> ctx;
    std::stop_token stop_token;
};

/**
 * @brief The Deleter class
 * Default deleter class to dealocate results when ctx object is gone
 */
class Deleter {
public:
    template<typename T>
    static void free(const T &) {
        LOG << "Non-deletable";
    }

    template<PtrContainer T>
    static void free(const T &arg) {
        LOG << "Deleting PtrContainer";
        qDeleteAll(arg);
    }

    template<PtrType T>
    static void free(const T &arg) {
        LOG << "Deleting Ptr";
        delete arg;
    }
};


template<typename T>
inline void deleteResults(const T& res) {
    Deleter::free(res);
}

}

