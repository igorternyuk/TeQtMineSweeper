#ifndef MINESWEEPERVIEW_H
#define MINESWEEPERVIEW_H

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QVector>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include "rightclickedbutton.h"
#include "minesweepermodel.h"
#include "lcdtimer.h"
#include "lcdcounter.h"

class MineSweeperController;
class MineSweeperView : public QMainWindow
{
    Q_OBJECT
public:
    static const int MIN_FORM_HEIGHT = 65;
    static const int MIN_FORM_WIDTH = 320;
    static const int CELL_WIDTH = 25;
    static const int CELL_HEIGHT = 25;
    static const int SPINBOX_WIDTH = 50;
    static const int SPINBOX_HEIGHT = 23;
    static const int LEVEL_COMBOBOX_WIDTH = 110;
    static const int LEVEL_COMBOBOX_HEIGHT = 23;
    static const int FIELD_GRID_LEFT_MARGIN = 0;
    static const int FIELD_GRID_RIGHT_MARGIN = 0;
    static const int FIELD_GRID_TOP_MARGIN = 0;
    static const int FIELD_GRID_BOTTOM_MARGIN = 0;
    static const int VERTICAL_SPACER_INDEX = -1;
    const std::string WINDOW_TITLE = "***MineSweeper***";
    const std::string WINDOW_ICON_PATH = ":/images/mine_exploded_flat_button.png";
    MineSweeperView(MineSweeperModel* model, MineSweeperController* controller, QMainWindow *parent = nullptr);
    void getGameSettings(int &rowCount, int &columnCount, int &mineCount) const;
signals:
    void gameStarted();
    void leftButtonClicked(int rowOfClickedButton, int columnOfClickedButton);
    void rightButoonClicked(int rowOfClickedButton, int columnOfClickedButton);
public slots:
    void createField();
    void update();
    void blockField();
    void showWinMessage();
    void showLostMessage();
    void showInfoAboutGame();
    void showHelp();
    void showInfoAboutAuthor();
private:
    MineSweeperModel* model_;
    MineSweeperController* controller_;
    QLabel* timerLabel;
    LCDTimer* timer;
    QLabel* flagsCounterLabel;
    LCDCounter* flagsCounter;
    QLabel* levelLabel;
    QComboBox* levelCombobox;
    QLabel* rowCountLabel;    
    QSpinBox* rowCountSpinBox;
    QLabel* columnCountLabel;
    QSpinBox* columnCountSpinBox;
    QLabel* mineCountLabel;
    QSpinBox* mineCountSpinBox;
    QGridLayout* fieldGrid;
    QMap<int, RightClickedButton*> cellTable;
    QPushButton* newGameButton;
    QPushButton* exitButton;
};

#endif // MINESWEEPERVIEW_H
