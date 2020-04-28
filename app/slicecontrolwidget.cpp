#include "slicecontrolwidget.h"

// --- CONSTRUCTORS --- //
SliceControlWidget::SliceControlWidget(QWidget *parent) : QWidget(parent), ui(new Ui::SliceControlWidget) {
    ui->setupUi(this);

    ui->orientationComboBox->setCurrentIndex(-1);
    ui->percentageDoubleSpinBox->setSuffix("%");
    allItemsEnabled = false;
    enableWidgets(false);

    this->setLayout(ui->totalWidget);
}

// --- FUNCTIONS --- //
void SliceControlWidget::enableWidgets(bool enable){
    ui->partyNameLineEdit->setEnabled(enable);
    ui->orientationComboBox->setEnabled(enable);
    ui->membersSpinBox->setEnabled(enable);
    ui->ownerLineEdit->setEnabled(enable);
    ui->consensusSpinBox->setEnabled(enable);
    ui->percentageDoubleSpinBox->setEnabled(enable);
    ui->removePartyButton->setEnabled(enable);
    ui->changeColorButton->setEnabled(false);       //TODO: da implementare
    allItemsEnabled = enable;
}

// --- DESTRUCTOR --- //
SliceControlWidget::~SliceControlWidget(){
    delete ui;
}
