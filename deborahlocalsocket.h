#ifndef DEBORAHLOCALSOCKET_H
#define DEBORAHLOCALSOCKET_H

#include <QObject>
#include "deborahsocket.h"

class DeborahLocalSocket : public DeborahSocket
{
    Q_OBJECT
public:
    explicit DeborahLocalSocket(qintptr handle, QObject *parent = nullptr);

    // DeborahSocket interface
public slots:
    void start();
};

#endif // DEBORAHLOCALSOCKET_H
