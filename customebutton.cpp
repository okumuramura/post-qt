#include "customebutton.h"
#include <QPainter>

CustomeButton::CustomeButton(QWidget* p, Types type) : QPushButton(p)
{
    this->type = type;
    this->active_color = Qt::black;
    this->not_active_color = QColor(180, 180, 180);
}

void CustomeButton::setType(Types type){
    this->type = type;
    this->update();
}

CustomeButton::Types CustomeButton::getType(){
    return this->type;
}

void CustomeButton::paintEvent(QPaintEvent* e){
    Q_UNUSED(e)
    QColor color = (this->isEnabled()) ? this->active_color : this->not_active_color;
    QRect rect = this->rect();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPen pen(Qt::black);
    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawRect(2, 2, rect.width() - 4, rect.height() - 4);
    QBrush brush(color);
    pen.setColor(color);
    painter.setPen(pen);
    painter.setBrush(brush);

    int adj = rect.width() / 4;
    if (this->type == Types::right || this->type == Types::play){
        QPolygon triangle;
        triangle << QPoint(rect.width() - adj, rect.height() / 2)
                 << QPoint(adj, adj)
                 << QPoint(adj, rect.height() - adj);
        painter.drawPolygon(triangle);
    }
    else if (this->type == Types::left){
        QPolygon triangle;
        triangle << QPoint(adj, rect.height() / 2)
                 << QPoint(rect.width() - adj, adj)
                 << QPoint(rect.width() - adj, rect.height() - adj);
        painter.drawPolygon(triangle);
    }
    else if (this->type == Types::stop){
        painter.drawRect(adj, adj, this->size().width() - adj - adj, this->size().height() - adj - adj);
    }
    else if (this->type == Types::step_right){
        QPolygon triangle;
        triangle << QPoint(rect.width() - adj, rect.height() / 2)
                 << QPoint(adj, adj)
                 << QPoint(adj, rect.height() - adj);
        painter.drawPolygon(triangle);
        painter.drawLine(rect.width() - adj, adj, rect.width() - adj, rect.height() - adj);
    }
    else if (this->type == Types::step_left){
        QPolygon triangle;
        triangle << QPoint(adj, rect.height() / 2)
                 << QPoint(rect.width() - adj, adj)
                 << QPoint(rect.width() - adj, rect.height() - adj);
        painter.drawPolygon(triangle);
        painter.drawLine(adj, adj, adj, rect.height() - adj);
    }
    else if (this->type == Types::pause){
        int hadj = adj / 2;
        painter.drawRect(adj, adj, hadj, rect.height() - adj - adj);
        painter.drawRect(adj * 2 +hadj, adj, hadj, rect.height() - adj - adj);
    }
}
