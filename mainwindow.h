#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include "graphicsscene.h"
#include <QSoundEffect>
#include <QSound>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    static MainWindow* getInstance();
    ~MainWindow();
    GraphicsScene* getGraphicsScene() {
        return this->graphicsScene;
    }
    void changeNewScene();
    void displayTime(int remainTime);

public slots:
    void gameOver(bool whoseWinner, int winType);

private:
    Ui::MainWindow *ui;
    GraphicsScene *graphicsScene;
    static MainWindow* instance;
    QSound *moveSound;
    QSound *gameOverSound;

    void createMenu();

    bool checkJiangjun(int type);


private slots:
    void createNewGame();
    void joinGame();
    void readOldGame();
    void saveGame();
    void quitGame();
    void giveUpGame();
    void beginGame();
    void prepareSound();
    void playJiangjun(bool winner, int type);
};

#endif // MAINWINDOW_H
