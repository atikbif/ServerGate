#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "deborahserver.h"
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QVector<DeborahServer*> servers;
    DeborahServer* curServer;
    static const QString confFileName;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void readConfig();
private slots:
    void message(const QString &m);
    void on_listWidget_currentRowChanged(int currentRow);
};

#endif // MAINWINDOW_H
