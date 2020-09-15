#include "widget.h"
#include "ui_widget.h"
#include "field.h"
#include "game.h"
#include <QGridLayout>
#include <math.h>
#include <string>
#include <algorithm>
#include <QMessageBox>

game::game(QWidget *parent)
        : QWidget(parent)
{
    QGridLayout * layout = new QGridLayout(this);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);
    this->mainLayout = layout;
    this->InitGame();
    this->startNewGame();
}

game::~game()
{
}

void game::startNewGame() {
    this->InitGame();
    std::vector<int> list;
    for (int i = 0; i < 100; ++i) {
        list.insert(list.end(), i+1);
    }
    std::vector<int> mines {};
    int max;
    max = 100;
    int mines_count = 20;
    this->numberOfMines = mines_count;
    srand(time(0));

    while (mines.size() < mines_count) {
        int num = rand()%max;
        if(std::find(mines.begin(), mines.end(), num) == mines.end()){
            mines.push_back(num);
        }
    }

    for (int i = 0; i < mines.size(); ++i) {
        this->Fields[mines[i]]->setText("");
        this->Fields[mines[i]]->isBomb = true;
    }

    for (int i = 0; i < 100; ++i) {
        if (std::find(mines.begin(), mines.end(), i) == mines.end()) {
            this->Fields[i]->setText("");
            this->Fields[i]->minesAround = this->MinesAround(i);
        }
    }
}


void game::resizeEvent(QResizeEvent *event)
{
    this->resize(this->width(), this->width());
}

void game::clickedField(int position) {
    std::vector<int> new_clear_positions = {};
    if (!this->Fields[position]->selected and !this->finished) {
        Fields[position]->setDisabled(true);
        if (Fields[position]->isBomb) {
            this->activateAllBombs();
            this->finished = true;
        } else {
            Fields[position]->isOpened = true;
            std::string numberText = std::to_string(this->Fields[position]->minesAround);
            QString qstr = QString::fromStdString(numberText);
            this->Fields[position]->setText(qstr);
            Fields[position]->setStyleSheet("color: rgb(0, 0, 0)");
        }
        if (this->MinesAround(position) == 0) {
            new_clear_positions.push_back(position);
            while (new_clear_positions.size()>0) {
                std::vector<int> additional_clear_positions = {};
                for (int i = 0; i < new_clear_positions.size(); ++i) {
                    std::vector<int> help_clear_positions = this->ClearAround(new_clear_positions[i]);
                    additional_clear_positions.insert(additional_clear_positions.end(), help_clear_positions.begin(), help_clear_positions.end());
                }
                new_clear_positions.clear();
                if (additional_clear_positions.size()>0) {
                    new_clear_positions.insert(new_clear_positions.end(), additional_clear_positions.begin(), additional_clear_positions.end());
                }

            }
        }
    }
    if (this->CountOpened(this->Fields) == 100 - this->numberOfMines) {
        QMessageBox msgBox;
        msgBox.setText("WIN!");
        msgBox.setInformativeText("You just won the game");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Save);
        msgBox.exec();
        this->finished = true;
    }

}

int game::MinesAround(int position) {
    int count = 0;
    int numberOfPositions = 0;
    if (position % 10 == 0 or position % 10 == 9) {
        numberOfPositions = 5;
    } else {
        numberOfPositions = 8;
    }
    int *positions;
    if (position % 10 == 0) {
        int SpecificPositions[] = { position - 10, position - 9, position + 10, position + 11, position + 1 };
        positions = SpecificPositions;
    } else if (position % 10 == 9)
    {
        int SpecificPositions[] = { position - 10, position - 11, position + 10, position + 9, position - 1 };
        positions = SpecificPositions;
    }
    else {
        int SpecificPositions[] = { position - 10, position - 11, position - 9, position + 10, position + 11,
                                    position + 9, position + 1, position - 1 };
        positions = SpecificPositions;
    }
    for (int i = 0; i < numberOfPositions; ++i) {
        if (positions[i] > -1 and positions[i] < 100 and this->Fields[positions[i]]->isBomb) {
            count ++;
        }
    }
    return count;
}

