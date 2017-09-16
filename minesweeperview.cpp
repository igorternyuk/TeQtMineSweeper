//#include <QMenu>
#include <QDebug>
#include <QSize>
#include <QAction>
#include <QMessageBox>
#include "minesweeperview.h"
#include "minesweepercontroller.h"

MineSweeperView::MineSweeperView(MineSweeperModel *model, MineSweeperController *controller, QMainWindow* parent) :
    QMainWindow(parent), model_(model), controller_(controller)
{   
    this->setWindowTitle(QString::fromStdString(WINDOW_TITLE));
    QIcon windowIcon(QString::fromStdString(WINDOW_ICON_PATH));
    this->setWindowIcon(windowIcon);
    this->controller_->setView(this);
    timer = new LCDTimer();
    flagsCounter = new LCDCounter();
    QMenu *gameMenu = menuBar()->addMenu("&Game");
    QAction *newGameAction = gameMenu->addAction("&NewGame");
    connect(newGameAction, SIGNAL(triggered(bool)), this->controller_, SLOT(startNewGame()));
    QAction *quitAction = gameMenu->addAction("&Quit");
    connect(quitAction, SIGNAL(triggered(bool)), this->controller_, SLOT(closeApplication()));
    QMenu *helpMenu = menuBar()->addMenu("&Help");
    QAction *helpAction = helpMenu->addAction("&Help");
    connect(helpAction, SIGNAL(triggered(bool)), this->controller_, SLOT(showHelp()));
    QAction *aboutGameAction = helpMenu->addAction("&About game");
    connect(aboutGameAction, SIGNAL(triggered(bool)), this->controller_, SLOT(showInfoAboutGame()));
    QAction *aboutAuthorAction = helpMenu->addAction("&About author");
    connect(aboutAuthorAction, SIGNAL(triggered(bool)), this->controller_, SLOT(showInfoAboutAuthor()));
    flagsCounterLabel = new QLabel("Flags number ");
    timerLabel = new QLabel("Time elapsed ");
    levelLabel = new QLabel("Level");
    levelCombobox = new QComboBox;
    levelCombobox->addItem("Beginer");
    levelCombobox->addItem("Intermediate");
    levelCombobox->addItem("Expert");
    levelCombobox->addItem("Custom");
    levelCombobox->setFixedSize(LEVEL_COMBOBOX_WIDTH, LEVEL_COMBOBOX_HEIGHT);
    rowCountLabel = new QLabel("Number of rows   ");
    rowCountSpinBox = new QSpinBox();
    rowCountSpinBox->setMinimum(MineSweeperModel::MIN_ROW_COUNT);
    rowCountSpinBox->setMaximum(MineSweeperModel::MAX_ROW_COUNT);
    rowCountSpinBox->setValue(MineSweeperModel::MIN_ROW_COUNT);
    rowCountSpinBox->setFixedSize(SPINBOX_WIDTH, SPINBOX_HEIGHT);
    columnCountLabel = new QLabel("Number od columns");
    columnCountSpinBox = new QSpinBox();
    columnCountSpinBox->setMinimum(MineSweeperModel::MIN_COLUMN_COUNT);
    columnCountSpinBox->setMaximum(MineSweeperModel::MAX_COLUMN_COUNT);
    columnCountSpinBox->setValue(MineSweeperModel::MIN_COLUMN_COUNT);
    columnCountSpinBox->setFixedSize(SPINBOX_WIDTH, SPINBOX_HEIGHT);
    mineCountLabel = new QLabel("Number of mines");
    mineCountSpinBox = new QSpinBox();
    mineCountSpinBox->setMinimum(MineSweeperModel::MIN_MINE_COUNT);
    mineCountSpinBox->setMaximum(MineSweeperModel::MAX_MINE_COUNT);
    mineCountSpinBox->setValue(MineSweeperModel::MIN_MINE_COUNT);
    mineCountSpinBox->setFixedSize(SPINBOX_WIDTH, SPINBOX_HEIGHT);
    fieldGrid = new QGridLayout();
    fieldGrid->setContentsMargins(FIELD_GRID_LEFT_MARGIN, FIELD_GRID_TOP_MARGIN,
                                  FIELD_GRID_RIGHT_MARGIN, FIELD_GRID_BOTTOM_MARGIN);
    fieldGrid->setSpacing(1);
    newGameButton = new QPushButton("New game");
    exitButton = new QPushButton("Exit");
    QHBoxLayout* hBoxLayout1 = new QHBoxLayout();
    hBoxLayout1->addWidget(timerLabel);
    hBoxLayout1->addWidget(timer);
    QHBoxLayout* hBoxLayout2 = new QHBoxLayout();
    hBoxLayout2->addWidget(flagsCounterLabel);
    hBoxLayout2->addWidget(flagsCounter);
    QHBoxLayout* hBoxLayout3 = new QHBoxLayout();
    hBoxLayout3->addWidget(levelLabel);
    hBoxLayout3->addWidget(levelCombobox);
    QHBoxLayout* hBoxLayout4 = new QHBoxLayout();
    hBoxLayout4->addWidget(rowCountLabel);
    hBoxLayout4->addWidget(rowCountSpinBox);
    QHBoxLayout* hBoxLayout5 = new QHBoxLayout();
    hBoxLayout5->addWidget(columnCountLabel);
    hBoxLayout5->addWidget(columnCountSpinBox);
    QHBoxLayout* hBoxLayout6 = new QHBoxLayout();
    hBoxLayout6->addWidget(mineCountLabel);
    hBoxLayout6->addWidget(mineCountSpinBox);
    QHBoxLayout* hBoxLayout7 = new QHBoxLayout();
    hBoxLayout7->addWidget(newGameButton);
    hBoxLayout7->addWidget(exitButton);
    QVBoxLayout* vBoxLayout = new QVBoxLayout();
    vBoxLayout->addLayout(hBoxLayout1);
    vBoxLayout->addLayout(hBoxLayout2);
    vBoxLayout->addLayout(hBoxLayout3);
    vBoxLayout->addLayout(hBoxLayout4);
    vBoxLayout->addLayout(hBoxLayout5);
    vBoxLayout->addLayout(hBoxLayout6);
    vBoxLayout->addLayout(hBoxLayout7);
    vBoxLayout->insertStretch(VERTICAL_SPACER_INDEX);
    QHBoxLayout* hBoxLayout = new QHBoxLayout();
    hBoxLayout->addLayout(fieldGrid);
    hBoxLayout->addLayout(vBoxLayout);
    this->setFixedSize(MIN_FORM_WIDTH + model->getColumnCount() * CELL_WIDTH,
                       MIN_FORM_HEIGHT + model->getRowCount() * CELL_HEIGHT);
    QWidget* mainWidget = new QWidget();
    mainWidget->setLayout(hBoxLayout);
    this->setCentralWidget(mainWidget);
    connect(this, SIGNAL(gameStarted()), timer, SLOT(startTimer()));
    connect(model_, SIGNAL(victory()), timer, SLOT(stopTimer()));
    connect(model_, SIGNAL(defeat()), timer, SLOT(stopTimer()));
    connect(this, SIGNAL(gameStarted()), flagsCounter, SLOT(reset()));
    connect(newGameButton, SIGNAL(clicked(bool)), controller_, SLOT(startNewGame()));
    connect(exitButton, SIGNAL(clicked(bool)), controller_, SLOT(closeApplication()));
    connect(model_, SIGNAL(flagsChanged(int)), flagsCounter, SLOT(update(int)));
    connect(model_, SIGNAL(fieldChanged()), this, SLOT(update()));
    connect(model_, SIGNAL(victory()), this, SLOT(showWinMessage()));
    connect(model_, SIGNAL(defeat()), this, SLOT(showLostMessage()));
}

