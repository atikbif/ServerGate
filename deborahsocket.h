#ifndef DEBORAHSOCKET_H
#define DEBORAHSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QMutex>
#include <QString>
#include <QByteArray>
#include <QVector>
#include <QDebug>

class DeborahSocket : public QTcpSocket
{
    Q_OBJECT

protected:
    struct ReqData{
        QByteArray data;
        int id;
    };
    QVector<ReqData*> reqs;
    bool stopCmd;
    qintptr descript;
    mutable QMutex mutex;

public:
    explicit DeborahSocket(qintptr handle, QObject *parent = 0);
    ~DeborahSocket();
    void stop();
    void treatExternalRequest(QByteArray &reqData, int reqID);
public slots:
    virtual void start() = 0;
signals:
    finished();
    newRequest(QByteArray reqData, int reqID);
    message(const QString&);
};

#endif // DEBORAHSOCKET_H
