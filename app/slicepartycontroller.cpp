#include "slicepartycontroller.h"
#include "slicecontrolwidget.h"
#include <QApplication>


// --- CONSTRUCTORS --- //
Party* SlicePartyController::createParty(QString name, Orientation o, int members){
    Party * party = new Party(name, o, members);
    party->setSlice(new Slice(party));
    party->getSlice()->setColorFromOrientation(party->getOrientation());

    connect(party->getSlice(), &Slice::hovered, this, &SlicePartyController::changeCursor);
    connect(party->getSlice(), &Slice::sliceHovered, this, &SlicePartyController::showBorder); 
    connect(party->getSlice(), &Slice::sliceHovered, this, &SlicePartyController::explodeSlice);
    connect(party->getSlice(), &Slice::sliceClicked, this, &SlicePartyController::handleSliceClicked);
    connect(party->getSlice(), &Slice::sliceRemoving, this, &SlicePartyController::removeParty);
    connect(this->scw->getUi()->membersSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SlicePartyController::updatePercentages);

    parties.push_back(party);
    return party;
}

Party* SlicePartyController::createPartyByCopy(Party* other){
    Party* party = new Party(other->getName(),other->getOrientation(),other->getMembers(),other->getOwner(),other->getConsensus());
    party->setSlice(new Slice(party));
    party->getSlice()->setColor(other->getSlice()->color());
    party->getSlice()->setLabelVisible(other->getSlice()->isLabelVisible());

    connect(party->getSlice(), &Slice::hovered, this, &SlicePartyController::changeCursor);
    connect(party->getSlice(), &Slice::sliceHovered, this, &SlicePartyController::showBorder);
    connect(party->getSlice(), &Slice::sliceHovered, this, &SlicePartyController::explodeSlice);
    connect(party->getSlice(), &Slice::sliceClicked, this, &SlicePartyController::handleSliceClicked);
    connect(party->getSlice(), &Slice::sliceRemoving, this, &SlicePartyController::removeParty);
    connect(this->scw->getUi()->membersSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SlicePartyController::updatePercentages);

    handleSliceClicked(party->getSlice());
    parties.push_back(party);
    return party;
}


// --- FUNCTIONS --- //
void SlicePartyController::refreshSliceControlWidget(Slice* slice){
    if(selectedSlice != nullptr){
        disconnectControlWidget(selectedSlice);
    }
    this->selectedSlice = slice;

    scw->getUi()->partyNameLineEdit->setText(this->selectedSlice->getParty()->getName());
    scw->getUi()->orientationComboBox->setCurrentIndex(this->selectedSlice->getParty()->getOrientation());
    scw->getUi()->membersSpinBox->setValue(this->selectedSlice->getParty()->getMembers());
    scw->getUi()->ownerLineEdit->setText(this->selectedSlice->getParty()->getOwner());
    scw->getUi()->percentageDoubleSpinBox->setValue(this->selectedSlice->percentage()*100);
    scw->getUi()->consensusSpinBox->setValue(this->selectedSlice->getParty()->getConsensus());

    connectControlWidget(selectedSlice);

    if(!scw->allItemsAreEnabled()){
       scw->enableWidgets(true);
    }
}

void SlicePartyController::disconnectControlWidget(Slice *slice){
    if(selectedSlice != nullptr){
    disconnect(scw->getUi()->partyNameLineEdit, &QLineEdit::textChanged, slice, &Slice::updateName);
    disconnect(scw->getUi()->ownerLineEdit, &QLineEdit::textChanged, slice, &Slice::updateOwner);
    disconnect(scw->getUi()->orientationComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), slice, &Slice::updateOrientation);
    disconnect(scw->getUi()->membersSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), slice, &Slice::updateMembers);
    disconnect(scw->getUi()->removePartyButton, &QPushButton::clicked, slice, &Slice::removeYourself);
    disconnect(scw->getUi()->consensusSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), slice, &Slice::updateConsensus);
    }
}

