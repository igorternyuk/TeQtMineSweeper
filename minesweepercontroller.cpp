#include "minesweepercontroller.h"

MineSweeperController::MineSweeperController(MineSweeperModel *model, QObject *parent) :
    QObject(parent), model_(model), isViewSetted_(false)
{}

void MineSweeperController::setView(MineSweeperView *view)
{
    view_ = view;
    isViewSetted_ = true;
}

void MineSweeperController::startNewGame()
{
    int rowCount, columnCount, mineCount;
    if(isViewSetted_)
    {
        view_->getGameSettings(rowCount, columnCount, mineCount);
        model_->startGame(rowCount, columnCount, mineCount);
        view_->createField();
    }
}

void MineSweeperController::onLeftClick(int row, int column)
{
    model_->openCell(row, column);
    if(isViewSetted_) view_->update();
}

void MineSweeperController::onRightClick(int row, int column)
{
    model_->nextCellMark(row, column);
    if(isViewSetted_) view_->update();
}

void MineSweeperController::showInfoAboutGame()
{
    if(isViewSetted_) view_->showInfoAboutGame();
}

void MineSweeperController::showHelp()
{
    if(isViewSetted_) view_->showHelp();
}

void MineSweeperController::showInfoAboutAuthor()
{
    if(isViewSetted_) view_->showInfoAboutAuthor();
}

void MineSweeperController::closeApplication()
{
   if(isViewSetted_)  view_->close();
}
