#include "deborahsocket.h"
#include <QMutexLocker>
#include <QThread>
#include <QDebug>



DeborahSocket::DeborahSocket(qintptr handle, QObject *parent): QTcpSocket(parent),
    waitMaxTime(3000), stopCmd(false), descript(handle)
{

}

DeborahSocket::~DeborahSocket()
{

}

void DeborahSocket::stop()
{
    QMutexLocker lock(&mutex);
    stopCmd = true;
}

void DeborahSocket::treatExternalRequest(QByteArray &reqData, int reqID)
{
    QMutexLocker lock(&mutex);
    ReqData *ptr = new ReqData;
    ptr->data = reqData;
    ptr->id = reqID;
    for(int i=0;i<reqs.count();) {
        ReqData *r = reqs.at(i);
        if(r->id == reqID) {reqs.remove(i);}
        else ++i;
    }
    reqs.append(ptr);
}
