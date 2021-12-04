#ifndef NODE_H
#define NODE_H

#include <QWidget>
#include <QLabel>

class Node : public QLabel
{
    Q_OBJECT
public:
    Node();
    Node(QWidget*, int);
    void setIndex(int);
    void setCurs(bool);
    void setActive(bool);
    void mousePressEvent(QMouseEvent*) override;
    void paintEvent(QPaintEvent*) override;


signals:
    void activated(int, bool);
private:
    int index;
    bool curs;
    bool active;
    QRect active_zone;
    bool enable;

};
#endif // NODE_H
