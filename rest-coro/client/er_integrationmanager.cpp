#include "er_integrationmanager.h"
#include <QSettings>

#include "DB/loghelper.h"


namespace er {

void IntegrationManager::initialise() {
    LOG;
    _server = "http://172.29.208.1:5000/v1";
    _version = "1";
    _serverDesc = "Ma nigga";
    _timeOut = 20000;
    _pageSize = 1;
}

}
