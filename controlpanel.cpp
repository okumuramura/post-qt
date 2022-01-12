#include "controlpanel.h"
#include <QBoxLayout>

ControlPanel::ControlPanel(QWidget* p) : QWidget(p)
{
    this->step_left = new CustomeButton(this, CustomeButton::Types::step_left);
    this->step_left->setEnabled(false);
    this->step_left->setMinimumSize(50, 50);
    this->step_left->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(this->step_left, &CustomeButton::pressed, this, &ControlPanel::step_left_pressed);

    this->stop = new CustomeButton(this, CustomeButton::Types::stop);
    this->stop->setEnabled(false);
    this->stop->setMinimumSize(50, 50);
    this->stop->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(this->stop, &CustomeButton::pressed, this, &ControlPanel::stop_pressed);

    this->play = new CustomeButton(this, CustomeButton::Types::play);
    this->play->setMinimumSize(50, 50);
    this->play->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(this->play, &CustomeButton::pressed, this, &ControlPanel::play_pressed);

    this->step_right = new CustomeButton(this, CustomeButton::Types::step_right);
    this->step_right->setMinimumSize(50, 50);
    this->step_right->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(this->step_right, &CustomeButton::pressed, this, &ControlPanel::step_right_pressed);

    QHBoxLayout* layout = new QHBoxLayout();
    layout->setMargin(10);
    layout->addStretch(10);
    layout->addWidget(this->step_left);
    layout->addWidget(this->stop);
    layout->addWidget(this->play);
    layout->addWidget(this->step_right);
    layout->addStretch(10);
    this->setLayout(layout);
}

void ControlPanel::play_pressed(){
    if (!this->stop->isEnabled())
        this->stop->setEnabled(true);
    if (this->play->getType() == CustomeButton::play){
        this->step_right->setEnabled(false);
        this->play->setType(CustomeButton::Types::pause);
        emit this->playing();
    }
    else {
        this->step_right->setEnabled(true);
        this->play->setType(CustomeButton::Types::play);
        emit this->pausing();
    }
}


void ControlPanel::stop_pressed(){
    if (!this->step_right->isEnabled())
        this->step_right->setEnabled(true);
    this->stop->setEnabled(false);
    this->play->setType(CustomeButton::Types::play);
    emit this->stoping();
}

void ControlPanel::step_left_pressed(){
    emit this->steping_left();
}

void ControlPanel::step_right_pressed(){
    this->stop->setEnabled(true);
    emit this->steping_right();
}

void ControlPanel::setMode(ControlPanel::Modes mode){
    if (mode == ControlPanel::Modes::PLAY){
        this->stop->setEnabled(true);
        this->play->setEnabled(true);
        this->play->setType(CustomeButton::Types::pause);
        this->step_right->setEnabled(true);
    }
    else if (mode == ControlPanel::Modes::STAY){
        this->stop->setEnabled(true);
        this->play->setEnabled(true);
        this->play->setType(CustomeButton::Types::play);
        this->step_right->setEnabled(true);
    }
    else if (mode == ControlPanel::Modes::STOP_ONLY){
        this->stop->setEnabled(true);
        this->play->setEnabled(false);
        this->step_right->setEnabled(false);
    }
}
