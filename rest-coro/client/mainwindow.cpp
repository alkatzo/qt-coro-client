#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>

#include <chrono>

#include "er_integrationmanager.h"
#include "DB/REST/restapiimpl.h"
#include "DB/REST/restexecutor.h"
#include "DB/REST/pager.h"
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
    db::Db::makeDB();
    er::IntegrationManager::initialise();
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

/**
 * @brief MainWindow::exec_await
 * Execute by calling co_await exec_awaitCo();
 * @return
 */
QCoro::Task<void> MainWindow::exec_await()
{
    LSCOPE
    QList<er::ER__people_get_200_response_inner> result = co_await exec_awaitCo();
    for (const er::ER__people_get_200_response_inner& r : result) {
        L << r.getFirstName() << r.getLastName() << r.getDateOfBirth();
    }
    co_return;
}

/**
 * @brief MainWindow::exec_awaitCo
 * execute by calling er::ApiDefault::peopleGet()
 * @return
 */
QCoro::Task<QList<er::ER__people_get_200_response_inner>> MainWindow::exec_awaitCo()
{
    LSCOPE
    auto api = er::IntegrationManager::erApi<er::ApiDefault>().release();

    QList<er::ER__people_get_200_response_inner> result;
    for (int i = 1; i < 4; i++) {
        result.append(co_await api->peopleGet(QDateTime::currentDateTime(), i));
    }

    api->deleteLater();

    co_return result;
}

/**
 * @brief MainWindow::exec_direct_rest
 * execute by calling db::rest::RestApiImpl::peopleGet()
 * @return
 */
QCoro::Task<void> MainWindow::exec_direct_rest()
{
    LSCOPE
    db::rest::RestApiImpl r;
    QList<QString> res = co_await r.peopleGet(QDateTime::currentDateTime());

    for (const auto &s : res) {
        L << s;
    }
}

/**
 * @brief MainWindow::exec_rest
 * execute by calling db::Backend::peopleGet() via RestExecutor executor
 * @return
 */
QCoro::Task<void> MainWindow::exec_rest()
{
    LSCOPE
    auto bk = db::Backend<db::rest::RestApiImpl, db::rest::RestExecutor>();
    QList<QString> res = co_await bk.peopleGet(QDateTime::currentDateTime());

    for (const auto &s : res) {
        L << s;
    }
}

QCoro::Task<void> MainWindow::exec_rest_via_db()
{
    LSCOPE
    QList<QString> res = co_await db::Db::the->peopleGet(QDateTime::currentDateTime());

    for (const auto &s : res) {
        L << s;
    }
}

QCoro::Task<void> MainWindow::exec_rest_via_generator()
{
    LSCOPE
    db::rest::Pager pager;
    auto peoplePager = pager.peopleGet(QDateTime::currentDateTime());
    // We must co_await begin() to obtain the initial iterator
    auto peopleIt = co_await peoplePager.begin();
    L << "Pages:";
    while (peopleIt != peoplePager.end()) {
        L << "Next page:";
        QList<QString> page = *peopleIt;
        for (const auto &s : page) {
            L << s;
        }
        // And we must co_await increment
        co_await ++(peopleIt);
    }
}

void MainWindow::on_pbStart_clicked()
{
    // exec_await();
    // exec_connect();
    // exec_direct_rest();
    // exec_rest();
    // exec_rest_via_db();
    exec_rest_via_generator();
}

