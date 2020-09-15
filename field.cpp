//
// Created by milan on 2.8.20..
//

#include "field.h"
#include <QPushButton>
#include <iostream>

field::field(QWidget *parent) :
        QPushButton(parent)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void field::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::RightButton)
            emit rightClicked();
    if(e->button()==Qt::LeftButton)
            emit leftClicked();
}