#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "qcoro/qcorotask.h"
#include "openapi/ER_DefaultApi.h"

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
    void exec_connect();

    QCoro::Task<void> exec();

    QCoro::Task<QList<QString>> exec_rest_via_db();

    QCoro::Task<void> exec_rest_pager_via_db();

private slots:
    void on_pbStart_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
