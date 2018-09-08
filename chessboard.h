#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <vector>
#include "chesspiece.h"
#include "chesspos.h"
#include <QFile>
#include <QTextStream>

using std::vector;

class ChessBoard
{
public:
    ChessBoard();
    ChessPiece *at(int x, int y) const {
        return this->pieces[x][y];
    }
    ChessPiece *at(const ChessPos &pos) const {
        return this->pieces[pos.x][pos.y];
    }

    void loadSavedFile(QTextStream &in);

    vector<ChessPiece*> &getPieces();
    void addPiece(ChessPiece *piece);
    void removePieceAt(const ChessPos &pos);
    void removePiece(ChessPiece* piece);
    void movePieceTo(ChessPiece* piece, int x, int y);
    void movePieceTo(ChessPiece* piece, const ChessPos &pos);
    void clearChessBoard();

    QString getSaveContent();
protected:
    ChessPiece *pieces[10][9];

    vector<ChessPiece*> remainingPieces;
};

#endif // CHESSBOARD_H
