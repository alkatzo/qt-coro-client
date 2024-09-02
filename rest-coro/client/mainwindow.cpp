#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>

#include <chrono>

#include "er_integrationmanager.h"
#include "DB/REST/restapiimpl.h"
#include "DB/REST/restexecutor.h"
#include "DB/backend.h"

#define L qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz") << __FUNCTION__
struct LogScope {
    QString msg;
    LogScope(const QString& m) : msg(m) {
        qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz") << msg << "->";
    }

    ~LogScope() {
        qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz") << msg << "<-";
    }
};
#define LSCOPE LogScope l(__FUNCTION__);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    modelFull = new QStringListModel(this);
    modelPaged = new QStringListModel(this);

    ui->listViewFull->setModel(modelFull);
    ui->listViewPaged->setModel(modelPaged);

    db::Db::makeDB();
    er::IntegrationManager::initialise();

    _stream = new db::Stream(db::Db::the->peopleGet(QDateTime::currentDateTime()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::exec_connect()
{
    er::ApiDefault *api = er::IntegrationManager::erApi<er::ApiDefault>().release();
    api->peopleGet(QDateTime::currentDateTime());
    connect(api, &er::ApiDefault::peopleGetCompletedOK, this, [api](QList<er::ER__people_get_200_response_inner> result){
        for (const er::ER__people_get_200_response_inner& r : result) {
            L << r.getFirstName() << r.getLastName() << r.getDateOfBirth();
        }
        api->deleteLater();
    });
}

QCoro::Task<void> MainWindow::exec()
{
    QList<QString> res = co_await exec_rest_via_db();
    for (const auto &s : res) {
        L << s;
    }
    exec_rest_pager_via_db();
}

/**
 * @brief MainWindow::exec_rest_via_db
 * Shows how to fetch full result set and also return it up the stack.
 * The result is returned to only demonstrate how it can be done if needed.
 * In reality Stream object can be returned directly from the method and co_awaited up the stack
 * @return
 */
QCoro::Task<QList<QString> > MainWindow::exec_rest_via_db()
{
    LSCOPE
    auto stream = db::Db::the->peopleGet(QDateTime::currentDateTime());
    QList<QString> res = co_await stream.result(); // full result set is in res
    co_return res;
}

/**
 * @brief MainWindow::exec_rest_pager_via_db
 * Shows how to fetch the result set by pages
 * @return
 */
QCoro::Task<void> MainWindow::exec_rest_pager_via_db()
{
    LSCOPE
    auto stream = db::Db::the->peopleGet(QDateTime::currentDateTime());

    // We must co_await begin() to obtain the initial iterator
    auto peopleIt = co_await stream.begin();

    L << "Pages:";
    while (peopleIt != stream.end()) {
        L << "Next page:";
        QList<QString> page = *peopleIt;
        for (const auto &s : page) {
            L << s;
        }
        // And we must co_await increment
        co_await ++(peopleIt);
    }
}

QCoro::Task<void> MainWindow::on_pbStart_clicked() {
    LSCOPE
    auto stream = db::Db::the->peopleGet(QDateTime::currentDateTime());
    QList<QString> res = co_await stream.result(); // full result set is in res

    QStringList lst = modelFull->stringList();
    for (const auto &s : res) {
        lst.append(s);
    }
    modelFull->setStringList(lst);
}


QCoro::Task<void> MainWindow::on_pushByPage_clicked()
{
    if (co_await _stream->hasNext()) {
        QList<QString> page = *_stream->next();

        QStringList lst = modelPaged->stringList();
        for (const auto &s : page) {
            lst.append(s);
        }
        modelPaged->setStringList(lst);
    }
}

