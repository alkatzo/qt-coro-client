#include "er_apidefault.h"
#include "er_integrationmanager.h"

namespace er {

RAPIFuture<QList<ER__people_get_200_response_inner> > ApiDefault::peopleGet(QDateTime dt, const OptionalParam<qint32> &page, int authAttempt) {
    ER_DEFINE_SIGNALS(ApiDefault, peopleGet, ER_DefaultApi, peopleGet, dt, page)
    post([=](){
        api->peopleGet(dt, page);
    });

    return f;
}

}
