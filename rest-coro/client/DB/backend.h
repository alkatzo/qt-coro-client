#pragma once

#include <QDateTime>

#include "DB/db.h"

namespace db {

/**
 *@brief The Backend class
 *Backend class implements the interface defined by DB:Db
 *It dispatches execution of the calls to appropriate implementation class Impl constructed during application initialisation stage
 *How the calls are executed is determined by the executor policy Ex class
 */
template<typename Impl, typename Ex>
class Backend : public Db
{
public:
    Backend() : impl{new Impl} {
    }

    ~Backend() {
        delete impl;
    }

private:
    Ex ex;
    Impl  *impl{};


    // DB interface
public:
    virtual Stream<QList<QString>> peopleGet(QDateTime dt, Cancel c) override { return ex.template sync_paged<Impl>(__func__, c, impl, &Impl::peopleGet, dt); }
    virtual Task<QList<QString>> peopleGetAll(QDateTime dt, Cancel c) override { return ex.template sync<Impl>(__func__, c, impl, &Impl::peopleGetAll, dt); }
};

}
