#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>

#include <chrono>

#include "er_integrationmanager.h"
#include "DB/REST/restapiimpl.h"
#include "DB/REST/restexecutor.h"
#include "DB/backend.h"
#include "DB/helper.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->listViewFull->setModel(&modelFull);
    ui->listViewFullAll->setModel(&modelFullAll);
    ui->listViewPaged->setModel(&modelPaged);

    db::Db::makeDB();
    er::IntegrationManager::initialise();

    _stream = new db::Stream(db::Db::the->peopleGet(QDateTime::currentDateTime()));
}

MainWindow::~MainWindow()
{
    delete _stream;
    delete ui;
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    LOG << arg1;
    if (arg1 == Qt::CheckState::Checked) {
        testCtx = new QObject;
    }
    else if (arg1 == Qt::CheckState::Unchecked) {
        delete testCtx;
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
    auto stream = db::Db::the->peopleGet(QDateTime::currentDateTime());
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
    if (co_await _stream->hasNext()) {
        const QList<QString> &res = _stream->next();
        showResult(modelPaged, res);
    }
}

/**
 * @brief MainWindow::on_pbSignalSlot_clicked
 * Load full result set via signal / slot with pages
 * @return
 */
void MainWindow::on_pbSignalSlot_clicked()
{
    LSCOPE
    auto stream = new db::Stream<QList<QString>>(db::Db::the->peopleGet(QDateTime::currentDateTime()));
    stream->result([stream, this](const auto &res) {
        showResult(modelFull, res);
        delete stream;
    });
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
void MainWindow::on_pbSignalSlotAll_clicked()
{
    LSCOPE
    auto task = new db::Task<QList<QString>>(db::Db::the->peopleGetAll(QDateTime::currentDateTime(), testCtx));
    task->result([task, this](const auto &res) {
        showResult(modelFullAll, res);
        delete task;
    });
}




