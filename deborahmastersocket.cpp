#include "deborahmastersocket.h"
#include <QThread>

DeborahMasterSocket::DeborahMasterSocket(qintptr handle, QObject *parent): DeborahSocket(handle, parent)
{

}

void DeborahMasterSocket::start()
{
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
            thread()->msleep(1);
            mutex.lock();
            if(reqs.count()) {
                ReqData *r = reqs.first();
                reqs.removeFirst();
                mutex.unlock();
                write(r->data);
                waitForBytesWritten(3000);
                delete r;
            }else mutex.unlock();
        }
    }else emit message("error: " + errorString());
}

