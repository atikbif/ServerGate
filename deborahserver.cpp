#include "deborahserver.h"
#include "deborahgatesocket.h"
#include "deborahmastersocket.h"
#include "deborahlocalsocket.h"
#include <QDebug>
#include <QDateTime>
#include <QHostAddress>

DeborahSocket *DeborahServer::createSocket(DeborahServer::SocketType sType, qintptr handle)
{
    DeborahSocket *s = nullptr;
    switch(sType) {
        case GATE: s = new DeborahGateSocket(handle);break;
        case MASTER: s = new DeborahMasterSocket(handle);break;
        case LOCAL: s = new DeborahLocalSocket(handle);break;
    }
    return s;
}

DeborahServer::DeborahServer(quint16 portNum, SocketType sType, QObject *parent): QTcpServer(parent),
    socket(nullptr),thread(nullptr),type(sType)
{
    if(listen(QHostAddress::Any,portNum)==false) emit message(QTime::currentTime().toString("hh:mm:ss.zzz") + ": error - " + errorString()+"\r\n");
}

DeborahServer::~DeborahServer()
{

}

bool DeborahServer::isClientConnected() const
{
    if(socket!=nullptr) return (socket->state()==QAbstractSocket::ConnectedState);
    return false;
}

void DeborahServer::connectLocalServer(DeborahServer &serv)
{
    if(((type==GATE)||(type==MASTER))&&(serv.getType()==LOCAL)) {
        connect(this,SIGNAL(newRequest(QByteArray,int)),&serv,SLOT(treatExternalRequest(QByteArray,int)));
        connect(&serv,SIGNAL(newRequest(QByteArray,int)),this,SLOT(treatExternalRequest(QByteArray,int)));
    }
}

void DeborahServer::incomingConnection(qintptr handle)
{
    if(socket!=nullptr) socket->stop();
    socket = createSocket(type,handle);

    thread = new QThread();
    socket->moveToThread(thread);
    emit message(QTime::currentTime().toString("hh:mm:ss.zzz") + ": new connection (" + socket->peerAddress().toString() + ")\r\n");

    connect(socket,SIGNAL(newRequest(QByteArray,int)),this,SIGNAL(newRequest(QByteArray,int)));
    connect(socket,SIGNAL(message(QString)),this,SIGNAL(message(QString)));
    connect(socket, SIGNAL (finished()), thread, SLOT (quit()));
    connect(socket, SIGNAL (finished()), socket, SLOT (deleteLater()));
    connect(thread, SIGNAL (finished()), thread, SLOT (deleteLater()));
    connect(this,SIGNAL(startSocket()),socket,SLOT(start()));
    thread->start();
    emit startSocket();
}

void DeborahServer::treatExternalRequest(QByteArray reqData, int reqID)
{
    if(socket!=nullptr) socket->treatExternalRequest(reqData, reqID);
}
