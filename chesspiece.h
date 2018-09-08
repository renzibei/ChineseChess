#ifndef CHESSPIECE_H
#define CHESSPIECE_H


#include "chesspos.h"
#include <QString>

enum PieceType {General = 2, Advisor = 3, Elephant = 4, Horse = 5, Chariot = 6, Cannon = 7, Soldier = 8};



class ChessPiece
{
public:
    ChessPiece(const ChessPos &chessPos, PieceType type, bool belong);
    ChessPiece(int x, int y, PieceType type, bool belong);
    int canMoveTo(int x, int y) const;
    int canMoveTo(ChessPos chesspos) const;
    ChessPos pos() const;
    static const QString pieceNames[7];
    /**
     * @brief 代表属于玩家0还是玩家1
     * @return
     */
    bool belong() const;
    int x() const;
    int y() const;
    PieceType type() const;
    virtual ~ChessPiece();

    /**
     * @brief moveTo 如果可以移动且能吃子，则返回吃的子的类型枚举，能移动但不能吃子则返回1，不能移动返回0
     * @param x
     * @param y
     * @return
     */
    int moveTo(int x, int y);
    int moveTo(ChessPos pos);

protected:
    ChessPos _pos;
    PieceType _type;
    bool _belong;


};





#endif // CHESSPIECE_H
