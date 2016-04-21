#include "deborahlocalsocket.h"
#include <QThread>
#include <QDebug>

DeborahLocalSocket::DeborahLocalSocket(qintptr handle, QObject *parent): DeborahSocket(handle, parent)
{

}

void DeborahLocalSocket::start()
{
    QByteArray data;
    if(setSocketDescriptor(descript)) {
        setSocketOption(QAbstractSocket::KeepAliveOption,true);
        forever {
            mutex.lock();
            if(stopCmd) {
                mutex.unlock();
                emit finished();
                break;
            }
            mutex.unlock();
            data.clear();
            waitForReadyRead(5);
            data.append(readAll());
            if(data.count()) {
                emit newRequest(data,socketDescriptor());
            }
            thread()->msleep(1);
            mutex.lock();
            if(reqs.count()) {
                ReqData *r = reqs.first();
                reqs.removeFirst();
                mutex.unlock();
                if(r->id == (int)socketDescriptor()) {
                    write(r->data);
                    waitForBytesWritten(3000);
                }
                delete r;
            }else mutex.unlock();
        }
    }else emit message("error: " + errorString());
}
