QT += network
equals(QT_MAJOR_VERSION, 6): QT += qml-private

INCLUDEPATH += $$PWD $$PWD/.. $$PWD/core
DEPENDPATH += $$PWD $$PWD/.. $$PWD/core

HEADERS += \
    $$PWD/concepts_p.h \
    $$PWD/core/impl/isqprivatesignal.h \
    $$PWD/core/qcorocore.h \
    $$PWD/core/qcorofuture.h \
    $$PWD/core/qcoroiodevice.h \
    $$PWD/core/qcoroiodevice_p.h \
    $$PWD/core/qcoroprocess.h \
    $$PWD/core/qcorosignal.h \
    $$PWD/core/qcorothread.h \
    $$PWD/core/qcorotimer.h \
    $$PWD/coroutine.h \
    $$PWD/impl/connect.h \
    $$PWD/impl/task.h \
    $$PWD/impl/taskawaiterbase.h \
    $$PWD/impl/taskfinalsuspend.h \
    $$PWD/impl/taskpromise.h \
    $$PWD/impl/taskpromisebase.h \
    $$PWD/impl/waitfor.h \
    $$PWD/macros_p.h \
    $$PWD/network/qcoroabstractsocket.h \
    $$PWD/network/qcorolocalsocket.h \
    $$PWD/network/qcoronetwork.h \
    $$PWD/network/qcoronetworkreply.h \
    $$PWD/network/qcorotcpserver.h \
    $$PWD/qcoro.h \
    $$PWD/qcoroasyncgenerator.h \
    $$PWD/qcorofwd.h \
    $$PWD/qcorogenerator.h \
    $$PWD/qcorotask.h \
    $$PWD/qml/qcoroqml.h \
    $$PWD/qml/qcoroqmltask.h \
    $$PWD/task.h \
    $$PWD/waitoperationbase_p.h

SOURCES += \
    $$PWD/core/qcoroiodevice.cpp \
    $$PWD/core/qcoroiodevice_p.cpp \
    $$PWD/core/qcoroprocess.cpp \
    $$PWD/core/qcorothread.cpp \
    $$PWD/core/qcorotimer.cpp \
    $$PWD/network/qcoroabstractsocket.cpp \
    $$PWD/network/qcorolocalsocket.cpp \
    $$PWD/network/qcoronetworkreply.cpp \
    $$PWD/network/qcorotcpserver.cpp \
    $$PWD/qml/qcoroqml.cpp \
    $$PWD/qml/qcoroqmltask.cpp
