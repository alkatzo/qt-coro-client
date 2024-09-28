#pragma once

#include <QObject>
#include "er_base.h"
#include "openapi/ER_DefaultApi.h"

namespace er {

class ApiDefault : public QObject, public Client
{
    Q_OBJECT
public:
    explicit ApiDefault() = default;

public:
    RAPIFuture<QList<ER__people_get_200_response_inner>> peopleGet(QDateTime dt, const OptionalParam<qint32> &page = OptionalParam<qint32>(), int authAttempt = 0);

    void requestCompleted();
};

}
