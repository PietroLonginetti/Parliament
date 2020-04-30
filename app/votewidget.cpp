#include "votewidget.h"
#include "ui_votewidget.h"

// --- CONSTRUCTORS --- //
VoteWidget::VoteWidget(QWidget *parent) : QWidget(parent), ui(new Ui::VoteWidget) {
    ui->setupUi(this);

    ui->yesButton->setChecked(true);

    this->setLayout(ui->totalWidget);
}


// --- DESTRUCTOR --- //
VoteWidget::~VoteWidget() {
    delete ui;
}
