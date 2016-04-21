#ifndef DEBORAHSERVER_H
#define DEBORAHSERVER_H

#include <QObject>
#include <QTcpServer>
#include "deborahsocket.h"
#include <QString>
#include <qThread>

class DeborahServer : public QTcpServer
{
    Q_OBJECT
    QString name;
public:
    enum SocketType {GATE, MASTER, LOCAL};
private:
    DeborahSocket* socket;
    QThread* thread;
    SocketType type;

    DeborahSocket *createSocket(SocketType sType, qintptr handle);

public:
    explicit DeborahServer(quint16 portNum, SocketType sType = GATE, QObject *parent = 0);
    ~DeborahServer();
    bool isClientConnected() const;
    quint16 getPort() const {return serverPort();}
    void connectLocalServer(DeborahServer &serv);
    SocketType getType() const {return type;}
    QString getName() const {return name;}
    void setName(const QString &value) {name = value;}
protected:
    void incomingConnection(qintptr handle);
private slots:
    void treatExternalRequest(QByteArray reqData, int reqID);
signals:
    startSocket();
    newRequest(QByteArray reqData, int reqID);
    message(const QString&);
};

#endif // DEBORAHSERVER_H