void game::ClearFields() {
    this->Fields.clear();
}

void game::InitGame() {
    this->finished = false;
    this->ClearFields();
    QGridLayout * layout = this->mainLayout;
    for (int i = 0; i < 100; ++i) {
        field *Field = new field(this);
        this->Fields.push_back(Field);
        layout->addWidget(Field, floor(i/10), i % 10);
        connect(Field, &field::leftClicked, this, [=]() {
            this->clickedField(i);
        });

        connect(Field, &field::rightClicked, this, [=]() {
            if (!this->finished) {
                this->selectField(i);
            }
        });
    }
}

void game::selectField(int position) {
    if (!this->Fields[position]->isOpened) {
        this->Fields[position]->selected = !this->Fields[position]->selected;
        this->Fields[position]->setText(this->Fields[position]->selected ? "?" : "");
    }
}

void game::activateAllBombs() {
    for (int i = 0; i < 100; ++i) {
        if (this->Fields[i]->isBomb) {
            this->Fields[i]->setStyleSheet("background-color: rgb(255, 0, 0); color: rgb(255, 255, 255)");
            if (this->Fields[i]->selected) {
                this->Fields[i]->setText("X");
            } else {
                this->Fields[i]->setText("BUM!");
            }
        }
    }
}

std::vector<int> game::ClearAround(int position) {
    std::vector<int> new_clear_positions = {};
    std::vector<int> positions_around = this->ClosedPositionsAround(position);
    for (int i = 0; i < positions_around.size(); ++i) {
        if (this->MinesAround(positions_around[i]) == 0 and !this->Fields[positions_around[i]]->isOpened) {
            new_clear_positions.push_back(positions_around[i]);
        }
        this->Fields[positions_around[i]]->isOpened = true;
        std::string numberText = std::to_string(this->Fields[positions_around[i]]->minesAround);
        QString qstr = QString::fromStdString(numberText);
        this->Fields[positions_around[i]]->setText(qstr);
        Fields[positions_around[i]]->setStyleSheet("color: rgb(0, 0, 0)");
    }
    return new_clear_positions;
}

std::vector<int> game::PositionsAround(int position) {
    std::vector<int> positions;
    if (position % 10 == 0) {
        positions = { position - 10, position - 9, position + 10, position + 11, position + 1 };
    } else if (position % 10 == 9)
    {
        positions = { position - 10, position - 11, position + 10, position + 9, position - 1 };
    }
    else {
        positions = { position - 10, position - 11, position - 9, position + 10, position + 11,
                                    position + 9, position + 1, position - 1 };
    }

    std::vector<int> to_remove;
    for (int i = 0; i < positions.size(); ++i) {
        if (positions[i] > 99 or positions[i] < 0) {
            to_remove.push_back(positions[i]);
        }
    }
    for (int i = 0; i < to_remove.size(); ++i) {
        positions.erase(std::remove(positions.begin(), positions.end(), to_remove[i]), positions.end());
    }
    return positions;
}

std::vector<int> game::ClosedPositionsAround(int position) {
    std::vector<int> positions;
    std::vector<int> to_remove;
    positions = this->PositionsAround(position);
    for (int i = 0; i < positions.size(); ++i) {
        if (this->Fields[positions[i]]->isOpened) {
            to_remove.push_back(positions[i]);
        }
    }
    for (int i = 0; i < to_remove.size(); ++i) {
        positions.erase(std::remove(positions.begin(), positions.end(), to_remove[i]), positions.end());
    }
    return positions;
}

size_t game::CountOpened(const std::vector<field *> &v) {
    return std::count_if(
            v.begin(),
            v.end(),
            [](field* i) { return i->isOpened; }
    );
}
