#ifndef INPUTBLOCK_H
#define INPUTBLOCK_H

#include "inputline.h"

#include <QWidget>
#include <QLabel>

#include <iostream>
#include <vector>

class InputBlock : public QLabel
{
    Q_OBJECT
public:
    enum Mode{
        show,
        setup,
        pause
    };
    struct Command{
        Command() : text(""), comment(""){};
        Command(QString a, QString b) : text(a), comment(b){};
        QString text;
        QString comment;
    };
    InputBlock(QWidget*, int);
    void setMode(Mode);
    int moveOnCommand(int);
    std::vector<Command>* getCommands();
    void setActiveLine(int);
    void setErrorLine(int);
    void updateLines();
    int getMaxCommands() {return this->MAX_COMMANDS;};
private:
    void move_down();
    void move_up();
    void wheelEvent(QWheelEvent*) override;
    void keyPressEvent(QKeyEvent*) override;

    std::vector<Command> commands;
    InputLine** lines;
    InputLine* head_block;
    int MAX_COMMANDS;
    int blocks;
    int focus_block;
    int start, end;
    int active_line;
    int error_line;
    Mode mode;

public slots:
    void setFocusLine(int);
    void show_mode() {this->setMode(Mode::show);};
    void setup_mode() {this->setMode(Mode::setup);};
    void pause_mode() {this->setMode(Mode::pause);};
    void clear_commands();


private slots:
    void commandChanged(int, int, QString, QString);

};

#endif // INPUTBLOCK_H
