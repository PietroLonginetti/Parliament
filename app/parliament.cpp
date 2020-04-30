#include "parliament.h"


// --- CONSTRUCTORS --- //
Parliament::Parliament(unsigned int parliamentMembers, unsigned int numOfPlayers){
    // initialization
    this->parliamentMembers = parliamentMembers;
    this->numOfPlayers = numOfPlayers;

    spc = new SlicePartyController();

    addPartyButton = new QPushButton("Add Party");
    addPartyButton->setMaximumSize(150,100);
    addPartyButton->setMinimumSize(100,40);
    voteButton = new QPushButton("Vote");
    voteButton->setMaximumSize(150,100);
    voteButton->setMinimumSize(100,40);

    chartControlWidget = new QHBoxLayout;
    chartControlWidget->addSpacerItem(new QSpacerItem(20,20));
    chartControlWidget->addWidget(addPartyButton);
    chartControlWidget->addWidget(voteButton);
    chartControlWidget->addSpacerItem(new QSpacerItem(20,20));

    scw = spc->getControlWidget();
    vw = spc->getVoteWidget();
    slices = new QPieSeries(nullptr);
    chart = new QChart();
    dvr = new DialogVoteResults();
    layout = new QVBoxLayout();

    connect(spc, &SlicePartyController::partyCreated, this, &Parliament::insertSlice);
    connect(this->addPartyButton, &QPushButton::clicked, this, &Parliament::addPartyButtonClicked);
    connect(this, &Parliament::totalMembersModified, spc, &SlicePartyController::updatePercentages);

    connect(this->voteButton, &QPushButton::clicked, this, &Parliament::startVotingProcess);
    connect(vw->getUi()->confirmButton, &QPushButton::clicked, this, &Parliament::showResultEndExitVotingProcess);
    connect(vw->getUi()->cancelButton, &QPushButton::clicked, this, &Parliament::exitVotingProcess);

    // create chart
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setAnimationDuration(1500);
    chart->legend()->hide();

    // create series
    auto mg = spc->createMixedGroup(parliamentMembers);
    connectStuffing(mg->getSlice());
    slices->append(mg->getSlice());

    slices->setPieStartAngle(-90);
    slices->setPieEndAngle(90);
    slices->setHoleSize(0.2);
    slices->setPieSize(1);
    slices->setVerticalPosition(0.73);
    chart->addSeries(slices);

    // create chart view
    chartview = new QChartView(chart);
    chartview->setRenderHint(QPainter::Antialiasing);
    chartview->setMinimumSize(420,270);

    // create main layout
    layout->addLayout(chartControlWidget);
    layout->addWidget(chartview);
    layout->addWidget(spc->getControlWidget());
    layout->setAlignment(addPartyButton,Qt::AlignHCenter);
    layout->setAlignment(voteButton,Qt::AlignHCenter);
    layout->setAlignment(spc->getControlWidget(),Qt::AlignHCenter);
    setLayout(layout);

}


// --- FUNCTIONS --- //
void Parliament::connectStuffing(Slice* slice){
    if(!slice->getParty()->getIsMixedGroup()){
        connect(slice, &Slice::orientationChanged, this , &Parliament::replaceSliceFromOrientation);
        connect(slice, &Slice::sliceRemovingWhileSelected, this, &Parliament::removeSliceWhileSelected);
    }
    connect(slice, &Slice::sliceRemovingWhileNotSelected, this, &Parliament::removeSliceWhileNotSelected);
}

void Parliament::transportSlice(Slice* sliceToMove, int pos){
    auto tmp = spc->copyParty(sliceToMove->getParty());
    for(unsigned int i = 0; i < spc->getPartiesSize(); ++i){
        if(spc->getPartyAt(i) == sliceToMove->getParty()){
            spc->erasePartyAt(i);
            break;
        }
    }
    sliceToMove->removeYourself(true);
    numOfPlayers++;
    spc->setSelectedSlice(tmp->getSlice());
    slices->insert(pos, tmp->getSlice());
    connectStuffing(tmp->getSlice());
    spc->refreshSliceControlWidget(tmp->getSlice());
}

