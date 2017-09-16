#ifndef MINESWEEPERMODEL_H
#define MINESWEEPERMODEL_H
#include <QObject>
#include <vector>
#include <string.h>
#include "cell.h"

typedef std::vector<std::vector<Cell*>> Matrix2D;

class MineSweeperModel : public QObject
{
    Q_OBJECT

public:
    static const int MIN_ROW_COUNT = 9;
    static const int MAX_ROW_COUNT = 25;
    static const int MIN_COLUMN_COUNT = 9;
    static const int MAX_COLUMN_COUNT = 25;
    static const int MIN_MINE_COUNT = 10;
    static const int MAX_MINE_COUNT = 600;
    static const int NEIGHBOURS_NUMBER = 8;
    static const int BEGINER_ROW_COUNT = 9;
    static const int BEGINER_COLUMN_COUNT = 9;
    static const int BEGINER_MINE_COUNT = 10;
    static const int INTERMEDIATE_ROW_COUNT = 16;
    static const int INTERMEDIATE_COLUMN_COUNT = 16;
    static const int INTERMEDIATE_MINE_COUNT = 40;
    static const int EXPERT_ROW_COUNT = 25;
    static const int EXPERT_COLUMN_COUNT = 25;
    static const int EXPERT_MINE_COUNT = 100;
    static const std::string WIN_MESSAGE;
    static const std::string LOST_MESSAGE;
    static const std::string INFO_ABOUT_GAME;
    static const std::string INFO_ABOUT_AUTHOR;
    static const std::string HELP_INFO;
    enum class CellState
    {
        CLOSED,
        OPENED,
        FLAGGED,
        QUESTIONED
    };
    MineSweeperModel(QObject* parent = nullptr);
    int getRowCount() const;
    int getColumnCount() const;
    int getMineCount() const;
    int getFlagsCount() const;
    CellState getCellState(int i, int j) const;
    bool isCellMined(int i, int j) const;
    bool isGameOver() const;
    bool isWin() const;
    int getMineCountAroundCell(int i, int j) const;
public slots:
    void startGame(int rowCount = MIN_ROW_COUNT, int columnCount = MIN_COLUMN_COUNT,
                   int mineCount = MIN_MINE_COUNT);
    void nextCellMark(int i, int j);
    void openCell(int i, int j);
signals:
    void fieldChanged();
    void flagsChanged(int flagsCount);
    void victory();
    void defeat();
private:
    int rowCount_;
    int columnCount_;
    int mineCount_;
    int flagsCount_;
    Matrix2D cells_;
    bool isFirstStep_;
    bool isGameOver_;    
    void generateMineField();
    std::vector<Cell*> getCellNeighbours(int i, int j);
    Cell *getCell(int i, int j);
    bool isInputCoordinatesInRange(int i, int j) const;
    Cell getCellByValue(int i, int j) const;
    bool isRowCountInRange(int rowCount) const;
    bool isColumnCountInRange(int columnCount) const;    
    int countMinesAroundCell(int i, int j) const;
};

#endif // MINESWEEPERMODEL_H
