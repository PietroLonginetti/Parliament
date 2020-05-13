#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "parliament.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);  
    Ui::MainWindow * getUi() const {return ui;}
    Parliament * getParliament() const {return p;}
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Parliament* p;

public slots:
    void startGame();
};
#endif // MAINWINDOW_H
