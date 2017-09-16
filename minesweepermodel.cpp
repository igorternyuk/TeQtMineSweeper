#include <time.h>
#include <math.h>

#include "minesweepermodel.h"

MineSweeperModel::MineSweeperModel(QObject* parent) :
QObject(parent)
{}
const std::string MineSweeperModel::WIN_MESSAGE = "Congratulations!!!You won! Play again?";
const std::string MineSweeperModel::LOST_MESSAGE = "Bang!!!You lost! Play again?";
const std::string MineSweeperModel::INFO_ABOUT_GAME = " The player is initially presented with a grid of "
                                           " undifferentiated squares. Some randomly selected squares,"
                                           " unknown to the player, are designated to contain mines. "
                                           " The size of the grid and the number of mines are set in advance"
                                           " by the user, either by entering the numbers or selecting from"
                                           " defined skill levels, depending on the implementation. The game"
                                           " is played by revealing squares of the grid by clicking. If a square"
                                           " containing a mine is revealed, the player loses the game. If no mine"
                                           " is revealed, a digit is instead displayed in the square, indicating "
                                           " how many adjacent squares contain mines; if no mines are adjacent, "
                                           "the square becomes blank, and all adjacent squares will be recursively"
                                           " revealed. The player uses this information. to deduce the contents of"
                                           " other squares, and may either safely reveal each square or mark the"
                                           " square as containing a mine.";
const std::string MineSweeperModel:: INFO_ABOUT_AUTHOR = "Ternyuk Igor Olexandrovytch, Ukraine";
const std::string MineSweeperModel:: HELP_INFO = "Left button click open cell. Right button click marks cell."
                                     "You can choose game level(there are three levels: beginner, intermediate and expert)"
                                     " using combobox. Choosing the custom level you can define field size and number of mines";
void MineSweeperModel::startGame(int rowCount, int columnCount, int mineCount)
{
    rowCount_ = isRowCountInRange(rowCount) ? rowCount : MIN_ROW_COUNT;
    columnCount_ = isColumnCountInRange(columnCount) ? columnCount : MIN_COLUMN_COUNT;
    bool isMineCountInRange = mineCount >= MIN_MINE_COUNT && mineCount <= MAX_MINE_COUNT;
    bool isMineCountLessThenField = mineCount < getRowCount() * getColumnCount();
    const double mineAreaRatio = MIN_MINE_COUNT / MIN_ROW_COUNT / MIN_COLUMN_COUNT;
    if(isMineCountInRange)
        mineCount_ = (isMineCountLessThenField) ? mineCount : floor(mineAreaRatio * getRowCount() * getColumnCount());
    else
        mineCount_ = MIN_MINE_COUNT;
    flagsCount_ = 0;
    isFirstStep_ = true;
    isGameOver_ = false;
    if(!cells_.empty()) cells_.clear();
    for(int i = 0; i < rowCount_; ++i)
    {
        std::vector<Cell*> tmp(columnCount_);
        for(int j = 0; j < columnCount_; ++j)
            tmp[j] = new Cell(i, j);
        cells_.push_back(tmp);
    }
}
bool MineSweeperModel::isInputCoordinatesInRange(int i, int j) const
{
    return i >= 0 && i < getRowCount() && j >= 0 && j < getColumnCount();
}

void MineSweeperModel::openCell(int i, int j)
{
    Cell* cell = getCell(i, j);
    if(!cell) return;
    cell->open();
    if(cell->isMined())
    {
        isGameOver_ = true;
        emit fieldChanged();
        emit defeat();
        return;
    }
    if(isFirstStep_)
    {
        isFirstStep_ = false;
        generateMineField();
    }
    cell->setCounter(countMinesAroundCell(i, j));
    emit fieldChanged();
    if(isWin()) emit victory();
    if(cell->getCounter() == 0)
    {
        std::vector<Cell*> cellNeighbours = getCellNeighbours(i, j);
        for(auto cell : cellNeighbours)
            if(cell->getState() == Cell::State::CLOSED)
                openCell(cell->getRow(), cell->getColumn());
    }
}

