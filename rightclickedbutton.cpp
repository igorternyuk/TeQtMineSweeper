#include "rightclickedbutton.h"

RightClickedButton::RightClickedButton(int rowNumber, int columnNumber, QWidget* parent) :
    QPushButton(parent), rowNumber_(rowNumber), columnNumber_(columnNumber)
{}

void RightClickedButton::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
        emit onLeftButtonClicked(rowNumber_, columnNumber_);
    if(event->button() == Qt::RightButton)
        emit onRightButtonClicked(rowNumber_, columnNumber_);
}
