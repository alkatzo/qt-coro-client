#include "restapiimpl.h"

#include "er_integrationmanager.h"

namespace db { namespace rest {

RestApiImpl::RestApiImpl() {}

QCoro::Task<QList<QString> > RestApiImpl::peopleGet(QDateTime dt)
{
    auto api = er::IntegrationManager::erApi<er::ApiDefault>().release();

    QList<er::ER__people_get_200_response_inner> result;
    for (int i = 1; i < 4; i++) {
        result.append(co_await api->peopleGet(QDateTime::currentDateTime(), i));
    }

    api->deleteLater();

    QList<QString> ret;
    for (const er::ER__people_get_200_response_inner& e : result) {
        ret.append(e.getFirstName());
    }

    co_return ret;
}

}}
