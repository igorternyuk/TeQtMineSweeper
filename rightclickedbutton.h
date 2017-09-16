#ifndef RIGHTCLICKEDBUTTON_H
#define RIGHTCLICKEDBUTTON_H

#include <QPushButton>
#include <QMouseEvent>

class RightClickedButton : public QPushButton
{
    Q_OBJECT
public:
    explicit RightClickedButton(int rowNumber, int columnNumber, QWidget* parent = nullptr);
signals:
    void onLeftButtonClicked(int clickedButtonRow, int clickedButtonColumn);
    void onRightButtonClicked(int clickedButtonRow, int clickedButtonColumn);
private slots:
    void mousePressEvent(QMouseEvent* event);
private:
    int rowNumber_;
    int columnNumber_;

};

#endif // RIGHTCLICKEDBUTTON_H
