#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "vectorline.h"
#include "inputblock.h"
#include "controlpanel.h"
#include <QMainWindow>

#include <QTimer>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QWidget* main_widget;
    VectorLine* vector_line;
    InputBlock* input_block;
    ControlPanel* control_panel;

    std::vector<InputBlock::Command>* commands;
    int current_command;
    std::vector<bool>* vector;
    int curs;
    bool core_prepared;
    bool playing;

    void prepareCore();
    bool processCommand(InputBlock::Command);

    QTimer* step_timer;

    QString SET_FLAG = "V";
    QString REMOVE_FLAG = "X";
    QString MOVE_LEFT = "<";
    QString MOVE_RIGHT = ">";
    QString CONDITION = "?";
    QString END = "!";


private slots:
    void start();
    void stop();
    void step();
    void pause();

};
#endif // MAINWINDOW_H
