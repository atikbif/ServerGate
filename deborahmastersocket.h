#ifndef DEBORAHMASTERSOCKET_H
#define DEBORAHMASTERSOCKET_H

#include <QObject>
#include "deborahsocket.h"

class DeborahMasterSocket : public DeborahSocket
{
    Q_OBJECT
public:
    explicit DeborahMasterSocket(qintptr handle, QObject *parent = nullptr);

    // DeborahSocket interface
public slots:
    void start();
};

#endif // DEBORAHMASTERSOCKET_H
