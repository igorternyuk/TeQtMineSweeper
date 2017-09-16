#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include <QDesktopWidget>

#include "minesweepercontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion");
    MineSweeperModel* model = new MineSweeperModel();
    MineSweeperController* controller = new MineSweeperController(model);
    MineSweeperView* mineSweeperView = new MineSweeperView(model, controller);
    controller->startNewGame();
    mineSweeperView->show();
    return a.exec();
}
