#include "parliament.h"


// --- CONSTRUCTORS --- //
Parliament::Parliament(int numOfPlayers){
    // initialization
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

    vw = new VoteWidget();
    spc->setVoteWidget(vw);
    slices = new QPieSeries();
    chart = new QChart();
    scw = new SliceControlWidget();
    spc->setControlWidget(scw);
    dvr = new DialogVoteResults();
    layout = new QVBoxLayout();

    connect(this->addPartyButton, &QPushButton::clicked, this, &Parliament::insertSlice);
    connect(this->voteButton, &QPushButton::clicked, this, &Parliament::votingProcess);
    connect(this, &Parliament::totalMembersModified, spc, &SlicePartyController::updatePercentages);
    connect(vw->getUi()->endVoteButton, &QPushButton::clicked, this, &Parliament::endVotingProcess);

    // create chart
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setAnimationDuration(1500);
    chart->legend()->hide();

    // create series
    for(int i = 0; i<numOfPlayers; i++){
        spc->createParty("Unknown", (Orientation)i, 20);
        slices->append(spc->getParties().at(i)->getSlice());
        connectSliceStuffing(spc->getParties().at(i)->getSlice());
    }
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
void Parliament::connectSliceStuffing(Slice* slice){
    connect(slice, &Slice::orientationChanged, this , &Parliament::placeSliceFromOrientation);
    connect(slice, &Slice::sliceRemoving, this, &Parliament::removeSlice);
}

void Parliament::transportSlice(Slice* sliceToMove, int pos){
    auto tmp = spc->createPartyByCopy(sliceToMove->getParty());
    sliceToMove->removeYourself();
    spc->getParties().push_back(tmp);
    spc->setSelectedSlice(tmp->getSlice());
    slices->insert(pos, tmp->getSlice());
    connectSliceStuffing(tmp->getSlice());
    spc->refreshSliceControlWidget(tmp->getSlice());
}



// --- SLOTS ---- //
int Parliament::placeSliceFromOrientation(Slice* s){
    if(s->getParty()->getOrientation() == RADICAL_LEFT){
        transportSlice(s,0);
        return 0;
    }
    if(s->getParty()->getOrientation() == LEFT){
        int count = 0;
        for(unsigned int i = 0; i < spc->getParties().size(); i++){
            if(spc->getParties().at(i)->getOrientation() == RADICAL_LEFT)
                count++;
        }
        transportSlice(s,count);
        return count;
    }
    if(s->getParty()->getOrientation() == CENTRE_LEFT){
        int count = 0;
        for(unsigned int i = 0; i < spc->getParties().size(); i++){
            if(spc->getParties().at(i)->getOrientation() == RADICAL_LEFT || spc->getParties().at(i)->getOrientation() == LEFT)
                count++;
        }
        transportSlice(s,count);
        return count;
    }
    if(s->getParty()->getOrientation() == CENTRE){
        int countleft = 0;
        for(unsigned int i = 0; i < spc->getParties().size(); i++){
            if(spc->getParties().at(i)->getOrientation() == RADICAL_LEFT || spc->getParties().at(i)->getOrientation() == LEFT
                    || spc->getParties().at(i)->getOrientation() == CENTRE_LEFT)
                countleft++;
        }
        int countright = 0;
        for(unsigned int i = 0; i < spc->getParties().size(); i++){
            if(spc->getParties().at(i)->getOrientation() == RADICAL_RIGHT || spc->getParties().at(i)->getOrientation() == RIGHT
                    || spc->getParties().at(i)->getOrientation() == CENTRE_RIGHT)
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
        for(unsigned int i = 0; i < spc->getParties().size(); i++){
            if(spc->getParties().at(i)->getOrientation() == RADICAL_RIGHT || spc->getParties().at(i)->getOrientation() == RIGHT)
                count++;
        }
        transportSlice(s, (slices->count() - count -1));
        return slices->count() - count -1;
    }
    if(s->getParty()->getOrientation() == RIGHT){
        int count = 0;
        for(unsigned int i = 0; i < spc->getParties().size(); i++){
            if(spc->getParties().at(i)->getOrientation() == RADICAL_RIGHT)
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

void Parliament::insertSlice(){
    auto p = spc->createParty("Unknown", CENTRE, 20);
    spc->getParties().push_back(p);
    placeSliceFromOrientation(p->getSlice());
    emit totalMembersModified();
}

void Parliament::removeSlice(Slice* slice){
    slices->remove(slice);
    spc->setSelectedSlice(nullptr);
    scw->enableWidgets(false);
    emit totalMembersModified();
}

void Parliament::votingProcess(){
    scw->enableWidgets(false);
    addPartyButton->setDisabled(true);
    voteButton->setDisabled(true);

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
        connect((Slice*)slices->slices().at(i), &Slice::sliceClicked, spc, &SlicePartyController::handleSliceClickedDuringVote);
    }
}

void Parliament::endVotingProcess(){ 
    if(spc->votePassed()){
        dvr->setLabel("Votes in favour: " + QString::number(spc->getVotingYes()) +"   Votes against: " + QString::number(spc->getVotingNo()) + "\nVote passed!");
    } else {
        dvr->setLabel("Votes in favour: " + QString::number(spc->getVotingYes()) +"   Votes against: " + QString::number(spc->getVotingNo()) + "\nVote rejected.");
    }
    dvr->exec();
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
