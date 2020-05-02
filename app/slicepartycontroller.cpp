#include "slicepartycontroller.h"
#include "slicecontrolwidget.h"
#include <QApplication>
#include <time.h>


// --- CONSTRUCTORS --- //
SlicePartyController::SlicePartyController(){
    this->scw = new SliceControlWidget();
    this->vw = new VoteWidget();
    selectedSlice = nullptr;
    votingYes = 0;
    votingNo = 0;

    connect(scw->getUi()->removePartyButton, &QPushButton::clicked, this, &SlicePartyController::removePartySelected);
}


// --- FUNCTIONS --- //
Party* SlicePartyController::createParty(QString name, Orientation o, int members){
    Party * party = new Party(name, o, members);
    party->setSlice(new Slice(party));
    party->getSlice()->setColorFromOrientation(party->getOrientation());
    party->getSlice()->setLabelVisible(true);

    connect(party->getSlice(), &Slice::hovered, this, &SlicePartyController::changeCursor);
    connect(party->getSlice(), &Slice::sliceHovered, this, &SlicePartyController::showBorder); 
    connect(party->getSlice(), &Slice::sliceHovered, this, &SlicePartyController::explodeSlice);
    connect(party->getSlice(), &Slice::sliceClicked, this, &SlicePartyController::handleSliceClicked);

    emit partyCreated(party);
    parties.push_back(party);
    return party;
}

Party* SlicePartyController::copyParty(Party* other){
    Party* party = new Party(other->getName(),other->getOrientation(),other->getMembers(),other->getOwner(),other->getConsensus());
    party->setSlice(new Slice(party));
    party->getSlice()->setColor(other->getSlice()->color());
    party->getSlice()->setLabelVisible(other->getSlice()->isLabelVisible());

    connect(party->getSlice(), &Slice::hovered, this, &SlicePartyController::changeCursor);
    connect(party->getSlice(), &Slice::sliceHovered, this, &SlicePartyController::showBorder);
    connect(party->getSlice(), &Slice::sliceHovered, this, &SlicePartyController::explodeSlice);
    connect(party->getSlice(), &Slice::sliceClicked, this, &SlicePartyController::handleSliceClicked);

    handleSliceClicked(party->getSlice());
    parties.push_back(party);
    return party;
}

Party* SlicePartyController::createMixedGroup(int members){
    Party* mixedGroup = new Party(true);
    mixedGroup->setMembers(members);
    mixedGroup->setSlice(new Slice(mixedGroup));
    mixedGroup->getSlice()->setColorFromOrientation(NONE);
    mixedGroup->getSlice()->setLabelVisible(true);

    connect(mixedGroup->getSlice(), &Slice::hovered, this, &SlicePartyController::changeCursor);
    connect(mixedGroup->getSlice(), &Slice::sliceClicked, this, &SlicePartyController::handleSliceClicked);

    emit partyCreated(mixedGroup);
    parties.push_back(mixedGroup);
    return mixedGroup;
}

void SlicePartyController::refreshSliceControlWidget(Slice* slice){
    if(selectedSlice != nullptr){
        disconnectControlWidget(selectedSlice);
    }
    this->selectedSlice = slice;
    scw->getUi()->membersSpinBox->setMaximum(1000);

    scw->getUi()->partyNameLineEdit->setText(selectedSlice->getParty()->getName());
    scw->getUi()->orientationComboBox->setCurrentIndex(selectedSlice->getParty()->getOrientation());
    scw->getUi()->membersSpinBox->setValue(selectedSlice->getParty()->getMembers());
    scw->getUi()->ownerLineEdit->setText(selectedSlice->getParty()->getOwner());
    scw->getUi()->percentageDoubleSpinBox->setValue(selectedSlice->percentage()*100);
    scw->getUi()->consensusSpinBox->setValue(selectedSlice->getParty()->getConsensus());

    //Setting del limite  di incremento di deputati:
    vector<Party *>::iterator it = parties.begin();
    int max = 0;
    if(findMixedGroup(it)){
        max = selectedSlice->value() + (*it)->getMembers();
        scw->getUi()->membersSpinBox->setMaximum(max);
    }
    else {
        max = selectedSlice->value();
        scw->getUi()->membersSpinBox->setMaximum(max);
    }
    if(!selectedSlice->getParty()->getIsMixedGroup())
        scw->getUi()->membersSpinBox->setSuffix("  ( max: " + QString::number(max) + " )");
    else scw->getUi()->membersSpinBox->setSuffix("");

    //Enabling control Widget..
    if(!scw->allItemsAreEnabled()){
       scw->enableWidgets(true);
    }
    if(!selectedSlice->getParty()->getIsMixedGroup())
        connectControlWidget(selectedSlice);
    else scw->enableWidgets(false);
}

void SlicePartyController::disconnectControlWidget(Slice *slice){
    if(selectedSlice != nullptr){
        disconnect(scw->getUi()->partyNameLineEdit, &QLineEdit::textChanged, slice, &Slice::updateName);
        disconnect(scw->getUi()->ownerLineEdit, &QLineEdit::textChanged, slice, &Slice::updateOwner);
        disconnect(scw->getUi()->orientationComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), slice, &Slice::updateOrientation);
        disconnect(scw->getUi()->membersSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), slice, &Slice::updateMembers);
        disconnect(scw->getUi()->consensusSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), slice, &Slice::updateConsensus);
    }
}

