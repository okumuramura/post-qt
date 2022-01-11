#include "mainwindow.h"
#include <QBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->updateTitle();
    this->main_widget = new QWidget();
    QVBoxLayout* main_layout = new QVBoxLayout();
    this->vector_line = new VectorLine(this, 21, 1000);
    this->input_block = new InputBlock(this, 10);

    this->setWindowIcon(QIcon(":/icons/icons/postm.png"));

    this->prepareMenu();

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

    this->prepareCore();

    main_layout->addWidget(this->vector_line);
    main_layout->addWidget(this->input_block);
    main_layout->addWidget(this->control_panel);
    this->main_widget->setLayout(main_layout);
    this->setCentralWidget(this->main_widget);

    connect(this->input_block, &InputBlock::changed, this, &MainWindow::unsave);
}

void MainWindow::updateTitle(){
    QString br = "[%1]";
    this->setWindowTitle(
                QString("%1 [%2]%3")
                .arg(this->WINDOW_TITLE)
                .arg((this->CURRENT_FILE.isEmpty()) ? tr("new file") : this->CURRENT_FILE)
                .arg((this->is_saved) ? tr("") : tr("*"))
                );
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

void MainWindow::prepareMenu(){
    // ---------ICONS------------
    QPixmap new_pixmap(":/icons/icons/new.png");
    QPixmap load_pixmap(":/icons/icons/load.png");
    QPixmap save_pixmap(":/icons/icons/save.png");
    QPixmap help_pixmap(":/icons/icons/help.png");
    QPixmap exit_pixmap(":/icons/icons/exit.png");

    this->new_action = new QAction(new_pixmap, "&New", this);
    this->save_action = new QAction(save_pixmap, "&Save", this);
    this->load_action = new QAction(load_pixmap, "&Load", this);
    this->help_action = new QAction(help_pixmap, "&Help", this);
    this->exit_action = new QAction(exit_pixmap, "&Quit", this);

    QMenu* file_menu = menuBar()->addMenu("&File");
    QMenu* about_menu = menuBar()->addMenu("&About");

    file_menu->addAction(this->new_action);
    file_menu->addAction(this->save_action);
    file_menu->addAction(this->load_action);
    file_menu->addSeparator();
    file_menu->addAction(this->exit_action);

    about_menu->addAction(this->help_action);

    // --------SHORTCUTS---------
    this->new_action->setShortcut(tr("CTRL+N"));
    this->exit_action->setShortcut(tr("CTRL+Q"));
    this->save_action->setShortcut(tr("CTRL+S"));
    this->load_action->setShortcut(tr("CTRL+O"));
    this->help_action->setShortcut(tr("F1"));

    // --------CONNECTIONS-------
    connect(this->exit_action, &QAction::triggered, this, &QWidget::close);
    connect(this->help_action, &QAction::triggered, this, &MainWindow::showHelp);
    connect(this->save_action, &QAction::triggered, this, &MainWindow::showSaveDialog);
    connect(this->load_action, &QAction::triggered, this, &MainWindow::showLoadDialog);
    connect(this->new_action, &QAction::triggered, this, &MainWindow::newFile);

}

// Help message window
void MainWindow::showHelp(){
    QMessageBox help_window;
    help_window.setWindowTitle("Help");
    help_window.setWindowIcon(QIcon(":/icons/icons/help.png"));
    QString help_text = "This is Post machine emulator by okumuramura.\n"
                        "Commands:\n"
                        "%1 - move to the right on the tape\n"
                        "%2 - move to the left on the tape\n"
                        "%3 - place a mark\n"
                        "%4 - remove a mark\n"
                        "%5{a};{b} - If there is a mark in the current cell, go to line a, otherwise go to line b.\n"
                        "%6 - end point\n\n"
                        "After the commands moving, place a mark and remove a mark, "
                        "the execution goes to the line whose number is specified after it, "
                        "if the number is not specified, the next line will be executed.\n\n"
                        "The column with comments is for ease of human perception of the program and has no effect on execution.";

    help_window.setText(help_text
                        .arg(this->MOVE_RIGHT)
                        .arg(this->MOVE_LEFT)
                        .arg(this->SET_FLAG)
                        .arg(this->REMOVE_FLAG)
                        .arg(this->CONDITION)
                        .arg(this->END)
                        );
    help_window.exec();
}

void MainWindow::showSaveDialog(){
    qDebug() << "debug still working!";
    QString file_name = QFileDialog::getSaveFileName(
                this,
                tr("Save Post programm"),
                "",
                tr("Post files (*.pst);;All files (*)")
                );

    if (file_name.isEmpty())
        return;

    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly)){
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_1);

    out << 0x66606660; // magic number
    out << this->vector_line->getCursorPos();
    out << static_cast<qint32>(this->vector->size());
    for (size_t i = 0; i < this->vector->size(); i++){
        out << static_cast<bool>(this->vector->at(i));
    }

    out << static_cast<qint32>(this->commands->size());

    for (auto it = this->commands->begin(); it != this->commands->end(); it++){
        out << it->text << it->comment;
    }

    this->CURRENT_FILE = file_name;
    this->is_saved = true;
    this->updateTitle();
}

void MainWindow::showLoadDialog(){
    QString file_name = QFileDialog::getOpenFileName(
                this,
                tr("Open Post programm"),
                "",
                tr("Post files (*pst);;All files (*)")
                );

    if (file_name.isEmpty())
        return;

    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_1);

    qint32 magic;
    in >> magic;

    if (magic != 0x66606660){
        QMessageBox::critical(this, "Unable to load from file", "Wrong file format!");
        return;
    }

    qint32 tape_size;
    int cur_position;

    in >> cur_position;
    in >> tape_size;

    size_t tape_size_t = static_cast<size_t>(tape_size);
    bool value;

    this->vector->clear();
    this->vector->resize(tape_size_t);

    for (size_t i = 0; i < tape_size_t; i++){
        in >> value;
        this->vector->at(i) = value;
    }

    size_t commands_size_t;
    qint32 commands_size;
    InputBlock::Command com;
    in >> commands_size;

    commands_size_t = static_cast<size_t>(commands_size);

    this->input_block->clear_commands();

    for (size_t i = 0; i < commands_size_t; i++){
        in >> com.text >> com.comment;
        this->commands->at(i) = com;
    }

    this->vector_line->moveOnCursor(cur_position);
    this->input_block->updateLines();

    this->CURRENT_FILE = file_name;
    this->is_saved = true;
    this->updateTitle();
}

void MainWindow::newFile(){
    for (auto it = this->vector->begin(); it != this->vector->end(); it++){
        *it = false;
    }
    this->vector_line->moveOnCursor();

    this->input_block->clear_commands();
    this->commands = this->input_block->getCommands();
    this->input_block->moveOnCommand(0);

    this->CURRENT_FILE = "";
    this->is_saved = false;
    this->updateTitle();
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

void MainWindow::unsave(){
    if (this->is_saved){
        this->is_saved = false;
        this->updateTitle();
    }
}



MainWindow::~MainWindow(){
}

