#include "gamecenter.h"
#include <cstdlib>
#include <time.h>
#include "mainwindow.h"

GameCenter *GameCenter::_instance = nullptr;

GameCenter *GameCenter::getInstance()
{
    if(GameCenter::_instance == nullptr)
        GameCenter::_instance = new GameCenter;
    return GameCenter::_instance;
}

GameCenter::GameCenter(bool localGamer, int colorSet, bool color, QObject *parent):
    QObject(parent), chessBoard(nullptr), totalRound(0), _localGamer(localGamer)
{
    srand((unsigned)time(0));
    if(colorSet == 0) {
        this->_gamerColor = rand()%2;
    }
    else _gamerColor = color;
    if(_gamerColor)
        this->_whoseRound = this->_localGamer;
    else this->_whoseRound = !_localGamer;
    this->chessBoard = new ChessBoard;
    this->netServer = NetServer::getInstance();
    this->gameClock = new QTimer;
    GameCenter::_instance = this;
}

void GameCenter::setGamerColor(bool color)
{
    this->_gamerColor = color;
    if(_gamerColor)
        this->_whoseRound = this->_localGamer;
    else this->_whoseRound = !_localGamer;
    this->newGame(1);
}

void GameCenter::setLocalGamer(bool gamer)
{
    this->_localGamer = gamer;
}

void GameCenter::newGame(bool colorSet)
{
    if(chessBoard != nullptr)
        delete chessBoard;
    this->chessBoard = new ChessBoard;

    totalRound = 0;
    if(!colorSet)
        this->_gamerColor = rand()%2;
    if(_gamerColor)
        this->_whoseRound = this->_localGamer;
    else this->_whoseRound = !_localGamer;
    MainWindow::getInstance()->changeNewScene();
}

void GameCenter::startGame()
{
#ifndef DEBUG_MODE
    this->gameClock->start(1000);
#endif
    remainTime = 30;
    connect(gameClock, SIGNAL(timeout()), this, SLOT(timeDecline()));
    connect(this, SIGNAL(gameOverSignal(bool, int)), gameClock, SLOT(stop()));
    MainWindow::getInstance()->displayTime(remainTime);
}

void GameCenter::timeDecline()
{
    this->remainTime--;
    MainWindow::getInstance()->displayTime(remainTime);
    if(remainTime == 0 && _whoseRound == _localGamer) {
        GameCenter::getInstance()->netServer->sendLossMessage(3);
        this->gameClock->stop();
        emit this->gameOverSignal(!_whoseRound, 3);
    }
}


void GameCenter::eatPiece(ChessPiece* eaterPiece, ChessPiece * eatenPiece)
{
    if(eaterPiece->canMoveTo(eatenPiece->pos())) {
        ChessPos tempPos = eatenPiece->pos(); int eatenType = eatenPiece->type();

        GameCenter::getChessBoard()->movePieceTo(eaterPiece, tempPos);
        this->chessBoard->removePiece(eatenPiece);
        //eaterPiece->moveTo(tempPos);
        if(eatenType == General) {
            emit gameOverSignal(eaterPiece->belong(), 1);
        }
    }
}

void GameCenter::roundEnd()
{
    this->totalRound++;
    this->_whoseRound = !_whoseRound;
    this->remainTime = 30;
    MainWindow::getInstance()->displayTime(remainTime);
}

GameCenter::~GameCenter()
{

}
