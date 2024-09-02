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
    delete modelFull;
    delete modelPaged;
    delete _stream;
    delete ui;
}

/**
 * @brief MainWindow::on_pbStart_clicked
 * Load full result set via coro
 * @return
 */
QCoro::Task<void> MainWindow::on_pbStart_clicked() {
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
 * Load full result set via signal / slot
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

