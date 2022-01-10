#include "mainwindow.h"
#include <QBoxLayout>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle(tr("Post Machine"));
    this->main_widget = new QWidget();
    QVBoxLayout* main_layout = new QVBoxLayout();
    this->vector_line = new VectorLine(this, 21, 1000);
    this->input_block = new InputBlock(this, 10);

    this->control_panel = new ControlPanel(this);

    this->step_timer = new QTimer();
    this->step_timer->setInterval(300); // Play speed
    connect(this->step_timer, &QTimer::timeout, this, &MainWindow::step);

    this->core_prepared = false;
    this->playing = false;

    connect(this->control_panel, &ControlPanel::playing, this, &MainWindow::start);
    connect(this->control_panel, &ControlPanel::stoping, this, &MainWindow::stop);
    connect(this->control_panel, &ControlPanel::pausing, this, &MainWindow::pause);
    connect(this->control_panel, &ControlPanel::steping_right, this, &MainWindow::step);

    main_layout->addWidget(this->vector_line);
    main_layout->addWidget(this->input_block);
    main_layout->addWidget(this->control_panel);
    this->main_widget->setLayout(main_layout);
    this->setCentralWidget(this->main_widget);
}

void MainWindow::start(){
    this->playing = true;
    this->step();
    this->step_timer->start();

}

void MainWindow::step(){

    if (!this->core_prepared){
        this->prepareCore();
        this->input_block->setActiveLine(0);
        this->input_block->moveOnCommand(0);
    }
    else {
        this->vector_line->play_mode();
        this->input_block->show_mode();
        if (!this->processCommand(this->commands->at(this->current_command))){
            this->input_block->setErrorLine(this->current_command);
            // if (this->step_timer->isActive())
                // this->step_timer->stop();
        }
        if (this->current_command != -1){
            this->input_block->moveOnCommand(this->current_command);
            this->input_block->setActiveLine(this->current_command);
            this->vector_line->moveOnCursor(this->curs);
            if (!this->playing){
                this->vector_line->pause_mode();
                this->input_block->pause_mode();
            }
        }
        else{
            this->control_panel->stop_pressed();
        }
    }
}

void MainWindow::pause(){
    this->vector_line->pause_mode();
    this->input_block->pause_mode();
    this->playing = false;
    if (this->step_timer->isActive())
        this->step_timer->stop();

}

void MainWindow::stop(){
    if (this->step_timer->isActive())
        this->step_timer->stop();
    this->core_prepared = false;
    this->playing = false;
    this->vector_line->setMode(VectorLine::setup);
    this->input_block->setMode(InputBlock::setup);
    this->input_block->setActiveLine(-1);
    this->input_block->setErrorLine(-1);

}

void MainWindow::prepareCore(){
    this->vector = this->vector_line->getVector();
    this->commands = this->input_block->getCommands();
    this->curs = this->vector_line->getCursorPos();
    this->current_command = 0;
    this->core_prepared = true;
}


bool MainWindow::processCommand(InputBlock::Command command){
    QString c = command.text.simplified().replace(" ", "");
    bool ok = true;
    if (c.left(this->SET_FLAG.length()) == this->SET_FLAG) // set flag
    {
        int next;
        QString substr = c.mid(this->SET_FLAG.length());
        if (substr == "") next = this->current_command + 1;
        else next = substr.toInt(&ok, 10);
        if (next < 0 || next >= this->input_block->getMaxCommands())
            ok = false;
        if (this->vector->at(this->curs))
            ok = false;
        if (ok){
            this->vector->at(this->curs) = true;
            this->current_command = next;
        }
    }
    else if (c.left(this->REMOVE_FLAG.length()) == this->REMOVE_FLAG)
    {
        int next;
        QString substr = c.mid(this->REMOVE_FLAG.length());
        if (substr == "") next = this->current_command + 1;
        else next = substr.toInt(&ok, 10);
        if (next < 0 || next >= this->input_block->getMaxCommands())
            ok = false;
        if (!this->vector->at(this->curs))
            ok = false;
        if (ok){
            this->vector->at(this->curs) = false;
            this->current_command = next;
        }
    }
    else if (c.left(this->MOVE_LEFT.length()) == this->MOVE_LEFT){
        int next;
        QString substr = c.mid(this->MOVE_LEFT.length());
        if (substr == "") next = this->current_command + 1;
        else next = substr.toInt(&ok, 10);
        if (next < 0 || next >= this->input_block->getMaxCommands())
            ok = false;
        if (this->curs - 1 < 0) ok = false;
        if (ok) {
            this->curs--;
            this->current_command = next;
        }

    }
    else if (c.left(this->MOVE_RIGHT.length()) == this->MOVE_RIGHT){
        int next;
        QString substr = c.mid(this->MOVE_RIGHT.length());
        if (substr == "") next = this->current_command + 1;
        else next = substr.toInt(&ok, 10);
        if (next < 0 || next >= this->input_block->getMaxCommands())
            ok = false;
        if (this->curs + 1 >= static_cast<int>(this->vector->size())){
            ok = false;
        }
        if (ok){
            this->curs++;
            this->current_command = next;
        }
    }
    else if (c.left(this->CONDITION.length()) == this->CONDITION){
        QString substr = c.mid(this->CONDITION.length());
        QString l, r;
        int r_next, l_next;
        int cd = substr.indexOf(";");
        if (cd == -1) ok = false;
        l = substr.left(cd);
        r = substr.mid(cd + 1);
        l_next = l.toInt(&ok);
        r_next = r.toInt(&ok);
        if (l_next < 0 || l_next >= this->input_block->getMaxCommands())
            ok = false;
        if (r_next < 0 || r_next >= this->input_block->getMaxCommands())
            ok = false;
        if (ok){
            if (this->vector->at(this->curs) == true)
                this->current_command = l_next;
            else
                this->current_command = r_next;
        }

    }
    else if (c.left(this->END.length()) == this->END){
        ok = (c == this->END);
        if (ok){
            this->current_command = -1;
        }
    }
    else ok = false;
    return ok;
}



MainWindow::~MainWindow(){
}