void MineSweeperView::createField()
{
    timer->reset();
    if(!cellTable.isEmpty())
    {
        for(int id = 1; id < model_->getRowCount() * model_->getColumnCount(); ++id)
             fieldGrid->removeWidget(cellTable.value(id));
        if(!cellTable.isEmpty())
        {
            int size = cellTable.size();
            for(int id = 1; id <= size; ++id)
                delete cellTable.take(id);
        }
     }
    for(int id = 1; id <= model_->getRowCount() * model_->getColumnCount(); ++id)
    {
        int row = floor((id - 1) / model_->getColumnCount());
        int col = (id - 1) % model_->getColumnCount();
        RightClickedButton* btn = new RightClickedButton(row, col);
        btn->setEnabled(true);
        QPalette palette = btn->palette();
        btn->setAutoFillBackground(true);
        btn->setStyleSheet("background-color: rgb(85, 0, 255); color: rgb(0, 0, 255);");
        btn->setText("");
        //palette.setColor(QPalette::Button, Qt::darkBlue);
        btn->setPalette(palette);
        btn->setFixedSize(CELL_WIDTH, CELL_HEIGHT);
        connect(btn, SIGNAL(onLeftButtonClicked(int, int)), controller_, SLOT(onLeftClick(int, int)));
        connect(btn, SIGNAL(onRightButtonClicked(int, int)), controller_, SLOT(onRightClick(int, int)));
        cellTable.insert(id, btn);        
        fieldGrid->addWidget(btn, row, col);
    }
    this->setFixedSize(MIN_FORM_WIDTH + model_->getColumnCount() * CELL_WIDTH,
                       MIN_FORM_HEIGHT + model_->getRowCount() * CELL_HEIGHT);
    emit gameStarted();
}

