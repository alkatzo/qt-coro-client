#pragma once

#include <QDateTime>
#include <QThread>
#include <QDebug>

#include "DB/concepts.h"

#define TID QString("(%1)").arg((long long)QThread::currentThreadId())

#define LOG qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz") << TID

struct LogScope {
    QString msg;
    LogScope(const QString& m) : msg(m) {
        LOG << msg << "->";
    }

    ~LogScope() {
        LOG << msg << "<-";
    }
};
#define LSCOPE LogScope l(__FUNCTION__);

namespace db {

// Helper logging functions maybe delete later
inline void log_start(QString s) {
    LOG << s << "Started";
}

template<typename R>
inline void log_finish(QString s, const R&) {
    LOG << s << "Finished";
}

template<QPtrContainer R>
inline void log_finish(QString s, const R& r) {
    LOG << s << "Finished #Entries:" << r.size();;
}

}
