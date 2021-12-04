#ifndef INPUTLINE_H
#define INPUTLINE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>

class MQLineEdit;

class InputLine : public QLabel
{
    Q_OBJECT
public:
    InputLine(QWidget*, int, bool);
    void setActive(bool);
    void setError(bool);
    void setLineNum(int);
    MQLineEdit* getCommand();
    MQLineEdit* getComment();
    int getId();
    int getLineNum();


private slots:
    void isChanging();
    void editOnFocus();
signals:
    void upd_command(int, int, QString, QString);
    void on_focus(int);
private:
    void paintEvent(QPaintEvent*) override;

    QFont main_font;
    QBrush no_active_brush;
    QBrush active_brush;
    QBrush error_brush;
    bool is_active;
    bool is_error;
    int id;
    int line_num;
    bool is_head;
    MQLineEdit* command;
    MQLineEdit* comment;

};

class MQLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    MQLineEdit(QWidget* p) : QLineEdit(p){};
private:
    void focusInEvent(QFocusEvent* ) override;
signals:
    void on_focus();
};


#endif // INPUTLINE_H