int Parliament::placeSliceFromOrientation(Slice* s){
    if(s->getParty()->getOrientation() == RADICAL_LEFT){
        slices->insert(0, s);
        return 0;
    }
    if(s->getParty()->getOrientation() == LEFT){
        int count = 0;
        for(unsigned int i = 0; i < spc->getPartiesSize(); i++){
            if(spc->getPartyAt(i)->getOrientation() == RADICAL_LEFT)
                count++;
        }
        slices->insert(count, s);
        return count;
    }
    if(s->getParty()->getOrientation() == CENTRE_LEFT){
        int count = 0;
        for(unsigned int i = 0; i < spc->getPartiesSize(); i++){
            if(spc->getPartyAt(i)->getOrientation() == RADICAL_LEFT || spc->getPartyAt(i)->getOrientation() == LEFT)
                count++;
        }
        slices->insert(count, s);
        return count;
    }
    if(s->getParty()->getOrientation() == CENTRE){
        int countleft = 0;
        for(unsigned int i = 0; i < spc->getPartiesSize(); i++){
            if(spc->getPartyAt(i)->getOrientation() == RADICAL_LEFT || spc->getPartyAt(i)->getOrientation() == LEFT
                    || spc->getPartyAt(i)->getOrientation() == CENTRE_LEFT)
                countleft++;
        }
        int countright = 0;
        for(unsigned int i = 0; i < spc->getPartiesSize(); i++){
            if(spc->getPartyAt(i)->getOrientation() == RADICAL_RIGHT || spc->getPartyAt(i)->getOrientation() == RIGHT
                    || spc->getPartyAt(i)->getOrientation() == CENTRE_RIGHT)
                countright++;
        }
        if(countleft <= countright){
            slices->insert(countleft, s);
            return (countleft);
        } else {
            slices->insert((slices->count() - countright - 1), s);
            return (slices->count() - countright - 1);
        }
    }
    if(s->getParty()->getOrientation() == CENTRE_RIGHT){
        int count = 0;
        for(unsigned int i = 0; i < spc->getPartiesSize(); i++){
            if(spc->getPartyAt(i)->getOrientation() == RADICAL_RIGHT || spc->getPartyAt(i)->getOrientation() == RIGHT)
                count++;
        }
        slices->insert((slices->count() - count -1), s);
        return slices->count() - count -1;
    }
    if(s->getParty()->getOrientation() == RIGHT){
        int count = 0;
        for(unsigned int i = 0; i < spc->getPartiesSize(); i++){
            if(spc->getPartyAt(i)->getOrientation() == RADICAL_RIGHT)
                count++;
        }
        slices->insert((slices->count() - count -1), s);
        return slices->count() - count;
    }

    if(s->getParty()->getOrientation() == RADICAL_RIGHT){
        slices->insert(slices->count()-1, s);
        return slices->count();
    }
    return -1;
}



// --- SLOTS ---- //
int Parliament::replaceSliceFromOrientation(Slice* s){
    if(s->getParty()->getOrientation() == RADICAL_LEFT){
        transportSlice(s,0);
        return 0;
    }
    if(s->getParty()->getOrientation() == LEFT){
        int count = 0;
        for(unsigned int i = 0; i < spc->getPartiesSize(); i++){
            if(spc->getPartyAt(i)->getOrientation() == RADICAL_LEFT)
                count++;
        }
        transportSlice(s,count);
        return count;
    }
    if(s->getParty()->getOrientation() == CENTRE_LEFT){
        int count = 0;
        for(unsigned int i = 0; i < spc->getPartiesSize(); i++){
            if(spc->getPartyAt(i)->getOrientation() == RADICAL_LEFT || spc->getPartyAt(i)->getOrientation() == LEFT)
                count++;
        }
        transportSlice(s,count);
        return count;
    }
    if(s->getParty()->getOrientation() == CENTRE){
        int countleft = 0;
        for(unsigned int i = 0; i < spc->getPartiesSize(); i++){
            if(spc->getPartyAt(i)->getOrientation() == RADICAL_LEFT || spc->getPartyAt(i)->getOrientation() == LEFT
                    || spc->getPartyAt(i)->getOrientation() == CENTRE_LEFT)
                countleft++;
        }
        int countright = 0;
        for(unsigned int i = 0; i < spc->getPartiesSize(); i++){
            if(spc->getPartyAt(i)->getOrientation() == RADICAL_RIGHT || spc->getPartyAt(i)->getOrientation() == RIGHT
                    || spc->getPartyAt(i)->getOrientation() == CENTRE_RIGHT)
                countright++;
        }
        if(countleft <= countright){
            transportSlice(s, countleft);
            return (countleft);
        } else {
            transportSlice(s, (slices->count() - countright - 1));
            return (slices->count() - countright - 1);
        }
    }
    if(s->getParty()->getOrientation() == CENTRE_RIGHT){
        int count = 0;
        for(unsigned int i = 0; i < spc->getPartiesSize(); i++){
            if(spc->getPartyAt(i)->getOrientation() == RADICAL_RIGHT || spc->getPartyAt(i)->getOrientation() == RIGHT)
                count++;
        }
        transportSlice(s, (slices->count() - count -1));
        return slices->count() - count -1;
    }
    if(s->getParty()->getOrientation() == RIGHT){
        int count = 0;
        for(unsigned int i = 0; i < spc->getPartiesSize(); i++){
            if(spc->getPartyAt(i)->getOrientation() == RADICAL_RIGHT)
                count++;
        }
        transportSlice(s, (slices->count() - count -1));
        return slices->count() - count;
    }

    if(s->getParty()->getOrientation() == RADICAL_RIGHT){
        transportSlice(s, slices->count()-1);
        return slices->count();
    }
    return -1;
}


