#pragma once

#include <QObject>

#include <stop_token>

#include "DB/concepts.h"


namespace db {

class Cancel {
public:
    Cancel(QObject* ctx) : ctx(ctx) {}
    Cancel(QObject* ctx, const std::stop_token& st) : ctx(ctx), stop_token(st) {}

    bool stop() const {
        return !ctx || stop_token.stop_requested();
    }

private:
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
    static void free(const T &) { }

    template<PtrContainer T>
    static void free(const T &arg) {
        qDeleteAll(arg);
    }

    template<PtrType T>
    static void free(const T &arg) {
        delete arg;
    }
};


template<typename T>
inline void free(const T& res) {
    Deleter::free(res);
}

}

