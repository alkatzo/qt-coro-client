#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>

#include <chrono>

#include "er_integrationmanager.h"
#include "DB/REST/restapiimpl.h"
#include "DB/REST/restexecutor.h"
#include "DB/backend.h"
#include "DB/loghelper.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->listViewFull->setModel(&modelFull);
    ui->listViewFullAll->setModel(&modelFullAll);
    ui->listViewPaged->setModel(&modelPaged);

    ui->checkBox->setChecked(true);

    db::Db::makeDB();
    er::IntegrationManager::initialise();

    _stream = createStream();
}

MainWindow::~MainWindow()
{
    delete ui;
}

db::Stream<QList<QString> > MainWindow::createStream() {
    return db::Db::the->peopleGet(QDateTime::currentDateTime(), {testCtx, stop_token});
}

QCoro::QmlTask MainWindow::qmlTaskTest()
{
    LSCOPE
    return db::Db::the->peopleGetAll(QDateTime::currentDateTime(), testCtx);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    LOG << index;
    switch (index) {
    case 0: {
        stop_token = db::stop_token();
        _stream = createStream();
        break;
    }
    case 1:
        stop_token.request_stop();
        break;
    }
}

/**
 * @brief MainWindow::on_checkBox_stateChanged
 * This is to test testCtx deletion while coro is suspended
 * Create testCtx before executing coro
 * Delete testCtx while the coro is suspended
 */
void MainWindow::on_checkBox_stateChanged(int arg1)
{    
    if (arg1 == Qt::CheckState::Checked) {
        LOG << "Created context testCtx";
        testCtx = new QObject;
    }
    else if (arg1 == Qt::CheckState::Unchecked) {
        LOG << "Deleted context testCtx";
        delete testCtx;
        testCtx = nullptr;
    }
}

/**
 * @brief MainWindow::on_pbStart_clicked
 * Load full result set via coro with pages
 * @return
 */
QCoro::Task<void> MainWindow::on_pbStart_clicked()
{
    LSCOPE
    auto stream = db::Db::the->peopleGet(QDateTime::currentDateTime(), {testCtx, stop_token});
    const QList<QString> &res = co_await stream.result(); // full result set is in res
    showResult(modelFull, res);
}

/**
 * @brief MainWindow::on_pushByPage_clicked
 * Load next page via coro
 * @return
 */
QCoro::Task<void> MainWindow::on_pushByPage_clicked()
{
    LSCOPE
    if (co_await _stream.hasNext()) {
        const QList<QString> &res = _stream.next();
        showResult(modelPaged, res);
    }
}

/**
 * @brief MainWindow::on_pbSignalSlot_clicked
 * Load full result set via signal / slot with pages
 * @return
 */
QCoro::Task<void> MainWindow::on_pbSignalSlot_clicked()
{
    LSCOPE
    LOG << "Sync";
    int res = co_await db::Db::the->peopleGet(QDateTime::currentDateTime(), testCtx).result([this](const auto &res) {
        showResult(modelFull, res);
        return res.size();
    });
    LOG << "Sync #Entries:" << res;
}

/**
 * @brief MainWindow::on_pbStartAll_clicked
 * Load full result set via coro
 */
QCoro::Task<void> MainWindow::on_pbStartAll_clicked()
{
    LSCOPE
    const QList<QString> &res = co_await db::Db::the->peopleGetAll(QDateTime::currentDateTime(), testCtx).result();
    showResult(modelFullAll, res);
}

/**
 * @brief MainWindow::on_pbSignalSlotAll_clicked
 * Load full result set via signal / slot
 * @return
 */
QCoro::Task<void> MainWindow::on_pbSignalSlotAll_clicked()
{
    LSCOPE
    LOG << "Sync with callback";
    int res = co_await db::Db::the->peopleGetAll(QDateTime::currentDateTime(), testCtx).result([this](const auto &res) {
        showResult(modelFullAll, res);
        return res.size();
    });
    LOG << "Sync with callback #Entries:" << res;

    LOG << "Sync with coro callback";
    int coro_res = co_await db::Db::the->peopleGetAll(QDateTime::currentDateTime(), testCtx).result([this](const auto &res) -> QCoro::Task<QList<QString>::size_type> {
        showResult(modelFullAll, res);
        co_return res.size();
    });
    LOG << "Sync with coro callback #Entries:" << coro_res;

    LOG << "Async";
    db::Db::the->peopleGetAll(QDateTime::currentDateTime(), testCtx).result([this](const auto &res) {
        showResult(modelFullAll, res);
    });
}

