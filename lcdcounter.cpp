#include "lcdcounter.h"

LCDCounter::LCDCounter(QWidget* parent) :
    QLCDNumber(NUM_DIGIT, parent)
{
    setFixedSize(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    setAutoFillBackground(true);
    setSegmentStyle(QLCDNumber::Flat);
    QPalette pal = palette();
    pal.setColor(QPalette::Normal, QPalette::WindowText, QColor(255,0,0));
    pal.setColor(QPalette::Normal, QPalette::Window, Qt::black);
    setPalette(pal);
}

void LCDCounter::reset()
{
    display(0);
}
void LCDCounter::update(int fagsCounter)
{
    display(fagsCounter);
}
