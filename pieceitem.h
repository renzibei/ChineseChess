#ifndef PIECEITEM_H
#define PIECEITEM_H

#include <QGraphicsPixmapItem>
#include "chesspiece.h"

class PieceItem : public QGraphicsPixmapItem
{
public:
    PieceItem(ChessPiece* piece, const QPixmap &pixmap);
    ChessPiece* chessPiece() const {
        return this->_chessPiece;
    }

protected:
    ChessPiece* _chessPiece;
};

#endif // PIECEITEM_H
