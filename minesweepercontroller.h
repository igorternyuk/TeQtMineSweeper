#ifndef MINESWEEPERCONTROLLER_H
#define MINESWEEPERCONTROLLER_H

#include <QMainWindow>

#include "minesweepermodel.h"
#include "minesweeperview.h"

class MineSweeperController : public QObject
{
    Q_OBJECT

public:
    MineSweeperController(MineSweeperModel* model, QObject *parent = nullptr);
    void setView(MineSweeperView* view);
public slots:
    void startNewGame();
    void onLeftClick(int row, int column);
    void onRightClick(int row, int column);
    void showInfoAboutGame();
    void showHelp();
    void showInfoAboutAuthor();
    void closeApplication();
private:
    MineSweeperModel* model_;
    MineSweeperView* view_;
    bool isViewSetted_;
};

#endif // MINESWEEPERCONTROLLER_H
