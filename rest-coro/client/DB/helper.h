#pragma once

#include <QDateTime>
#include <QThread>
#include <QDebug>

#include "DB/concepts.h"

#define TID QString("(%1)").arg((long long)QThread::currentThreadId())

#define LOG_NO_FUNC qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz") << TID
#define LOG LOG_NO_FUNC << __func__

struct LogScope {
    QString msg;
    LogScope(const QString& m) : msg(m) {
        LOG_NO_FUNC << msg << "->";
    }

    ~LogScope() {
        LOG_NO_FUNC << msg << "<-";
    }
};
#define LSCOPE LogScope l(__func__);

namespace db {

// Helper logging functions maybe delete later
inline void log_start(QString s) {
    LOG_NO_FUNC << s << "Started";
}

template<typename R>
inline void log_finish(QString s, const R&) {
    LOG_NO_FUNC << s << "Finished";
}

template<QPtrContainer R>
inline void log_finish(QString s, const R& r) {
    LOG_NO_FUNC << s << "Finished #Entries:" << r.size();;
}

}