void MineSweeperView::getGameSettings(int &rowCount, int &columnCount, int &mineCount) const
{
    switch(levelCombobox->currentIndex())
    {
        case 1 :
            rowCount =    model_->INTERMEDIATE_ROW_COUNT;
            columnCount = model_->INTERMEDIATE_COLUMN_COUNT;
            mineCount =   model_->INTERMEDIATE_MINE_COUNT;
            break;
        case 2 :
            rowCount =    model_->EXPERT_ROW_COUNT;
            columnCount = model_->EXPERT_COLUMN_COUNT;
            mineCount =   model_->EXPERT_MINE_COUNT;
            break;
        case 3 :
            rowCount =    rowCountSpinBox->value();
            columnCount = columnCountSpinBox->value();
            mineCount =   mineCountSpinBox->value();
            break;
        case 0 :
        default :
            rowCount =    model_->BEGINER_ROW_COUNT;
            columnCount = model_->BEGINER_COLUMN_COUNT;
            mineCount =   model_->BEGINER_MINE_COUNT;
            break;
    }
}

void MineSweeperView::update()
{
    for(int i = 0; i < model_->getRowCount(); ++i)
        for(int j = 0; j < model_->getColumnCount(); ++j)
        {
           MineSweeperModel::CellState cellState = model_->getCellState(i, j);
           QPushButton* btn = cellTable[i * model_->getColumnCount() + j + 1];
           btn->setAutoFillBackground(true);
           if(cellState == MineSweeperModel::CellState::CLOSED)
           {
               if(model_->isGameOver() && model_->isCellMined(i, j))
               {
                   btn->setStyleSheet("background-color: rgb(255, 255, 0); color: rgb(0, 0, 0);font-size: 14pt;font-weight:bold");
                   btn->setText("✸");
               }
               else
               {
                   btn->setStyleSheet("background-color: rgb(85, 0, 255); color: rgb(0, 0, 255);");
                   btn->setText("");
               }
           }
           else if(cellState == MineSweeperModel::CellState::OPENED)
           {
               if(!model_->isCellMined(i, j))
               {
                   int mineCountAroundCell = model_->getMineCountAroundCell(i, j);
                   if(mineCountAroundCell > 0)
                   {
                       switch(mineCountAroundCell)
                       {
                           case 1 :
                               btn->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 125, 0);font-size: 14pt;font-weight:bold");
                               break;
                           case 2 :
                               btn->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(255, 0, 0);font-size: 14pt;font-weight:bold");
                               break;
                           case 3 :
                               btn->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 0, 255);font-size: 14pt;font-weight:bold");
                               break;
                           case 4 :
                               btn->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(85, 0, 127);font-size: 14pt;font-weight:bold");
                               break;
                           case 5 :
                               btn->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(255, 40, 120);font-size: 14pt;font-weight:bold");
                               break;
                           case 6 :
                               btn->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 85, 0);font-size: 14pt;font-weight:bold");
                               break;
                           case 7 :
                               btn->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(255, 0, 255);font-size: 14pt;font-weight:bold");
                               break;
                           case 8 :
                               btn->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(255, 0, 0);font-size: 14pt;font-weight:bold");
                               break;
                       }
                       btn->setText(QString::number(mineCountAroundCell));
                   }
                   else
                   {
                       btn->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(255, 255, 255);");
                       btn->setText("");
                   }
               }
               else
               {
                   btn->setStyleSheet("background-color: rgb(255, 255, 0); color: rgb(255, 83, 15);font-size: 22pt;font-weight:bold");
                   btn->setText("✸");
               }
           }
           else if(cellState == MineSweeperModel::CellState::FLAGGED)
           {
               if((model_->isWin() || model_->isGameOver()) && model_->isCellMined(i, j))
               {
                   btn->setStyleSheet("background-color: rgb(255, 255, 0); color: rgb(0, 170, 0);font-size: 20pt;font-weight:bold");
                   btn->setText("✔");
               }
               else
               {
                   btn->setStyleSheet("background-color: rgb(0, 0, 255); color: rgb(255, 0, 0);font-size: 16pt;font-weight:bold");
                   btn->setText("F");
               }
           }
           else if(cellState == MineSweeperModel::CellState::QUESTIONED)
           {
               btn->setStyleSheet("background-color: rgb(0, 0, 255); color: rgb(255, 255, 0);font-size: 14pt;font-weight:bold");
               btn->setText("?");
           }
           btn->update();
        }
}

