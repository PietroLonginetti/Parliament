#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setCentralWidget(ui->wizardWidget);
    connect(ui->newGameButton, &QPushButton::clicked, this, &MainWindow::startGame);
}

void MainWindow::startGame(){
    if(ui->membersSpinBox->value() > 0){
        p = new Parliament(ui->membersSpinBox->value(), ui->playersSpinBox->value());

        this->setCentralWidget(p);
        this->resize(900,600);

        QRect screenGeometry = QApplication::desktop()->screenGeometry();
        int x = (screenGeometry.width() - this->width()) / 2;
        int y = (screenGeometry.height() - this->height()) / 2;
        this->move(x, y);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

