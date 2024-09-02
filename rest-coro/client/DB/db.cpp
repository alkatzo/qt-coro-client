#include "db.h"
#include "DB/backend.h"
#include "DB/REST/restapiimpl.h"
#include "DB/REST/restexecutor.h"

namespace db {

Db *Db::makeDB() {
    if (the != nullptr) {
        delete the;
        the = nullptr;
    }

    the = new db::Backend<db::rest::RestApiImpl, db::rest::RestExecutor>;

    return the;
}

}
