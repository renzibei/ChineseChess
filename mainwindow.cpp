#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLayout>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QDebug>
#include <QMessageBox>

#include "waitdialog.h"
#include "gamecenter.h"
#include "connectdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

   // double pixelRatio = this->window()->devicePixelRatio();

    this->createMenu();

    //qDebug() << "ratio" << this->window()->devicePixelRatio();
    //QWidget *centerWidget = new QWidget;
    this->graphicsScene = new GraphicsScene;
    //QBrush backgroundBrush(QImage(":/images/chessboard.png"));

    //this->graphicsScene->setBackgroundBrush(backgroundBrush);
   // QHBoxLayout *HLayout = new QHBoxLayout;
    //ui->centralWidget->setLayout(HLayout);
   // centerWidget->setLayout(HLayout);
    //QGraphicsView *graphicsView = new QGraphicsView;
    ui->graphicsView->setMouseTracking(true);
    //graphicsView->setMouseTracking(true);
   // HLayout->addWidget(ui->graphicsView);
    //ui->centralWidget->layout()->addWidget(graphicsView);
    //this->setCentralWidget(centerWidget);
    ui->graphicsView->setScene(this->graphicsScene);
    connect(ui->giveUpButton, SIGNAL(clicked()), this, SLOT(giveUpGame()));
    connect(GameCenter::getInstance(), SIGNAL(gameOverSignal(bool, int)), this, SLOT(gameOver(bool, int)));
    //this->resize(1400 / pixelRatio, 1400/pixelRatio);
    //this->setMouseTracking(true);
    //this->repaint();
    MainWindow::instance = this;
}

void MainWindow::changeNewScene()
{
    if(this->graphicsScene != nullptr)
        delete this->graphicsScene;
    this->graphicsScene = new GraphicsScene;
    ui->graphicsView->setScene(this->graphicsScene);
}

MainWindow* MainWindow::instance = nullptr;

MainWindow* MainWindow::getInstance()
{
    if(MainWindow::instance == nullptr)
        MainWindow::instance = new MainWindow();
    return MainWindow::instance;
}


void MainWindow::createMenu()
{
    QMenu *gameMenu = new QMenu(tr("游戏"));
    QAction *newGameAction = new QAction(tr("新建"));
    QAction *joinGameAction = new QAction(tr("加入"));
    QAction *saveAction = new QAction(tr("保存"));
    QAction *openAction = new QAction(tr("读取"));
    QAction *quitAction = new QAction(tr("退出"));
    newGameAction->setShortcut(QKeySequence::New);
    saveAction->setShortcut(QKeySequence::Save);
    openAction->setShortcut(QKeySequence::Open);
    quitAction->setShortcut(QKeySequence::Quit);
    gameMenu->addAction(newGameAction);
    gameMenu->addAction(joinGameAction);
    gameMenu->addAction(saveAction);
    gameMenu->addAction(openAction);
    gameMenu->addAction(quitAction);
    connect(newGameAction, SIGNAL(triggered()), this, SLOT(createNewGame()));
    connect(joinGameAction, SIGNAL(triggered()), this, SLOT(joinGame()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveGame()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(readOldGame()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

    menuBar()->addMenu(gameMenu);
    gameMenu->show();

}

void MainWindow::displayTime(int remainTime)
{
    ui->lcdNumber->display(QString::number(remainTime));
    this->repaint();
    this->update();
}

void MainWindow::createNewGame()
{
    GameCenter::getInstance()->netServer->initServer();
    GameCenter::getInstance()->newGame();
    waitDialog *waitWindow = new waitDialog;
    connect(GameCenter::getInstance()->netServer, SIGNAL(connectBuilt()), waitWindow, SLOT(close()));
    connect(GameCenter::getInstance()->netServer, SIGNAL(connectBuilt()), this, SLOT(beginGame()));
    waitWindow->show();
}

void MainWindow::beginGame()
{
    GameCenter::getInstance()->startGame();
}

void MainWindow::joinGame()
{
    GameCenter::getInstance()->setLocalGamer(1);
    ConnectDialog* connectDialog = new ConnectDialog;
    connectDialog->show();
}

void MainWindow::saveGame()
{

}

void MainWindow::readOldGame()
{

}

void MainWindow::gameOver(bool whoseWinner, int winType)
{
    if(whoseWinner == GameCenter::getInstance()->localGamer()) {
        if(winType == 1)
            QMessageBox::information(this, tr("胜利"), tr("将军，you WIN"));
        else if(winType == 2)
            QMessageBox::information(this, tr("胜利"), tr("对方认输, you WIN"));
        else if(winType == 3)
            QMessageBox::information(this, tr("胜利"), tr("对方操作超时, you WIN"));
    }
    else {
        if(winType == 1)
            QMessageBox::information(this, tr("输了"), tr("将军，you Lose"));
        else if(winType == 2)
            QMessageBox::information(this, tr("输了"), tr("认输, you Lose"));
        else if(winType == 3)
            QMessageBox::information(this, tr("输了"), tr("操作超时, you Lose"));
    }
}

void MainWindow::giveUpGame()
{
    GameCenter::getInstance()->netServer->sendLossMessage(2);
    emit GameCenter::getInstance()->gameOverSignal(!GameCenter::getInstance()->localGamer(), 2);
}

void MainWindow::quitGame()
{

    this->close();
}

MainWindow::~MainWindow()
{
    delete ui;
}
