#ifndef CELL_H
#define CELL_H

class Cell
{
public:
    Cell(int row = 0, int column = 0);
    enum class State
    {
        CLOSED,
        OPENED,
        FLAGGED,
        QUESTIONED
    };
    int getRow() const;
    int getColumn() const;
    void open();
    void nextMark();
    void setCounter(int counter);
    int getCounter() const;
    State getState() const;
    void setMined();
    bool isMined() const;

private:
    int row_;
    int column_;
    bool isMined_;
    int counter_;
    State state_;

};

#endif // CELL_H
