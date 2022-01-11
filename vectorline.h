#ifndef VECTORLINE_H
#define VECTORLINE_H

#include "customebutton.h"
#include "node.h"

#include <iostream>
#include <vector>

#include <QWidget>
#include <QLabel>

class Node;

class VectorLine : public QWidget
{
    Q_OBJECT


public:
    enum Mode {
        setup,
        show,
        pause
    };

    VectorLine(QWidget*, int, int);
    void setMode(Mode);
    void moveOnCursor();
    void moveOnCursor(int);
    std::vector<bool>* getVector();
    int getCursorPos();
private:
    void keyPressEvent(QKeyEvent*) override;
    void wheelEvent(QWheelEvent*) override;

    int vec_size;
    std::vector<bool> vector;
    std::vector<bool> backup_vector;
    int spos;
    int curs;
    int backup_curs;
    int vbegin, vend;
    int node_count;
    Node** nodes;
    CustomeButton* back_btn;
    CustomeButton* next_btn;
    Mode mode;
    bool need_backup;

public slots:
    void play_mode() {this->setMode(Mode::show);};
    void setup_mode() {this->setMode(Mode::setup);};
    void pause_mode() {this->setMode(Mode::pause);};
    void move_back();
    void move_next();
    // void view_back();
    // void view_next();

private slots:
    void upd_vector(int, bool);





};

#endif // VECTORLINE_H
