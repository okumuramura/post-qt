#ifndef CUSTOMEBUTTON_H
#define CUSTOMEBUTTON_H

#include <QWidget>
#include <QPushButton>

class CustomeButton : public QPushButton
{
    Q_OBJECT
public:
    enum Types {
        left,
        right,
        stop,
        play,
        step_right,
        step_left,
        pause
    };
    CustomeButton(QWidget*, Types);
    void setType(Types);
    Types getType();
private:
    Types type;
    void paintEvent(QPaintEvent*);
    QColor active_color;
    QColor not_active_color;
};

#endif // CUSTOMEBUTTON_H
