#include "er_apidefault.h"
#include "er_integrationmanager.h"

namespace er {

RAPIFuture<QList<ER__people_get_200_response_inner> > ApiDefault::peopleGet(QDateTime dt, const OptionalParam<qint32> &page, int authAttempt) {
    WIRE_API_SIGNALS(QList<ER__people_get_200_response_inner>, ER_DefaultApi, peopleGet)
    post([=](){
        api->peopleGet(dt, page);
    });

    return f;
}

}