void MineSweeperModel::nextCellMark(int i, int j)
{
     Cell* cell = getCell(i,j);
     if(cell) cell->nextMark();
     if(cell->getState() == Cell::State::FLAGGED)
     {
         if(flagsCount_ <= mineCount_)
         {
             ++flagsCount_;
             emit flagsChanged(flagsCount_);
         }
         else
         {
            cell->nextMark();
            cell->nextMark();
         }
     }
     else if(cell->getState() != Cell::State::QUESTIONED)
     {
         --flagsCount_;
         emit flagsChanged(flagsCount_);
     }
     emit fieldChanged();
     if(isWin())
     {
        emit fieldChanged();
        emit victory();
     }
}

int MineSweeperModel::getRowCount() const
{
    return rowCount_;
}

int MineSweeperModel::getColumnCount() const
{
    return columnCount_;
}

int MineSweeperModel::getMineCount() const
{
    return mineCount_;
}

int MineSweeperModel::getFlagsCount() const
{
    return flagsCount_;
}

MineSweeperModel::CellState MineSweeperModel::getCellState(int i, int j) const
{
    MineSweeperModel::CellState cellState = CellState::CLOSED;
    if(isInputCoordinatesInRange(i, j))
    {
        switch(getCellByValue(i, j).getState())
        {
           case Cell::State::CLOSED :
               cellState = CellState::CLOSED; break;
           case Cell::State::FLAGGED :
               cellState = CellState::FLAGGED; break;
           case Cell::State::QUESTIONED :
               cellState = CellState::QUESTIONED; break;
           case Cell::State::OPENED :
               cellState = CellState::OPENED; break;
           default:
               cellState = CellState::CLOSED; break;
        }
    }
    return cellState;
}

bool MineSweeperModel::isCellMined(int i, int j) const
{
    return cells_[i][j]->isMined();
}

int MineSweeperModel::getMineCountAroundCell(int i, int j) const
{
    return countMinesAroundCell(i, j);
}

Cell* MineSweeperModel::getCell(int i, int j)
{
    if(isInputCoordinatesInRange(i, j))
        return cells_[i][j];
    else return nullptr;
}

Cell MineSweeperModel::getCellByValue(int i, int j) const
{
    Cell cell;
    if(isInputCoordinatesInRange(i, j))
        cell = *cells_[i][j];
    return cell;
}

bool MineSweeperModel::isRowCountInRange(int rowCount) const
{
    return rowCount >= MIN_ROW_COUNT && rowCount <= MAX_ROW_COUNT;
}

bool MineSweeperModel::isColumnCountInRange(int columnCount) const
{
    return columnCount >= MIN_COLUMN_COUNT && columnCount <= MAX_COLUMN_COUNT;
}

bool MineSweeperModel::isWin() const
{
    for(int i = 0; i < rowCount_; ++i)
        for(int j = 0; j < columnCount_; ++j)
        {
            if(!cells_[i][j]->isMined() && cells_[i][j]->getState() != Cell::State::OPENED)
                return false;
            if(cells_[i][j]->isMined() && (cells_[i][j]->getState() != Cell::State::OPENED &&
               cells_[i][j]->getState() != Cell::State::FLAGGED))
                return false;
        }
    return true;
}

bool MineSweeperModel::isGameOver() const
{
    return isGameOver_;
}

void MineSweeperModel::generateMineField()
{
    srand(time(NULL));
    for(int i = 0; i < mineCount_; ++i)
    {
        while(true)
        {
            int row = rand() % rowCount_;
            int column = rand() % columnCount_;
            Cell *cell = getCell(row, column);
            if(cell->getState() != Cell::State::OPENED &&
               !cell->isMined())
            {
                cell->setMined();
                break;
            }
        }
    }
}

int MineSweeperModel::countMinesAroundCell(int i, int j) const
{
    int mineAroundCount = 0;
    if(isInputCoordinatesInRange(i, j))
    {
        for(int di = -1; di <= 1; ++di)
            for(int dj = -1; dj <= 1; ++dj)
                 mineAroundCount += getCellByValue(i + di, j + dj).isMined() ? 1 : 0;
    }
    return mineAroundCount;
}

std::vector<Cell*> MineSweeperModel::getCellNeighbours(int i, int j)
{
    std::vector<Cell*> cellNeighbours;
    if(getCell(i, j))
    {
       for(int di = -1; di <= 1; ++di)
           for(int dj = -1; dj <= 1; ++dj)
           {
               Cell* cell = getCell(i + di, j + dj);
               if(cell)
               cellNeighbours.push_back(cell);
           }
    }
    return cellNeighbours;
}
