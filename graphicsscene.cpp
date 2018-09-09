#include "graphicsscene.h"
#include <cmath>
#include "gamecenter.h"
#include "chessboard.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>


GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent), selectedPieceItem(nullptr), willBeEatenPiece(nullptr), backgroundItem(nullptr)
{
    QPixmap backgroundPixmap;
    //backgroundPixmap.setDevicePixelRatio(2);
    //qDebug() << backgroundPixmap.devicePixelRatio();
    backgroundPixmap.load(":/images/chessboard.png");

    //qDebug() << "size " << backgroundPixmap.size();
#ifdef LOW_DPI
    backgroundPixmap.setDevicePixelRatio(2);
#else
    backgroundPixmap.setDevicePixelRatio(1);
#endif
    //qDebug() << "size " << backgroundPixmap.size() << "\n" <<  backgroundPixmap.devicePixelRatio();
    backgroundItem = new QGraphicsPixmapItem(backgroundPixmap);
    this->addItem(backgroundItem);
    for(auto it = GameCenter::getPieces().begin(); it != GameCenter::getPieces().end(); it++) {
        ChessPiece* tempPiece = *it;
        this->addItem(getPieceItem(tempPiece));
    }
    connect(this, SIGNAL(selectionChanged()), this, SLOT(modifySelection()));
    this->setSelectable();
}


ChessPos GraphicsScene::posFromPixel(const QPointF& pos)
{
    double xDis = (pos.y() - topLimit) / squareLen, yDis = (pos.x() - leftLimit) / squareLen ;
    int posX = abs(9 * GameCenter::getInstance()->localGamer() - static_cast<int>(floor(xDis)) );
    int posY = abs(8 * GameCenter::getInstance()->localGamer() - static_cast<int>(floor(yDis) ) );
    return ChessPos(posX, posY);
}

bool GraphicsScene::inChessboard(const QPointF& pos)
{
    return (pos.x() >= leftLimit  && pos.x() <= rightLimit && pos.y() >= topLimit && pos.y() <= bottomLimit);
}

QPointF GraphicsScene::posFromXY(const ChessPos& pos)
{
    return QPointF(abs(8 * GameCenter::getInstance()->localGamer() - pos.y) * squareLen + leftPointx, abs(9 * GameCenter::getInstance()->localGamer() - pos.x) * squareLen + leftPointy);
}

PieceItem* GraphicsScene::getPieceItem(ChessPiece* chesspiece)
{
    QString filePath = ":/images/" + ChessPiece::pieceNames[chesspiece->type() - 2];
    bool gamerColor = GameCenter::getInstance()->gamerColor();

    if(chesspiece->belong() != GameCenter::getInstance()->localGamer())
        gamerColor = !gamerColor;

    QString gamerName = QString::number(gamerColor);
    filePath += "_" + gamerName;


    QPixmap piecePixmap(filePath);
#ifdef LOW_DPI
    piecePixmap.setDevicePixelRatio(1);
#else
    //piecePixmap.setDevicePixelRatio(0.5);
    piecePixmap = piecePixmap.scaled(piecePixmap.width() * 2, piecePixmap.height() * 2);
#endif
    PieceItem *pieceItem = new PieceItem(chesspiece, piecePixmap);
    //pieceItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
    pieceItem->setPos(this->posFromXY(chesspiece->pos()));
    this->pieceItems.push_back(pieceItem);
    //this->addItem(pieceItem);
    return pieceItem;
}

PieceItem* GraphicsScene::getExistedItem(ChessPiece* chesspiece)
{
    for(list<PieceItem*>::iterator it = this->pieceItems.begin(); it!=pieceItems.end(); ++it)
        if((*it)->chessPiece() == chesspiece)
            return (*it);
    return nullptr;
}

