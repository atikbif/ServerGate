#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScrollBar>
#include <QRegExp>
#include <QDateTime>
#include <QDomDocument>
#include <QFile>

const QString MainWindow::confFileName = "gateway.xml";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),curServer(nullptr),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readConfig();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readConfig()
{
    QString fName = QApplication::applicationDirPath()+"/" + confFileName;
    QDomDocument doc("config");
    QFile file(fName);
    if (!file.open(QIODevice::ReadOnly)) return;
    if (!doc.setContent(&file)) {
        return;
    }
    if(doc.elementsByTagName("gateConf").count()!=1) return;

    QDomNodeList gates = doc.elementsByTagName("gate");
    for(int i=0;i<gates.count();i++) {
        QDomNode n = gates.item(i);
        QDomElement e = n.toElement();
        if(!e.isNull()) {
            QString gName = e.attribute("name");
            QString gPort = e.attribute("port");
            QString gWait = e.attribute("waitTime");

            DeborahServer *server = new DeborahServer(gPort.toInt(), DeborahServer::GATE, this);
            server->setWaitMaxTime(gWait.toInt());
            servers.append(server);
            ui->listWidget->addItem(gName);

            QDomNodeList childs = e.childNodes();
            for(int j=0;j<childs.count();j++) {
                QDomNode n = childs.item(j);
                QDomElement e = n.toElement();
                if(e.tagName()=="localClient") {
                   QString lPort = e.attribute("port");
                   //QString lName = e.attribute("name");
                   DeborahServer *local = new DeborahServer(lPort.toInt(), DeborahServer::LOCAL, this);
                   server->connectLocalServer(*local);
                }
            }
            connect(server,SIGNAL(message(QString)),this,SLOT(message(QString)));
        }
    }
    if(servers.count()) {
        ui->listWidget->setCurrentRow(0);
        curServer = servers.at(0);
    }
}

void MainWindow::message(const QString &m)
{
    DeborahServer *ptr = dynamic_cast<DeborahServer*>(sender());
    if(ptr!=nullptr) {
        if(ptr==curServer) {
            ui->textBrowser->clear();
            ui->textBrowser->insertPlainText(ptr->getLog());
            ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());
        }
        QRegExp e("new connection(.*)");
        if (e.indexIn(m) != -1) {
            for(int i=0;i<servers.count();i++) {
                DeborahServer *s = servers.at(i);
                if(s==ptr) {
                    QString ip = e.cap(1);
                    ip.remove("\r\n");
                    QString obName = ui->listWidget->item(i)->text();
                    obName.remove(QRegExp("IP:.*"));
                    obName+=" IP:" + ip;
                    obName += " " + QDateTime::currentDateTime().toString("dd MMMM yyyy HH:mm:ss");
                    ui->listWidget->item(i)->setText(obName);
                    ui->listWidget->item(i)->setTextColor(Qt::black);
                    break;
                }
            }

        }else {
            QRegExp e("RX:");
            if(e.indexIn(m) != -1) {
                for(int i=0;i<servers.count();i++) {
                    DeborahServer *s = servers.at(i);
                    if(s==ptr) {
                        ui->listWidget->item(i)->setTextColor(Qt::darkGreen);
                        break;
                    }
                }
            }else {
                QRegExp e("timeout");
                if(e.indexIn(m) != -1) {
                    for(int i=0;i<servers.count();i++) {
                        DeborahServer *s = servers.at(i);
                        if(s==ptr) {
                            ui->listWidget->item(i)->setTextColor(Qt::darkYellow);
                            break;
                        }
                    }
                }else {
                    QRegExp e("not connected");
                    if(e.indexIn(m) != -1) {
                        for(int i=0;i<servers.count();i++) {
                            DeborahServer *s = servers.at(i);
                            if(s==ptr) {
                                ui->listWidget->item(i)->setTextColor(Qt::darkRed);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    if(currentRow < servers.count()) {
        DeborahServer *ptr = servers.at(currentRow);
        if(ptr!=curServer) {
            ui->textBrowser->clear();
            ui->textBrowser->insertPlainText(ptr->getLog());
        }
        curServer = ptr;
    }
}
