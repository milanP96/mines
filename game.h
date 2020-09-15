#ifndef FIRST_QT_APP_TEST_H
#define FIRST_QT_APP_TEST_H
#include <QWidget>
#include <QtWidgets/QGridLayout>
#include "field.h"

//QT_BEGIN_NAMESPACE
//namespace Ui { class Widget; }
//QT_END_NAMESPACE

class game : public QWidget
{
    Q_OBJECT

public:
    game(QWidget *parent = nullptr);
    ~game();
    std::vector<field *> Fields;
    QGridLayout * mainLayout;
    void InitGame();
    int MinesAround(int position);
    std::vector<int> ClearAround(int position);
    std::vector<int> PositionsAround(int position);
    std::vector<int> ClosedPositionsAround(int position);
    void ClearFields();
    void selectField(int position);
    bool finished{false};
    void activateAllBombs();
    int numberOfMines {0};
    size_t CountOpened(const std::vector<field*>& v);

public slots:
    void startNewGame();
    void clickedField(int position);

protected:
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
};


#endif //FIRST_QT_APP_TEST_H