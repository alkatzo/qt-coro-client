QT       += core gui sql concurrent network quickcontrols2

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

equals(QT_MAJOR_VERSION, 6): CONFIG += c++20
else:win32: QMAKE_CXXFLAGS *= -std:c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DB/RDBMS/odbcclient.cpp \
    DB/RDBMS/rdbmsimpl.cpp \
    DB/RDBMS/sqlquery.cpp \
    DB/REST/restapiimpl.cpp \
    DB/db.cpp \
    er_apidefault.cpp \
    er_base.cpp \
    er_dispatcher.cpp \
    er_integrationmanager.cpp \
    main.cpp \
    mainwindow.cpp \

HEADERS += \
    DB/RDBMS/connectionmanager.h \
    DB/RDBMS/dbpoolexecutor.h \
    DB/RDBMS/dbtraits.h \
    DB/RDBMS/mssql.h \
    DB/RDBMS/odbcclient.h \
    DB/RDBMS/rdbmsimpl.h \
    DB/RDBMS/sqlquery.h \
    DB/REST/helper.h \
    DB/REST/restapiimpl.h \
    DB/REST/restexecutor.h \
    DB/backend.h \
    DB/concepts.h \
    DB/db.h \
    DB/helper.h \
    DB/simpleexecutor.h \
    DB/stream.h \
    er_apidefault.h \
    er_base.h \
    er_dispatcher.h \
    er_integrationmanager.h \
    er_rapifuture.h \
    mainwindow.h \

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# ZLIB
win32:INCLUDEPATH *= $$[QT_INSTALL_HEADERS]/QtZlib

include($$PWD/qcoro/qcoro.pri)
include($$PWD/openapi/ER_client.pri)

# For QtCreator to navigate through <coroutine>
# DEFINES += __cpp_impl_coroutine
