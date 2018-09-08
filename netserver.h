#ifndef NETSERVER_H
#define NETSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include "chesspos.h"

class NetServer : public QObject
{
    Q_OBJECT
public:
    NetServer();
    static NetServer* getInstance() {
        if(NetServer::_instance == nullptr)
            NetServer::_instance = new NetServer;
        return NetServer::_instance;
    }
    void setHostIp(const QString& ipAddress);

    void connectServer();
    int readIntoBuffer(QByteArray& buffer, int len);
    int sendObjectMessage(const QJsonObject& object);
    int sendMessage(const QByteArray& message);
    void initServer();
    void sendMoveMessage(ChessPos oldPos, ChessPos newPos);
    void sendLossMessage(int loseType = 1);
    ~NetServer();

protected:
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QString _ipAddress;

    QJsonDocument readJsonDocument();
    int getIntFromBuffer(const QByteArray &buffer);

    void sendFeedBackPackage(int statusType);

    static NetServer *_instance;

protected slots:
    void getConnection();
    void recvMessage();
    void sendBeginInfo(bool beginWithSavedGame = false);

signals:
    void getFeedBack();
    void connectBuilt();
    void gameBegin(bool hostColor);


};

#endif // NETSERVER_H