void MineSweeperView::blockField()
{
    for(int id = 1; id < cellTable.size(); ++id)
        cellTable[id]->setEnabled(false);
}

void MineSweeperView::showWinMessage()
{
    blockField();
    QMessageBox::StandardButton answer =
    QMessageBox::question(this, QString::fromStdString(MineSweeperModel::WIN_MESSAGE),
    " Elapsed time : " + timer->time()->toString("hh:mm:ss.zzz"),
    QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(answer == QMessageBox::Yes)
    {
        controller_->startNewGame();
    }
    if(answer == QMessageBox::No)
    {
        controller_->closeApplication();
    }
}

void MineSweeperView::showLostMessage()
{
    blockField();
    QMessageBox::StandardButton answer =
    QMessageBox::question(this, QString::fromStdString(MineSweeperModel::LOST_MESSAGE),
    " Elapsed time : " + timer->time()->toString("hh:mm:ss.zzz"),
    QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(answer == QMessageBox::Yes)
    {
        controller_->startNewGame();
    }
    if(answer == QMessageBox::No)
    {
        controller_->closeApplication();
    }
}

void MineSweeperView::showInfoAboutGame()
{
    QString text = QString::fromStdString(MineSweeperModel::INFO_ABOUT_GAME);
    QMessageBox::about(this, "Information about game", text);
}

void MineSweeperView::showHelp()
{
    QString text = QString::fromStdString(MineSweeperModel::HELP_INFO);
    QMessageBox::about(this, "Help information", text);
}

void MineSweeperView::showInfoAboutAuthor()
{
    QString text = QString::fromStdString(MineSweeperModel::INFO_ABOUT_AUTHOR);
    QMessageBox::about(this, "Information about author", text);
}
