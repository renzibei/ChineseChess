#include "netserver.h"
#include "gamecenter.h"
#include <QDataStream>
#include <QJsonValue>
#include <QJsonObject>
#include "mainwindow.h"
#include <QTextStream>

NetServer* NetServer::_instance = nullptr;

NetServer::NetServer()
{
    this->_ipAddress = "127.0.0.1";
    NetServer::_instance = this;
}

void NetServer::connectServer()
{
    this->tcpSocket = new QTcpSocket;
    this->tcpSocket->connectToHost(QHostAddress(this->_ipAddress), 8731);
    connect(this->tcpSocket, SIGNAL(readyRead()), this, SLOT(recvMessage()));

}

void NetServer::initServer()
{
    this->tcpServer = new QTcpServer;
    this->tcpServer->listen(QHostAddress::Any, 8731);
    connect(this->tcpServer, SIGNAL(newConnection()), this, SLOT(getConnection()));
}

void NetServer::getConnection()
{
    this->tcpSocket = this->tcpServer->nextPendingConnection();
    this->sendBeginInfo(GameCenter::getInstance()->isSavedGame());
    emit this->connectBuilt();
    //connect(this->tcpSocket, SIGNAL(connected()), this, SLOT(sendBeginInfo()));
    connect(this->tcpSocket, SIGNAL(readyRead()), this, SLOT(recvMessage()));

}

void NetServer::sendBeginInfo(bool beginWithSavedGame)
{

    QJsonObject beginObject;
    beginObject.insert("packageType", 3);
    beginObject.insert("hostColor", GameCenter::getInstance()->gamerColor());
    if(beginWithSavedGame) {
        beginObject.insert("gameMode", 1);
        beginObject.insert("map", GameCenter::getChessBoard()->getSaveContent());
    }
    else beginObject.insert("gameMode", 0);
    this->sendObjectMessage(beginObject);
}

ChessPos getPosFromJsonObject(const QJsonObject& object)
{
    QJsonValue xValue = object.value("x");
    QJsonValue yValue = object.value("y");
    return ChessPos(xValue.toInt(), yValue.toInt());
}

void NetServer::sendMoveMessage(ChessPos oldPos, ChessPos newPos)
{
    QJsonObject moveMessageObject;
    moveMessageObject.insert("packageType", 1);
    QJsonObject fromObject, toObject;
    fromObject.insert("x", oldPos.x); fromObject.insert("y", oldPos.y);
    toObject.insert("x", newPos.x); toObject.insert("y",newPos.y);
    moveMessageObject.insert("from", fromObject);
    moveMessageObject.insert("to", toObject);
    this->sendObjectMessage(moveMessageObject);
}

void NetServer::sendFeedBackPackage(int statusType)
{
    QJsonObject returnObject;
    returnObject.insert("packageType", 0);
    returnObject.insert("result", statusType);
    this->sendObjectMessage(returnObject);
}

void NetServer::sendLossMessage(int loseType)
{
    QJsonObject lossMessageObject;
    lossMessageObject.insert("packageType", 2);
    lossMessageObject.insert("loseType", loseType);
    this->sendObjectMessage(lossMessageObject);
}



