#ifndef SLICECONTROLWIDGET_H
#define SLICECONTROLWIDGET_H

#include <QWidget>
#include "slice.h"
#include "ui_slicecontrolwidget.h"


class SliceControlWidget : public QWidget {
    Q_OBJECT

private:
    Ui::SliceControlWidget *ui;
    bool allItemsEnabled;

public:
    explicit SliceControlWidget(QWidget *parent = nullptr);

    Ui::SliceControlWidget * getUi() const {return ui;}
    bool allItemsAreEnabled() const {return allItemsEnabled;}

    void enableWidgets(bool enable);

    ~SliceControlWidget();
};

#endif // SLICECONTROLWIDGET_H
