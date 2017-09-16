#ifndef LCDCOUNTER_H
#define LCDCOUNTER_H

#include <QLCDNumber>

class LCDCounter : public QLCDNumber
{
    Q_OBJECT
public:
    LCDCounter(QWidget* parent = nullptr);
public slots:
    void reset();
    void update(int fagsCounter);
private:
    static const int NUM_DIGIT = 3;
    static const int DISPLAY_WIDTH = 70;
    static const int DISPLAY_HEIGHT = 40;
};

#endif // LCDCOUNTER_H
