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

    QCoro::Task<void> exec_await();
    QCoro::Task<QList<er::ER__people_get_200_response_inner>> exec_awaitCo();

    QCoro::Task<void> exec_direct_rest();

    QCoro::Task<void> exec_rest();

    QCoro::Task<void> exec_rest_via_db();

    QCoro::Task<void> exec_rest_via_generator();

private slots:
    void on_pbStart_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
