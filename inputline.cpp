#include "inputline.h"

#include <QBoxLayout>
#include <QPainter>
#include <QFocusEvent>

InputLine::InputLine(QWidget* p, int id, bool head) : QLabel(p)
{
    this->setMinimumHeight(50);
    this->main_font = QFont("Arial", 16);
    this->no_active_brush = Qt::NoBrush;
    this->active_brush = QBrush(QColor(180, 180, 180));
    this->error_brush = QBrush(QColor(255, 0, 0, 40));
    this->is_active = false;
    this->is_error = false;
    this->id = id;
    this->line_num = id;
    this->is_head = head;
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    if (!this->is_head){
        this->command = new MQLineEdit(this);
        this->command->setMinimumHeight(50);
        this->command->setStyleSheet(tr("background: rgba(0, 0, 0, 0); border: none;"));
        this->command->setAlignment(Qt::AlignCenter);
        this->command->setFont(this->main_font);
        connect(this->command, &QLineEdit::editingFinished, this, &InputLine::isChanging);
        connect(this->command, &MQLineEdit::on_focus, this, &InputLine::editOnFocus);

        this->comment = new MQLineEdit(this);
        this->comment->setMinimumHeight(50);
        this->comment->setStyleSheet(tr("background: rgba(0, 0, 0, 0); border: none;"));
        this->comment->setAlignment(Qt::AlignCenter);
        this->comment->setFont(this->main_font);
        connect(this->comment, &QLineEdit::editingFinished, this, &InputLine::isChanging);
        connect(this->comment, &MQLineEdit::on_focus, this, &InputLine::editOnFocus);

        QHBoxLayout* line_layout = new QHBoxLayout();
        line_layout->setMargin(0);
        line_layout->addSpacing(50);
        line_layout->addWidget(this->command);
        line_layout->addWidget(this->comment);

        this->setLayout(line_layout);
    }
}

void InputLine::setActive(bool value){
    this->is_active = value;
}

void InputLine::setError(bool value) {
    this->is_error = value;
}

void InputLine::setLineNum(int num){
    this->line_num = num;
    this->update();
}

void InputLine::isChanging(){
    emit this->upd_command(this->id, this->line_num, this->command->text(), this->comment->text());
}

void InputLine::editOnFocus(){
    emit this->on_focus(this->id);
}

MQLineEdit* InputLine::getCommand(){
    return this->command;
}

MQLineEdit* InputLine::getComment(){
    return this->comment;
}

int InputLine::getId(){
    return this->id;
}

int InputLine::getLineNum(){
    return this->line_num;
}


void InputLine::paintEvent(QPaintEvent* e){
    Q_UNUSED(e)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPen pen(Qt::black);
    pen.setWidth(3);
    painter.setPen(pen);

    if (this->is_error)
        painter.setBrush(this->error_brush);
    else if (this->is_active)
        painter.setBrush(this->active_brush);
    else
        painter.setBrush(this->no_active_brush);
    painter.drawRect(this->rect());
    painter.setBrush(Qt::NoBrush);
    painter.drawLine(this->size().height(), 0, this->size().height(), this->size().height());
    int t = (this->size().width() - this->size().height()) / 2;
    painter.drawLine(this->size().height() + t, 0, this->size().height() + t, this->size().height());
    painter.setFont(this->main_font);

    if (this->is_head){
        painter.drawText(0, 0, this->size().height(), this->size().height(), Qt::AlignCenter, tr("#"));
        painter.drawText(this->size().height(), 0, t, this->size().height(), Qt::AlignCenter, tr("Command"));
        painter.drawText(this->size().height() + t, 0, t, this->size().height(), Qt::AlignCenter, tr("Comment"));
    }
    else
        painter.drawText(0, 0, this->size().height(), this->size().height(), Qt::AlignCenter, QString::number(this->line_num));
}

void MQLineEdit::focusInEvent(QFocusEvent* e){
    QLineEdit::focusInEvent(e);
    emit this->on_focus();
}
