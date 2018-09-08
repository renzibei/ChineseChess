#include "pieceitem.h"

PieceItem::PieceItem(ChessPiece* piece, const QPixmap &pixmap):
     QGraphicsPixmapItem (pixmap),_chessPiece(piece)
{
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
}
