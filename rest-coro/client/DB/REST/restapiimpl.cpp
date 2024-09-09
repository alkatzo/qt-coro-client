#include "restapiimpl.h"

#include "er_integrationmanager.h"
#include "DB/loghelper.h"

namespace db { namespace rest {

RestApiImpl::RestApiImpl() {}

QCoro::AsyncGenerator<QList<QString> > RestApiImpl::peopleGet(QDateTime dt, db::Cancel c)
{
    auto api = er::IntegrationManager::erApi<er::ApiDefault>().release();

    for (int i = 1; i < 4; i++) {
        if (c.stop()) {
            LOG << "Cancelled";
            break;
        }
        LOG << "Awaiting next page";
        const QList<er::ER__people_get_200_response_inner> result = co_await api->peopleGet(dt, i);
        if (c.stop()) {
            free(result);
            LOG << "Cancelled";
            break;
        }
        QList<QString> ret;
        for (const er::ER__people_get_200_response_inner& e : result) {
            QString str = QString("%1 %2 %3").arg(e.getFirstName()).arg(e.getLastName()).arg(e.getDateOfBirth().toString());
            ret.append(str);
        }
        LOG << "Return result and suspend";
        co_yield ret;
        LOG << "Resume";
    }

    api->deleteLater();
}

QCoro::Task<QList<QString> > RestApiImpl::peopleGetAll(QDateTime dt)
{
    auto api = er::IntegrationManager::erApi<er::ApiDefault>().release();
    const QList<er::ER__people_get_200_response_inner> result = co_await api->peopleGet(dt);
    QList<QString> ret;
    for (const er::ER__people_get_200_response_inner& e : result) {
        QString str = QString("%1 %2 %3").arg(e.getFirstName()).arg(e.getLastName()).arg(e.getDateOfBirth().toString());
        ret.append(str);
    }

    api->deleteLater();

    co_return ret;
}

}}
