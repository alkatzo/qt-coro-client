#pragma once

#include "DB/db.h"
#include "DB/helper.h"

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
    virtual QCoro::Task<QList<QString>> peopleGet(QDateTime dt) override { co_return co_await ex.template sync<Impl>(__func__, impl, &Impl::peopleGet, dt); }
    virtual void peopleGet(QDateTime dt, std::function <void(QList<QString>)> cb) override { ex.template async<Impl>(__func__, cb, impl, &Impl::peopleGet, dt); }
    virtual void peopleGet(QDateTime dt, QObject *ctx, std::function <void(QList<QString>)> cb) override { ex.template async<Impl>(__func__, ctx, cb, impl, &Impl::peopleGet, dt); }
};

}
