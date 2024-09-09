#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListView>

#include <stop_token>

#include <QCoroTask>
#include <QCoroQmlTask>
#include "openapi/ER_DefaultApi.h"
#include "DB/stream.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void showResult(QStringListModel &m, const auto &res) {
        QStringList lst = m.stringList();
        for (const auto &s : res) {
            lst.append(s);
        }
        m.setStringList(lst);
    }

    db::Stream<QList<QString>> createStream();

    Q_INVOKABLE QCoro::QmlTask qmlTaskTest();

private slots:
    // Paged
    QCoro::Task<void> on_pbStart_clicked();

    QCoro::Task<void> on_pushByPage_clicked();

    void on_pbSignalSlot_clicked();

    // Full
    QCoro::Task<void> on_pbStartAll_clicked();

    void on_pbSignalSlotAll_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_tabWidget_currentChanged(int index);

private:
    Ui::MainWindow *ui;

    QStringListModel modelFull;
    QStringListModel modelFullAll;
    QStringListModel modelPaged;

    db::Stream<QList<QString>> _stream;

    std::stop_source stop_source;

    QObject *testCtx = nullptr;
};
#endif // MAINWINDOW_H
