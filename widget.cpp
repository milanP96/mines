#include "widget.h"
#include "ui_widget.h"
#include <iostream>
#include <QVBoxLayout>
#include <QPushButton>
#include "game.h"
#include "field.h"

Widget::Widget(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::Widget)
{
    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);
    layout->addSpacing(50);

    QPushButton *startGameBtn = new QPushButton("Start new game");
    startGameBtn->setMinimumSize(250, 40);
    startGameBtn->setMaximumSize(250, 40);
    game *Game = new game();
    layout->addWidget(startGameBtn);
    layout->addWidget(Game);
    setLayout(layout);

    connect(startGameBtn, &QPushButton::clicked, Game, &game::startNewGame);
}

Widget::~Widget()
{
    delete ui;
}