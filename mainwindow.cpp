#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLayout>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>

#include "waitdialog.h"
#include "gamecenter.h"
#include "connectdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->createMenu();
    this->prepareSound();
    //this->moveSound = new QSoundEffect(this);
    //moveSound->setSource(QUrl(":/media/give.wav"));
    qDebug() << this->devicePixelRatio();

    //moveSound->setVolume(0.5);
#ifdef MACOS
    this->resize(800, 700);
#else
    this->resize(1600, 1400);
#endif

    this->graphicsScene = new GraphicsScene;
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->setScene(this->graphicsScene);
    connect(ui->giveUpButton, SIGNAL(clicked()), this, SLOT(giveUpGame()));
    connect(GameCenter::getInstance(), SIGNAL(gameOverSignal(bool, int)), this, SLOT(gameOver(bool, int)));
    MainWindow::instance = this;
}

bool MainWindow::checkJiangjun(int type)
{
    if(type == 1)
        return 1;
    return 0;
}

void MainWindow::prepareSound()
{
    this->moveSound = new QSound(":/media/give.wav");
    connect(GameCenter::getInstance(), SIGNAL(pieceMoved()), moveSound, SLOT(play()));
    this->gameOverSound = new QSound(":/media/check.wav");
    connect(GameCenter::getInstance(), SIGNAL(gameOverSignal(bool, int)), this, SLOT(playJiangjun(bool, int)));
}

void MainWindow::playJiangjun(bool winner, int type)
{
    if(checkJiangjun(type))
        this->gameOverSound->play();
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
    //this->update();
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
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               QDir::homePath(),
                               tr("存档文件 (*.txt)"));
    if(fileName != "") {
        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            QMessageBox::critical(this, tr("File permission denied"), tr("无写入权限"));
        else file.write(GameCenter::getChessBoard()->getSaveContent().toLocal8Bit().data());
    }
}

void MainWindow::readOldGame()
{

    QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath(), tr("存档文件 (*.txt)"));
    if(filePath != "")
        GameCenter::getInstance()->loadOldGame(filePath);
    GameCenter::getInstance()->setSavedGame(true);
    waitDialog *waitWindow = new waitDialog;
    GameCenter::getInstance()->netServer->initServer();
    connect(GameCenter::getInstance()->netServer, SIGNAL(connectBuilt()), waitWindow, SLOT(close()));
    connect(GameCenter::getInstance()->netServer, SIGNAL(connectBuilt()), this, SLOT(beginGame()));
    waitWindow->show();
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
        else if(winType == 4)
            QMessageBox::information(this, tr("胜利"), tr("对方对将, you WIN"));
    }
    else {
        if(winType == 1)
            QMessageBox::information(this, tr("输了"), tr("将军，you Lose"));
        else if(winType == 2)
            QMessageBox::information(this, tr("输了"), tr("认输, you Lose"));
        else if(winType == 3)
            QMessageBox::information(this, tr("输了"), tr("操作超时, you Lose"));
        else if(winType == 4)
            QMessageBox::information(this, tr("输了"), tr("对将， you Lose"));
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
