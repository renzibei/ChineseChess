#include "chessboard.h"
#include <cstring>

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

void ChessBoard::removePiece(ChessPiece* piece)
{
    for(vector<ChessPiece*>::iterator it = remainingPieces.begin(); it != remainingPieces.end(); ++it)
        if(*it == piece) {
            this->remainingPieces.erase(it);
            for(int i = 0; i < 10; ++i)
                for(int j = 0; j < 9; ++j)
                    if(this->pieces[i][j] == piece)
                        this->pieces[i][j] = nullptr;
            delete piece;
            break;
        }
}
