#include "deborahgatesocket.h"
#include <QThread>
#include <QDebug>
#include <QDateTime>
#include <QHostAddress>

void DeborahGateSocket::txRxMessage(const QByteArray &data, bool txFlag)
{
    QString res = QTime::currentTime().toString("hh:mm:ss.zzz");
    if(txFlag) res+= " TX:"; else res+= " RX:";
    foreach (quint8 v, data) {
        QString code = QString::number(v,16);
        if(code.count()<2) code = "0" + code;
        res+=code + " ";
    }
    res += "\r\n";
    emit message(res);
}

void DeborahGateSocket::txMessage(const QByteArray &data)
{
    txRxMessage(data,true);
}

void DeborahGateSocket::rxMessage(const QByteArray &data)
{
    txRxMessage(data,false);
}

DeborahGateSocket::DeborahGateSocket(qintptr handle, QObject *parent): DeborahSocket(handle,parent)
{

}

void DeborahGateSocket::start()
{
    SocketState st;
    QByteArray data;
    if(setSocketDescriptor(descript)) {
        setSocketOption(QAbstractSocket::KeepAliveOption,true);
        waitForConnected();
        emit message(QTime::currentTime().toString("hh:mm:ss.zzz") + ": new connection (" + peerAddress().toString() + ")\r\n");
        forever {
            mutex.lock();
            if(stopCmd) {
                mutex.unlock();
                emit finished();
                break;
            }
            mutex.unlock();

            thread()->msleep(1);
            mutex.lock();
            if(reqs.count()) {
                waitForReadyRead(1);
                readAll();
                ReqData *r = reqs.first();
                reqs.removeFirst();
                mutex.unlock();
                write(r->data);
                st = state();
                if(st != SocketState::ConnectedState) {
                    emit message(QTime::currentTime().toString("hh:mm:ss.zzz")+ ": not connected\r\n");
                }
                else {
                    waitForBytesWritten(1000);
                    st = state();

                    txMessage(r->data);

                    data.clear();
                    int tmr = 0;
                    while(tmr<waitMaxTime/100) {
                        if(waitForReadyRead(100)) {
                            data.append(readAll());
                            break;
                        }
                        tmr++;
                    }
                    if(data.count()) {
                        emit newRequest(data,r->id);
                        rxMessage(data);
                    }else emit message(QTime::currentTime().toString("hh:mm:ss.zzz")+ ": answer timeout\r\n");
                }
                delete r;
            }else mutex.unlock();
        }
    }else emit message(QTime::currentTime().toString("hh:mm:ss.zzz") + ": error - " + errorString() + "\r\n");
}
