#include "node.h"

#include <QMouseEvent>
#include <QPainter>

Node::Node(QWidget* p, int id) : QLabel(p)
{
    this->index = id;
    this->curs = false;
    this->setIndex(index);
    this->active = false;
    this->active_zone = QRect(0, this->size().height() / 3, this->size().width(), this->size().height() / 3);
}

Node::Node() : QLabel(NULL){

}

void Node::setIndex(int index){
    this->curs = false;
    this->index = index;
    this->update();
}

void Node::setCurs(bool flag){
    this->curs = flag;
    this->update();
}

void Node::setActive(bool active){
    this->active = active;
    this->update();
}

void Node::mousePressEvent(QMouseEvent* e){
    if (e->button() == Qt::LeftButton){
        this->setFocus();
        if (this->active_zone.contains(e->pos())){
            this->active = !this->active;
            emit this->activated(this->index, this->active);
            this->update();
        }
    }
}

void Node::paintEvent(QPaintEvent *e){
    Q_UNUSED(e);

    this->active_zone = QRect(0, this->size().height() / 3, this->size().width(), this->size().height() / 3);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPen pen(Qt::black);
    pen.setWidth(3);
    painter.setPen(pen);

    painter.drawRect(this->active_zone);

    if (this->active){
        int radius = ((this->active_zone.height() < this->active_zone.width()) ? this->active_zone.height() : this->active_zone.width()) / 2;
        painter.setBrush(QBrush(Qt::black));
        painter.drawEllipse(this->size().width() / 2 - radius / 2, this->size().height() / 2 - radius / 2, radius, radius);
    }

    painter.setFont(QFont("Arial", 16));
    painter.drawText(0, this->active_zone.height() * 2, this->size().width(), this->active_zone.height(), Qt::AlignCenter, QString::number(this->index));

    if (this->curs){
        QPolygon arrow;
        int t = this->size().height() / 15;
        int hw = this->size().width() / 2;
        arrow << QPoint(hw, t)
              << QPoint(hw, this->size().height() / 3 - t)
              << QPoint(hw + t, this->size().height() / 3 - t - t)
              << QPoint(hw - t, this->size().height() / 3 - t - t)
              << QPoint(hw, this->size().height() / 3 - t);
        painter.drawPolygon(arrow);
    }
}