void SlicePartyController::connectControlWidget(Slice *slice){
    connect(scw->getUi()->partyNameLineEdit, &QLineEdit::textChanged, slice, &Slice::updateName);
    connect(scw->getUi()->ownerLineEdit, &QLineEdit::textChanged, slice, &Slice::updateOwner);
    connect(scw->getUi()->orientationComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), slice, &Slice::updateOrientation);
    connect(scw->getUi()->membersSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), slice, &Slice::updateMembers);
    connect(scw->getUi()->membersSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SlicePartyController::updatePercentages);
    connect(scw->getUi()->removePartyButton, &QPushButton::clicked, slice, &Slice::removeYourself);
    connect(scw->getUi()->consensusSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), slice, &Slice::updateConsensus);
}

bool SlicePartyController::votePassed(){
    return (votingYes > votingNo);
}

void SlicePartyController::resetVotingParameters(){
    votingYes = 0;
    votingNo = 0;
}

// ---- SLOTS ---- //
void SlicePartyController::showBorder(bool show, Slice* slice){
    if(show){
        slice->setBorderWidth(3);
        slice->setBorderColor(QColor("silver"));
    } else {
        slice->setBorderColor(QColor("white"));
        slice->setBorderWidth(2);
    }
}

void SlicePartyController::changeCursor(bool change){
    if(change){
       QApplication::setOverrideCursor(Qt::PointingHandCursor);
    } else{
       QApplication::restoreOverrideCursor();
    }
}

void SlicePartyController::explodeSlice(bool hover, Slice* slice){
    if(hover){
        slice->setExplodeDistanceFactor(0.04);
        slice->setExploded(true);
    } else {
        slice->setExploded(false);
    }
}

void SlicePartyController::removeParty(Slice* slice){
    parties.erase(find(parties.begin(),parties.end(),slice->getParty()));
}

void SlicePartyController::updatePercentages(){
    if(selectedSlice != nullptr)
        scw->getUi()->percentageDoubleSpinBox->setValue(selectedSlice->percentage()*100);
}

void SlicePartyController::handleSliceClicked(Slice* slice){
    if(slice != selectedSlice){
        disconnect(slice, &Slice::sliceHovered, this, &SlicePartyController::explodeSlice);
        disconnect(slice, &Slice::sliceHovered, this, &SlicePartyController::showBorder);
        slice->setExploded(true);
        slice->setExplodeDistanceFactor(0.1);
        slice->setBorderColor("silver");
        slice->setBorderWidth(4);
        releaseLockedSlice(selectedSlice);
        refreshSliceControlWidget(slice);
        selectedSlice = slice;
    }
}

void SlicePartyController::releaseLockedSlice(Slice* slice){
    if(slice != nullptr){
        connect(slice, &Slice::sliceHovered, this, &SlicePartyController::explodeSlice);
        connect(slice, &Slice::sliceHovered, this, &SlicePartyController::showBorder);
        slice->setExploded(false);
        slice->setBorderWidth(0);
        slice->setBorderColor("white");
    }
}

void SlicePartyController::handleSliceClickedDuringVote(Slice* slice){
    if(vw->getUi()->yesButton->isChecked()){
        if(slice->color() == "crimson"){
            votingNo -= slice->getParty()->getMembers();
            slice->setColor("mediumseagreen");
            votingYes += slice->getParty()->getMembers();
        } else if(slice->color() == "lightgray"){
            slice->setColor("mediumseagreen");
            votingYes += slice->getParty()->getMembers();
        }
    }
    if(vw->getUi()->noButton->isChecked()){
        if(slice->color() == "mediumseagreen"){
            votingYes -= slice->getParty()->getMembers();
            slice->setColor("crimson");
            votingNo += slice->getParty()->getMembers();
        } else if(slice->color() == "lightgray"){
            slice->setColor("crimson");
            votingNo += slice->getParty()->getMembers();
        }
    }
    if(vw->getUi()->absteinedButton->isChecked()){
        if(slice->color() == "mediumseagreen"){
            votingYes -= slice->getParty()->getMembers();
            slice->setColor("lightgray");
        } else if(slice->color() == "crimson"){
            votingNo -= slice->getParty()->getMembers();
            slice->setColor("lightgray");
        }
    }
}



// ---- DESTRUCTOR ---- //
SlicePartyController::~SlicePartyController(){
    vector<Party *>::iterator i;
    for(i = parties.begin(); i!=parties.end(); ++i){
        delete *i;
    }
}
