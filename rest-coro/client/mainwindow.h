#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListView>

#include "qcoro/qcorotask.h"
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

private slots:
    // Paged
    QCoro::Task<void> on_pbStart_clicked();

    QCoro::Task<void> on_pushByPage_clicked();

    void on_pbSignalSlot_clicked();

    // Full
    QCoro::Task<void> on_pbStartAll_clicked();

    void on_pbSignalSlotAll_clicked();

private:
    Ui::MainWindow *ui;

    QStringListModel modelFull;
    QStringListModel modelFullAll;
    QStringListModel modelPaged;

    db::Stream<QList<QString>> * _stream;
};
#endif // MAINWINDOW_H