void Parliament::addPartyButtonClicked(){
    spc->createParty("Unknown", CENTRE, 0);
}

void Parliament::insertSlice(Party * party){
    vector<Party *>::iterator it = spc->getPartiesBegin();

    if(party->getIsMixedGroup()){
        if(!spc->findMixedGroup(it)){
            slices->insert(numOfPlayers/2, party->getSlice());
            connectStuffing(party->getSlice());
        }
    }
    else { //party is not Mixed Group
        numOfPlayers++;
        if(!spc->findMixedGroup(it)){   // mixed group non è presente
            // TODO: mostrare un messaggio che dice che non è possibile aggiungere un nuovo partito perchè non ci sono sufficienti membri disponibili
        } else {    // mixed group è presente
            if((*it)->getMembers() <= 10){
                party->setMembers((*it)->getMembers());
                spc->updateMixedGroupMembers(-10);
                placeSliceFromOrientation(party->getSlice());
                connectStuffing(party->getSlice());
            } else {
                party->setMembers(10);
                spc->updateMixedGroupMembers(-10);
                placeSliceFromOrientation(party->getSlice());
                connectStuffing(party->getSlice());
            }
        }
    }
    //emit totalMembersModified();
}

void Parliament::removeSliceWhileSelected(Slice* slice){
    numOfPlayers--;
    slices->remove(slice);
    spc->setSelectedSlice(nullptr);
    scw->enableWidgets(false);
}

void Parliament::removeSliceWhileNotSelected(Slice* slice){
    numOfPlayers--;
    slices->remove(slice);
    //emit totalMembersModified();
}

void Parliament::startVotingProcess(){
    scw->enableWidgets(false);
    addPartyButton->setDisabled(true);
    voteButton->setDisabled(true);

    //Animate and insert the widget
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    vw->setGraphicsEffect(eff);
    QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
    a->setDuration(300);
    a->setStartValue(0);
    a->setEndValue(1);
    a->setEasingCurve(QEasingCurve::InBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);
    layout->insertWidget(2, vw);
    layout->setAlignment(vw, Qt::AlignHCenter);
    vw->setHidden(false);
    vw->getUi()->yesButton->setChecked(true);

    if(spc->getSelectedSlice() != nullptr){
        spc->disconnectControlWidget(spc->getSelectedSlice());
        spc->releaseLockedSlice(spc->getSelectedSlice());
        spc->setSelectedSlice(nullptr);
    }
    for(int i = 0; i<slices->count(); i++){
        disconnect((Slice*)slices->slices().at(i), &Slice::sliceClicked, spc, &SlicePartyController::handleSliceClicked);
        slices->slices().at(i)->setExploded(false);
        slices->slices().at(i)->setBorderColor("white");
        slices->slices().at(i)->setColor("lightgray");
        if(static_cast<Slice*>(slices->slices().at(i))->getParty()->getIsMixedGroup() == false)
            connect((Slice*)slices->slices().at(i), &Slice::sliceClicked, spc, &SlicePartyController::handleSliceClickedDuringVote);
    }
}

void Parliament::showResultEndExitVotingProcess(){
    if(spc->votePassed()){
        dvr->setLabel("Votes in favour: " + QString::number(spc->getVotingYes()) +"   Votes against: " + QString::number(spc->getVotingNo()) + "\nVote passed!");
    } else {
        dvr->setLabel("Votes in favour: " + QString::number(spc->getVotingYes()) +"   Votes against: " + QString::number(spc->getVotingNo()) + "\nVote rejected.");
    }
    dvr->exec();
    exitVotingProcess();
}

void Parliament::exitVotingProcess(){
    spc->resetVotingParameters();

    addPartyButton->setDisabled(false);
    voteButton->setDisabled(false);

    layout->removeWidget(vw);
    vw->setHidden(true);

    for(int i = 0; i<slices->count(); i++){
        disconnect((Slice*)slices->slices().at(i), &Slice::sliceClicked, spc, &SlicePartyController::handleSliceClickedDuringVote);
        static_cast<Slice*>(slices->slices().at(i))->setColorFromOrientation(static_cast<Slice*>(slices->slices().at(i))->getParty()->getOrientation());
        connect((Slice*)slices->slices().at(i), &Slice::sliceClicked, spc, &SlicePartyController::handleSliceClicked);
    }
}
