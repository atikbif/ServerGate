#ifndef DEBORAHGATESOCKET_H
#define DEBORAHGATESOCKET_H

#include <QObject>
#include "deborahsocket.h"

class DeborahGateSocket : public DeborahSocket
{
    Q_OBJECT

    void txRxMessage(const QByteArray &data, bool txFlag);
    void txMessage(const QByteArray &data);
    void rxMessage(const QByteArray &data);
public:
    explicit DeborahGateSocket(qintptr handle, QObject *parent =0);

    // DeborahSocket interface
public slots:
    void start();
};

#endif // DEBORAHGATESOCKET_H
