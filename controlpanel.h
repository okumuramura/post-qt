#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include "customebutton.h"
#include <QWidget>

class ControlPanel : public QWidget
{
    Q_OBJECT
public:
    ControlPanel(QWidget*);

    enum Modes{
        STAY,
        PLAY,
        STOP_ONLY,
    };

    void setMode(ControlPanel::Modes);

private:
    CustomeButton* step_left;
    CustomeButton* stop;
    CustomeButton* play;
    CustomeButton* step_right;

public slots:
    void stop_pressed();
    void play_pressed();
private slots:

    void step_left_pressed();
    void step_right_pressed();
signals:
    void playing();
    void stoping();
    void steping_left();
    void steping_right();
    void pausing();
};

#endif // CONTROLPANEL_H
