#ifndef GAMECENTER_H
#define GAMECENTER_H

#include <QObject>
#include <QTimer>
#include "chessboard.h"
#include "chesspiece.h"
#include "netserver.h"

//#define DEBUG_MODE

#ifdef __APPLE__

#define MACOS

#endif

class GameCenter : public QObject
{
    Q_OBJECT
public:
    GameCenter(bool localGamer = 0, int colorSet = 1, bool color = 0, QObject *parent = nullptr);
    NetServer *netServer;
    static GameCenter* getInstance();
    QTimer *gameClock;

    /**
     * @brief gamerColor
     * @return  0代表自己是黑色，1代表自己是红色
     */
    bool gamerColor() const {
        return this->_gamerColor;
    }

    /**
     * @brief whoseRound 下一回合是谁
     * @return 0代表主机，1代表对面
     */
    bool whoseRound() const {
        return this->_whoseRound;
    }

    void newGame(bool colorSet = 0);
    void startGame();


    void loadOldGame(const QString& filePath);



    /**
     * @brief roundEnd   调用来结束当前会和，开启下一回合
     */
    void roundEnd();
    void eatPiece(ChessPiece* eaterPiece, ChessPiece * eatenPiece);

    static ChessBoard* getChessBoard() {
        if(GameCenter::getInstance()->chessBoard == nullptr)
            GameCenter::getInstance()->chessBoard = new ChessBoard;
        return GameCenter::getInstance()->chessBoard;
    }

    static vector<ChessPiece*>& getPieces() {
        return GameCenter::getChessBoard()->getPieces();
    }

    ~GameCenter();

    void setLocalGamer(bool gamer);

    void setGamerColor(bool color);

    void simplyChangeColor(bool color) {
        this->_gamerColor = color;
        if(_gamerColor)
            this->_whoseRound = this->_localGamer;
        else this->_whoseRound = !_localGamer;
    }

    bool localGamer() const {
        return this->_localGamer;
    }

    bool isSavedGame() const {
        return this->_savedGame;
    }

    bool setSavedGame(bool isSavedGame = true) {
        this->_savedGame = isSavedGame;
    }

protected:
    ChessBoard *chessBoard;
    bool _whoseRound;
    bool _gamerColor;
    int totalRound;
    bool _localGamer;
    int remainTime;
    bool _savedGame;
    static GameCenter *_instance;

protected slots:
    void timeDecline();

signals:
    void gameOverSignal(bool whoseWinner, int winType);
    void pieceMoved();
};



#endif // GAMECENTER_H
