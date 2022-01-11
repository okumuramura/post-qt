#include "vectorline.h"
#include <QBoxLayout>
#include <QKeyEvent>
#include <QWheelEvent>

VectorLine::VectorLine(QWidget* p, int _enum, int width) : QWidget(p)
{
    this->vec_size = 201;
    this->vector = std::vector<bool>(this->vec_size);
    QHBoxLayout* main_layout = new QHBoxLayout();
    QHBoxLayout* nodes_layout = new QHBoxLayout();
    this->spos = this->vec_size / 2;
    this->curs = this->spos;
    this->node_count = _enum;
    this->mode = VectorLine::Mode::setup;

    this->need_backup = true;

    this->vbegin = -(_enum - 1) / 2;
    this->vend = this->vbegin + _enum - 1;
    int nw = width / _enum;

    this->back_btn = new CustomeButton(this, CustomeButton::Types::left);
    this->back_btn->setFixedSize(nw - 10, nw - 10);
    connect(this->back_btn, &CustomeButton::clicked, this, &VectorLine::move_back);
    this->back_btn->setFocusPolicy(Qt::NoFocus);

    this->next_btn = new CustomeButton(this, CustomeButton::Types::right);
    this->next_btn->setFixedSize(nw - 10, nw - 10);
    connect(this->next_btn, &CustomeButton::clicked, this, &VectorLine::move_next);
    this->next_btn->setFocusPolicy(Qt::NoFocus);

    main_layout->addWidget(this->back_btn, 0, Qt::AlignRight);
    this->nodes = new Node*[_enum];
    int sp = this->vbegin;
    for (int n = 0; n < _enum; n++){
        Node* node = new Node(this, sp);

        connect(node, &Node::activated, this, &VectorLine::upd_vector);
        node->setFixedSize(nw, nw * 2);
        if (this->spos + sp == this->curs)
            node->setCurs(true);
        sp++;
        this->nodes[n] = node;
        nodes_layout->addWidget(node);
    }
    main_layout->addLayout(nodes_layout);
    main_layout->addWidget(this->next_btn, 0, Qt::AlignLeft);
    this->setLayout(main_layout);


}

void VectorLine::upd_vector(int n, bool value){
    this->vector[this->spos + n] = value;
}

std::vector<bool>* VectorLine::getVector(){
    return &this->vector;
}

int VectorLine::getCursorPos(){
    return this->curs;
}

void VectorLine::move_back(){
    this->setFocus();
    if (this->mode == VectorLine::Mode::pause){
            if (this->spos + this->vbegin > 0){
            this->vbegin--;
            this->vend--;
            int sp = this->vbegin;
            for (int i = 0; i < this->node_count; i++){
                this->nodes[i]->setIndex(sp);
                this->nodes[i]->setActive(this->vector[this->spos + sp]);
                if (this->spos + sp == this->curs){
                    this->nodes[i]->setCurs(true);
                }
                sp++;
            }
        }
    }
    else {
        if (this->curs - 1 >= 0){
            this->moveOnCursor(this->curs - 1);
            if (!this->next_btn->isEnabled() && this->mode != VectorLine::Mode::show)
                this->next_btn->setEnabled(true);
        }
        if (this->curs - 1 < 0){
            if (this->back_btn->isEnabled())
                this->back_btn->setEnabled(false);
        }
    }
}

void VectorLine::move_next(){
    this->setFocus();
    if (this->mode == VectorLine::Mode::pause){
        if (this->spos + this->vend + 1 < this->vec_size){
            this->vbegin++;
            this->vend++;
            int sp = this->vbegin;
            for (int i = 0; i < this->node_count; i++){
                this->nodes[i]->setIndex(sp);
                this->nodes[i]->setActive(this->vector[this->spos + sp]);
                if (this->spos + sp == this->curs){
                    this->nodes[i]->setCurs(true);
                }
                sp++;
            }
        }

    }
    else {
        if (this->curs + 1 < static_cast<int>(this->vector.size())){
            this->moveOnCursor(this->curs + 1);
            if (!this->back_btn->isEnabled() && this->mode != VectorLine::Mode::show)
                this->back_btn->setEnabled(true);
        }
        if (this->curs + 1 >= static_cast<int>(this->vector.size()) && this->next_btn->isEnabled())
            this->next_btn->setEnabled(false);
    }
}

void VectorLine::setMode(VectorLine::Mode mode){

    if (mode == VectorLine::Mode::setup){
        this->next_btn->setEnabled(true);
        this->back_btn->setEnabled(true);
        for (int i = 0; i < this->node_count; i++)
            this->nodes[i]->setEnabled(true);
        this->need_backup = true;
        this->vector = this->backup_vector;
        this->curs = this->backup_curs;
        this->moveOnCursor(this->curs);
    }
    else if (mode == VectorLine::Mode::show) {
        this->next_btn->setEnabled(false);
        this->back_btn->setEnabled(false);
        for (int i = 0; i < this->node_count; i++)
            this->nodes[i]->setEnabled(false);

        if (this->need_backup){
            this->backup_vector = this->vector;
            this->backup_curs = this->curs;
        }
        this->need_backup = false;
        this->moveOnCursor(this->curs);
    }
    else if (mode == VectorLine::Mode::pause) {
        this->next_btn->setEnabled(true);
        this->back_btn->setEnabled(true);
    }
    this->mode = mode;
}

void VectorLine::moveOnCursor(){
    this->moveOnCursor(this->vec_size / 2);
}

void VectorLine::moveOnCursor(int pos){
    int left_node = pos - (this->node_count - 1) / 2;
    int right_node = pos + (this->node_count - 1) / 2;
    this->curs = pos;
    if (right_node >= static_cast<int>(this->vector.size())){
        right_node = static_cast<int>(this->vector.size()) - 1;
        this->vend = right_node - this->spos;
        this->vbegin = this->vend - this->node_count + 1;
    }
    else if (left_node < 0) {
        left_node = 0;
        this->vbegin = left_node - this->spos;
        this->vend = this->vbegin + this->node_count - 1;
    }
    else {
        this->vbegin = left_node - this->spos;
        this->vend = this->vbegin + this->node_count - 1;
    }
    int n = this->spos + this->vbegin;
    for (int i = 0; i < this->node_count; i++){
        this->nodes[i]->setIndex(n - this->spos);
        if (n == pos)
            this->nodes[i]->setCurs(true);
        this->nodes[i]->setActive(this->vector[n]);
        n++;
    }


}

void VectorLine::keyPressEvent(QKeyEvent* e){
    if (this->mode != VectorLine::Mode::show){
        if (e->key() == Qt::Key_Right)
            this->move_next();
        else if (e->key() == Qt::Key_Left)
            this->move_back();
    }

}

void VectorLine::wheelEvent(QWheelEvent* e){
    if (this->mode != VectorLine::Mode::show){
        if (e->delta() > 0)
            this->move_next();
        else
            this->move_back();
    }

}



