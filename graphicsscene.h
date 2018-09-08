#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QList>
#include <list>
#include "chesspiece.h"
#include "pieceitem.h"
#include "chesspos.h"

using std::list;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent = nullptr);
    void setSelectable();
    void movePieceItemTo(ChessPos oldPos, ChessPos newPos);

signals:

public slots:

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    ChessPos posFromPixel(const QPointF& pos);
    QPointF posFromXY(const ChessPos& pos);
    bool inChessboard(const QPointF& pos);
    PieceItem *getPieceItem(ChessPiece* chesspiece);

    PieceItem *getExistedItem(ChessPiece* chesspiece);

    list<PieceItem*> pieceItems;

    bool pieceSelected() const {
        return selectedItems().size() > 0;
    }

    void removePieceItem(PieceItem* item);

    PieceItem *selectedPieceItem;
    ChessPiece *willBeEatenPiece;
    QGraphicsPixmapItem *backgroundItem;
#ifdef MACOS

    const double squareLen = 109.6/2;
    const double leftPointx = 61/2 + 1, leftPointy = 59/2;
    const double leftLimit = 56.2/2, topLimit = 54.2/2, bottomLimit = 1150.2/2, rightLimit = 1042.6/2;
    const double pieceLen = 50;
#else
    const double squareLen = 109.6;
    const double leftPointx = 61 + 3, leftPointy = 59 + 3;
    const double leftLimit = 56.2, topLimit = 54.2, bottomLimit = 1150.2, rightLimit = 1042.6;
    const double pieceLen = 100;
#endif

protected slots:
    void modifySelection();
};

#endif // GRAPHICSSCENE_H
