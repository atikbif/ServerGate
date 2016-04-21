#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScrollBar>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server = new DeborahServer(1234, DeborahServer::GATE, this);
    DeborahServer *local1 = new DeborahServer(1235, DeborahServer::LOCAL, this);
    DeborahServer *local2 = new DeborahServer(1236, DeborahServer::LOCAL, this);
    server->connectLocalServer(*local1);
    server->connectLocalServer(*local2);
    connect(server,SIGNAL(message(QString)),this,SLOT(message(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::message(const QString &m)
{
    if(ui->textBrowser->document()->blockCount()>=100) ui->textBrowser->clear();
    ui->textBrowser->insertPlainText(m);
    ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());
}
