#include "chesspiece.h"
#include "gamecenter.h"

const QString ChessPiece::pieceNames[7] = {"general", "advisor", "elephant", "horse", "chariot", "cannon", "soldier"};

ChessPiece::ChessPiece(const ChessPos &chessPos, PieceType type, bool belong): _pos(chessPos), _type(type), _belong(belong)
{

}

ChessPiece::ChessPiece(int x, int y, PieceType type, bool belong): _pos(ChessPos(x, y)), _type(type), _belong(belong)
{

}

PieceType ChessPiece::type() const {
    return this->_type;
}


ChessPiece::~ChessPiece()
{

}

ChessPos ChessPiece::pos() const
{
    return this->_pos;
}

int ChessPiece::x() const
{
    return this->_pos.x;
}

int ChessPiece::y() const
{
    return this->_pos.y;
}

bool ChessPiece::belong() const
{
    return this->_belong;
}

int ChessPiece::moveTo(ChessPos pos)
{
    return this->moveTo(pos.x, pos.y);
}

int ChessPiece::moveTo(int x, int y)
{
    //if(this->canMoveTo(x, y)) {
        this->_pos.x = x; this->_pos.y = y;

    //}
}

int getDis(ChessPos pos1, ChessPos pos2)
{
    return abs(pos1.x - pos2.x) + abs(pos1.y - pos2.y);
}

bool inHomeArea(int x, int y, bool belong)
{
    if(belong) {
        return (0 <= x && 2 >= x && 3 <= y && 5 >= y);
    }
    else {
        return (7 <= x && 9 >= x && 3 <=y && 5 >= y);
    }
}

bool inHomeHalf(int x, int y, int belong)
{
    if(belong) {
        return (0 <= x && 4 >= x && 0 <= y && 8 >= y);
    }
    else {
        return (5 <= x && 9 >= x && 0 <=y && 8 >= y);
    }
}

bool inChessBoard(int x, int y)
{
    return (0 <= x && 9 >= x && 0 <= y && 8 >= y);
}

bool belongToSamePlayer(ChessPos pos1, ChessPos pos2)
{
    return GameCenter::getChessBoard()->at(pos1)->belong() ==  GameCenter::getChessBoard()->at(pos2)->belong();
}

int ChessPiece::canMoveTo(int x, int y) const
{
    if(inChessBoard(x, y)) {
        if(GameCenter::getChessBoard()->at(x, y) != nullptr)
            if(belongToSamePlayer(this->pos(), ChessPos(x, y)))
                return 0;
        switch (this->_type) {
            case General:
                if(getDis(this->_pos, ChessPos(x, y)) == 1) {
                    return inHomeArea(x, y, this->_belong);
                }
                 return 0;
            case Advisor:
                if(abs(x - this->x()) == 1 && abs(y - this->y()) == 1)
                    return inHomeArea(x, y, this->_belong);
                return 0;
            case Elephant:
                if(abs(x - this->x()) == 2 && abs(y - this->y()) == 2)
                    if(inHomeHalf(x, y, this->_belong))
                        if(GameCenter::getChessBoard()->at((x + this->x()) / 2, (y + this->y()) / 2) == nullptr)
                            return 1;
                    return 0;
            case Horse: {
                int xdis = abs(x - this->x()), ydis = abs(y - this->y());
                if(xdis == 2 && ydis == 1) {
                    if(GameCenter::getChessBoard()->at((x + this->x()) / 2, this->y()) == nullptr) {
                        return 1;
                    }
                }
                else if(xdis == 1 && ydis == 2) {
                    if(GameCenter::getChessBoard()->at(this->x(), (y + this->y()) / 2) == nullptr)
                        return 1;
                }
                return 0;
             }
            case Chariot: {
                int xdis = abs(x - this->x()), ydis = abs(y - this->y());
                if(xdis == 0) {
                    int mulRatio = -1;
                    if(y > this->y())
                        mulRatio = 1;
                    for(int i = 1; i < ydis; ++i)
                        if(GameCenter::getChessBoard()->at(x, this->y() + i * mulRatio) != nullptr)
                            return 0;
                    return 1;

                }
                else if(ydis == 0) {
                    int mulRatio = -1;
                    if(x > this->x())
                        mulRatio = 1;
                    for(int i = 1; i < xdis; ++i)
                        if(GameCenter::getChessBoard()->at(this->x() + i * mulRatio, y) != nullptr)
                            return 0;
                    return 1;
                }
                return 0;
             }

            case Soldier:
                if(getDis(_pos, ChessPos(x, y)) == 1) {
                    if(inHomeHalf(this->x(), this->y(), _belong)) {
                        if(this->y() == y) {
                            if(_belong && x > this->x())
                                return 1;
                            else if((!_belong) && x < this->x())
                                return 1;
                         }
                     }
                     else {
                        if(_belong && x >= this->x())
                            return 1;
                        else if(!_belong && x <= this->x())
                            return 1;
                     }
                  }
                    return 0;
            case Cannon: {
                int xdis = abs(x - this->x()), ydis = abs(y - this->y());
                bool beatSomeone = (GameCenter::getChessBoard()->at(x, y) != nullptr);
                if(xdis == 0) {
                    int numOfPieceInLine = 0;
                    int mulRatio = (y - this->y()) / ydis;
                    for(int i = 1; i < ydis; ++i) {
                        if(GameCenter::getChessBoard()->at(x, this->y() + i * mulRatio) != nullptr)
                            numOfPieceInLine ++;
                    }
                    if((numOfPieceInLine == 1 && beatSomeone) || (numOfPieceInLine == 0 && !beatSomeone) )
                        return 1;
                }
                else if(ydis == 0) {
                    int numOfPieceInLine = 0;
                    int mulRatio = (x - this->x()) / xdis;
                    for(int i = 1; i < xdis; ++i)
                        if(GameCenter::getChessBoard()->at(this->x() + i * mulRatio, y) != nullptr)
                            numOfPieceInLine++;
                    if((numOfPieceInLine == 1 && beatSomeone) || (numOfPieceInLine == 0 && !beatSomeone))
                        return 1;
                }
                return 0;
            }

        }
    }
    else return 0;
}

int ChessPiece::canMoveTo(ChessPos chesspos) const
{
    return this->canMoveTo(chesspos.x, chesspos.y);
}
