QT += network

HEADERS += \
# Models
    $${PWD}/ER__people_get_200_response_inner.h \
# APIs
    $${PWD}/ER_DefaultApi.h \
# Others
    $${PWD}/ER_Helpers.h \
    $${PWD}/ER_HttpRequest.h \
    $${PWD}/ER_Object.h \
    $${PWD}/ER_Enum.h \
    $${PWD}/ER_HttpFileElement.h \
    $${PWD}/ER_ServerConfiguration.h \
    $${PWD}/ER_ServerVariable.h \
    $${PWD}/ER_Oauth.h

SOURCES += \
# Models
    $${PWD}/ER__people_get_200_response_inner.cpp \
# APIs
    $${PWD}/ER_DefaultApi.cpp \
# Others
    $${PWD}/ER_Helpers.cpp \
    $${PWD}/ER_HttpRequest.cpp \
    $${PWD}/ER_HttpFileElement.cpp \
    $${PWD}/ER_Oauth.cpp
