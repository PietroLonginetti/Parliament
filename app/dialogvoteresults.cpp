#include "dialogvoteresults.h"
#include "ui_dialogvoteresults.h"

DialogVoteResults::DialogVoteResults(QWidget *parent) :  QDialog(parent), ui(new Ui::DialogVoteResults){
    ui->setupUi(this);

}

void DialogVoteResults::setLabel(QString text){
    ui->label->setText(text);
}

DialogVoteResults::~DialogVoteResults() {
    delete ui;
}