void SlicePartyController::connectControlWidget(Slice *slice){
    connect(scw->getUi()->partyNameLineEdit, &QLineEdit::textChanged, slice, &Slice::updateName);
    connect(scw->getUi()->ownerLineEdit, &QLineEdit::textChanged, slice, &Slice::updateOwner);
    connect(scw->getUi()->orientationComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), slice, &Slice::updateOrientation);
    connect(scw->getUi()->membersSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), slice, &Slice::updateMembers);
    connect(scw->getUi()->consensusSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), slice, &Slice::updateConsensus);
}

bool SlicePartyController::findMixedGroup(vector<Party *>::iterator &it){
    // La funzione ritorna un valore booleano che indica se il gruppo misto si trova nell'intervallo di partiti
    // che partono dal valore dell'iteratore "it" passato fino alla fine del vettore. Alla fine della ricerca it "punterà"
    // al gruppo misto se esso era presente nel suddetto intervallo, altrimenti a vector.end

    bool found = false;
    while(it!=parties.end() && found == false){
        if((*it)->getIsMixedGroup()){
            found = true;
            break;
        }
        ++it;
    }
    return found;
}

//funzioni per la votazione
bool SlicePartyController::votePassed(){
    if(vw->getUi()->mixedGroupCheckBox->isChecked()){
        vector<Party *>::iterator it = parties.begin();
        if(findMixedGroup(it)){
            srand(time(NULL));
            if(rand() %2 == 0)
                votingYes += (*it)->getMembers();
            else votingNo += (*it)->getMembers();
        }
    }
    return (votingYes > votingNo);
}
void SlicePartyController::resetVotingParameters(){
    votingYes = 0;
    votingNo = 0;
}

//funzioni di controllo del vettore//
vector<Party *>::iterator SlicePartyController::getPartiesBegin(){
    return parties.begin();
}
void SlicePartyController::pushBackParty(Party* p){
    parties.push_back(p);
}
unsigned int SlicePartyController::getPartiesSize(){
    return parties.size();
}
Party* SlicePartyController::getPartyAt(int i){
    return parties.at(i);
}
void SlicePartyController::erasePartyAt(int i){
    std::vector<Party *>::iterator it = parties.begin() + i;
    parties.erase(it);
}

// ---- SLOTS ---- //
void SlicePartyController::showBorder(bool show, Slice* slice){
    if(show){
        slice->setBorderWidth(2);
        slice->setBorderColor(QColor("silver"));
    } else {
        slice->setBorderColor(QColor("white"));
        slice->setBorderWidth(1);
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

void SlicePartyController::removePartySelected(){
    if(selectedSlice != nullptr){
        updateMixedGroupMembers(selectedSlice->value());
        parties.erase(find(parties.begin(), parties.end(), selectedSlice->getParty()));
        selectedSlice->removeYourself(true);
    }
}

void SlicePartyController::updatePercentages(){
    if(selectedSlice != nullptr)
        scw->getUi()->percentageDoubleSpinBox->setValue(selectedSlice->percentage()*100);
}

void SlicePartyController::updateMixedGroupMembers(int membersToAdd){
    vector<Party *>::iterator it = parties.begin();
    if(!findMixedGroup(it)){
        createMixedGroup(membersToAdd);
    } else {
        if((*it)->getMembers() + membersToAdd <= 0){
            if(selectedSlice != nullptr && selectedSlice->getParty()->getIsMixedGroup())
                selectedSlice = nullptr;
            (*it)->getSlice()->removeYourself(false);
            parties.erase(it);
        } else {
            (*it)->getSlice()->updateMembers((*it)->getMembers() + membersToAdd);
        }
    }
}

void SlicePartyController::handleSliceClicked(Slice* slice){
    if(slice != selectedSlice){         
        disconnect(slice, &Slice::sliceHovered, this, &SlicePartyController::explodeSlice);
        disconnect(slice, &Slice::sliceHovered, this, &SlicePartyController::showBorder);
        if(!slice->getParty()->getIsMixedGroup()){
           slice->setExploded(true);
           slice->setExplodeDistanceFactor(0.1);
           slice->setBorderColor("gainsboro");
           slice->setBorderWidth(3);
        }
        releaseLockedSlice(selectedSlice);
        refreshSliceControlWidget(slice);
        selectedSlice = slice;
        connect(selectedSlice, &Slice::percentageChanged, this, &SlicePartyController::updatePercentages);
        connect(selectedSlice, &Slice::membersChanged, this, &SlicePartyController::updateMixedGroupMembers);
    }
}

void SlicePartyController::releaseLockedSlice(Slice* slice){
    if(slice != nullptr){
        disconnect(slice, &Slice::percentageChanged, this, &SlicePartyController::updatePercentages);
        disconnect(slice, &Slice::membersChanged, this, &SlicePartyController::updateMixedGroupMembers);
    }
    if(slice != nullptr && !slice->getParty()->getIsMixedGroup()){
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
