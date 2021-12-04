#include "inputblock.h"
#include <QBoxLayout>
#include <QWheelEvent>

InputBlock::InputBlock(QWidget* p, int blocks) : QLabel(p)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->lines = new InputLine*[blocks];
    this->MAX_COMMANDS = 100;
    this->blocks = blocks;
    this->setMinimumHeight((this->blocks + 1) * 50);
    this->focus_block = 0;
    this->start = 0;
    this->end = this->blocks - 1;
    this->active_line = -1;
    this->error_line = -1;
    this->mode = InputBlock::Mode::setup;

    QVBoxLayout* input_layout = new QVBoxLayout();
    this->head_block = new InputLine(this, -1, true);
    input_layout->addWidget(this->head_block);

    for (int i = 0; i < this->blocks; i++){
        InputLine* line = new InputLine(this, i, false);
        connect(line, &InputLine::upd_command, this, &InputBlock::commandChanged);
        connect(line, &InputLine::on_focus, this, &InputBlock::setFocusLine);
        this->lines[i] = line;
        input_layout->addWidget(line);
    }
    this->clear_commands();
    this->setLayout(input_layout);
}

void InputBlock::clear_commands(){
    this->commands = std::vector<Command>(this->MAX_COMMANDS);
}

void InputBlock::move_down(){
    if (this->end + 1 < this->MAX_COMMANDS){
        this->start++;
        this->end++;
        this->updateLines();
    }
}

void InputBlock::move_up(){
    if (this->start - 1 >= 0){
        this->start--;
        this->end--;
        this->updateLines();

    }
}

void InputBlock::updateLines(){
    for (int i = this->start; i <= this->end; i++){
        InputLine* line = this->lines[i - this->start];
        line->getCommand()->setText(this->commands[i].text);
        line->getComment()->setText(this->commands[i].comment);
        if (i == this->active_line)
            line->setActive(true);
        else
            line->setActive(false);
        if (i == this->error_line)
            line->setError(true);
        else
            line->setError(false);
        line->setLineNum(i);
    }
}

int InputBlock::moveOnCommand(int c){
    int top_block = c - this->blocks / 2 + 1;
    if (top_block < 0){
        this->start = 0;
        this->end = this->blocks - 1;
        this->updateLines();
        return c;
    }
    int bot_block = c + this->blocks / 2;
    if (bot_block >= this->MAX_COMMANDS){
        bot_block = this->MAX_COMMANDS - 1;
        this->end = bot_block;
        this->start = bot_block - this->blocks + 1;
        this->updateLines();
        return this->blocks - (this->MAX_COMMANDS - c);
    }
    this->start = top_block;
    this->end = top_block + this->blocks - 1;
    this->updateLines();
    return this->blocks / 2 - 1;
}

void InputBlock::setMode(Mode mode){
    this->mode = mode;
    if (mode == InputBlock::Mode::show){
        for (int i = 0; i < this->blocks; i++)
            if (this->lines[i]->isEnabled())
                this->lines[i]->setEnabled(false);
    }
    else if (mode == InputBlock::Mode::setup) {
        for (int i = 0; i < this->blocks; i++)
            if (!this->lines[i]->isEnabled())
                this->lines[i]->setEnabled(true);
    }
}

std::vector<InputBlock::Command>* InputBlock::getCommands(){
    return &this->commands;
}

void InputBlock::wheelEvent(QWheelEvent* e){
    if (this->mode != InputBlock::Mode::show){
        this->head_block->setFocus();
        if (e->delta() > 0)
            this->move_up();
        else
            this->move_down();
    }
}

void InputBlock::commandChanged(int id, int num, QString new_str, QString new_comment){
    Q_UNUSED(id);
    this->commands[num].text = new_str;
    this->commands[num].comment = new_comment;
}

void InputBlock::setFocusLine(int id){
    this->focus_block = id;
    this->updateLines();
}

void InputBlock::setActiveLine(int line){
    this->active_line = line;
    this->updateLines();
}

void InputBlock::setErrorLine(int line){
    this->error_line = line;
    this->updateLines();
}

void InputBlock::keyPressEvent(QKeyEvent* e){
    if (this->mode != InputBlock::Mode::show){
        if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Down){
            if (this->focus_block + 1 < this->blocks)
                this->lines[this->focus_block + 1]->getCommand()->setFocus();
            else {
                InputLine* line = this->lines[this->focus_block];
                this->commandChanged(line->getId(), line->getLineNum(), line->getCommand()->text(), line->getComment()->text());
                this->move_down();
            }
        }
        else if (e->key() == Qt::Key_Up){
            if (this->focus_block - 1 >= 0)
                this->lines[this->focus_block - 1]->getCommand()->setFocus();
            else{
                InputLine* line = this->lines[this->focus_block];
                this->commandChanged(line->getId(), line->getLineNum(), line->getCommand()->text(), line->getComment()->text());
                this->move_up();
            }
        }
    }
}
