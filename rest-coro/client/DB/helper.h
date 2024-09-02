#pragma once

#include <QDebug>

#include "DB/concepts.h"

namespace db {

// Helper logging functions maybe delete later
inline void log_start(QString s) {
    qDebug() << s.toLatin1().data() << "Started";
}

template<typename R>
inline void log_finish(QString s, const R&) {
    qDebug() << s.toLatin1().data() << "Finished";
}

template<QPtrContainer R>
inline void log_finish(QString s, const R& r) {
    qDebug() << s.toLatin1().data() << "Finished #Entries:" << r.size();;
}

}
