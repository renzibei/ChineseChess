#include "chessboard.h"
#include <cstring>
#include <QTextStream>
#include <gamecenter.h>

inline int abs(int x) { return x > 0 ? x : -x ;}

ChessBoard::ChessBoard()
{
    memset(this->pieces, 0, sizeof(this->pieces));
    for(int i = 0; i < 2; ++i) {
        remainingPieces.push_back(new ChessPiece(9*i, 0, Chariot, 1-i));
        //pieces[9*i][0] = new ChessPiece(9*i, 0, Chariot, 1-i);
        remainingPieces.push_back(new ChessPiece(9*i, 8, Chariot, 1-i));
        remainingPieces.push_back(new ChessPiece(9*i, 1, Horse, 1-i));
        remainingPieces.push_back(new ChessPiece(9*i, 7, Horse, 1-i));
        remainingPieces.push_back(new ChessPiece(9*i, 2, Elephant, 1-i));
        remainingPieces.push_back(new ChessPiece(9*i, 6, Elephant, 1-i));
        remainingPieces.push_back(new ChessPiece(9*i, 3, Advisor, 1-i));
        remainingPieces.push_back(new ChessPiece(9*i, 5, Advisor, 1-i));
        remainingPieces.push_back(new ChessPiece(9*i, 4, General, 1-i));
        remainingPieces.push_back(new ChessPiece(abs(9*i-2), 1, Cannon, 1-i));
        remainingPieces.push_back(new ChessPiece(abs(9*i-2), 7, Cannon, 1-i));
        remainingPieces.push_back(new ChessPiece(abs(9*i-3), 0, Soldier, 1-i));
        remainingPieces.push_back(new ChessPiece(abs(9*i-3), 8, Soldier, 1-i));
        remainingPieces.push_back(new ChessPiece(abs(9*i-3), 2, Soldier, 1-i));
        remainingPieces.push_back(new ChessPiece(abs(9*i-3), 6, Soldier, 1-i));
        remainingPieces.push_back(new ChessPiece(abs(9*i-3), 4, Soldier, 1-i));
    }
    for(unsigned long i = 0; i < this->remainingPieces.size(); ++i)
        this->addPiece(this->remainingPieces[i]);


}

void ChessBoard::clearChessBoard()
{
    for(int i = 0; i < 10; ++i)
        for(int j = 0; j < 9; ++j)
            if(this->pieces[i][j] != nullptr) {
                delete this->pieces[i][j];
                this->pieces[i][j] = nullptr;
            }
    this->remainingPieces.clear();
}

void ChessBoard::addPiece(ChessPiece *piece)
{
    this->pieces[piece->x()][piece->y()] = piece;
}

void ChessBoard::movePieceTo(ChessPiece* piece, int x, int y)
{
    if(piece->canMoveTo(x, y)) {
        this->pieces[x][y] = piece;
        this->pieces[piece->x()][piece->y()] = nullptr;
        piece->moveTo(x, y);
    }
    emit GameCenter::getInstance()->pieceMoved();
}

vector<ChessPiece* >& ChessBoard::getPieces()
{
    return this->remainingPieces;
}

void ChessBoard::movePieceTo(ChessPiece* piece, const ChessPos &pos)
{
    this->movePieceTo(piece, pos.x, pos.y);
}

void ChessBoard::removePieceAt(const ChessPos &pos)
{
    if(this->pieces[pos.x][pos.y] != nullptr) {
           for(vector<ChessPiece*>::iterator it = remainingPieces.begin(); it != remainingPieces.end(); ++it)
               if(*it == this->pieces[pos.x][pos.y]) {
                   this->remainingPieces.erase(it);
                   delete this->pieces[pos.x][pos.y];
                   this->pieces[pos.x][pos.y] = nullptr;
                   break;
               }
    }
}

QString ChessBoard::getSaveContent()
{
    QString tempStr;
    vector<ChessPiece*> tempVector;
    QTextStream out(&tempStr, QIODevice::WriteOnly | QIODevice::Text);
    for(int k = 0; k < 2; ++k) {
        if(GameCenter::getInstance()->gamerColor() == 0) {
            if(k == 0)
                out << "black" << '\n';
            else out << "red" << '\n';
        }
        else {
            if(k == 0)
                out << "red" << '\n';
            else out << "black" << '\n';
        }
        for(int i = 0; i < 7; ++i) {
            for(vector<ChessPiece*>::iterator it = remainingPieces.begin(); it != remainingPieces.end(); it++)
                if((*it)->type() == i + 2 && (*it)->belong() == k)
                    tempVector.push_back(*it);
            out << tempVector.size() << " ";
            for(vector<ChessPiece*>::iterator it = tempVector.begin(); it != tempVector.end(); ++it)
                out << "<" <<  (*it)->y() << "," << -(*it)->x() + 9 << "> " ;
            out << '\n';
            tempVector.clear();
        }
    }
    return tempStr;
}

void ChessBoard::loadSavedFile(QTextStream &in)
{

    QString colorStr;
    int pieceNum = 0, x = 0, y = 0;
    QString xStr, yStr;
    QString piecePosStr;
    for(int k = 0; k < 2; ++k) {
        in >> colorStr;
        if(k == GameCenter::getInstance()->localGamer()) {
            if(colorStr == "red")
                GameCenter::getInstance()->simplyChangeColor(1);
            else if(colorStr == "black")
                GameCenter::getInstance()->simplyChangeColor(0);
            else throw "Cant not load file, can't get color";
        }
        for(int i = 0; i < 7; ++i) {
            in >> pieceNum;
            for(int j = 0; j < pieceNum; ++j) {
                in >> piecePosStr;
                xStr = piecePosStr[1]; yStr = piecePosStr[3];
                x = xStr.toInt(); y = yStr.toInt();
                remainingPieces.push_back(new ChessPiece(-y+9, x, PieceType(i+2), k) );
            }
        }

    }
    for(unsigned long i = 0; i < this->remainingPieces.size(); ++i)
        this->addPiece(this->remainingPieces[i]);

}

void ChessBoard::removePiece(ChessPiece* piece)
{
    for(vector<ChessPiece*>::iterator it = remainingPieces.begin(); it != remainingPieces.end(); ++it)
        if(*it == piece) {
            this->remainingPieces.erase(it);
            break;
        }

    for(int i = 0; i < 10; ++i)
        for(int j = 0; j < 9; ++j)
            if(this->pieces[i][j] == piece)
                this->pieces[i][j] = nullptr;
    delete piece;
}