void GraphicsScene::modifySelection()
{
    QList<QGraphicsItem*> selectItems = this->selectedItems();
    if(selectItems.size() > 0) {
        PieceItem* selectedItem = static_cast<PieceItem*>(selectItems.first());
        this->selectedPieceItem = selectedItem;
        for(list<PieceItem*>::iterator it = this->pieceItems.begin(); it!=pieceItems.end(); ++it) {
            if(*it != selectedItem) {
                (*it)->setFlag(QGraphicsItem::ItemIsSelectable, false);
            }
        }
    }
    else {
        this->setSelectable();
        /*
        for(list<PieceItem*>::iterator it = this->pieceItems.begin(); it!=pieceItems.end(); ++it)
            (*it)->setFlag(QGraphicsItem::ItemIsSelectable, true);
            */
        this->selectedPieceItem = nullptr;
    }
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //qDebug() << "pos " << mouseEvent->scenePos().x() << " " << mouseEvent->scenePos().y();
//    qDebug() << "is in chessboard " << this->inChessboard(mouseEvent->scenePos());

    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GraphicsScene::removePieceItem(PieceItem* item)
{
    this->removeItem(item);
    for(list<PieceItem*>::iterator it = pieceItems.begin(); it != pieceItems.end(); ++it)
        if(*it == item) {
            pieceItems.erase(it);
            break;
        }
}

void GraphicsScene::movePieceItemTo(ChessPos oldPos, ChessPos newPos)
{
    ChessPiece *selectedPiece = GameCenter::getChessBoard()->at(oldPos);
    PieceItem* eaterItem = this->getExistedItem(selectedPiece);
    ChessPiece *chesspiece = GameCenter::getChessBoard()->at(newPos);
    if(chesspiece != nullptr) {
        PieceItem* pieceItem = this->getExistedItem(chesspiece);
        GameCenter::getInstance()->roundEnd();
        this->removePieceItem(pieceItem);
        GameCenter::getInstance()->eatPiece(selectedPiece, chesspiece);
        eaterItem->setPos(this->posFromXY(selectedPiece->pos()));
    }
    else {
        GameCenter::getInstance()->roundEnd();
        GameCenter::getChessBoard()->movePieceTo(selectedPiece, newPos);
        eaterItem->setPos(this->posFromXY(newPos));
    }
    this->setSelectable();
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //this->setSelectable();
    bool movedFlag = 0;
    if(event->button() == Qt::LeftButton) {
        if(pieceSelected()) {
            QGraphicsItem *tempItem = this->itemAt(event->scenePos(), QTransform());
            PieceItem* eaterItem = this->selectedPieceItem;
            ChessPiece* selectedPiece = eaterItem->chessPiece();
            if(tempItem != this->backgroundItem && tempItem != nullptr) {
                PieceItem* pieceItem = static_cast<PieceItem* >(tempItem);
                //ChessPos mousePos = this->posFromPixel(event->scenePos());
                //ChessPiece* chesspiece = GameCenter::getChessBoard()->at(mousePos);
                ChessPiece* chesspiece = pieceItem->chessPiece();
                if(chesspiece != nullptr) {
                    if(chesspiece != eaterItem->chessPiece()) {
                        if(selectedPiece->canMoveTo(chesspiece->pos())) {
                            //selectedPiece->moveTo(chesspiece->pos());
                            GameCenter::getInstance()->roundEnd();
                            this->removePieceItem(pieceItem);
                            GameCenter::getInstance()->netServer->sendMoveMessage(selectedPiece->pos(), chesspiece->pos());
                            GameCenter::getInstance()->eatPiece(selectedPiece, chesspiece);

                            eaterItem->setPos(this->posFromXY(selectedPiece->pos()));
                            eaterItem->setSelected(false);

                            movedFlag = 1;
                            //this->willBeEatenPiece = chesspiece;
                        }
                    }
                }
            }
            else if(this->inChessboard(event->scenePos()) ) {
                ChessPos emptyPos = this->posFromPixel(event->scenePos());
                if(selectedPiece->canMoveTo(emptyPos)) {
                    GameCenter::getInstance()->roundEnd();
                    //selectedPiece->moveTo(emptyPos);
                    GameCenter::getInstance()->netServer->sendMoveMessage(selectedPiece->pos(), emptyPos);
                    GameCenter::getChessBoard()->movePieceTo(selectedPiece, emptyPos);
                    eaterItem->setPos(this->posFromXY(emptyPos));
                    eaterItem->setSelected(false);

                    movedFlag = 1;
                }
            }
        }

    }
    if(!movedFlag)
        QGraphicsScene::mousePressEvent(event);

}

void GraphicsScene::setSelectable()
{
    if(!GameCenter::getInstance()->isGameOver() && GameCenter::getInstance()->whoseRound() == GameCenter::getInstance()->localGamer()) {
        for(list<PieceItem*>::iterator it = this->pieceItems.begin(); it != pieceItems.end(); it++)
            (*it)->setFlag(QGraphicsItem::ItemIsSelectable, (*it)->chessPiece()->belong() == GameCenter::getInstance()->whoseRound());
    }
    else for(list<PieceItem*>::iterator it = this->pieceItems.begin(); it != pieceItems.end(); it++)
        (*it)->setFlag(QGraphicsItem::ItemIsSelectable, false);
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    /*
    if(mouseEvent->button() == Qt::LeftButton) {

        if(this->willBeEatenPiece != nullptr) {
            //ChessPos mousePos = this->posFromPixel(mouseEvent->scenePos());
            //ChessPiece* chesspiece = GameCenter::getChessBoard()->at(mousePos);
            QGraphicsItem *tempItem = this->itemAt(mouseEvent->scenePos(), QTransform());
            if(tempItem != this->backgroundItem) {
                PieceItem* pieceItem = static_cast<PieceItem* >(tempItem);
                if(pieceItem != this->selectedPieceItem) {
                ChessPiece* chesspiece = pieceItem->chessPiece();
                    if(chesspiece != nullptr && chesspiece == this->willBeEatenPiece) {
                            ChessPiece* selectedPiece = selectedPieceItem->chessPiece();
                            selectedPiece->moveTo(chesspiece->pos());
                            this->removePieceItem(pieceItem);

                    }

                }
            }
            this->willBeEatenPiece = nullptr;
        }


    }
    */
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
