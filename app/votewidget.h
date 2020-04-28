#ifndef VOTEWIDGET_H
#define VOTEWIDGET_H

#include <QWidget>
#include "ui_votewidget.h"

namespace Ui {class VoteWidget;}

class VoteWidget : public QWidget {
    Q_OBJECT

private:
    Ui::VoteWidget *ui;



public:
    explicit VoteWidget(QWidget *parent = nullptr);
    Ui::VoteWidget* getUi() {return ui;}
    ~VoteWidget();

};

#endif // VOTEWIDGET_H
