#include "pager.h"

#include "er_integrationmanager.h"

namespace db { namespace rest {

Pager::Pager() {}

QCoro::AsyncGenerator<QList<QString> > Pager::peopleGet(QDateTime dt)
{
    auto api = er::IntegrationManager::erApi<er::ApiDefault>().release();

    for (int i = 1; i < 4; i++) {
        const QList<er::ER__people_get_200_response_inner> result = co_await api->peopleGet(dt, i);
        QList<QString> ret;
        for (const er::ER__people_get_200_response_inner& e : result) {
            ret.append(e.getFirstName());
        }
        co_yield ret;
    }

    api->deleteLater();
}

}}