void NetServer::recvMessage()
{
    QByteArray getbuffer;
    QJsonDocument getDocument = this->readJsonDocument();
    QJsonObject rootObject = getDocument.object();
#ifdef DEBUG_MODE
    qDebug() << "recive " << rootObject;
#endif
    QJsonValue packageType = rootObject.value("packageType");
    int typeValue = packageType.toInt();
    try {
        if(typeValue == 0) {
            QJsonValue resultValue = rootObject.value("result");
            int resultInt = resultValue.toInt();
            if(resultInt == 0)
                emit this->getFeedBack();
        }
        else if(typeValue == 1) {
            QJsonValue fromValue = rootObject.value("from"); QJsonValue toValue = rootObject.value("to");
            ChessPos fromPos = getPosFromJsonObject(fromValue.toObject()), toPos = getPosFromJsonObject(toValue.toObject());
            MainWindow::getInstance()->getGraphicsScene()->movePieceItemTo(fromPos, toPos);
            this->sendFeedBackPackage(0);
        }
        else if(typeValue == 2) {
            QJsonValue winTypeValue = rootObject.value("loseType");
            int winType = winTypeValue.toInt();
            emit GameCenter::getInstance()->gameOverSignal(GameCenter::getInstance()->localGamer(), winType);
        }
        else if(typeValue == 3) {
            QJsonValue gameModeValue = rootObject.value("gameMode");
            int gameMode = gameModeValue.toInt();
            if(gameMode == 0) {
                QJsonValue hostColorValue = rootObject.value("hostColor");
                bool hostColor = hostColorValue.toBool();
                GameCenter::getInstance()->setGamerColor(!hostColor);
                GameCenter::getInstance()->startGame();
            }
            else {
                QJsonValue mapValue = rootObject.value("map");
                QString map = mapValue.toString();
                QTextStream in(&map, QIODevice::ReadOnly | QIODevice::Text);
                GameCenter::getChessBoard()->clearChessBoard();
                GameCenter::getChessBoard()->loadSavedFile(in);
                MainWindow::getInstance()->changeNewScene();
            }
            GameCenter::getInstance()->startGame();
        }
    } catch (const char* msg) {
        qDebug() << msg;
    }


}



int NetServer::readIntoBuffer(QByteArray& buffer, int len)
{
    int leftBytes = len, tempLen;
    QByteArray tempBuffer, result;
    char *tempStr;
    int i = 0;
    while(leftBytes > 0) {
        tempStr = new char[leftBytes+1];
        memset(tempStr, 0, leftBytes + 1);
        tempLen = this->tcpSocket->read(tempStr, leftBytes);
        leftBytes -= tempLen;
        result.append(tempStr);
        /*
        if(leftBytes > 0) {
            if(WebIO::getSocket()->waitForReadyRead(30000) == false)
                qDebug() << "接受超时";
        }
        */
        i++;
        if(i > 1000000) {
            qDebug() << this->tcpSocket->errorString();
            return -1;

        }
    }
    buffer = std::move(result);
#ifdef DEBUG_MODE
    qDebug() << buffer;
#endif
    return buffer.length();
}

int NetServer::getIntFromBuffer(const QByteArray &buffer)
{
    QDataStream tempStream(buffer);
    tempStream.setByteOrder(QDataStream::BigEndian);
    int x;
    tempStream >> x;
    return x;
}

QJsonDocument NetServer::readJsonDocument()
{
    QByteArray byteBuffer = this->tcpSocket->read(4);
    int len = this->getIntFromBuffer(byteBuffer);
    int status = this->readIntoBuffer(byteBuffer, len);
    QJsonDocument tempJson;
    if(status > 0)
        tempJson = QJsonDocument::fromJson(byteBuffer);
    return tempJson;
}

NetServer::~NetServer()
{

}

void NetServer::setHostIp(const QString& ipAddress)
{
    this->_ipAddress = ipAddress;
}

int NetServer::sendMessage(const QByteArray& message)
{
    QDataStream dataStream(this->tcpSocket);
    dataStream.setByteOrder(QDataStream::BigEndian);
    dataStream << message.length();
    int leftBytes = message.length(), tempLen;
    while(leftBytes > 0) {
        tempLen = this->tcpSocket->write(message);
        if(tempLen == -1) {
            qDebug() << "errorCode " << this->tcpSocket->error();
            qDebug() << this->tcpSocket->errorString();
            if(this->tcpSocket->error() == QTcpSocket::UnknownSocketError) {
                //socket->connectToHost(QHostAddress("35.194.106.246"), 8333);
            }
        }

        leftBytes -= tempLen;
    }
    return 0;
}

int NetServer::sendObjectMessage(const QJsonObject &object)
{
#ifdef DEBUG_MODE
    qDebug() << "state" << this->tcpSocket->state();
    qDebug() << "send message" << object;
#endif
    QJsonDocument jsonDocument(object);
    QByteArray message = jsonDocument.toJson(QJsonDocument::Compact);
    return this->sendMessage(message);
}
