#include "restapiimpl.h"

#include "er_integrationmanager.h"
#include "DB/loghelper.h"
#include "zeus/expected.hpp"

namespace db { namespace rest {

RestApiImpl::RestApiImpl() {}

QCoro::AsyncGenerator<QList<QString> > RestApiImpl::peopleGet(QDateTime dt, db::Cancel c)
{
    auto api = er::IntegrationManager::erApi<er::ApiDefault>();

    for (int i = 1; i < 4; i++) {
        if (c.stop()) {
            LOG << "Cancelled";
            break;
        }
        LOG << "Awaiting next page";
        auto res = co_await api->peopleGet(dt, i);
        if (res) {
            const QList<er::ER__people_get_200_response_inner> result = res.value();
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
        else {
            LOG << "Error" << res.error();
            break;
        }
    }
}

QCoro::Task<QList<QString> > RestApiImpl::peopleGetAll(QDateTime dt)
{
    auto api = er::IntegrationManager::erApi<er::ApiDefault>();
    auto res = co_await api->peopleGet(dt);
    if (res) {
        const QList<er::ER__people_get_200_response_inner> result = res.value();
        QList<QString> ret;
        for (const er::ER__people_get_200_response_inner& e : result) {
            QString str = QString("%1 %2 %3").arg(e.getFirstName()).arg(e.getLastName()).arg(e.getDateOfBirth().toString());
            ret.append(str);
        }

        co_return ret;
    }
    else {
        LOG << "Error" << res.error();
        co_return {};
    }
}

}}
