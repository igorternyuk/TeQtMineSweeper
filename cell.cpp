#include "cell.h"

Cell::Cell(int row, int column) :
    row_(row), column_(column)
{
    state_ = State::CLOSED;
    isMined_ = false;
    counter_ = 0;
}

int Cell::getRow() const
{
    return row_;
}

int Cell::getColumn() const
{
    return column_;
}

void Cell::open()
{
    if(state_ != State::FLAGGED)
        state_ = State::OPENED;
}

void Cell::nextMark()
{
    if(state_ == State::OPENED) return;
    switch(state_)
    {
        case State::CLOSED :
            state_ = State::FLAGGED;
            break;

        case State::FLAGGED :
            state_ = State::QUESTIONED;
            break;

        case State::QUESTIONED :
            state_ = State::CLOSED;
            break;

        case State::OPENED :
        default:
            return;
    }
}

void Cell::setCounter(int counter)
{
    counter_ = counter;
}

int Cell::getCounter() const
{
    return counter_;
}

Cell::State Cell::getState() const
{
    return state_;
}

void Cell::setMined()
{
    isMined_ = true;
}

bool Cell::isMined() const
{
    return isMined_;
}


