//
// Created by milan on 2.8.20..
//
#include <QPushButton>
#include <string>
#include <QMouseEvent>

#ifndef FIRST_QT_APP_FIELD_H
#define FIRST_QT_APP_FIELD_H


class field : public QPushButton {
    Q_OBJECT
public:
    bool isBomb{false};
    bool isOpened{false};
    bool selected{false};
    int minesAround{0};
    std::string text;
    explicit field(QWidget *parent = 0);

private slots:
    void mousePressEvent(QMouseEvent *e);

signals:
    void rightClicked();
    void leftClicked();
};


#endif //FIRST_QT_APP_FIELD_H
